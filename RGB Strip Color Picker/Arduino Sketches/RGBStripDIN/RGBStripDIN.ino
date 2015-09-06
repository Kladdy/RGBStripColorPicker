#include "FastSPI_LED2.h"

#define NUM_LEDS 180 
//Enter the number of LED's on the strip.

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
int colorState = 0; 
//0=goRed 1=goGreen 2=goBlue

int time = 10;    
int roundNum1 = 1;
int nextRound = 1;
int fadeMode;

byte dir;

CRGB rgb;

void setup() {
  
  LED.init();
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  randomSeed(analogRead(A0));
  
  rgb.r = 0;
  rgb.g = 0;
  rgb.b = 0;
  
  sendData();
  
}

void loop() {
  
  while(Serial.available() >= 2){
    firstChar = Serial.read();
    delay(30);
    if(firstChar == 'p'){
      for(int i=0; i<3; i++){
          dataChar[i] = Serial.read();
      }
      flushReceive();
      lightMode = 0; 
      valRed = 0;
      valGreen = 0;
      valBlue = 0; 
      sendData();  
      for(int i = 0; i < 20; i++){
        digitalWrite(13, HIGH);
        delay(50);
        digitalWrite(13, LOW);
        delay(50);
      }
    }
    else if(firstChar == 'q'){
      for(int i=0; i<9; i++){
          dataChar[i] = Serial.read();
      }
      flushReceive();
      lightMode = 1;  
      staticLight();
    }
    else if(firstChar == 'w'){
      for(int i=0; i<2; i++){
          dataChar[i] = Serial.read();
      }
      flushReceive();
      lightMode = 2;  
      if(dataChar[0] == '0'){
        dir = 1;
      }else if(dataChar[0] == '1'){
        dir = 0;
      }
      if(dataChar[1] == '0'){
        time = 30;
      }else if(dataChar[1] == '1'){
        time = 10;
      }else if(dataChar[1] == '2'){
        time = 5;
      }
    }
    else if(firstChar == 'e'){
      for(int i=0; i<1; i++){
          dataChar[i] = Serial.read();
      }
      flushReceive();
      lightMode = 3; 
      if(dataChar[0] == '0'){
        time = 40;
      }else if(dataChar[0] == '1'){
        time = 10;
      }else if(dataChar[0] == '2'){
        time = 5;
      }
    }
    else if(firstChar == 'r'){
      for(int i=0; i<2; i++){
          dataChar[i] = Serial.read();
      }
      flushReceive();
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
    }
  }
  if(lightMode == 2){
    rainbowLight();
  }else if(lightMode == 3){
    cycleLight();
  }else if(lightMode == 4){
    fadeLight();
  }
}

void flushReceive(){
  while(Serial.available())
    Serial.read(); 
    
  rgb.r = 0;
  rgb.g = 0;
  rgb.b = 0;
  valRed = 0;
  valGreen = 0;
  valBlue = 0;
  
  roundNum1 = 1;
  nextRound = 1;
  colorState = 0;
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
  int cycles, j, k;
  
  for(cycles=0;cycles<1;cycles++){
    k=255;
    
    for (j=0; j < 256; j++,k--) {
      
      if(k<0)k=255;
      
      for(int i=0; i<NUM_LEDS; i+=1) {
        Wheel(((i * 256 / NUM_LEDS) + (dir==0?j:k)) % 256,4);        
        leds[i]=rgb;
      }
      LED.showRGB((byte*)leds, NUM_LEDS);;
      delay(time);
    }
  }
  
  
}
void cycleLight(){
  if(colorState==0){
        goRed();
      }else if(colorState==1){
        goGreen();
      }else if(colorState==2){
        goBlue();
      }
      for(int i=0; i<NUM_LEDS; i++) {
        leds[i]=rgb;
      }
      LED.showRGB((byte*)leds, NUM_LEDS);;
      delay(time);
  
  sendData();
}
void fadeLight(){
  delay(time);
  if (roundNum1 == 0){
    if (valRed == 0 && valGreen == 0 && valBlue == 0){
      if (fadeMode == 1){
      roundNum1 = random(1,7);
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

void Wheel(byte WheelPos,byte dim){
  
  if (WheelPos < 85) {
   rgb.r=0;
   rgb.g=WheelPos * 3/dim;
   rgb.b=(255 - WheelPos * 3)/dim;;
   return;
  } 
  else if (WheelPos < 170) {
   WheelPos -= 85;
   rgb.r=WheelPos * 3/dim;
   rgb.g=(255 - WheelPos * 3)/dim;
   rgb.b=0;
   return;
  }
  else {
   WheelPos -= 170; 
   rgb.r=(255 - WheelPos * 3)/dim;
   rgb.g=0;
   rgb.b=WheelPos * 3/dim;
   return;
  }
}

void goRed(){
  if(rgb.r != 255){
  rgb.r++;
  return;
  }else if(rgb.r == 255 && rgb.b != 0){
  rgb.b--;
  return;
  }else if(rgb.r == 255 && rgb.b == 0){
  colorState = 1;
  return;
  }
}

void goGreen(){
  if(rgb.g != 255){
  rgb.g++;
  return;
  }else if(rgb.g == 255 && rgb.r != 0){
  rgb.r--;
  return;
  }else if(rgb.g == 255 && rgb.r == 0){
  colorState = 2;
  return;
  }
}

void goBlue(){
  if(rgb.b != 255){
  rgb.b++;
  return;
  }else if(rgb.b == 255 && rgb.g != 0){
  rgb.g--;
  return;
  }else if(rgb.b == 255 && rgb.g == 0){
  colorState = 0;
  return;
  }
}
