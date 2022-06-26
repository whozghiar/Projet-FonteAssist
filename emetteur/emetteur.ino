/**
 * 
 * Programme Arduino Fonte-assist
 * by Roi Théo & John
 * @Copyright 2022
*/

#include <Wire.h> // Librairie pour la communication I2C.
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SoftwareSerial.h>
#include <TheThingsNetwork.h>


// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "1AA8511C456060AE37118D7F366C3E72";


#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

/* Communication RX / TX  */
SoftwareSerial mySerial(9, 10); // RX, TX

int comptage = 0;
int buzzer = 7; // BUZZER.
int LEDred = 6; // LED rouge.
int memoire = LOW;
boolean rollFTrue = false;

//boolean de check for rep

boolean check1 = false;
boolean check2 = false;

unsigned int NBREP = 0;
int repetitions = 0;

// Acceléromètre.
int ADXL345 = 0x53; // Adresse I2C du Capteur ADXL345
float X_out, Y_out, Z_out;  // Outputs
float roll,pitch,rollF,pitchF=0;

float acceleration_max = 0,acceleration_min = 100;
float valeurRep = 15;


void setup() {
  Serial.begin(9600); // Initialisation de la communication série.
  mySerial.begin(57600);
  
  pinMode (LEDred, OUTPUT);
  pinMode(buzzer, OUTPUT);
 
  
  if(!accel.begin()){
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
  accel.setRange(ADXL345_RANGE_16_G);
  
  Wire.begin(); // Initialisation librairie Wire.
  // Mettre ADXL345 en mode mesure.
  Wire.beginTransmission(ADXL345); // Début de la transmission avec l'appareil.
  Wire.write(0x2D); 
  // Activation Mesure
  Wire.write(8); // Bit D3 High for measuring enable (8dec -> 0000 1000 binary)
  Wire.endTransmission();
  delay(10);
  
  //off-set Calibrage
  //X-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1E);
  Wire.write(1);
  Wire.endTransmission();
  delay(10);
  
  //Y-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1F);
  Wire.write(-2);
  Wire.endTransmission();
  delay(10);

  //Z-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x20);
  Wire.write(-9);
  Wire.endTransmission();
  delay(10);

  loraSerial.begin(57600);
  debugSerial.begin(9600);
  
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);
  
  debugSerial.println("-- STATUS");
  ttn.showStatus();
 
  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
}

void loop() {
  int signalDataReception;
  int data[3];
  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);

  send_data(event.acceleration.y); // Envoie de l'accélération.
  
    // === Read acceleromter data === //
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  X_out = ( Wire.read() | Wire.read() << 8); // X-axis value
  X_out = X_out / 256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  Y_out = ( Wire.read() | Wire.read() << 8); // Y-axis value
  Y_out = Y_out / 256;
  Z_out = ( Wire.read() | Wire.read() << 8); // Z-axis value
  Z_out = Z_out / 256;
  
  // Calculate Roll and Pitch (rotation around X-axis, rotation around Y-axis)
  roll = atan(Y_out / sqrt(pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI;
  pitch = atan(-1 * X_out / sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;

  // Low-pass filter
  rollF = 0.94 * rollF + 0.06 * roll;
  pitchF = 0.94 * pitchF + 0.06 * pitch;

  if(mySerial.available() > 0){
    signalDataReception = recevInt(data);
  }
  if(signalDataReception != 0){
    byte payload[3];
    payload[0] = data[0];
    Serial.print("Payload 0 : ");Serial.print(payload[0]);Serial.print("\n");
    payload[1] = data[1];
    Serial.print("Payload 1 : ");Serial.print(payload[1]);Serial.print("\n");
    payload[2] = data[2];
    Serial.print("Payload 2 : ");Serial.print(payload[2]);Serial.print("\n");
  
    // Send it off
    ttn.sendBytes(payload, sizeof(payload));
  }

  
  //delay(300);
  //if (rollF >= 80) rollFTrue = true;
  //if (rollFTrue )  buzzerBipSecu(rollF,pitchF);
}


/*
void buzzerBipSecu(float rollF, float pitchF){
  Serial.print("Rollf : ");Serial.print(rollF);
  if(rollF < 55){
     tone(buzzer,1500,500); //turn on the buzzer at 1535 frequency for 500 milliseconds
     digitalWrite(LEDred,HIGH);//HYPER RAPIDE BUG
     delay (500);
     tone(buzzer,1900,500);
     digitalWrite (LEDred, LOW);
     delay (500);
   //  delay(1000);     //add another 500 milliseconds of silence
  }
}
*/

int send_data(float acceleration){

    
    if(acceleration > acceleration_max){
      acceleration_max = acceleration;
      /*
      Serial.print("Max : ");
      Serial.println(acceleration_max);
      */
      delay(100);
    }
    if (acceleration < acceleration_min){
      acceleration_min = acceleration;
      /*
      Serial.print("Min : ");
      Serial.println(acceleration_min);
      */
      delay(100);
    }
    //Serial.print("Acceleration Difference : ");
    //Serial.println(acceleration_max - acceleration_min);
    if (acceleration_max - acceleration_min > 15 && acceleration_max - acceleration_min < 20 ) {
      //Serial.print("Acceleration : ");Serial.print(acceleration);Serial.print("\n");
      Serial.println(" REPETITION ");
      mySerial.write((byte)1);
      acceleration_max = 0;
      acceleration_min = 100;

      /*
      byte payload[4];
      payload[0] = acceleration_max - acceleration_min;
      payload[1] = 0x12;
      payload[2] = 0x12;
      payload[3] = 0x12;

      ttn.sendBytes(payload, sizeof(payload));
      */
      delay(200);
    }

    if(acceleration_max - acceleration_min > 20){
      acceleration_max = 0;
      acceleration_min = 100;
    }
    

    

 /*
      
    Serial.print("Acceleration : ");Serial.print(acceleration);Serial.print("\n");
    mySerial.write((byte)1);
    /*
    // Prepare payload of 1 byte to indicate LED status
    byte payload[4];
    payload[0] = acceleration;
    payload[1] = 0x12;
    payload[2] = 0x12;
    payload[3] = 0x12;
  
    // Send it off
    ttn.sendBytes(payload, sizeof(payload));
    */
  //return;
}



int recevInt(int tab[]){
  if(mySerial.available() == 3){
    tab[0] = mySerial.read();
    tab[1] = mySerial.read();
    tab[2] = mySerial.read();
    Serial.println("ARDUINO TTN -> : Paquet Reçu");
    Serial.print("\tPoids : ");Serial.print(tab[0]);Serial.print("\n");
    Serial.print("\tSérie : ");Serial.print(tab[1]);Serial.print("\n");
    Serial.print("\tRep : ");Serial.print(tab[2]);Serial.print("\n");
    
    return 1;
  }else{
    return 0;
  }
}
