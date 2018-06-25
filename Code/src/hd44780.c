#include <hd44780.h>


static void DelayMs(uint32_t nTime){
	uint32_t n;
	n = nTime * SystemCoreClock / 100000;
	while(n > 0) { n--; }
}


void LcdGPIO(RWLcdMode mode){
	GPIOA->CRL &= ~GPIO_CRL_CNF1   & ~GPIO_CRL_CNF2   &	~GPIO_CRL_CNF3;	 							/*	~0x0000CCC0	*/
	GPIOA->CRL |= GPIO_CRL_MODE1_0 | GPIO_CRL_MODE2_0 |	GPIO_CRL_MODE3_0;							/*	0x00001110	*/

	if (mode){
		GPIOA->BSRR |= GPIO_BSRR_BS2;/*	LCD READ (RW PIN 1 / mode==1)*/
		GPIOA->CRL &= ~GPIO_CRL_MODE4_0 & ~GPIO_CRL_MODE5_0 & ~GPIO_CRL_MODE6_0 & ~GPIO_CRL_MODE7_0;
	}else{
		GPIOA->BSRR |= GPIO_BSRR_BR2;/*	LCD WRITE (RW PIN 0 / mode==0)*/
		GPIOA->CRL &= ~GPIO_CRL_CNF4   & ~GPIO_CRL_CNF5	  & ~GPIO_CRL_CNF6   & ~GPIO_CRL_CNF7;		/*	~0xCCCC0000	*/
		GPIOA->CRL |= GPIO_CRL_MODE4_0 | GPIO_CRL_MODE5_0 | GPIO_CRL_MODE6_0 | GPIO_CRL_MODE7_0;	/*	 0x11110000	*/
	}
}

void LcdWriteBit(unsigned char command) {
		(command & 0x01) ? ( GPIOA->ODR |= GPIO_ODR_ODR4 ) : ( GPIOA->ODR &= ~GPIO_ODR_ODR4 );
		(command & 0x02) ? ( GPIOA->ODR |= GPIO_ODR_ODR5 ) : ( GPIOA->ODR &= ~GPIO_ODR_ODR5 );
		(command & 0x04) ? ( GPIOA->ODR |= GPIO_ODR_ODR6 ) : ( GPIOA->ODR &= ~GPIO_ODR_ODR6 );
		(command & 0x08) ? ( GPIOA->ODR |= GPIO_ODR_ODR7 ) : ( GPIOA->ODR &= ~GPIO_ODR_ODR7 );
	DelayMs(1);
	GPIOA->ODR |= GPIO_ODR_ODR3;
	DelayMs(1);
	GPIOA->ODR &= ~GPIO_ODR_ODR3;
	DelayMs(1);
}

unsigned char LcdReadBit() {
	unsigned char status = 0;
	GPIOA->BSRR |= GPIO_BSRR_BS3;
		if (GPIOA->IDR & GPIO_IDR_IDR4) { status |= (1 << 0); }
		if (GPIOA->IDR & GPIO_IDR_IDR5) { status |= (1 << 1); }
		if (GPIOA->IDR & GPIO_IDR_IDR6) { status |= (1 << 2); }
		if (GPIOA->IDR & GPIO_IDR_IDR7)	{ status |= (1 << 3); }
	GPIOA->BSRR |= GPIO_BSRR_BR3;
	return status;
}

void LcdWrite(unsigned char bits, RSLcdMode rw) {
	rw ? (GPIOA->ODR |= GPIO_ODR_ODR1) : (GPIOA->ODR &= ~GPIO_ODR_ODR1);
	LcdWriteBit(bits >> 4);
	LcdWriteBit(bits);
	while (LcdReadBusy() & 0x80);
}

unsigned char LcdReadBusy() {
	unsigned char status = 0;
		GPIOA->ODR &= ~GPIO_ODR_ODR1;
		LcdGPIO(RWLcdRead);
			status |= (LcdReadBit() << 4);
			status |= LcdReadBit();
		LcdGPIO(RWLcdWrite);
	return (status);
}


void InitLCD(){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	LcdGPIO(RWLcdWrite);

	DelayMs(2000);
	LcdWriteBit(0x03);
	DelayMs(5000);
	LcdWriteBit(0x03);
	DelayMs(2000);
	LcdWriteBit(0x03);
	DelayMs(2000);
	LcdWriteBit(0x02);

/*
	DelayMs(400);
	LcdWriteBit(0x02);	//4bit
	DelayMs(100);
	LcdWriteBit(0x02);	//4bit
	DelayMs(100);
*/
	LcdWrite(0x02, RSLcdCommand); 	// 4bit Return Home
	DelayMs(2000);
	LcdWrite(0x28, RSLcdCommand); 	//4bit, 2 line, 5x8 matrix
	DelayMs(2000);
	LcdWrite(0x08, RSLcdCommand);	// Display off
	DelayMs(2000);
	LcdWrite(0x01, RSLcdCommand);	// clear display
	DelayMs(2000);
	LcdWrite(0x06, RSLcdCommand);	// next step right
	DelayMs(2000);
	LcdWrite(0x0C, RSLcdCommand);  	// display on, cursor on, blink on
	DelayMs(2000);
}
