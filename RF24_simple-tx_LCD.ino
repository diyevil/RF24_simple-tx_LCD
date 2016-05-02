
/*
* Getting Started example sketch for nRF24L01+ radios
* This is an example of how to send data from one node to another using data structures
* Based off of code by TMRh20
*/

#include <SoftwareSerial.h>
#include <serLCD.h>
#include <SPI.h>
#include "RF24.h"

serLCD lcd(2);
int c;

byte addresses[][6] = {"1Node","2Node"};


/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;
// Used to control whether this node is sending or receiving
bool role = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

void setup() {

  Serial.begin(115200);  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  radio.startListening();
  
}

void loop() {
/****************** Ping Out Role ***************************/  
  if (role == 1)  {
    radio.stopListening(); 
    if ( Serial.available() >= 1){ 
       c = Serial.read();  
       if (!radio.write( &c, sizeof(c) )){
         Serial.println("failed");
       }
    }
  }

/****************** Pong Back Role ***************************/
  if ( role == 0 ){
    if( radio.available()){
      while (radio.available()) {                     
        radio.read( &c, sizeof(c) );
        char m = c;
        lcd.print(m);
      }
    }
  }
} 
