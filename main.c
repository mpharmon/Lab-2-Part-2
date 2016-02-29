
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "keypad.h"
#include "lcd.h"
#include "timer.h"

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
  SYSTEMConfigPerformance(10000000);
  enableInterrupts();
  LCD_Init();
  KeyPad_Init();
  char lastChar = NULL;
  char currentChar = NULL;
  int numCount = 0;
  while(1){
    switch(state){
      case PRE_ENTER:
        LCD_Clear();
        LCD_PrintString("Enter PIN:");
        LCD_MoveCursor(1,2);
        numCount = 0;
        currentChar = NULL;
        lastChar = NULL;
        state = ENTER_WAIT;
        break;
      case ENTER:
        currentChar = KeyPad_Scan();
        if(numCount == 0){
          if(currentChar == '*'){
            state = ENTER_WAIT_RELEASE;
          }else if(currentChar == '#'){
            state = ENTER_INVALID;
          }else{
            LCD_PrintChar(currentChar);
            state = ENTER_WAIT_RELEASE;
          }
        }else if(numCount == 1){
          if(lastChar == '*' && currentChar == '*'){
            state = PRE_SET;
          }else if(( lastChar == '*' && currentChar != '*')|| (lastChar != '*' && currentChar == '*') || currentChar == '#'){
            state = ENTER_INVALID;
          }else{
            LCD_PrintChar(currentChar);
            state = ENTER_WAIT_RELEASE;
          }
        }else if(numCount == 2){
          if(currentChar == '*' || currentChar == '#'){
            state = ENTER_INVALID;
          }else{
            LCD_PrintChar(currentChar);
            state = ENTER_WAIT_RELEASE;
          }
        }else if(numCount == 3){
          if(currentChar == '*' || currentChar == '#'){
            state = ENTER_INVALID;
          }else{
            LCD_PrintChar(currentChar);
            delayMs(500);
            state = ENTER_VALID;
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
        LCD_PrintString("Valid");
        state = PRE_ENTER;
        lastChar = NULL;
        numCount = 0;
        delayMs(2000);
        break;
      case ENTER_INVALID:
        LCD_Clear();
        LCD_PrintString("Invalid");
        state = PRE_ENTER;
        lastChar = NULL;
        numCount = 0;
        delayMs(2000);
        break;
      case PRE_SET:
        numCount = 0;
        lastChar = NULL;
        currentChar = NULL;
        LCD_Clear();
        LCD_PrintString("Set Mode:");
        LCD_MoveCursor(1,2);
        state = SET_WAIT_RELEASE;
        break;
      case SET:
        delayMs(100);
        currentChar = KeyPad_Scan();
        if(currentChar == '*' || currentChar == '#'){
          state = SET_INVALID;
        }else{
          if(numCount < 3){
            LCD_PrintChar(currentChar);
            numCount++;
            state = SET_WAIT_RELEASE;
          }else{
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