#include <Arduino.h>
#include "DHT.h"
#include "Adafruit_CCS811.h"

// Matrix Pins D0, D3, D4, D7, D8, TX, SD3, SD2
#define MATRIX_SIZE_X 2
int matrixPinsX[] = {2, 15}; // High to be on Pins D4, D7
#define MATRIX_SIZE_Y 2
int matrixPinsY[] = {10, 9}; // Low to be on Pins SD3, SD2
#define MATRIX_DELAY 10
bool matrix[MATRIX_SIZE_X][MATRIX_SIZE_Y];

// DHT 11 Pin D5
#define DHT_PIN 14
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);
float temperatureDHT;
float humidity;
float heatIndex;

// CO2 Pins D1, D2
Adafruit_CCS811 ccs;
float temperatureCSS;
float CO2;
float TVOC;

//RCWL Pin D6
#define RCWL_PIN 12
bool movement;

void setUpLEDMatrix(){

  for(int i = 0; i < MATRIX_SIZE_X; i++){
    pinMode(matrixPinsX[i], OUTPUT);
    //digitalWrite(matrixPinsX[i], 1);
  }

  for(int i = 0; i < MATRIX_SIZE_Y; i++){
    pinMode(matrixPinsY[i], OUTPUT);
    digitalWrite(matrixPinsY[i], 0);
  }

  for(int i = 0; i < MATRIX_SIZE_X; i++){
    //pinMode(matrixPinsX[i], OUTPUT);
    digitalWrite(matrixPinsX[i], 1);
  }
  for(int j = 0; j < MATRIX_SIZE_Y; j++){
    digitalWrite(matrixPinsY[j], 0);
  }
}

void setUpCSS(){

  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  //calibrate temperature sensor
  while(!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp -25.0);
}

void setUpRCWL(){
  pinMode(RCWL_PIN, INPUT);
  movement = 0;
}

void setup() {
  Serial.begin(9600);

  setUpLEDMatrix();

  //dht.begin();

  //setUpCSS();
  //setUpRCWL();
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
  temperatureDHT = dht.readTemperature();
  humidity = dht.readHumidity();
  heatIndex = dht.computeHeatIndex();
}
void printDHT(){
  Serial.print("Temperature DHT: ");
  Serial.print(temperatureDHT);
  Serial.print("°C | Humidity: ");
  Serial.print(humidity);
  Serial.print("% | Index: ");
  Serial.print(heatIndex); 
  Serial.print("°C | ");
}


void readCSS(){
  if(ccs.available()){

    temperatureCSS = ccs.calculateTemperature();

    if(!ccs.readData()){

      CO2 = ccs.geteCO2();
      TVOC = ccs.getTVOC();

    }
    else{
      Serial.println("CSS Sensor error!");
      while(1);
    }
  }
}
void printCSS(){
  Serial.print("Temperature CSS: ");
  Serial.print(temperatureCSS);
  Serial.print("°C | CO2: ");
  Serial.print(CO2);
  Serial.print("ppm | TVOC: ");
  Serial.print(TVOC); 
  Serial.print("ppb | ");
}


void readRCWL(){
  movement = digitalRead(RCWL_PIN) == 1;
}
void printRCWL(){
  Serial.print("Movement: ");
  Serial.print(movement); 
  Serial.print(" | ");
}


void loop() {
  //readDHT();
  //readCSS();
  //readRCWL();

  //printDHT();
  //printCSS();
  //printRCWL();
  //Serial.print("\n");
  //delay(1000);

  //showMatrix();
}
