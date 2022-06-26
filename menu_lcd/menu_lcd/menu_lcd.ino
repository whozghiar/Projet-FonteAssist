// include the library code:
#include <LiquidCrystal.h>
#include "dev_coucher.h"
#include "affichage.h"
#include "joystick.h"
#include "communication_rx_tx.h"

// LoRa DATA
int TPS_PAUSE;

// Menus
String main_menu[] = {"1. D.C","2. S.D.T","3. Squat"};
String sous_menu[] = {"1. Masse","2. Force","3. Cardio"};

// Current menu and item to be displayed
int current_menu_item;
int current_sous_menu_item;
String *current_menu;
String *current_sous_menu;

// Used to check joystick state
int last_joy_read;

void print_line(int line, int postition, String text) {
    lcd.setCursor(postition, line);
    lcd.print("               ");
    lcd.setCursor(postition, line);
    lcd.print(text);
}

void move_up(){
  if (current_menu_item <= 0){
    current_menu_item = sizeof(current_menu);
  } else {
    current_menu_item--;
  }  
}

void move_down(){
  if (current_menu_item >= sizeof(current_menu)){
    current_menu_item = 0;
  } else {
    current_menu_item++;
  }
}

void move_right(){
  if (current_sous_menu_item >= sizeof(current_sous_menu)){
    current_sous_menu_item = 0;
  } else {
    current_sous_menu_item++;
  }  
}
void move_left(){
  if (current_sous_menu_item <= 0){
    current_sous_menu_item = sizeof(current_sous_menu);
  } else {
    current_sous_menu_item--;
  }  
}



void start_exercise(int exo, int mode){
  switch(exo){
    case 0:
      start_dc(mode);
      break;
      
  /*  
  case(1,2):
      break;
      
    case(1,3):
      break;
      
    case(2,1):
      break;
      
    case(2,2):
      break;
      
     case(2,3):
      break;
      
    case(3,1):
      break;
      
    case(3,2):
      break;
      
    case(3,3):
      break;*/
  }
}

void setup() {
  // arduino default once time function
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // set up joy pins
  pinMode(joystick_switch_pin, INPUT_PULLUP);
  
  // Print template on lcd.
  lcd.setCursor(0, 0);
  lcd.print("Choix de l'exo:");
  lcd.setCursor(0, 1);
  lcd.print(main_menu[current_menu_item]);
  lcd.print(sous_menu[current_sous_menu_item]); 

  // Init vars
  current_menu_item = 0;
  current_sous_menu_item = 0;
  last_joy_read = none;
  
  current_menu = main_menu;
  current_sous_menu = sous_menu;
  communication();
}

void loop() {
  


     
  // "main" arduino program
  int current_joy_read = read_joystick();

  if (current_joy_read != last_joy_read) {
    last_joy_read = current_joy_read;
    
    switch (current_joy_read) {
      case up:
        move_up();
        break;
      case down:
        move_down();
        break;
      case right:
        move_right();
        break;
      case left:
        move_left();
        break;
      case enter:
        start_exercise(current_menu_item,current_sous_menu_item);
        effacer_ecran(0);
        print_line(0,0,"Choix de l'exo");
        break;
      default: break;
    }
    
    // clear line and print values to lcd
    print_line(1,0, current_menu[current_menu_item]);
    print_line(1,8, current_sous_menu[current_sous_menu_item]);
    delay(100);
    
  } 
}
