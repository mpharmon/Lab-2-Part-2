#ifndef KEYPAD_H_
#define	KEYPAD_H_

#define TRIS_OUTPUT 0
#define TRIS_INPUT 1

#define ODC_ACTIVE 1

#define CNPU_ENABLE 1

#define CNEN_ENABLE 1

#define ODC_ENABLE 0
#define ODC_DISABLE 1

#define ANSEL_DISABLED 0

#define PORTx_LOW 0
#define PORTx_HIGH 1

// Keypad Pin #2
#define KEYPAD_ROW1       LATCbits.LATC1
#define KEYPAD_ROW1_TRIS  TRISCbits.TRISC1
#define KEYPAD_ROW1_ODC   ODCCbits.ODCC1
//#define KEYPAD_ROW1_ANSEL

// Keypad Pin 7
#define KEYPAD_ROW2       LATDbits.LATD4
#define KEYPAD_ROW2_TRIS  TRISDbits.TRISD4
#define KEYPAD_ROW2_ODC   ODCDbits.ODCD4
//#define KEYPAD_ROW2_ANSEL

//Keypad Pin 6
#define KEYPAD_ROW3       LATDbits.LATD11
#define KEYPAD_ROW3_TRIS  TRISDbits.TRISD11
#define KEYPAD_ROW3_ODC   ODCDbits.ODCD11
//#define KEYPAD_ROW3_ANSEL

//Keypad Pin 4
#define KEYPAD_ROW4       LATDbits.LATD10
#define KEYPAD_ROW4_TRIS  TRISDbits.TRISD10
#define KEYPAD_ROW4_ODC   ODCDbits.ODCD10
//#define KEYPAD_ROW4_ANSEL

//Keypad Column Change Notification 
#define KEYPAD_COL_CNCON  CNCONEbits.ON
#define KEYPAD_COL_IP     IPC8bits.CNIP
#define KEYPAD_COL_IE     IEC1bits.CNEIE
#define KEYPAD_COL_IF     IFS1bits.CNEIF

//Keypad Pin 3
#define KEYPAD_COL1       PORTEbits.RE4
#define KEYPAD_COL1_TRIS  TRISEbits.TRISE4
#define KEYPAD_COL1_CNPU  CNPUEbits.CNPUE4
#define KEYPAD_COL1_ANSEL ANSELEbits.ANSE4
#define KEYPAD_COL1_CNEN  CNENEbits.CNIEE4

//Keypad Pin 1
#define KEYPAD_COL2       PORTEbits.RE2
#define KEYPAD_COL2_TRIS  TRISEbits.TRISE2
#define KEYPAD_COL2_CNPU  CNPUEbits.CNPUE2
#define KEYPAD_COL2_ANSEL ANSELEbits.ANSE2
#define KEYPAD_COL2_CNEN  CNENEbits.CNIEE2

//Keypad Pin 5
#define KEYPAD_COL3       PORTEbits.RE6
#define KEYPAD_COL3_TRIS  TRISEbits.TRISE6
#define KEYPAD_COL3_CNPU  CNPUEbits.CNPUE6
#define KEYPAD_COL3_ANSEL ANSELEbits.ANSE6
#define KEYPAD_COL3_CNEN  CNENEbits.CNIEE6

void KeyPad_Init(void);
char KeyPad_Scan(void);

#endif	/* KEYPAD_H */