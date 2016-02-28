
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "keypad.h"
#include "lcd.h"
#include "timer.h"

typedef enum enumState{
  PRE_ENTER,
  ENTER,
  ENTER_WAIT,
  ENTER_VALID,
  ENTER_INVALID,
  PRE_SET,
  SET,
  SET_WAIT,
  SET_VALID,
  SET_INVALID
}STATE;

volatile STATE state = PRE_ENTER;

void main(){
  int x;
  SYSTEMConfigPerformance(10000000);
  enableInterrupts();
  TRISDbits.TRISD0 = 0;
  TRISDbits.TRISD1 = 0;
  TRISDbits.TRISD2 = 0;
  LATDbits.LATD0 = 0;
  LATDbits.LATD1 = 0;
  LATDbits.LATD2 = 0;
  LCD_Init();
  KeyPad_Init();
  char lastChar = NULL;
  char currentChar = NULL;
  int numCount = 0;
  while(1){
    switch(state){
      case PRE_ENTER:
        LATDbits.LATD0 = 1;
        LATDbits.LATD1 = 1;
        LATDbits.LATD2 = 1;
        LCD_Clear();
        //LCD_MoveCursor(1,1);
        LCD_PrintString("Enter PIN:");
        state = ENTER_WAIT;
        break;
      case ENTER:
        LATDbits.LATD0 = 1;
        LATDbits.LATD1 = 0;
        LATDbits.LATD2 = 0;
        currentChar = KeyPad_Scan();
        if(lastChar == '*' && currentChar == '*'){
          state = PRE_SET;
        }else if(lastChar == '*'){
          state = ENTER_INVALID;
        }else{
          LCD_MoveCursor(numCount + 1,2);
          if(numCount < 4){
            LCD_PrintChar(currentChar);
            numCount++;
          }else state = ENTER_VALID;
          lastChar = currentChar;
        }
        state = ENTER_WAIT;
        break;
      case ENTER_WAIT:
        LATDbits.LATD0 = 0;
        LATDbits.LATD1 = 1;
        LATDbits.LATD2 = 0;
        break;
      case ENTER_VALID:
        LCD_Clear();
        //LCD_MoveCursor(1,1);
        LCD_PrintString("Valid");
        state = PRE_ENTER;
        break;
      case ENTER_INVALID:
        LCD_Clear();
        //LCD_MoveCursor(1,1);
        LCD_PrintString("Invalid");
        state = PRE_ENTER;
        break;
      case PRE_SET:
        LCD_Clear();
        //LCD_MoveCursor(1,1);
        LCD_PrintString("Set Mode");
        state = SET_WAIT;
        break;
      case SET:
        currentChar = KeyPad_Scan();
        if(currentChar == '*' || currentChar == '#'){
          state = SET_INVALID;
        }else{
          if(numCount < 4){
            LCD_PrintChar(currentChar);
            numCount++;
          }else{
            state = SET_VALID;
          }
        }
        state = SET_WAIT;
        break;
      case SET_WAIT:
        break;
      case SET_VALID:
        LCD_Clear();
        //LCD_MoveCursor(1,1);
        LCD_PrintString("Valid");
        state = PRE_ENTER;
        break;
      case SET_INVALID:
        LCD_Clear();
        //LCD_MoveCursor(1,1);
        LCD_PrintString("Invalid");
        state = PRE_ENTER;
        break;
    }
  }
}

void __ISR(_CHANGE_NOTICE_VECTOR,IPL7SRS) _CNInterrupt(void){
  PORTD;
  IFS1bits.CNEIF = 0;// Put Flag Down
  LATDbits.LATD0 = 1;
  LATDbits.LATD1 = 0;
  LATDbits.LATD2 = 1;
  if(state == ENTER_WAIT)state = ENTER;
  if(state == SET_WAIT)state = SET;
}