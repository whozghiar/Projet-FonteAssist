#include "dev_coucher.h"
#include "affichage.h"
#include "chronometre.h"
#include "joystick.h"
#include "communication_rx_tx.h"

// Used to check joystick state
int joystick_last_read_dc;


void start_dc(int mode){
  
  int tps_dc= 0;
  
  int poids = 20;
  
  boolean poids_start = false; //  Indicator used to confirm that a weight entered by the user is taken into account.
  boolean clear_lcd = false; // Indicator to define when the screen must be cleaned.
  
  
  unsigned long chrono; // TIMER.

  
  boolean exo_fini = false; // Boolean used to check the validity of the ex.

  // Managing PAUSE
  boolean en_cours= false;
  boolean pause = false;
  unsigned long TEMPS_EXERCICE = 0;
  unsigned long chrono_pause;

  // Info exercices
  int serie = 0,rep = 0; // No of series & repetitions
  int rep_total = 0;
  int serie_total = 0;
  
  // RX TX Communication
  unsigned int data;
  
  switch (mode){
    // Strenght
    case 0:
      serie = 1;
      rep = 20;
      rep_total = rep;
      serie_total = serie;
      break;
    // Cardio
    case 1:
      serie = 15;
      rep = 25;
      rep_total = rep;
      serie_total = serie;
      break;
    // 
    case 2:
      serie = 10;
      rep = 15;
      rep_total = rep;
      serie_total = serie;
      break;
  }
  lcd.clear();
    do{
    int current_joy_read = read_joystick(); // Reading Joystick input.
  
    // If the INPUT sent by the joystick is different from the last one then the last INPUT is saved. 
    if (current_joy_read != joystick_last_read_dc) {
      joystick_last_read_dc = current_joy_read;
  
      // Depending on the INPUT sent by the JOYSTICK.
      switch (current_joy_read) {
        case up: // If the Joystick goes up : 
          poids++; // The weight is incremented by 1.
          break;
          
        case down: // If the Joystick goes down : 
          if(poids > 20 ){poids--;} // If the weight is greater than 20, then the weight is decremented by 1.
          break;
          
        case left: // If the Joystick goes to the left : 
          break;
          
        case enter: // If the Joystick is pressed : 
        
          lcd.clear(); // We clean the LCD screen.
          
          // If the exercise is in progress : 
          if(en_cours == true){ 
            Serial.println("PAUSE EXERCICE");
            en_cours = false; // We pause the exercise.
            pause = true; // We start the pause.
            chrono_pause = chronometre(); // A stopwatch associated with the pause is started.
            Serial.println("\n");
            
          // Sif the exercise is NOT in progress :   
          }else{ 
            // If the pause is in progress : 
            if(pause == true){ 
              Serial.println("REPRISE EXERCICE");
              pause = false; // We end the pause.
              en_cours = true; // Exercise resume.
              
          
            // If the break is not in progress AND the weight is over 20 :  
            }else if(poids > 20){ 
                poids_start = true; // Session starts
                en_cours = true; // Exercise is in progress
            }
          }
          break;
  
          
        default: 
          break;
      }   
    } 
   
    // Timer starts
     // If weight selected
     if(poids_start == true){ 
      // If lcd is unclear
      if(clear_lcd == false){ 
        chrono = chronometre(); // Exercise timer starts
        lcd.clear(); // lcd is clear.
        clear_lcd = true; // if clear_lcd is true it means lcd is clean.
        
      // If screen has been cleaned.
      }else{ 

        // If pause
        if(pause == true){ 
          affichage_ecran(0,0,"||"); // Logo of "pause" is printed.
          affichage_ecran(0,4,_getTime(chrono_pause)); // Print pause timer.

        // If Exercise :
        }else{  
          affichage_ecran(0,0,"TPS"); // "TPS" is printed on screen.
          affichage_ecran(0,4,_getTime(chrono)); // Time is printed meaning the whole duration of the exercise..
          affichage_ecran(1,0,"REP:");
          affichage_ecran(1,8,"SERIE:");
          affichage_ecran(1,14,String(serie));
          affichage_ecran(1,5,String(rep_total));
          
          data = _getData(); // getting data of the exercise.
        
          // If data : 
          if(data == 1){
            rep_total--; // We decrement repetitions by 1.
            Serial.println("Répétition : " + String(rep_total));
          }
          
          if(rep_total == 9){effacer_ecran_precis(6,1);}
          
          // If we end all the 10 repetitions ; Series are decremented by one.
          if(rep_total == 0){
            rep_total = rep;
            serie --;
          }
          
          // If series are end : 
          if(serie == 0){
            exo_fini = true; // Exercise boolean goes to true.
            Serial.println("Fin de l'exercice : ");
            int tab[3];
            tab[0] = poids;
            tab[1] = serie_total;
            tab[2] = rep;
            Serial.print("\t Poids : ");Serial.print(tab[0]);Serial.print("\n");
            Serial.print("\t Série : ");Serial.print(tab[1]);Serial.print("\n");
            Serial.print("\t Rép : ");Serial.print(tab[2]);Serial.print("\n");
            send_data(tab);
       
          }
          
        }
        affichage_ecran(0,10,"W:"+String(poids));
      }

      
     // If weight isn't selected.
     }else{  
      affichage_ecran(0,0,"Chose ur Weight"); // We ask user to chose the weight
      affichage_ecran(1,6,"W:"+String(poids)); // Weight is printed.
     }

  }while(exo_fini == false);
}
