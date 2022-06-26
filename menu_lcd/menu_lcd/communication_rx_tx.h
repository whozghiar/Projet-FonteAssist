#ifndef COMMUNICATION_RX_TX_H
#define COMMUNICATION_RX_TX_H
#include <Arduino.h>
//-- Déclaration du port Série Logiciel
#include <SoftwareSerial.h>

extern SoftwareSerial mySerial;

#define RX 10
#define TX 11

int communication();
int _getData();
void send_data(int data[] );
#endif
