
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "keypad.h"
#include "lcd.h"
#include "timer.h"

typedef enum enumState{
  ENTER,
  ENTER_VALID,
  ENTER_INVALID,
  SET,
  SET_VALID,
  SET_INVALID
}STATE;

volatile STATE state = ENTER;

void main(){
  SYSTEMConfigPerformance(10000000);
  enableInterrupts();
  LCD_Init();
  KeyPad_Init();
  while(1){
    switch(state){
    case ENTER:
      break;
    case ENTER_VALID:
      break;
    case ENTER_INVALID:
      break;
    case SET:
      break;
    case SET_VALID:
      state = ENTER;
      break;
    case SET_INVALID:
      break;
    }
  }
}