const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
//Enter the pins that the strip is connected to.

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

void setup() {

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(9600);
  pinMode(13, OUTPUT);
  randomSeed(analogRead(A0));

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
      lightMode = 3;
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
    cycleLight();
  }else if(lightMode == 3){
    cycleLight();
  }else if(lightMode == 4){
    fadeLight();
  }
}

void flushReceive(){
  while(Serial.available())
    Serial.read(); 
   
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
  
  sendData();
}

void cycleLight(){
  if(colorState==0){
        goRed();
      }else if(colorState==1){
        goGreen();
      }else if(colorState==2){
        goBlue();
      }
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
  sendData();
}

void sendData(){
  analogWrite(redPin, valRed);
  analogWrite(greenPin, valGreen);
  analogWrite(bluePin, valBlue);
}

void goRed(){
  if(valRed != 255){
  valRed++;
  return;
  }else if(valRed == 255 && valBlue != 0){
  valBlue--;
  return;
  }else if(valRed == 255 && valBlue == 0){
  colorState = 1;
  return;
  }
}

void goGreen(){
  if(valGreen != 255){
  valGreen++;
  return;
  }else if(valGreen == 255 && valRed != 0){
  valRed--;
  return;
  }else if(valGreen == 255 && valRed == 0){
  colorState = 2;
  return;
  }
}

void goBlue(){
  if(valBlue != 255){
  valBlue++;
  return;
  }else if(valBlue == 255 && valGreen != 0){
  valGreen--;
  return;
  }else if(valBlue == 255 && valGreen == 0){
  colorState = 0;
  return;
  }
}
