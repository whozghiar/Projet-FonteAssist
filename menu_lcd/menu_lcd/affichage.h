#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include <Arduino.h>
#include <LiquidCrystal.h>

extern LiquidCrystal lcd;

void affichage_ecran(int line, int postition, String text);
void effacer_ecran(int line);
void effacer_ecran_precis(int curseur,int line);
#endif
