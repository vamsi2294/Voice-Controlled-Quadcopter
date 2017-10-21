/*=====================================Libraries==================================*/
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
#include <RH_ASK.h>
#include <SPI.h>        // Not actually used but needed to compile
RH_ASK driver;
VR myVR(2,3);           // 2:RX 3:TX, you can choose your favourite pins.
/*==============================Declaration of Vartiables=========================*/
char msg[3];
const int led_pin = 10;
const int le = 11;
const int transmit_pin = 12;
uint8_t records[7]; 
uint8_t buf[64];
int led = 13;
byte count = 1;
#define fly    (0)
#define st     (1) 
#define down   (2)
#define ll     (3)
#define ff     (4) 
#define bb     (5)
#define rr     (6)
/*===============Function for printing the Transmitted data Signature==============*/
void printSignature(uint8_t *buf, int len)
{
    int i;
    for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}
/*====================== Function for printing the Transmitted data ==================*/
void printVR(uint8_t *buf)
{
      Serial.println("VR Index\tGroup\tRecordNum\tSignature");
      Serial.print(buf[2], DEC);
      Serial.print("\t\t");
  if(buf[0] == 0xFF){
      Serial.print("NONE");
  }
  else if(buf[0]&0x80){
      Serial.print("UG ");
      Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
      Serial.print("SG ");
      Serial.print(buf[0], DEC);
  }
      Serial.print("\t");
      Serial.print(buf[1], DEC);
      Serial.print("\t\t");
  if(buf[3]>0){
      printSignature(buf+4, buf[3]);
     }
  else{
      Serial.print("NONE");
  }
      Serial.println("\r\n");
}
void setup()
{
      myVR.begin(9600);     // Baud rate of Voice module  
      Serial.begin(115200); // Baud rate for pc
/*=================================Transmission Setup============================*/
  if (!driver.init()){
      Serial.println("init failed");
}
      Serial.print("setup");// Bits per sec
      pinMode(led_pin, OUTPUT);
      Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
      pinMode(led, OUTPUT);
      pinMode(le,OUTPUT);
  if(myVR.clear() == 0){
      Serial.println("Recognizer cleared.");
  }else{
      Serial.println("Not find VoiceRecognitionModule.");
      Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  if(myVR.load((uint8_t)fly) >= 0){
      Serial.println("fly loaded");
  }
  if(myVR.load((uint8_t)st) >= 0){
      Serial.println("stop loaded");
  }
  if(myVR.load((uint8_t)down) >= 0){
      Serial.println("down loaded");
  }
  if(myVR.load((uint8_t)ll) >= 0){
      Serial.println("front loaded");
  }
  if(myVR.load((uint8_t)ff) >= 0){
      Serial.println("back loaded");
  }
  if(myVR.load((uint8_t)bb) >= 0){
      Serial.println("right loaded");
  }
  if(myVR.load((uint8_t)rr) >= 0){
      Serial.println("right loaded");
  }
}
void loop()
{
/*==================================Main Code here =============================*/
     int ret;
     ret = myVR.recognize(buf, 50);
     if(ret>0){
  switch(buf[1]){
      case 0:   msg[1] = {'U'};      
      break;
      case 1:   msg[1] = {'D'};
      break;
      case 2:   msg[1] = {'S'};
      break;
      case 3:   msg[1] = {'L'};
      break;
      case 4:   msg[1] = {'F'};
      break;
      case 5:   msg[1] = {'B'};
      break;
      case 6:   msg[1] = {'R'};
      break;
      default:  msg[1] = {'I'};
      break;      
    }  
      printVR(buf);
    }
      msg[2] = count;
      digitalWrite(led_pin, HIGH);        // Flash a light to show transmitting
      driver.send((uint8_t *)msg, 2);     // Transmitting the message
      driver.waitPacketSent();            // Wait until the whole message is gone
      digitalWrite(led_pin, LOW);
      Serial.print(msg[1]);
      Serial.print("\n");
      count = count + 1;
}
