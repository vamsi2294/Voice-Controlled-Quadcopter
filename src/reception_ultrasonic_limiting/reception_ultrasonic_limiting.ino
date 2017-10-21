/*==============================Libraries==================================*/
#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#include <Servo.h>
/*==============================Declaration of Vartiables==================*/
RH_ASK driver;
double duration, d; 
 Servo Throttle;
 Servo Rudder;
 Servo Aileron;
 Servo Elevator;
 Servo Auxiliary;
 int m[5];
 const int receive_pin = 11;
 
void setup()
{
    Serial.begin(115200);  // Debugging only
    Serial.print("setup");
/*================= Attaching the motor pins to the arduino =================*/
    Aileron.attach(5);        //Attach pin 5 
    Elevator.attach(6);       //Attach pin 6
    Throttle.attach(7);       //Attach pin 7
    Rudder.attach(8);         //Attach pin 8
    Auxiliary.attach(9);      //Attach pin 9
    pinMode(22, OUTPUT);      // Trigger pin as Output
    pinMode(24, INPUT);       // Echo pin as input
/*======================== Setup for Reception================================*/
  if (!driver.init())
    Serial.println("init failed");
    pinMode(11,INPUT);              // Receiver pin
/*========================Arming the kk 2 board===============================*/     
    Throttle.writeMicroseconds(2400);  
    Rudder.writeMicroseconds(2000);    
    Aileron.writeMicroseconds(1000);   
    Elevator.writeMicroseconds(1000);
    delay(4000);
    Throttle.writeMicroseconds(1000);  
    Rudder.writeMicroseconds(1000);    
    Aileron.writeMicroseconds(1000);   
    Elevator.writeMicroseconds(1000);
    delay(4000);          // Delay for Calibration
/*=======================Initiializing Motor Speeds to Zero ===================*/
    m[0]=700;
    m[1]=700;
    m[2]=700;
    m[3]=700;
}

void loop()
{
            if(m[0]>1500){
              m[0]=1500; }
            if(m[1]>1500){
              m[1]=1500; }
            if(m[2]>1500){
              m[2]=1500; }
            if(m[3]>1500){
              m[3]=1500; }
            if(m[4]>1500){
              m[4]=1500; }            
    digitalWrite(22, LOW);            //Sending a pulse with 10 microseconds pulse width
    delayMicroseconds(2);
    digitalWrite(22, HIGH);
    delayMicroseconds(10);  
    digitalWrite(22, LOW);
    int duration = pulseIn(24, HIGH);    //Calculating the distance (in cm) based on the speed of sound.
    int d = duration/58.2;
    delay(50);    
    Serial.print("Distance to the obsctacle:\t");
    Serial.print(d);
    Serial.print("\n");//Delay to receive the Echo from the sound wave
/*================================Code for Reception ===========================*/
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
     if (driver.recv(buf, &buflen)) {
    int i;
    Serial.print("Got: ");
     }
    uint8_t msg=buf[1];
/*=================================Main Code here================================*/    
     if(d<140){
          msg = 'S';
          Serial.print("\n");
          Serial.print("Obstacle distance is:\t");
          Serial.print(d);
          Serial.print("in cm \t");
          Serial.print("motor speed \t");
          Serial.print(m[0]);
          }
     switch (msg){
      case 'U':
      Serial.print("up\n");
      
      Throttle.writeMicroseconds(1500);
      break;
      case 'D':    
      Serial.print("Down\n");
        if((m[0]>1100)){
      m[0] = m[0]-1 ;}
      Throttle.writeMicroseconds(m[0]);
      break;
      case 'S':
      Serial.print("\t Stop\n");
       if((m[0]>=700)){
      m[0] = m[0]-10 ;}
      Throttle.writeMicroseconds(m[0]);
      break;
      case 'L':
      Serial.print("left\n");
       if((m[1]<=1500)){
      m[1] = m[1]+1 ;}
      Throttle.writeMicroseconds(m[0]);    
      Aileron.writeMicroseconds(m[1]);
      break;
      case 'R':
      Serial.print("right\n");
        if((m[2]>=1000)){
      m[2] = m[2]-1 ;}
      Throttle.writeMicroseconds(m[0]);     
      Aileron.writeMicroseconds(m[2]);
      break;
      case 'B':
      Serial.print("back\n");
       if((m[3]>=1000)){
      m[3] = m[3]-1 ;}
      Throttle.writeMicroseconds(m[0]);
      Elevator.writeMicroseconds(m[3]);
      break;
      case 'F':
      Serial.print("Front\n");
        if((m[4]<=1500)){
      m[4] = m[4]+1 ;}
      Throttle.writeMicroseconds(m[0]);
      Elevator.writeMicroseconds(m[4]);
      break;
      default:Serial.print("default\n");    
      Throttle.writeMicroseconds(700);
      break;
}   }  
