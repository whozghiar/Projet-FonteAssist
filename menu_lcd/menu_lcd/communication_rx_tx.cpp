#include "affichage.h"
#include "communication_rx_tx.h"


SoftwareSerial mySerial(RX,TX);

/**
* Function communication opens the communication canal.
*/
int communication(){
  
  //while (!Serial) {;} //-- Needed for Leonardo
  Serial.println("Waiting for signal !");

  mySerial.begin(57600);
}


/**
*
* Function _getData allows Arduino to read received thanks to RX_TX.
*/
int _getData(){
  
  unsigned int lowerByte = 0;
  int data;
  
  //-- if data exists
  if (mySerial.available() > 1){
      
     lowerByte = mySerial.read();
  }

  data = lowerByte;
  
  return data;
}


/**
* Function send_data allows Arduino to send data through RX_TX.
* @param {int[]} data - Array of data.
*/
void send_data(int data[]){
  mySerial.write(data[0] & 0xFF);
  mySerial.write((data[1]) & 0xFF);
  mySerial.write((data[2])& 0xFF);
}
