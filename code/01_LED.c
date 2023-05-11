#include "main.h"

void Delay_Timer(uint32_t time) {
	for (; time > 0; time--) {

	}
}
int main(void) {
	//RCC->APB2ENR |= 0x4; // PORTA 버스 탑승권 허용
	RCC->APB2ENR |= (1<<2);

	//GPIOA->CRL &= ~0x400000;
	GPIOA->CRL &= ~(1<<22);

	//GPIOA->CRL |= 0x100000; // PA5 출력
	GPIOA->CRL |= (1<<20);

	while(1) {
		//GPIOA->ODR |= 0x20; // LED ON
		GPIOA->BSRR |= (1<<5);

		Delay_Timer(100000);

		//GPIOA->ODR &= ~0x20; // LED OFF
		GPIOA->BRR |= (0x1<<5);

		Delay_Timer(50000);
	}
}
