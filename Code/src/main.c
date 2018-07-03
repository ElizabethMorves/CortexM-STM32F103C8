#include "main.h"


int main(void){
    InitHSE();
    //GenMCO();
    InitBlinkPC13();

    InitLCD();
    LcdWrite(*"H", RSLcdData);
    LcdWrite(*"E", RSLcdData);
    LcdWrite(*"L", RSLcdData);
    LcdWrite(*"L", RSLcdData);
    LcdWrite(*"O", RSLcdData);
    LcdWrite(*" ", RSLcdData);
    LcdWrite(*"W", RSLcdData);
    LcdWrite(*"O", RSLcdData);
    LcdWrite(*"R", RSLcdData);
    LcdWrite(*"D", RSLcdData);
    LcdWrite(*"!", RSLcdData);

    xTaskCreate(vTaskLedBlink, "LED_PC13", 32, NULL, 1, NULL);
    xTaskCreate(vTaskLCD, "LCD", 32, NULL, 2, NULL);
    vTaskStartScheduler();


    while(1){}
}


void vTaskLedBlink(void *arg){
    while(1){
        GPIOC->BSRR |= GPIO_BSRR_BR13;
        vTaskDelay(40);
        GPIOC->BSRR |= GPIO_BSRR_BS13;
        vTaskDelay(80);
        GPIOC->BSRR |= GPIO_BSRR_BR13;
        vTaskDelay(40);
        GPIOC->BSRR |= GPIO_BSRR_BS13;
        vTaskDelay(1500);
    }
}

void vTaskLCD(void *arg){
    unsigned char i=0;
    while(1){
        vTaskDelay(1000);
        //LcdWrite(0x01, RSLcdCommand);  //clear display
        LcdWrite(0x02, RSLcdCommand);    //set position home
vTaskDelay(100);
        LcdWrite(*"1", RSLcdData);
        LcdWrite(*"2", RSLcdData);
        LcdWrite(*"3", RSLcdData);
        LcdWrite(*"4", RSLcdData);
        LcdWrite(*"5", RSLcdData);
        LcdWrite(*"6", RSLcdData);
        LcdWrite(*"7", RSLcdData);
        LcdWrite(*"8", RSLcdData);
        LcdWrite(*"9", RSLcdData);
        LcdWrite(*"0", RSLcdData);
        LcdWrite(*" ", RSLcdData);
        LcdWrite(*" ", RSLcdData);
        LcdWrite(*" ", RSLcdData);
        LcdWrite(*" ", RSLcdData);
        LcdWrite(*" ", RSLcdData);
        LcdWrite(*" ", RSLcdData);
        LcdWrite(*" ", RSLcdData);
        LcdWrite(*" ", RSLcdData);
        LcdWrite(*" ", RSLcdData);
        LcdWrite(i, RSLcdData);         //print all symbols
        i++;

    }
}
