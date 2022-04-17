#define __SFR_OFFSET 0x00

#include "avr/io.h"

// zmienne przechowujace nazwy pinow
#define tempBlue 2 // pd2 - 2
#define tempGreen 3 // pd3 - 3
#define tempRed 4 // pd4 - 4
#define tempInputPrevious 5 // pd5 - 5
#define tempInputNext 6 // pd6 - 6

#define humBlue 0 // pb0 - 8
#define humGreen 1 // pb1 - 9
#define humRed 2 // pb2 - 10
#define humInputPrevious 3 // pb3 - 11
#define humInputNext 4 // pb4 - 12

#define lowerUpperBound 5 // pb5 - 13


.global setupPins

.global tempLightReset
.global humLightReset

.global tempLightBlue
.global tempLightGreen
.global tempLightRed
.global humLightBlue
.global humLightGreen
.global humLightRed

.global changeBounds
.global changeTempColor
.global changeHumColor

.global tempNext

setupPins:
  sbi DDRD, tempBlue
  sbi DDRD, tempGreen
  sbi DDRD, tempRed
  cbi DDRD, tempInputPrevious
  cbi DDRD, tempInputNext

  sbi DDRB, humBlue
  sbi DDRB, humGreen
  sbi DDRB, humRed
  cbi DDRB, humInputPrevious
  cbi DDRB, humInputNext

  cbi DDRB, lowerUpperBound
  ret

tempLightReset:
  cbi PORTD, tempBlue
  cbi PORTD, tempGreen
  cbi PORTD, tempRed
  ret

humLightReset:
  cbi PORTB, humBlue
  cbi PORTB, humGreen
  cbi PORTB, humRed
  ret

tempLightBlue:
  sbi PORTD, tempBlue
  ret

tempLightGreen:
  sbi PORTD, tempGreen
  ret

tempLightRed:
  sbi PORTD, tempRed
  ret

humLightBlue:
  sbi PORTB, humBlue
  ret

humLightGreen:
  sbi PORTB, humGreen
  ret

humLightRed:
  sbi PORTB, humRed
  ret



changeTempColor:
    lds r11, tempUpperThreshold + 1
    lds r12, tempUpperThreshold
    lds r13, tempLowerThreshold + 1
    lds r14, tempLowerThreshold
    lds r15, temp + 1
    lds r16, temp
    lds r20, tempColor
    
  checkRedTemp:
    cp r15, r11 // temp < tempU H
    brlo checkBlueTemp
    cp r11, r15 // tempU < temp H
    brlo lightRedTemp
    cp r16, r12 // temp < tempU L
    brlo checkBlueTemp
    
  lightRedTemp:
    cpi r20, 2
    breq checkBlueTemp
    
    call tempLightReset
    call tempLightRed
    ldi r20, 2
    sts tempColor, r20
    ret
    
  checkBlueTemp:
    cp r13, r15 // tempL < temp H
    brlo checkGreenTemp
    cp r15, r13 // temp < tempL H
    brlo lightBlueTemp
    cp r14, r16 // tempL < temp L
    brlo checkGreenTemp

  lightBlueTemp:
    cpi r20, 0
    breq checkGreenTemp
    
    call tempLightReset
    call tempLightBlue
    ldi r20, 0
    sts tempColor, r20
    ret
    
  checkGreenTemp:
    cp r11, r15 // tempU < temp H
    brlo endTemp
    cp r15, r11 // temp < tempU H
    brlo checkGreen2Temp
    cp r12, r16 // tempU < temp L
    brlo endTemp

  checkGreen2Temp:
    cp r15, r13 // temp < tempL H
    brlo endTemp
    cp r13, r15 // temp < tempL H
    brlo lightGreenTemp
    cp r16, r14 // temp < tempL L
    brlo endTemp

  lightGreenTemp:
    cpi r20, 1
    breq endTemp

    call tempLightReset
    call tempLightGreen
    ldi r20, 1
    sts tempColor, r20
  endTemp:    
    ret




changeHumColor:
    lds r11, humUpperThreshold + 1
    lds r12, humUpperThreshold
    lds r13, humLowerThreshold + 1
    lds r14, humLowerThreshold
    lds r15, hum + 1
    lds r16, hum
    lds r20, humColor
    
  checkRedHum:
    cp r15, r11 // hum < humU H
    brlo checkBlueHum
    cp r11, r15 // humU < hum H
    brlo lightRedHum
    cp r16, r12 // hum < humU L
    brlo checkBlueHum
    
  lightRedHum:
    cpi r20, 2
    breq checkBlueHum
    
    call humLightReset
    call humLightRed
    ldi r20, 2
    sts humColor, r20
    ret
    
  checkBlueHum:
    cp r13, r15 // humL < hum H
    brlo checkGreenHum
    cp r15, r13 // hum < humL H
    brlo lightBlueHum
    cp r14, r16 // humL < hum L
    brlo checkGreenHum

  lightBlueHum:
    cpi r20, 0
    breq checkGreenHum
    
    call humLightReset
    call humLightBlue
    ldi r20, 0
    sts humColor, r20
    ret
    
  checkGreenHum:
    cp r11, r15 // humU < hum H
    brlo endHum
    cp r15, r11 // hum < humU H
    brlo checkGreen2Hum
    cp r12, r16 // humU < hum L
    brlo endHum

  checkGreen2Hum:
    cp r15, r13 // hum < humL H
    brlo endHum
    cp r13, r15 // hum < humL H
    brlo lightGreenHum
    cp r16, r14 // hum < humL L
    brlo endHum

  lightGreenHum:
    cpi r20, 1
    breq endHum

    call humLightReset
    call humLightGreen
    ldi r20, 1
    sts humColor, r20
  endHum:    
    ret



changeBounds:
    lds r21, tempButtonNext
    lds r22, tempButtonPrevious
    lds r23, humButtonNext
    lds r24, humButtonPrevious
    lds r25, chooseBound
    // if() tylko 1 zwiekszenie na nacisniecie

    
    cpi r21, 1
    breq buttonPressed

    cpi r22, 1
    breq buttonPressed

    cpi r23, 1
    breq buttonPressed

    cpi r24, 1
    breq buttonPressed
    
    ldi r18, 0
    sts isButtonPressed, r18
    //
  buttonPressed:
    lds r18, isButtonPressed
    cpi r18, 1
    breq endChange

    cpi r21, 0
    breq tip
    call tempNext
  tip:
    cpi r22, 0
    breq hin
    call tempPrevious

  hin:
    cpi r23, 0
    breq hip
    call humNext

  hip:
    cpi r24, 0
    breq endChange
    call humPrevious

    
  endChange:
    ret


tempNext: //////////////////////
    ldi r18, 1
    sts isButtonPressed, r18
    cpi r25, 1
    breq tempNext2

    lds r5, tempLowerThreshold + 1
    lds r6, tempLowerThreshold
    clc
    ldi r18, 0
    ldi r19, 100
    add r6, r19
    adc r5, r18

    lds r18, tempUpperThreshold + 1
    lds r19, tempUpperThreshold

    cp r18, r5
    brne tempNextW
    cp r19, r6
    brne tempNextW
    ret
  
  tempNextW:
    sts tempLowerThreshold + 1, r5
    sts tempLowerThreshold, r6

    ret
  tempNext2:

    lds r5, tempUpperThreshold + 1
    lds r6, tempUpperThreshold
    clc
    ldi r18, 0
    ldi r19, 100
    add r6, r19
    adc r5, r18

    sts tempUpperThreshold + 1, r5
    sts tempUpperThreshold, r6
    
    ret
  
tempPrevious:
    ldi r18, 1
    sts isButtonPressed, r18
    cpi r25, 1
    breq tempPrevious2

    lds r5, tempLowerThreshold + 1
    lds r6, tempLowerThreshold
    clc
    ldi r18, 0
    ldi r19, 100
    sub r6, r19
    sbc r5, r18

    sts tempLowerThreshold + 1, r5
    sts tempLowerThreshold, r6

    ret
  tempPrevious2: /////////////////////

    lds r5, tempUpperThreshold + 1
    lds r6, tempUpperThreshold
    clc
    ldi r18, 0
    ldi r19, 100
    sub r6, r19
    sbc r5, r18

    lds r18, tempLowerThreshold + 1
    lds r19, tempLowerThreshold

    cp r18, r5
    brne tempPreviousW
    cp r19, r6
    brne tempPreviousW
    ret
  
  tempPreviousW:

    sts tempUpperThreshold + 1, r5
    sts tempUpperThreshold, r6
    
    ret
  
humNext: //////////////////
    ldi r18, 1
    sts isButtonPressed, r18
    cpi r25, 1
    breq humNext2

    lds r5, humLowerThreshold + 1
    lds r6, humLowerThreshold
    clc
    ldi r18, 0
    ldi r19, 100
    add r6, r19
    adc r5, r18

    lds r18, humUpperThreshold + 1
    lds r19, humUpperThreshold

    cp r18, r5
    brne humNextW
    cp r19, r6
    brne humNextW
    ret
  
  humNextW:

    sts humLowerThreshold + 1, r5
    sts humLowerThreshold, r6


    ret
  humNext2:

    lds r5, humUpperThreshold + 1
    lds r6, humUpperThreshold
    clc
    ldi r18, 0
    ldi r19, 100
    add r6, r19
    adc r5, r18

    sts humUpperThreshold + 1, r5
    sts humUpperThreshold, r6
    
    ret
  
humPrevious:
    ldi r18, 1
    sts isButtonPressed, r18
    cpi r25, 1
    breq humPrevious2

    lds r5, humLowerThreshold + 1
    lds r6, humLowerThreshold
    clc
    ldi r18, 0
    ldi r19, 100
    sub r6, r19
    sbc r5, r18

    sts humLowerThreshold + 1, r5
    sts humLowerThreshold, r6

    ret
  humPrevious2: ///////////////

    lds r5, humUpperThreshold + 1
    lds r6, humUpperThreshold
    clc
    ldi r18, 0
    ldi r19, 100
    sub r6, r19
    sbc r5, r18

    lds r18, humLowerThreshold + 1
    lds r19, humLowerThreshold

    cp r18, r5
    brne humPreviousW
    cp r19, r6
    brne humPreviousW
    ret
  
  humPreviousW:

    sts humUpperThreshold + 1, r5
    sts humUpperThreshold, r6
    
    ret
