#include <Arduino.h>

#define rowSize 3
int row[] = {5, 4, 14};

#define collumSize 3
int column[] = {12, 15, 10};


void setup() {

  Serial.begin(115200);

  for(int i = 0; i < rowSize; i++){
    pinMode(row[i], OUTPUT);
  }
  for(int i = 0; i < collumSize; i++){
    pinMode(column[i], OUTPUT);
  }
}

void setAllLEDs(bool on){

  if(on){

    for(int i = 0; i < rowSize; i++){
      digitalWrite(row[i], HIGH); 
    }
    for(int i = 0; i < collumSize; i++){
      digitalWrite(column[i], LOW);
    } 

  }
  else{

    for(int i = 0; i < rowSize; i++){
      digitalWrite(row[i], LOW); 
    }
    for(int i = 0; i < collumSize; i++){
      digitalWrite(column[i], HIGH);
    } 

  }
}
void setLED(int r, int c, bool on){

  if(on){

    digitalWrite(r, HIGH);
    digitalWrite(c, LOW);

  }
  else{

    digitalWrite(r, LOW);
    digitalWrite(c, HIGH);

  }
}


void loop() {
  for(int i = 0; i < rowSize; i++){

    for(int j = 0; j < collumSize; j++){

      setLED(row[i], column[j], 1);
      delay(1000);
      setLED(row[i], column[j], 0);
      delay(1000);
    }
  }
}