#ifndef JOYSTICK_H
#define JOYSTICK_H
#include <Arduino.h>
#include <LiquidCrystal.h>

//Arduino pins attached to joystick 
#define joystick_switch_pin  9
#define joystick_x_pin       A0
#define joystick_y_pin       A1

//Joystick values
#define up    0
#define right 1
#define down  2
#define left  3
#define enter 4
#define none  5

int read_joystick();

#endif
