#define numberOfBytes 10
char dataChar [numberOfBytes];   // for incoming serial data
int led = 13;
int time;
void setup()
{
  Serial.begin(9600); // // opens serial port, sets data rate to 9600 bps
  
  pinMode(led, OUTPUT);
}

void loop()
{   
    digitalWrite(led, LOW);
    while(Serial.available() >= numberOfBytes){
    if(Serial.read() == 'q'){

        for(int i=0; i<numberOfBytes-1; i++){
          dataChar[i] = Serial.read();
        }    
    }
    
    for(int i = 0; i < numberOfBytes; i++){
      Serial.print(dataChar[i]);
    }
    
    
    if(dataChar [0]=='2' && dataChar[1]=='5' && dataChar[2]=='5'){
      time = 3000;
    }else if(dataChar[0]=='1' && dataChar[1]=='0' && dataChar[2]=='0'){
      time = 500;
    }else if(dataChar [0]=='1' && dataChar[1]=='2' && dataChar[2]=='2'){
      time = 200;
    }else{
      time = 1000;
    }
    
    
    digitalWrite(led, HIGH);
    delay(time);
  }  
}
