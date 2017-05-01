#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <MeMegaPi.h>


//Encoder Motor
MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeEncoderOnBoard Encoder_3(SLOT3);
MeEncoderOnBoard Encoder_4(SLOT4);

void isr_process_encoder1(void)
{
      if(digitalRead(Encoder_1.getPortB()) == 0){
            Encoder_1.pulsePosMinus();
      }else{
            Encoder_1.pulsePosPlus();
      }
}

void isr_process_encoder2(void)
{
      if(digitalRead(Encoder_2.getPortB()) == 0){
            Encoder_2.pulsePosMinus();
      }else{
            Encoder_2.pulsePosPlus();
      }
}

void isr_process_encoder3(void)
{
      if(digitalRead(Encoder_3.getPortB()) == 0){
            Encoder_3.pulsePosMinus();
      }else{
            Encoder_3.pulsePosPlus();
      }
}

void isr_process_encoder4(void)
{
      if(digitalRead(Encoder_4.getPortB()) == 0){
            Encoder_4.pulsePosMinus();
      }else{
            Encoder_4.pulsePosPlus();
      }
}

void move(int direction, int speed)
{
      int leftSpeed = 0;
      int rightSpeed = 0;
      if(direction == 1){
            leftSpeed = -speed;
            rightSpeed = speed;
      }else if(direction == 2){
            leftSpeed = speed;
            rightSpeed = -speed;
      }else if(direction == 3){
            leftSpeed = speed;
            rightSpeed = speed;
      }else if(direction == 4){
            leftSpeed = -speed;
            rightSpeed = -speed;
      }
      Encoder_1.setTarPWM(rightSpeed);
      Encoder_2.setTarPWM(leftSpeed);
}
void moveDegrees(int direction,long degrees, int speed_temp)
{
      speed_temp = abs(speed_temp);
      if(direction == 1)
      {
            Encoder_1.move(degrees,(float)speed_temp);
            Encoder_2.move(-degrees,(float)speed_temp);
      }
      else if(direction == 2)
      {
            Encoder_1.move(-degrees,(float)speed_temp);
            Encoder_2.move(degrees,(float)speed_temp);
      }
      else if(direction == 3)
      {
            Encoder_1.move(degrees,(float)speed_temp);
            Encoder_2.move(degrees,(float)speed_temp);
      }
      else if(direction == 4)
      {
            Encoder_1.move(-degrees,(float)speed_temp);
            Encoder_2.move(-degrees,(float)speed_temp);
      }
    
}

double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
MeStepperOnBoard stepper_1(1);
MeStepperOnBoard stepper_2(2);
MeStepperOnBoard stepper_3(3);
MeStepperOnBoard stepper_4(4);

MeSoundSensor soundsensor_7(7);
MeUltrasonicSensor ultrasonic_6(6);
MeRGBLed rgbled_8(8, 4);
char data = 0;
MeBluetooth bluetooth(5);




void setup(){
    //Set Pwm 8KHz
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(WGM12);
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS21);
    
    stepper_1.setMicroStep(500);
    stepper_1.enableOutputs();
    stepper_2.setMicroStep(500);
    stepper_2.enableOutputs();
    stepper_3.setMicroStep(500);
    stepper_3.enableOutputs();
    stepper_4.setMicroStep(500);
    stepper_4.enableOutputs();

    Serial.begin(115200);
    bluetooth.begin(115200);
    Serial.println("Bluetooth Start!!");
    
}

void loop(){
    if((ultrasonic_6.distanceCm()) < (15) || soundsensor_7.strength() >= 600){
        changeSpeed(0,0);
        rgbled_8.setColor(0,255,0,0);
        rgbled_8.show();
        Serial.println("Stopped due to collision or screaming!");
        data = 5;
      } 
       

    if(bluetooth.available()){
      data = bluetooth.read();
      Serial.println("Reading from BT");
    }
    if(data == '1'){ // forward direction
            changeSpeed(200, 100);
            rgbled_8.setColor(0,0,255,0);
        rgbled_8.show();
          
        Serial.println(data);
      } 
      
      else if(data == '0'){ // stop
        changeSpeed(0,0);
        rgbled_8.setColor(0,0,0,255);
        rgbled_8.show();
        Serial.println(data);
      }
      
      else if(data == '2'){ // reverse
            changeSpeed(-200,-1);
            rgbled_8.setColor(0,0,255,0);
        rgbled_8.show();
             Serial.println(data);
          }
        
  
      
      
      else if(data == '3'){ // turn left
        stepper_1.move(-100);
        stepper_1.setMaxSpeed(-200);
        stepper_1.setSpeed(-200);
        stepper_2.move(-100);
        stepper_2.setMaxSpeed(-200);
        stepper_2.setSpeed(-200);
        stepper_3.move(-100);
        stepper_3.setMaxSpeed(-200);
        stepper_3.setSpeed(-200);
        stepper_4.move(-100);
        stepper_4.setMaxSpeed(-200);
        stepper_4.setSpeed(-200);
        rgbled_8.setColor(0,0,255,0);
        rgbled_8.show();
        
        Serial.println(data);
      } 
      
      else if(data == '4'){ // turn right
        stepper_1.move(100);
        stepper_1.setMaxSpeed(200);
        stepper_1.setSpeed(200);
        stepper_2.move(100);
        stepper_2.setMaxSpeed(200);
        stepper_2.setSpeed(200);
        stepper_3.move(100);
        stepper_3.setMaxSpeed(200);
        stepper_3.setSpeed(200);
        stepper_4.move(100);
        stepper_4.setMaxSpeed(200);
        stepper_4.setSpeed(200);
        rgbled_8.setColor(0,0,255,0);
        rgbled_8.show();
        Serial.println(data);
      } 
      
    
    
    _loop();
      
      
}

void changeSpeed(int speeds, int distance){
    stepper_1.move(distance);
    stepper_1.setMaxSpeed(speeds);
    stepper_1.setSpeed(speeds);
    stepper_2.move(distance*(-1));
    stepper_2.setMaxSpeed(speeds*(-1));
    stepper_2.setSpeed(speeds*(-1));
    stepper_3.move(distance);
    stepper_3.setMaxSpeed(speeds);
    stepper_3.setSpeed(speeds);
    stepper_4.move(distance* (-1));
    stepper_4.setMaxSpeed(speeds*(-1));
    stepper_4.setSpeed(speeds*(-1));
    
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
    stepper_1.runSpeedToPosition();
    
    stepper_2.runSpeedToPosition();
    
    stepper_3.runSpeedToPosition();
    
    stepper_4.runSpeedToPosition();
    
    
}
