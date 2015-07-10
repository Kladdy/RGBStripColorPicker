#include "FastSPI_LED2.h"

#define NUM_LEDS 6 
//Enter the number of LED's on the strip here.

struct CRGB { byte g; byte r; byte b; };
struct CRGB leds[NUM_LEDS];

WS2811Controller800Mhz<12> LED; 
//Enter the pin that the strip is connected to.

char firstChar;
char dataChar [9];
int valRed = 0;
int valBlue = 0;
int valGreen = 0;
int lightMode = 0; 
//0 = Test, 1: Static, 2: Rainbow, 3: Cycle, 4: Fade

int time = 10;
int roundNum1 = 1;
int nextRound = 1;
int fadeMode;

void setup() {
  
  LED.init();
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  randomSeed(analogRead(A0));
  
}

CRGB rgb;

void loop() {
  
  while(Serial.available() >= 2){
    firstChar = Serial.read();
    delay(30);
    if(firstChar == 'p'){
      for(int i=0; i<3; i++){
          dataChar[i] = Serial.read();
      }
      lightMode = 0;  
      for(int i = 0; i < 20; i++){
        digitalWrite(13, HIGH);
        delay(50);
        digitalWrite(13, LOW);
        delay(50);
      }
      flushReceive();
    }
    else if(firstChar == 'q'){
      for(int i=0; i<9; i++){
          dataChar[i] = Serial.read();
      }
      lightMode = 1;  
      
      flushReceive();
      staticLight();
    }
    else if(firstChar == 'w'){
      for(int i=0; i<2; i++){
          dataChar[i] = Serial.read();
      }
      lightMode = 2;  
      
      flushReceive();
    }
    else if(firstChar == 'e'){
      for(int i=0; i<1; i++){
          dataChar[i] = Serial.read();
      }
      lightMode = 3; 
      
      flushReceive();
    }
    else if(firstChar == 'r'){
      for(int i=0; i<2; i++){
          dataChar[i] = Serial.read();
      }
      lightMode = 4; 
      if(dataChar[1] == '0'){
      time = 20;
      }else if(dataChar[1] == '1'){
      time = 10;
      }else if(dataChar[1] == '2'){
      time = 5;
      }
      if(dataChar[0] == '0'){
      fadeMode = 0;
      }else if(dataChar[0] == '1'){
      fadeMode = 1;
      }
      flushReceive();
      valRed = 0;
      valGreen = 0;
      valBlue = 0;
    }
  }
  if(lightMode == 2){
    rainbowLight();
  }else if(lightMode == 3){
    cycleLight();
  }else if(lightMode == 4){
    delay(time);
    fadeLight();
  }
}

void flushReceive(){
  while(Serial.available())
    Serial.read(); 
}
void staticLight(){
  valRed = 0;
  valGreen = 0;
  valBlue = 0;
  
  valRed += (dataChar[0] - '0') * 100;
  valRed += (dataChar[1] - '0') * 10;
  valRed += dataChar[2] - '0';
  
  valGreen += (dataChar[3] - '0') * 100;
  valGreen += (dataChar[4] - '0') * 10;
  valGreen += dataChar[5] - '0';
  
  valBlue += (dataChar[6] - '0') * 100;
  valBlue += (dataChar[7] - '0') * 10;
  valBlue += dataChar[8] - '0';
  
  rgb.r = valRed;
  rgb.g = valGreen;
  rgb.b = valBlue;
  
  sendData();
}
void rainbowLight(){
  
  
  sendData();
}
void cycleLight(){
  
  
  sendData();
}
void fadeLight(){
  if (roundNum1 == 0){
    if (valRed == 0 && valGreen == 0 && valBlue == 0){
      if (fadeMode == 1){
      roundNum1 = random(1,7);
      Serial.print(roundNum1);
      }else if (fadeMode == 0){
      if (nextRound == 1){
	roundNum1 = 2;
      }
      else if (nextRound == 2){
	roundNum1 = 3;
      }      
      else if (nextRound == 3){
	roundNum1 = 4;
      }
      else if (nextRound == 4){
	roundNum1 = 5;
      }
      else if (nextRound == 5){
	roundNum1 = 6;
      }
      else if (nextRound == 6){
	roundNum1 = 1;
      }
      }
    }   
    else{
      if (valRed != 0){
	valRed--;
      }
      if (valGreen != 0){
	valGreen--;
      }
      if (valBlue != 0){
	valBlue--;
      }
    }
  }
  else if (roundNum1 == 1){
    if (valRed == 255){
      roundNum1 = 0;
      nextRound = 1;
    }
    else{
      valRed++;
    }
  }
  else if (roundNum1 == 2){
    if (valRed == 255 && valGreen == 255){
       roundNum1 = 0;
       nextRound = 2;
    }
    else{
       valRed++;
       valGreen++;
    }
  }
  else if (roundNum1 == 3){
    if (valGreen == 255){
      roundNum1 = 0;
      nextRound = 3;
    }
    else{
      valGreen++;
    }
  }
  else if (roundNum1 == 4){
    if (valGreen == 255 && valBlue == 255){
      roundNum1 = 0;
      nextRound = 4;
    }
    else{
      valGreen++;
      valBlue++;
    }
  }
  else if (roundNum1 == 5){
    if (valBlue == 255){
      roundNum1 = 0;
      nextRound = 5;
    }
    else{
      valBlue++;
    }
  }
  else if (roundNum1 == 6){
    if (valBlue == 255 && valRed == 255){
      roundNum1 = 0;
      nextRound = 6;
    }
    else{
      valBlue++;
      valRed++;
    }
  }

  rgb.r = valRed;
  rgb.g = valGreen;
  rgb.b = valBlue;
  sendData();
}
void sendData(){
  for(int i=0; i<NUM_LEDS; i+=1) {    
    leds[i]=rgb;
  }
  LED.showRGB((byte*)leds, NUM_LEDS);
}
