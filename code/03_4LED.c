#include "main.h"

void Delay_Timer(uint32_t time) {
   for (; time > 0; time--) {

   }
}
void set_led(uint8_t n) {
   switch(n) {
   case 1:
      GPIOA->BSRR |= 0x01<<12;// 껐다가 켰다가 하려면 BSRR
      break;
   case 2:
      GPIOA->BSRR |= 0x01<<11;// 껐다가 켰다가 하려면 BSRR
      break;
   case 3:
      GPIOB->BSRR |= 0x01<<12;// 껐다가 켰다가 하려면 BSRR
      break;

   case 4:
      GPIOB->BSRR |= 0x01<<11;// 껐다가 켰다가 하려면 BSRR
      break;
   }
}
void reset_led(uint8_t n) {
   GPIOA->BSRR |= (0x01<<28) | (0x01<<27);
   GPIOB->BSRR |= (0x01<<28) | (0x01<<27);
   }
int main(void) {
   RCC->APB2ENR |= (0x01<<2)| (0x01<<3);
   GPIOA->CRH &= ~(0x44000);
   GPIOA->CRH |= 0x11000;
   GPIOB->CRH &= ~(0x44000);
   GPIOB->CRH |= 0x11000;

   while(1){
      for(uint8_t i=1; i < 5; i++) {
         set_led(i);
         Delay_Timer(500000);
         reset_led(i);
         Delay_Timer(500000);
      }
   }
}
