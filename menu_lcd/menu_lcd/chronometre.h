#ifndef CHRONOMETRE_H
#define CHRONOMETRE_H
#include <Arduino.h>
#include <math.h>

#include <LiquidCrystal.h>

extern unsigned long TEMPS_EXERCICE;

unsigned long chronometre();
String _getMinute(unsigned long chronometre);
String _getSecond(unsigned long chronometre);
String _getTime(unsigned long chronometre);
String miseEnForme(unsigned long chronometre);
unsigned long addTime(unsigned long chrono1, unsigned long chrono2);
#endif
