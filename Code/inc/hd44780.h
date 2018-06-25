
#include "stm32f10x.h"

typedef enum{
	RWLcdWrite,
	RWLcdRead
}RWLcdMode;

typedef enum{
	RSLcdCommand,
	RSLcdData
}RSLcdMode;


void InitLCD(void);
void LcdWrite(unsigned char bits, RSLcdMode rw);
unsigned char LcdReadBusy();

