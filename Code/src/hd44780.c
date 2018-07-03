#include <hd44780.h>


static void DelayMs(uint32_t nTime){
    uint32_t n;
    n = nTime * SystemCoreClock / 100000;
    while(n > 0) { n--; }
}


void LcdGPIO(RWLcdMode mode){
    PORT->CRL &= ~(CNF_RS | CNF_RW | CNF_E);                        /* ~0x0000CCC0 */
    PORT->CRL |= MODE_RS | MODE_RW | MODE_E;                        /*  0x00001110 */

    if (mode){
        PORT->ODR |= ODR_RW;
        /*    LCD READ (RW PIN 1 / mode 1)*/
        PORT->CRL &= ~(CNF_DB4   | CNF_DB5    | CNF_DB6 | CNF_DB7); /* ~0xCCCC0000 */
        PORT->CRL &= ~(MODE_DB4 | MODE_DB5 | MODE_DB6 | MODE_DB7);
    }else{
        PORT->ODR &= ~ODR_RW;
        /*    LCD WRITE (RW PIN 0 / mode 0)*/
        PORT->CRL &= ~(CNF_DB4 | CNF_DB5 | CNF_DB6 | CNF_DB7);      /* ~0xCCCC0000 */
        PORT->CRL |= MODE_DB4 | MODE_DB5 | MODE_DB6 | MODE_DB7;     /*  0x11110000 */
    }
}

void LcdWriteBit(unsigned char command) {
        (command & 0x01) ? ( PORT->ODR |= ODR_DB4 ) : ( PORT->ODR &= ~ODR_DB4 );
        (command & 0x02) ? ( PORT->ODR |= ODR_DB5 ) : ( PORT->ODR &= ~ODR_DB5 );
        (command & 0x04) ? ( PORT->ODR |= ODR_DB6 ) : ( PORT->ODR &= ~ODR_DB6 );
        (command & 0x08) ? ( PORT->ODR |= ODR_DB7 ) : ( PORT->ODR &= ~ODR_DB7 );
    DelayMs(LCD_DELAY);
    PORT->ODR |= ODR_E;
    DelayMs(LCD_DELAY);
    PORT->ODR &= ~ODR_E;
    DelayMs(LCD_DELAY);
}

unsigned char LcdReadBit() {
    unsigned char status = 0;
    PORT->ODR |= ODR_E;
        if (PORT->IDR & IDR_DB4) { status |= (1 << 0); }
        if (PORT->IDR & IDR_DB5) { status |= (1 << 1); }
        if (PORT->IDR & IDR_DB6) { status |= (1 << 2); }
        if (PORT->IDR & IDR_DB7) { status |= (1 << 3); }
    PORT->ODR &= ~ODR_E;
    return status;
}

void LcdWrite(unsigned char bits, RSLcdMode rw) {
    while (LcdReadBusy() & 0x80);
    rw ? (PORT->ODR |= ODR_RS) : (PORT->ODR &= ~ODR_RS);
    LcdWriteBit(bits >> 4);
    LcdWriteBit(bits);
}

unsigned char LcdReadBusy() {
    unsigned char status = 0;
        PORT->ODR &= ~ODR_RS;
        LcdGPIO(RWLcdRead);
            status |= (LcdReadBit() << 4);
            status |= LcdReadBit();
        LcdGPIO(RWLcdWrite);
    return (status);
}


void InitLCD(){
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    LcdGPIO(RWLcdWrite);
    //hd44780 very slow init Hmmm...
    DelayMs(6000000);
    LcdWriteBit(0x03);
    DelayMs(500000);
    LcdWriteBit(0x03);
    DelayMs(500000);
    LcdWriteBit(0x03);
    DelayMs(500000);
    LcdWriteBit(0x02);

    LcdWrite(0x02, RSLcdCommand);    // 4bit Return Home
    DelayMs(2000);
    LcdWrite(0x28, RSLcdCommand);    //4bit, 2 line, 5x8 matrix
    DelayMs(2000);
    LcdWrite(0x08, RSLcdCommand);    // Display off
    DelayMs(2000);
    LcdWrite(0x01, RSLcdCommand);    // clear display
    DelayMs(2000);
    LcdWrite(0x06, RSLcdCommand);    // next step right
    DelayMs(2000);
    LcdWrite(0x0C, RSLcdCommand);    // display on, cursor on, blink on
    DelayMs(2000);
}
