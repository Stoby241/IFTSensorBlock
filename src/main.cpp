#include <Arduino.h>
#include "DHT.h"
#include "Adafruit_CCS811.h"
#include <Ticker.h>

Ticker timer;

// Matrix
#define MATRIX_SIZE_X 4
int matrixPinsX[] = {D6, D7, D8, 16}; // High to be on
#define MATRIX_SIZE_Y 2
int matrixPinsY[] = {10, 3}; // Low to be on
#define MATRIX_DELAY 1
bool matrix[MATRIX_SIZE_X][MATRIX_SIZE_Y];

// DHT 11 Pin D4
#define DHT_PIN D4
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);
volatile float temperatureDHT;
volatile float humidity;
volatile float heatIndex;

// CO2 Pins D1, D2
Adafruit_CCS811 ccs;
volatile float temperatureCSS;
volatile float CO2;
volatile float TVOC;

//RCWL Pin D5
#define RCWL_PIN 14
volatile bool movement;

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
      matrix[i][j] = 1;
    }
  }
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

void setUpDHT(){
  dht.begin();
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

void setUpRCWL(){
  pinMode(RCWL_PIN, INPUT);
  movement = 0;
}
void readRCWL(){
  movement = digitalRead(RCWL_PIN) == 1;
}
void printRCWL(){
  Serial.print("Movement: ");
  Serial.print(movement); 
  Serial.print(" | ");
}

// ISR to Fire when Timer is triggered
void ICACHE_RAM_ATTR onTime() {
  readDHT();
  printDHT();

  
	// Re-Arm the timer as using TIM_SINGLE
	timer1_write(2500000);//12us
}

void setup()
{
  Serial.begin(115200);
  setUpLEDMatrix();
  setUpDHT();
  //setUpCSS();
  setUpRCWL();

	//Initialize Ticker every 0.5s
	timer1_attachInterrupt(onTime); // Add ISR Function
	timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
	/* Dividers:
		TIM_DIV1 = 0,   //80MHz (80 ticks/us - 104857.588 us max)
		TIM_DIV16 = 1,  //5MHz (5 ticks/us - 1677721.4 us max)
		TIM_DIV256 = 3  //312.5Khz (1 tick = 3.2us - 26843542.4 us max)
	Reloads:
		TIM_SINGLE	0 //on interrupt routine you need to write a new value to start the timer again
		TIM_LOOP	1 //on interrupt the counter will start with the same value again
	*/
	
	// Arm the Timer for our 0.5s Interval
	timer1_write(2500000); // 2500000 / 5 ticks per us from TIM_DIV16 == 500,000 us interval 

}

void loop() {
  showMatrix();
}
