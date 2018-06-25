#include <TIM.h>

void InitTIM2(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;			/*	Enable clock TIM2	*/
	TIM2->ARR = 1000-1;							/*	Period				*/
	TIM2->PSC = (SystemCoreClock/1000)-1;		/*	Prescaler 			*/

	TIM2->DIER |= TIM_DIER_UIE;					/*	Enable interrupt 	*/
	TIM2->CR1 |= TIM_CR1_CEN;					/*	Enable counter		*/

	NVIC_EnableIRQ(TIM2_IRQn);					/*	Interrupt			*/

}

void TIM2_IRQHandler(void){
	TIM2->SR &= ~TIM_SR_UIF;					/*	Reset Flag			*/
}
