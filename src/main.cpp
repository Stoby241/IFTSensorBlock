#include <Arduino.h>
#include "DHT.h"

#define MATRIX_SIZE_X 3
int matrixPinsX[] = {5, 4, 14}; // High to be on

#define MATRIX_SIZE_Y 3
int matrixPinsY[] = {12, 15, 10}; // Low to be on

#define MATRIX_DELAY 1
bool matrix[MATRIX_SIZE_X][MATRIX_SIZE_Y];

#define DHT_PIN 2
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);
float temperature;
float humidity;
float heatIndex;

void setUpLEDMatrix(){

  for(int i = 0; i < MATRIX_SIZE_X; i++){
    pinMode(matrixPinsX[i], OUTPUT);
    digitalWrite(matrixPinsX[i], 0);
  }

  for(int i = 0; i < MATRIX_SIZE_Y; i++){
    pinMode(matrixPinsY[i], OUTPUT);
    digitalWrite(matrixPinsY[i], 0);
  }

  for(int i = 0; i < MATRIX_SIZE_X; i++){
    for(int j = 0; j < MATRIX_SIZE_Y; j++){
      matrix[i][j] = 0;
    }
  }

  matrix[0][0] = 1;
  matrix[1][1] = 1;
  matrix[2][2] = 1;
}

void setup() {
  Serial.begin(115200);

  setUpLEDMatrix();

  dht.begin();
}


void showMatrix(){
  for(int i = 0; i < MATRIX_SIZE_X; i++){

    for(int j = 0; j < MATRIX_SIZE_Y; j++){
      digitalWrite(matrixPinsY[j], !matrix[i][j]);
    }

    digitalWrite(matrixPinsX[i], 1);
    delay(MATRIX_DELAY);
    digitalWrite(matrixPinsX[i], 0);
  }
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
  showMatrix();
}
