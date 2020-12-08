#include <Arduino.h>
#include "DHT.h"

#define rowSize 3
int rowPins[] = {5, 4, 14};

#define collumSize 3
int columnPins[] = {12, 15, 10};

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float temperature;
float humidity;
float heatIndex;

void setup() {

  Serial.begin(115200);

  for(int i = 0; i < rowSize; i++){
    pinMode(rowPins[i], OUTPUT);
  }
  for(int i = 0; i < collumSize; i++){
    pinMode(columnPins[i], OUTPUT);
  }

  dht.begin();
}

void setAllLEDs(bool on){

  for(int i = 0; i < rowSize; i++){
    digitalWrite(rowPins[i], on); 
  }
  for(int i = 0; i < collumSize; i++){
    digitalWrite(columnPins[i], !on);
  } 
}

void setLED(int r, int c, bool on){
  digitalWrite(r, on);
  digitalWrite(c, !on);
}

void readDHT(){
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  heatIndex = dht.computeHeatIndex();
}

void printDHT(){
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C | Humidity: ");
  Serial.print(humidity);
  Serial.print("% | Index: ");
  Serial.print(heatIndex);
  Serial.println("°C");

}

void loop() {
  setAllLEDs (0);
  
  for(int i = 0; i < rowSize; i++){

    for(int j = 0; j < collumSize; j++){

      setLED(rowPins[i], columnPins[j], 1);
      delay(1000);
      setLED(rowPins[i], columnPins[j], 0);
      delay(1000);

      readDHT();
      printDHT();

    }
  }
  
}
