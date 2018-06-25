#include "RCC.h"


void InitHSE(void){

	RCC->CR |= RCC_CR_HSEON; 											// Enable HSE
	while (!(RCC->CR & RCC_CR_HSERDY));									// Ready start HSE
	RCC->CR |= RCC_CR_CSSON;											// Protect HSE

	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;					// Cloclk Flash memory

	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;									// AHB = SYSCLK/1
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;									// APB1 = HCLK/2
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;									// APB2 = HCLK/1

	RCC->CFGR &= ~RCC_CFGR_PLLMULL;               						// clear PLLMULL bits
	RCC->CFGR &= ~RCC_CFGR_PLLSRC;										// clearn PLLSRC bits
	RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;									// clearn PLLXTPRE bits

	RCC->CFGR |= RCC_CFGR_PLLSRC;
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE;
	RCC->CFGR |= RCC_CFGR_PLLMULL9; 									// PLL x6: clock = 4 MHz * 6 = 24 MHz

	RCC->CR |= RCC_CR_PLLON;                      						// enable PLL
	while((RCC->CR & RCC_CR_PLLRDY) == 0) {}      						// wait till PLL is ready

	RCC->CFGR &= ~RCC_CFGR_SW;                   						// clear SW bits
    RCC->CFGR |= RCC_CFGR_SW_PLL;                 						// select source SYSCLK = PLL
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1) {} 				// wait till PLL is used
	RCC->CR &= ~RCC_CR_HSION;											// Off HSI
	while ((RCC->CR & RCC_CR_HSIRDY)==0){};								// Ready stop HSI

}

/* My LSE DEAD ( */
void InitLSE(void){
	/* Enable the LSE clock (external 32.768 kHz oscillator) */
//	RCC->BDCR &= ~(RCC_BDCR_LSEON);
//	RCC->BDCR &= ~(RCC_BDCR_LSEBYP);
//	RCC->BDCR |= RCC_BDCR_LSEON;
//	while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0) {}

	//RCC->BDCR |= RCC_BDCR_LSEON;
	//while (!(RCC->BDCR & RCC_BDCR_LSERDY));
	//RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;
	//RCC->BDCR |= RCC_BDCR_RTCEN;
	//RCC->BDCR |= RCC_BDCR_BDRST;
}

void GenMCO(void){

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                       	// enable clock for port A
	GPIOA->CRH &= ~GPIO_CRH_CNF8_0;								//
	GPIOA->CRH |= GPIO_CRH_CNF8_1;								// setting out alternative push-pull for PA8
	GPIOA->CRH |= (GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1);		// gpio speed 50 MHz
	RCC->CFGR |= RCC_CFGR_MCO_SYSCLK;							// select source clokc SYSCLK

}

