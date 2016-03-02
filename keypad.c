#include <xc.h>
#include "keypad.h"
#include "timer.h"

void KeyPad_EnableAllODC(void){
  KEYPAD_ROW1 = ODC_ENABLE;
  KEYPAD_ROW2 = ODC_ENABLE;
  KEYPAD_ROW3 = ODC_ENABLE;
  KEYPAD_ROW4 = ODC_ENABLE;
}

void KeyPad_Init(void){
  // Make Analog Pins Digital
  #ifdef KEYPAD_ROW1_ANSEL
    if(KEYPAD_ROW1_ANSEL > 0)KEYPAD_ROW1_ANSEL = ANSEL_DISABLED;
  #endif
  #ifdef KEYPAD_ROW2_ANSEL  
  if(KEYPAD_ROW2_ANSEL > 0)KEYPAD_ROW2_ANSEL = ANSEL_DISABLED;
  #endif
  #ifdef KEYPAD_ROW3_ANSEL
    if(KEYPAD_ROW3_ANSEL > 0)KEYPAD_ROW1_ANSEL = ANSEL_DISABLED;
  #endif
  #ifdef KEYPAD_ROW4_ANSEL
    KEYPAD_ROW1_ANSEL = ANSEL_DISABLED;// Disable Analog Pin
  #endif
  #ifdef KEYPAD_COL1_ANSEL
    KEYPAD_COL1_ANSEL = ANSEL_DISABLED;// Disable Analog Pin
  #endif
  #ifdef KEYPAD_COL2_ANSEL
    KEYPAD_COL2_ANSEL = ANSEL_DISABLED;// Disable Analog Pin
  #endif
  #ifdef KEYPAD_COL3_ANSEL  
    KEYPAD_COL3_ANSEL = ANSEL_DISABLED;// Disable Analog Pin
  #endif

  //Set Tristate Registers
  KEYPAD_ROW1_TRIS = TRIS_OUTPUT;
  KEYPAD_ROW2_TRIS = TRIS_OUTPUT;
  KEYPAD_ROW3_TRIS = TRIS_OUTPUT;
  KEYPAD_ROW4_TRIS = TRIS_OUTPUT;
  KEYPAD_COL1_TRIS = TRIS_INPUT;
  KEYPAD_COL2_TRIS = TRIS_INPUT;
  KEYPAD_COL3_TRIS = TRIS_INPUT;
  
  // Enable Pull Up Resistors on Inputs
  KEYPAD_COL1_CNPU = CNPU_ENABLE;
  KEYPAD_COL2_CNPU = CNPU_ENABLE;
  KEYPAD_COL3_CNPU = CNPU_ENABLE;
  
  // Enable Change Notifications on Inputs
  KEYPAD_COL1_CNEN = CNEN_ENABLE;
  KEYPAD_COL2_CNEN = CNEN_ENABLE;
  KEYPAD_COL3_CNEN = CNEN_ENABLE;
  
  // Set Outputs as OCD's
  KEYPAD_ROW1_ODC = ODC_ACTIVE;
  KEYPAD_ROW2_ODC = ODC_ACTIVE;
  KEYPAD_ROW3_ODC = ODC_ACTIVE;
  KEYPAD_ROW4_ODC = ODC_ACTIVE;
  
  // Enable the ODC's 
  KeyPad_EnableAllODC();
  
  //Setup Change Notifications
  KEYPAD_COL_CNCON = 1;// Enable Change Notifications on Port
  KEYPAD_COL_IP = 7;// Set Interrupt Priority Default
  KEYPAD_COL_IE = 1;// Enable Interrupt on Port
  KEYPAD_COL_IF = 0;// Put Interrupt Flag Down
}

char KeyPad_Scan(void){
  //Disable Interrupts While Scanning Keypad
  //KEYPAD_COL_IE = 0;
  
  //Scan First Row
  KEYPAD_ROW1 = ODC_ENABLE;// Enable ODC on Row 1
  KEYPAD_ROW2 = ODC_DISABLE;
  KEYPAD_ROW3 = ODC_DISABLE;
  KEYPAD_ROW4 = ODC_DISABLE;
  delayUs(100);
  if(KEYPAD_COL1 == 0){
    KeyPad_EnableAllODC();
    return '1';
  }
  if(KEYPAD_COL2 == 0){
    KeyPad_EnableAllODC();
    return '2';
  }
  if(KEYPAD_COL3 == 0){
    KeyPad_EnableAllODC();
    return '3';
  }
  
  //Scan Second Row
  KEYPAD_ROW1 = ODC_DISABLE;
  KEYPAD_ROW2 = ODC_ENABLE;// Enable ODC on Row 2
  KEYPAD_ROW3 = ODC_DISABLE;
  KEYPAD_ROW4 = ODC_DISABLE;
  delayUs(100);
  if(KEYPAD_COL1 == 0){
    KeyPad_EnableAllODC();
    return '4';
  }
  if(KEYPAD_COL2 == 0){
    KeyPad_EnableAllODC();
    return '5';
  }
  if(KEYPAD_COL3 == 0){
    KeyPad_EnableAllODC();
    return '6';
  }
  
  //Scan Third Row
  KEYPAD_ROW1 = ODC_DISABLE;
  KEYPAD_ROW2 = ODC_DISABLE;
  KEYPAD_ROW3 = ODC_ENABLE;// Enable ODC on Row 3
  KEYPAD_ROW4 = ODC_DISABLE;
  delayUs(100);
  if(KEYPAD_COL1 == 0){
    KeyPad_EnableAllODC();
    return '7';
  }
  if(KEYPAD_COL2 == 0){
    KeyPad_EnableAllODC();
    return '8';
  }
  if(KEYPAD_COL3 == 0){
    KeyPad_EnableAllODC();
    return '9';
  }
  
  //Scan Fourth Row
  KEYPAD_ROW1 = ODC_DISABLE;
  KEYPAD_ROW2 = ODC_DISABLE;
  KEYPAD_ROW3 = ODC_DISABLE;
  KEYPAD_ROW4 = ODC_ENABLE;// Enable ODC on Row 4
  delayUs(100);
  if(KEYPAD_COL1 == 0){
    KeyPad_EnableAllODC();
    return '*';
  }
  if(KEYPAD_COL2 == 0){
    KeyPad_EnableAllODC();
    return '0';
  }
  if(KEYPAD_COL3 == 0){
    KeyPad_EnableAllODC();
    return '#';
  }
  
  // Just Incase We Got All The Way Here
  KeyPad_EnableAllODC();
  return 'e';
  
  //Enable Interrupt Notification
  //KEYPAD_COL_IE = 1;
}

