
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "keypad.h"
#include "lcd.h"
#include "timer.h"
#include <math.h>

#define MAX_PINS 10
#define something

typedef enum enumState{
  PRE_ENTER,
  ENTER,
  ENTER_WAIT_RELEASE,
  ENTER_WAIT,
  ENTER_VALID,
  ENTER_INVALID,
  PRE_SET,
  SET,
  SET_WAIT_RELEASE,
  SET_WAIT,
  SET_VALID,
  SET_INVALID
}STATE;

volatile STATE state = PRE_ENTER;

void main(void){
  int x;
  int pinTrack = 0;
  SYSTEMConfigPerformance(10000000);
  enableInterrupts();
  LCD_Init();
  KeyPad_Init();
  char lastChar = NULL;
  char currentChar = NULL;
  int numCount = 0;
  short int pin = 0;
  short int pins[MAX_PINS];
  for(x = 0; x < MAX_PINS; x++)pins[x] = NULL;
  while(1){
    switch(state){
      case PRE_ENTER:
        LCD_Clear();
        LCD_PrintString("Enter PIN:");
        LCD_MoveCursor(1,2);
        numCount = 0;
        pin = 0;
        currentChar = NULL;
        lastChar = NULL;
        state = ENTER_WAIT;
        break;
      case ENTER:
        currentChar = KeyPad_Scan();
        if(numCount == 0){
          if(currentChar == '*'){
            LCD_PrintChar(currentChar);
            state = ENTER_WAIT_RELEASE;
          }else if(currentChar == '#'){
            state = ENTER_INVALID;
          }else{
            pin = atoi(&currentChar) * 1000;
            LCD_PrintChar(currentChar);
            state = ENTER_WAIT_RELEASE;
          }
        }else if(numCount == 1){
          if(lastChar == '*' && currentChar == '*'){
            LCD_PrintChar(currentChar);
            delayMs(100);
            state = PRE_SET;
          }else if(( lastChar == '*' && currentChar != '*')|| (lastChar != '*' && currentChar == '*') || currentChar == '#'){
            state = ENTER_INVALID;
          }else{
            pin = pin + atoi(&currentChar) * 100;
            LCD_PrintChar(currentChar);
            state = ENTER_WAIT_RELEASE;
          }
        }else if(numCount == 2){
          if(currentChar == '*' || currentChar == '#'){
            state = ENTER_INVALID;
          }else{
            pin = pin + atoi(&currentChar) * 10;
            LCD_PrintChar(currentChar);
            state = ENTER_WAIT_RELEASE;
          }
        }else if(numCount == 3){
          if(currentChar == '*' || currentChar == '#'){
            state = ENTER_INVALID;
          }else{
            pin = pin + atoi(&currentChar);
            LCD_PrintChar(currentChar);
            delayMs(500);
            state = ENTER_INVALID;
            for(x = 0; x < MAX_PINS; x++){
              if(pins[x] == pin)state = ENTER_VALID;
            };
            if(pin == 0)state = ENTER_INVALID;
          }
        }
        numCount++;
        lastChar = currentChar;
        break;
      case ENTER_WAIT_RELEASE:
      case ENTER_WAIT:
      case SET_WAIT:
      case SET_WAIT_RELEASE:
        break;
      case ENTER_VALID:
        LCD_Clear();
        LCD_PrintString("Good");
        state = PRE_ENTER;
        lastChar = NULL;
        numCount = 0;
        delayMs(2000);
        break;
      case ENTER_INVALID:
        LCD_Clear();
        LCD_PrintString("Bad");
        state = PRE_ENTER;
        lastChar = NULL;
        numCount = 0;
        delayMs(2000);
        break;
      case PRE_SET:
        numCount = 0;
        pin = 0;
        lastChar = NULL;
        currentChar = NULL;
        LCD_Clear();
        LCD_PrintString("Set Mode:");
        LCD_MoveCursor(1,2);
        state = SET_WAIT_RELEASE;
        break;
      case SET:
        currentChar = KeyPad_Scan();
        if(currentChar == '*' || currentChar == '#'){
          state = SET_INVALID;
        }else{
          pin = pin + atoi(&currentChar) * pow( 10, (3 - numCount));
          if(numCount < 3){
            LCD_PrintChar(currentChar);
            numCount++;
            state = SET_WAIT_RELEASE;
          }else{
            LCD_PrintChar(currentChar);
            delayMs(500);
            if(pinTrack == MAX_PINS)pinTrack = 0;
            pins[pinTrack] = pin;
            pinTrack++;
            state = SET_VALID;
          }
        }
        break;
      case SET_VALID:
        LCD_Clear();
        LCD_PrintString("Valid");
        state = PRE_ENTER;
        lastChar = NULL;
        numCount = 0;
        delayMs(2000);
        break;
      case SET_INVALID:
        LCD_Clear();
        LCD_PrintString("Invalid");
        state = PRE_ENTER;
        lastChar = NULL;
        numCount = 0;
        delayMs(2000);
        break;
    }
  }
}

void __ISR(_CHANGE_NOTICE_VECTOR,IPL7SRS) _CNInterrupt(void){
  PORTE;
  IFS1bits.CNEIF = 0;// Put Flag Down
  if(state == ENTER_WAIT)state = ENTER;
  if(state == ENTER_WAIT_RELEASE)state = ENTER_WAIT;
  if(state == SET_WAIT)state = SET;
  if(state == SET_WAIT_RELEASE)state = SET_WAIT;
}