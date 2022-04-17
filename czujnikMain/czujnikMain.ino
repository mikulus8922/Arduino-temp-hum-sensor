#include <Keyboard.h>

const int tempBlue = 2;
const int tempGreen = 3;
const int tempRed = 4;
const int tempInputPrevious = 5;
const int tempInputNext = 6;
int tempColor = -1;
float tempLowerThreshold = 22.0;
float tempUpperThreshold = 25.0;
int tempButtonNext = 0;
int tempButtonPrevious = 0;

const int humBlue = 8;
const int humGreen = 9;
const int humRed = 10;
const int humInputPrevious = 11;
const int humInputNext = 12;
int humColor = -1;
float humLowerThreshold = 50.0;
float humUpperThreshold = 70.0;
int humButtonNext = 0;
int humButtonPrevious = 0;

const int lowerUpperBound = 13;
int chooseBound = 0;

bool isButtonPressed = false;

float temp, hum;

#include "Seeed_SHT35.h"

#define SDAPIN  A4
#define SCLPIN  A5
#define RSTPIN  2
#define SERIAL Serial



int loopN = 0;

SHT35 sensor(SCLPIN);

void setup() {

  SERIAL.begin(115200);
  delay(10);
  SERIAL.println("serial start!!");
  if (sensor.init()) {
      SERIAL.println("sensor init failed!!!");
  }
  delay(500);
  
  pinMode(tempBlue, OUTPUT);
  pinMode(tempGreen, OUTPUT);
  pinMode(tempRed, OUTPUT);
  pinMode(tempInputPrevious, INPUT);
  pinMode(tempInputNext, INPUT);

  pinMode(humBlue, OUTPUT);
  pinMode(humGreen, OUTPUT);
  pinMode(humRed, OUTPUT);
  pinMode(humInputPrevious, INPUT);
  pinMode(humInputNext, INPUT);

  pinMode(lowerUpperBound, INPUT);
}

void tempLightReset() {
  digitalWrite(tempBlue, LOW);
  digitalWrite(tempGreen, LOW);
  digitalWrite(tempRed, LOW);
}

void humLightReset() {
  digitalWrite(humBlue, LOW);
  digitalWrite(humGreen, LOW);
  digitalWrite(humRed, LOW);
}

void tempLightBlue() {
  digitalWrite(tempBlue, HIGH);
}

void tempLightGreen() {
  digitalWrite(tempGreen, HIGH);
}

void tempLightRed() {
  digitalWrite(tempRed, HIGH);
}

void humLightBlue() {
  digitalWrite(humBlue, HIGH);
}

void humLightGreen() {
  digitalWrite(humGreen, HIGH);
}

void humLightRed() {
  digitalWrite(humRed, HIGH);
}

void changeBounds() {
  tempButtonNext = digitalRead(tempInputNext);
  tempButtonPrevious = digitalRead(tempInputPrevious);
  humButtonNext = digitalRead(humInputNext);
  humButtonPrevious = digitalRead(humInputPrevious);
  chooseBound = digitalRead(lowerUpperBound);
  
  if (tempButtonNext == 0 && tempButtonPrevious == 0 && humButtonNext == 0 && humButtonPrevious == 0){
      isButtonPressed = false;
    }

    if (!isButtonPressed) {
      if (tempButtonNext) {
          if (chooseBound == 0) {
            tempLowerThreshold += 1;
            isButtonPressed = true;
          }
          if (chooseBound == 1) {
            tempUpperThreshold += 1;
            isButtonPressed = true;
        }
      }
      if (tempButtonPrevious) {
        if (chooseBound == 0) {
          tempLowerThreshold -= 1;
          isButtonPressed = true;
        }   
        if (chooseBound == 1) {
          tempUpperThreshold -= 1;
          isButtonPressed = true;
        }   
      }
      if (humButtonNext) {
        if (chooseBound == 0) {
          humLowerThreshold += 1;
          isButtonPressed = true;
        }
        if (chooseBound == 1) {
          humUpperThreshold += 1;
          isButtonPressed = true;
        }
      }
      if (humButtonPrevious) {      
        if (chooseBound == 0) {
          humLowerThreshold -= 1;
          isButtonPressed = true;
        }
        if (chooseBound == 1) {
          humUpperThreshold -= 1;
          isButtonPressed = true;
        }
      }   
    }
}

void changeTempColor() {
    if (temp > tempUpperThreshold && tempColor != 2) {
      tempLightReset();
      tempLightRed();
      tempColor = 2;
    }
    else if (temp < tempLowerThreshold && tempColor != 0) {
      tempLightReset();
      tempLightBlue();
      tempColor = 0;
    } else if (temp > tempLowerThreshold && temp < tempUpperThreshold && tempColor != 1) {
      tempLightReset();
      tempLightGreen();
      tempColor = 1;
    }  
}

void changeHumColor(){
    if (hum > humUpperThreshold && humColor != 2) {
      humLightReset();
      humLightRed();
      humColor = 2;
    }
    else if (hum < humLowerThreshold && humColor != 0) {
      humLightReset();
      humLightBlue();
      humColor = 0;
    } else if (hum > humLowerThreshold && hum < humUpperThreshold && humColor != 1) {
      humLightReset();
      humLightGreen();
      humColor = 1;
    }
}



void loop() {
    if (loopN == 0) {
      if (NO_ERROR != sensor.read_meas_data_single_shot(HIGH_REP_WITH_STRCH, &temp, &hum)) {
          SERIAL.println("read temp failed!!");
          SERIAL.println("   ");
          SERIAL.println("   ");
          SERIAL.println("   ");
      } else {
          SERIAL.println("read data :");
          
          SERIAL.print("temperature = ");
          SERIAL.print(temp);
          SERIAL.println(" ℃ ");

          SERIAL.print("temperature lower threshold = ");
          SERIAL.print(tempLowerThreshold);
          SERIAL.println(" ℃ ");

          SERIAL.print("temperature upper threshold = ");
          SERIAL.print(tempUpperThreshold);
          SERIAL.println(" ℃ ");

          
          SERIAL.print("humidity = ");
          SERIAL.print(hum);
          SERIAL.println(" % ");

          SERIAL.print("humidity lower threshold = ");
          SERIAL.print(humLowerThreshold);
          SERIAL.println(" % ");

          SERIAL.print("humidity upper threshold = ");
          SERIAL.print(humUpperThreshold);
          SERIAL.println(" % ");
          
          SERIAL.println("   ");
          SERIAL.println("   ");
          SERIAL.println("   ");
      }
    }

    changeBounds();
    
    changeTempColor();

    changeHumColor();
        
    delay(10);
    
    loopN++;
    if (loopN == 100) {
      loopN = 0;
    }
    
}
