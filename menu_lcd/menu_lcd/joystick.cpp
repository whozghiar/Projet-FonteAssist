#include "affichage.h"
#include "joystick.h"

int read_joystick() {
  int output = none;
  
  // Data send by joystick sensor.
  int X_Axis = analogRead(joystick_x_pin);     // Axe_X
  int Y_Axis = analogRead(joystick_y_pin);     // Axe_Y
  Y_Axis = map(Y_Axis, 0, 1023, 1023, 0);      // invert the input from the y axis so that pressing the stick forward gives larger values
  int SwitchValue = digitalRead(joystick_switch_pin);  // read the state of the switch
  SwitchValue = map(SwitchValue, 0, 1, 1, 0);  // invert the input from the switch to be high when pressed
  
  if (SwitchValue == 1){
    output = enter;
  } else if (X_Axis >= 900) {
    output = right;
  } else if (X_Axis <= 100) {
    output = left;
  } else if (Y_Axis >= 900) {
    output = up;
  } else if (Y_Axis <= 100) {
    output = down;
  }
  return output;
}
