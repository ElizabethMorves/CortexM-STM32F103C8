
#include "stm32f10x.h"

typedef enum{
    RWLcdWrite,
    RWLcdRead
}RWLcdMode;

typedef enum{
    RSLcdCommand,
    RSLcdData
}RSLcdMode;


#define LCD_DELAY   1            // LCD Delay. E pin flip/flop
#define READ_BUSY   0            // Enable (1) or disable (0) read LCD BUSY 0x80
                                 // hd44780 recommended set 1 BUSY enable
                                 // PT6314-001 need set 0 BUSY disable

#define PORT        GPIOA        //PORT

#define CNF_RS      GPIO_CRL_CNF1
#define CNF_RW      GPIO_CRL_CNF2
#define CNF_E       GPIO_CRL_CNF3
#define CNF_DB4     GPIO_CRL_CNF4
#define CNF_DB5     GPIO_CRL_CNF5
#define CNF_DB6     GPIO_CRL_CNF6
#define CNF_DB7     GPIO_CRL_CNF7

#define MODE_RS     GPIO_CRL_MODE1_0
#define MODE_RW     GPIO_CRL_MODE2_0
#define MODE_E      GPIO_CRL_MODE3_0
#define MODE_DB4    GPIO_CRL_MODE4_0
#define MODE_DB5    GPIO_CRL_MODE5_0
#define MODE_DB6    GPIO_CRL_MODE6_0
#define MODE_DB7    GPIO_CRL_MODE7_0
//WRITE
#define ODR_RS      GPIO_ODR_ODR1
#define ODR_RW      GPIO_ODR_ODR2
#define ODR_E       GPIO_ODR_ODR3

#define ODR_DB4     GPIO_ODR_ODR4
#define ODR_DB5     GPIO_ODR_ODR5
#define ODR_DB6     GPIO_ODR_ODR6
#define ODR_DB7     GPIO_ODR_ODR7
//READ
#define IDR_DB4     GPIO_IDR_IDR4
#define IDR_DB5     GPIO_IDR_IDR5
#define IDR_DB6     GPIO_IDR_IDR6
#define IDR_DB7     GPIO_IDR_IDR7


void InitLCD(void);
void LcdWrite(unsigned char bits, RSLcdMode rw);
unsigned char LcdReadBusy();
