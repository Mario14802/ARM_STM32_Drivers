/*
 * TIMER.c
 *
 *  Created on: Apr 11, 2024
 *      Author: MARIO
 */


#include "TIMER.h"

void (*GP) (void);




void Enable_NVIC_TIM(uint32_t IRQ)
{
	switch(IRQ)
	{
	//GENERAL PURPOSE
	case TIM2_IRQ : NVIC_IRQ28_TIM2_EN ; break;
	case TIM3_IRQ : NVIC_IRQ29_TIM3_EN ; break;
	case TIM4_IRQ : NVIC_IRQ30_TIM4_EN ; break;
	case TIM5_IRQ : NVIC_IRQ50_TIM5_EN ; break;
	//BASIC TIMER
	case TIM6_IRQ : NVIC_IRQ54_TIM6_EN ; break;
	case TIM7_IRQ : NVIC_IRQ55_TIM7_EN ; break;
	//GENERAL PURPOSE
	case TIM9_IRQ : NVIC_IRQ24_TIM9_EN  ; break;
	case TIM10_IRQ :NVIC_IRQ25_TIM10_EN ; break;
	case TIM11_IRQ :NVIC_IRQ26_TIM11_EN ; break;
	case TIM12_IRQ :NVIC_IRQ43_TIM12_EN ; break;
	case TIM13_IRQ :NVIC_IRQ44_TIM13_EN ; break;
	case TIM14_IRQ :NVIC_IRQ45_TIM14_EN ; break;
	}
}

void Disable_NVIC_TIM(uint32_t IRQ)
{
	switch(IRQ)
	{
	//GENERAL PURPOSE
	case TIM2_IRQ : NVIC_IRQ28_TIM2_DIS ; break;
	case TIM3_IRQ : NVIC_IRQ29_TIM3_DIS ; break;
	case TIM4_IRQ : NVIC_IRQ30_TIM4_DIS ; break;
	case TIM5_IRQ : NVIC_IRQ50_TIM5_DIS ; break;
	//BASIC TIMER
	case TIM6_IRQ : NVIC_IRQ54_TIM6_DIS ; break;
	case TIM7_IRQ : NVIC_IRQ55_TIM7_DIS ; break;
	//GENERAL PURPOSE
	case TIM9_IRQ : NVIC_IRQ24_TIM9_DIS  ; break;
	case TIM10_IRQ :NVIC_IRQ25_TIM10_DIS ; break;
	case TIM11_IRQ :NVIC_IRQ26_TIM11_DIS ; break;
	case TIM12_IRQ :NVIC_IRQ43_TIM12_DIS ; break;
	case TIM13_IRQ :NVIC_IRQ44_TIM13_DIS ; break;
	case TIM14_IRQ :NVIC_IRQ45_TIM14_DIS ; break;
	}
}




void TIMER_update_PWM(TIMER_TypeDef* TIMX, Timer_pwm_Mode* PWM,uint16_t dutyCycle)
{

	switch (PWM->Channel) {

	case Channel_1:
		TIMX->CCR1 = dutyCycle;
		break;
	case Channel_2:
		TIMX->CCR2 = dutyCycle;
		break;
	case Channel_3:
		TIMX->CCR3= dutyCycle;
	case Channel_4:
		TIMX->CCR4 = dutyCycle;
		break;

	default:
		break;
	}

	// Trigger an update to apply the new duty cycle
	TIMX->EGR |= (1 << 0); // Set UG bit to update registers

}

TIMER_TypeDef* GET_TIMER_NUM(uint32_t Number)
{
	switch (Number)
	{
	case TIMER_1:
		return TIMER1;
		break;
	case TIMER_2:
		return TIMER2;
		break;
	case TIMER_3:
		return TIMER3;
		break;
	case TIMER_4:
		return TIMER4;
		break;
	case TIMER_5:
		return TIMER5;
		break;
	case TIMER_6:
		return TIMER6;
		break;
	case TIMER_7:
		return TIMER7;
		break;
	case TIMER_8:
		return TIMER8;
		break;
	case TIMER_9:
		return TIMER9;
		break;
	case TIMER_10:
		return TIMER10;
		break;
	case TIMER_11:
		return TIMER11;
		break;
	case TIMER_12:
		return TIMER12;
		break;
	case TIMER_13:
		return TIMER13;
		break;
	case TIMER_14:
		return TIMER14;
		break;
	default:
		return NULL;// invalid
		break;
	}
}

void TIMER_init(TIMER_TypeDef* TIMX, Timer_Config* CONFIG) {
	TIMX = GET_TIMER_NUM(CONFIG->timerNumber);


	// Disable the timer before configuration
	TIMX->CR1 &= ~(1 << 0); // Clear CEN bit

	// Configure the prescaler (PSC) and auto-reload (ARR)
	TIMX->PSC = CONFIG->prescaler;
	TIMX->ARR = CONFIG->AutoReload;


	TIMX->CCR1 &= ~(0b11 << 8);

	// Set bits 8 and 9 in CCMR1 register with the desired clock value (CK_INT, CK_2, CK_4)
	TIMX->CCR1 |= (CONFIG->ClockDiv << 8);

	// Enable auto-reload preload
	TIMX->CR1 |= (1 << 7); // Set ARPE bit

	// Enable the update event (to apply the new settings)
	// TIMX->EGR |= TIM_EGR_UG;

	// Enable the timer
	TIMX->CR1 |= (1 << 0); // Set CEN bit to start the timer

	TIMX->DIER |= (1<<0);

	GP=CONFIG->callback;
	if(CONFIG->interruptEnable==INT_EN)
	{
		Enable_NVIC_TIM(CONFIG->IRQ);
	}
	else
	{
		Disable_NVIC_TIM(CONFIG->IRQ);
	}




}


void TIMER_up_down_counting_mode(TIMER_TypeDef* TIMX, uint32_t Mode)
{
	// Configure timer mode based on the specified mode
	switch (Mode ) {

	case TIMER_MODE_UP_COUNTER:
		// Set timer mode to UP-counting (DIR bit cleared for UP-counting mode)
		TIMX->CR1 &= ~(1 << 4); // Clear DIR bit
		break;

	case TIMER_MODE_Down_COUNTER:
		// Set timer mode to DOWN-counting
		TIMX->CR1 |= (1 << 4); // Set DIR bit
		break;


	}

}




void TIMER_PWM(TIMER_TypeDef* TIMX, Timer_pwm_Mode* PWM)
{
	switch (PWM->Channel) {
	case Channel_1:
		TIMX->CCMR1 |= (1<<3);/// enable output preload OC1PE
		if ((PWM->mode)==PWM_Mode_2){
			// Configure Channel 1 for PWM Mode 1
			TIMX->CCMR1 &= ~(0b111 << 4); //clear it first 0C1M
			TIMX->CCMR1 |= (0b110 <<4);
		}// Set OC1M bits for PWM Mode 1
		else
		{
			TIMX->CCMR1 &= ~(0b111 << 4); //clear it first
			TIMX->CCMR1 |= (0b111<<4);//Set OC1M bits for PWM Mode 2
		}
		/////POLARITY
		if (PWM->Polarity == ACTIVE_HIGH)
		{
			// Set CC1P = 0 for active high
			TIMX->CCER &= ~(1 << 1); // Clear CC1P bit
		}
		else if (PWM->Polarity == ACTIVE_LOW)
		{
			// Set CC1P = 1 for active low
			TIMX->CCER |= (1 << 1); // Set CC1P bit
		}

		TIMX->CCER |= (1<<0); //ENABLE COMPARE/OUPUT 1 CC1E
		TIMX->CCMR1 |= (0b00<<0);//enable CCXS AS output CC1S
		TIMX->CCR1 = (((PWM->Duty_Cycle)*(TIMX->ARR))/100);

		break;

	case Channel_2:
		TIMX->CCMR1 |= (1<<11);/// enable output preload
		// Configure Channel 2 for PWM Mode 1
		if ((PWM->mode)==PWM_Mode_2){
			// Configure Channel 1 for PWM Mode 1
			TIMX->CCMR1 &= ~(0b111 << 12); //clear it first 0C1M
			TIMX->CCMR1 |= (0b110 << 12);
		} // Set OC1M bits for PWM Mode 1
		else
		{
			TIMX->CCMR1 &= ~(0b111 << 12); //clear it first 0C1M
			TIMX->CCMR1 |= (0b111 << 12);
		}//Set OC1M bits for PWM Mode 2
		/////POLARITY
		if (PWM->Polarity == ACTIVE_HIGH)
		{
			// Set CC1P = 0 for active high
			TIMX->CCER &= ~(1 << 5); // Clear CC1P bit
		}
		else if (PWM->Polarity == ACTIVE_LOW)
		{
			// Set CC1P = 1 for active low
			TIMX->CCER |= (1 << 5); // Set CC1P bit
		}
		TIMX->CCER |= (1<<4); //ENABLE COMPARE/OUPUT 2
		TIMX->CCMR1 |= (0b00<<8); //enable CCXS AS output

		TIMX->CCR2 = (((PWM->Duty_Cycle)*(TIMX->ARR))/100);  //DUTY CYCLE DETERMINATION
		break;

	case Channel_3:
		TIMX->CCMR2 |= (1<<3);/// enable output preload
		if ((PWM->mode)==PWM_Mode_2)
		{
			// Configure Channel 1 for PWM Mode 1
			TIMX->CCMR2 &= ~(0b111 << 4); //clear it first 0C1M
			TIMX->CCMR2 |= (0b110 << 4);
		} // Set OC1M bits for PWM Mode 1
		else
		{
			TIMX->CCMR2 &= ~(0b111 << 4); //clear it first 0C1M
			TIMX->CCMR2 |= (0b111 << 4);//Set OC1M bits for PWM Mode 2
		}
		/////POLARITY
		if (PWM->Polarity == ACTIVE_HIGH)
		{
			// Set CC1P = 0 for active high
			TIMX->CCER &= ~(1 << 9); // Clear CC1P bit
		}
		else if (PWM->Polarity == ACTIVE_LOW)
		{
			// Set CC1P = 1 for active low
			TIMX->CCER |= (1 << 9); // Set CC1P bit
		}
		TIMX->CCER |= (1<<8); //ENABLE COMPARE/OUPUT 3

		TIMX->CCMR2 |= (0b00<<0); //enable CCXS AS output

		TIMX->CCR3 = (((PWM->Duty_Cycle)*(TIMX->ARR))/100);  //DUTY CYCLE DETERMINATION
		break;

	case Channel_4:
		TIMX->CCMR2 |= (1<<11);/// enable output preload
		// Configure Channel 4 for PWM Mode 3
		if ((PWM->mode)==PWM_Mode_2)
		{
			// Configure Channel 1 for PWM Mode 1
			TIMX->CCMR2 &= ~(0b111 << 12); //clear it first 0C1M
			TIMX->CCMR2 |= (0b110 << 12);
		} // Set OC1M bits for PWM Mode 1
		else
		{
			TIMX->CCMR2 &= ~(0b111 << 12); //clear it first 0C1M
			TIMX->CCMR2 |= (0b111 << 12);
		}
		/////POLARITY
		if (PWM->Polarity == ACTIVE_HIGH)
		{
			// Set CC1P = 0 for active high
			TIMX->CCER &= ~(1 << 13); // Clear CC1P bit
		} else if (PWM->Polarity == ACTIVE_LOW)
		{
			// Set CC1P = 1 for active low
			TIMX->CCER |= (1 << 13); // Set CC1P bit
		}

		TIMX->CCER |= (1<<12); //ENABLE COMPARE/OUPUT 4

		TIMX->CCMR2 |= (0b00<<8); //enable CCXS AS output

		TIMX->CCR4 = (((PWM->Duty_Cycle)*(TIMX->ARR))/100);  //DUTY CYCLE DETERMINATION

		break;

	default:
		// Handle unsupported channel
		break;
	}


}




void TIM2_IRQHandler()
{
	TIMER2->SR=~0x01;//update interrupt flag
	GP();
}
void TIM3_IRQHandler()
{
	TIMER3->SR=~0x01;//update interrupt flag
	GP();
}
void TIM4_IRQHandler()
{
	TIMER4->SR=~0x01;//update interrupt flag
	GP();
}
void TIM9_IRQHandler()
{
	TIMER9->SR=~0x01;//update interrupt flag
	GP();
}
void TIM10_IRQHandler()
{
	TIMER10->SR=~0x01;//update interrupt flag
	GP();
}
void TIM11_IRQHandler()
{
	TIMER11->SR=~0x01;//update interrupt flag
	GP();
}
void TIM12_IRQHandler()
{
	TIMER12->SR=~0x01;//update interrupt flag
	GP();
}

