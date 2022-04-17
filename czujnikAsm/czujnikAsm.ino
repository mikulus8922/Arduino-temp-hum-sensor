
const int tempInputPrevious = 5;
const int tempInputNext = 6;
//int tempColor = -1;
//int tempLowerThreshold = 2200;
//int tempUpperThreshold = 2500;
//int tempButtonNext = 0;
//int tempButtonPrevious = 0;


const int humInputPrevious = 11;
const int humInputNext = 12;
//int humColor = -1;
//int humLowerThreshold = 5000;
//int humUpperThreshold = 7000;
//int humButtonNext = 0;
//int humButtonPrevious = 0;

const int lowerUpperBound = 13;
//int chooseBound = 0;
//
//bool isButtonPressed = false;

float tempF, humF;

volatile int16_t temp asm("temp") __attribute__ ((used)) = 0;
volatile int16_t hum asm("hum") __attribute__ ((used)) = 0;

// Kolory i przyciski zwiazane z temeratura
volatile int8_t tempColor asm("tempColor") __attribute__ ((used)) = -1;
volatile int8_t tempButtonNext asm("tempButtonNext") __attribute__ ((used)) = 0;
volatile int8_t tempButtonPrevious asm("tempButtonPrevious") __attribute__ ((used)) = 0;

// Kolory i przyciski zwiazane z wilgotnoscia
volatile int8_t humColor asm("humColor") __attribute__ ((used)) = -1;
volatile int8_t humButtonNext asm("humButtonNext") __attribute__ ((used)) = 0;
volatile int8_t humButtonPrevious asm("humButtonPrevious") __attribute__ ((used)) = 0;

// 2200
volatile int16_t tempLowerThreshold asm("tempLowerThreshold") __attribute__ ((used)) = 2250;
// 0x0898
// 2500
volatile int16_t tempUpperThreshold asm("tempUpperThreshold") __attribute__ ((used)) = 2500;
// 0x09c4
// 5000
volatile int16_t humLowerThreshold asm("humLowerThreshold") __attribute__ ((used)) = 5000;
// 0x1388
// 7000
volatile int16_t humUpperThreshold asm("humUpperThreshold") __attribute__ ((used)) = 7000;
// 0x1b58

volatile int8_t isButtonPressed asm("isButtonPressed") __attribute__ ((used)) = 0;

volatile int8_t chooseBound asm("chooseBound") __attribute__ ((used)) = 0;


#include "Seeed_SHT35.h"

#define SDAPIN  A4
#define SCLPIN  A5
#define RSTPIN  2
#define SERIAL Serial

extern "C" {
  void setupPins();
  
  void tempLightReset();
  void humLightReset();
  
  void tempLightBlue();
  void tempLightGreen();
  void tempLightRed();   
  void humLightBlue(); 
  void humLightGreen();
  void humLightRed();
  
  
  void changeBounds();
  void changeTempColor();
  void changeHumColor();
}



int loopN = 0;

SHT35 sensor(SCLPIN);

void setup() {

  SERIAL.begin(115200);
  delay(10);
  SERIAL.println("serial start!!");
  if (sensor.init()) {
      SERIAL.println("sensor init failed!!!");
  }
  delay(100);

  setupPins();
}


void loop() {
    if (loopN == 0) {
      if (NO_ERROR != sensor.read_meas_data_single_shot(HIGH_REP_WITH_STRCH, &tempF, &humF)) {
          SERIAL.println("read temp failed!!");
          SERIAL.println("   ");
          SERIAL.println("   ");
          SERIAL.println("   ");
      } else {
          temp = int(tempF * 100);
          hum = int(humF * 100);
          SERIAL.println("read data :");
          SERIAL.print("temperature = ");
          SERIAL.print(tempF);
          SERIAL.println(" ℃ ");
          SERIAL.println(float(tempLowerThreshold) / 100);
          SERIAL.println(float(tempUpperThreshold) / 100);
          
          
          SERIAL.print("humidity = ");
          SERIAL.print(humF);
          SERIAL.println(" % ");
          SERIAL.println(float(humLowerThreshold) / 100);
          SERIAL.println(float(humUpperThreshold) / 100);
          
          SERIAL.println("   ");
          SERIAL.println("   ");
          SERIAL.println("   ");
      }
    }

    tempButtonNext = digitalRead(tempInputNext);
    tempButtonPrevious = digitalRead(tempInputPrevious);
    humButtonNext = digitalRead(humInputNext);
    humButtonPrevious = digitalRead(humInputPrevious);
    chooseBound = digitalRead(lowerUpperBound);


    changeBounds();
    
    changeTempColor();

    changeHumColor();

    
    delay(10);
    
    loopN++;
    if (loopN == 100) {
      loopN = 0;
    }
    
}
