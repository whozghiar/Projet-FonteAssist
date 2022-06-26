#include "chronometre.h"

/**
 * @Copyright Théo Lomège & John Padé
 */
 
unsigned long chronometre(){
  return millis();
}

/**
  function _getMinute return the number of minute of the chronomètre.
  @param {unsigned long} chronometre
*/
String _getMinute(unsigned long chronometre){
  String minute;
  minute = chronometre/60000;
  return minute;
}

String _getSecond(unsigned long chronometre){
  String second;
  second = (chronometre/1000)%60;
  return second;
}

String _getTime(unsigned long chronometre){
  String temps;
  unsigned long now = millis();
  unsigned long TEMPS_EXERCICE = (now - chronometre);
  String second = _getSecond(TEMPS_EXERCICE);
  String minute = _getMinute(TEMPS_EXERCICE);
  if(second.length() == 1)temps = minute + ":0" + second;
  else temps = minute + ":" + second;
 
  return temps;
}

unsigned long addTime(unsigned long chrono1, unsigned long chrono2){
  unsigned long now = millis();
  unsigned long tps_chrono1 = (now - chrono1);
  unsigned long tps_chrono2 = (now - chrono2);
  unsigned long tps_total = tps_chrono1 + tps_chrono2;
  return tps_total;
}
