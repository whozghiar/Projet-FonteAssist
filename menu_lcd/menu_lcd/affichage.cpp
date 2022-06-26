#include "affichage.h"

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 6, en = 7, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/**
 * Function affichage_ecran allow user to print on the lcd.
 * 
 * @param {int} line - line where you want to write (0 | 1).
 * @param {int} position - between 0 & 15.
 * @param {String} text - the text to print
 * 
 * @return
 */
void affichage_ecran(int line, int postition, String text){
  lcd.setCursor(postition, line);
  lcd.print(text);
  
}

/**
 * Function effacer_ecran allow user to erase what's currently printed on a line  * of the lcd.
 * 
 * @param {int} line - the line you want to erase.
 * 
 * @return
 */ 
void effacer_ecran(int line){
  lcd.setCursor(0, line);
  lcd.print("                            ");
}

/**
 * Function effacer_ecran_precis allow user to erase what's currently printed on  * a line of the lcd.
 * 
 * @param {int} curseur - position you want to erase.
 * @param {int} line - line where you want to erase
 * 
 * @return
 */
void effacer_ecran_precis(int curseur,int line){
  lcd.setCursor(curseur,line);
  lcd.print(" ");  
}
