#include "stm8s.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SWITCH_PORT GPIOD
#define SWITCH GPIO_PIN_6

#define TIM4_PERIOD       124

volatile uint16_t count;

INTERRUPT_HANDLER(IRQ_Handler_TIM4, 23)
{
  if (count)
    count--;

    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
}

void delay_ms(uint16_t ms)
{
        TIM4_Cmd(DISABLE);       // stop
        TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
        TIM4_ClearFlag(TIM4_FLAG_UPDATE);
        TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);

        count = ms;

        TIM4_Cmd(ENABLE);       // let's go

  while(count);
}



ErrorStatus status = FALSE;


int main( void )
{

  // ---------- GPIO CONFIG -------------
  GPIO_DeInit(SWITCH_PORT);
  GPIO_Init(SWITCH_PORT, SWITCH, GPIO_MODE_IN_PU_NO_IT );

  // ---------- CLK CONFIG ---------------
  CLK_DeInit();

  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1); // set 16 MHz for CPU
  
  // ---------- TIM CONFIG ---------------
  TIM4_DeInit();
  TIM2_DeInit();

  TIM2_TimeBaseInit(TIM2_PRESCALER_1, 999);
  TIM2_TimeBaseInit(TIM2_PRESCALER_2, 999);
  TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE,500, TIM2_OCPOLARITY_HIGH);
  TIM2_OC2Init(TIM2_OCMODE_PWM2, TIM2_OUTPUTSTATE_ENABLE,500, TIM2_OCPOLARITY_LOW);
  TIM2_OC1PreloadConfig(ENABLE);
  TIM2_OC2PreloadConfig(ENABLE);
  
  TIM2_ARRPreloadConfig(ENABLE);

  TIM2_Cmd(ENABLE);

  enableInterrupts();
  
  srand(500);
  
  int count3 = 1;
  int count4 = 0; 
  int mode = 0;
 int i;
  for(;;)
  {
    if(!GPIO_ReadInputPin(SWITCH_PORT, SWITCH)){
    	mode++;
		delay_ms(250);
    }
	 int random1 = rand() % 999;
	 int random2 = rand() % 999;
	 int rand_delay = rand() % 150;
  	switch(mode) {
 		case 0:
	    delay_ms(rand_delay);

	    TIM2_SetCompare1(random1);
	    TIM2_SetCompare2(random2);
 		break;
 		
 		case 1:
 		case 2:
 		case 3:
 		case 4:
 		case 5:
 		case 6:
 		case 7:
 		case 8:
 		case 9:
	    TIM2_SetCompare1(100 * mode);
	    TIM2_SetCompare2(100 * mode);
	   break;

	   case 10:
	    if(count3 >= 1 && count3 <= 6) {
	 		count3++;
	 		if(count3 % 2) {
	   		TIM2_SetCompare1(999);
	   	 	TIM2_SetCompare2(999);
	 		}
	 		else {
	   		TIM2_SetCompare1(1);
	   	 	TIM2_SetCompare2(1);
	 		}
			delay_ms(250);
	 		//continue;
	 	 }
	 	 if(count3 > 6) {
	 	 	count3 = 0;
	   	TIM2_SetCompare1(999);
	   	TIM2_SetCompare2(999);
	  	 }
		break;
		
	   case 11:
		count3 = 1;
		delay_ms(250);
	   if(count4 == 0) {
	   	count4 = 1;
	   	TIM2_SetCompare1(1);
	   	TIM2_SetCompare2(999);
	   }
	   else {
	   	count4 = 0;
	   	TIM2_SetCompare1(999);
	   	TIM2_SetCompare2(1);
	   }
	   break;
	   
	   case 12:
			delay_ms(250);
		   if(count4 == 0) {
		   	count4 = 1;
		   	TIM2_SetCompare1(999);
		   	TIM2_SetCompare2(999);
		   }
		   else {
		   	count4 = 0;
		   	TIM2_SetCompare1(1);
		   	TIM2_SetCompare2(1);
		   }
	   break;

		case 13:	
				rand_delay = rand() % 1050;
				if(rand_delay > 500) {
			  		 delay_ms(rand_delay); 
				}
			   if(random1 > 100 && random1 < 200){
			   	if(random1 % 200) {
			   	//for(i=random1; i<250; i--) {
		   			TIM2_SetCompare1(random1);
		   		//}	
		   		}
			   	if(random2 % 200) {
			   	//for(i=random2; i<250; i--) {
		   			TIM2_SetCompare2(random2);
		   		//}	
		  		 }
		  	}
		  	
	   break;
	   case 14:
	   	mode = 0;
	   break;
	}
  }
}