/*
 * Stm32_EXTI.c
 *
 *  Created on: Sep 19, 2023
 *      Author: MARIO
 */


#include "Stm32_EXTI.h"

void (*GP_IRQ_CallBack[15])(void);


#define AFIO_GPIO_EXTI_MAPPING(x) ( (x==GPIOA)?0:\
									(x==GPIOB)?1:\
									(x==GPIOC)?2:\
									(x==GPIOD)?3:0    )


void Enable_NVIC(uint16_t IRQ)
{
	switch(IRQ)
	{
	case 0 :
		NVIC_IRQ6_EXTI0_EN ;
		break;
	case 1 :
		NVIC_IRQ7_EXTI1_EN ;
		break;
	case 2 :
		NVIC_IRQ8_EXTI2_EN ;
		break;
	case 3 :
		NVIC_IRQ9_EXTI3_EN ;
		break;
	case 4 :
		NVIC_IRQ10_EXTI4_EN ;
		break;
	case 5 :
	case 6 :
	case 7 :
	case 8 :
	case 9 :
		NVIC_IRQ23_EXTI5_9_EN;
		break;
	case 10 :
	case 11 :
	case 12 :
	case 13 :
	case 14 :
	case 15 :
		NVIC_IRQ40_EXTI10_15_EN;
		break;
	}
}

void Disable_NVIC(uint16_t IRQ)
{
	switch(IRQ)
	{
	case 0 :
		NVIC_IRQ6_EXTI0_DIS ;
		break;
	case 1 :
		NVIC_IRQ7_EXTI1_DIS ;
		break;
	case 2 :
		NVIC_IRQ8_EXTI2_DIS ;
		break;
	case 3 :
		NVIC_IRQ9_EXTI3_DIS ;
		break;
	case 4 :
		NVIC_IRQ10_EXTI4_DIS ;
		break;
	case 5 :
	case 6 :
	case 7 :
	case 8 :
	case 9 :
		NVIC_IRQ23_EXTI5_9_DIS;
		break;
	case 10 :
	case 11 :
	case 12 :
	case 13 :
	case 14 :
	case 15 :
		NVIC_IRQ40_EXTI10_15_DIS;
		break;
	}
}

void Update_EXTI(EXTI_PinConfig_t* EXTI_Config)
{
	//1.config gpio
	GPIO_PinConfig PinCfg;

	PinCfg.GPIO_PinNumber=EXTI_Config->EXTI_PIN.GPIO_PIN;
	PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_init( EXTI_Config->EXTI_PIN.GPIO_PORT, &PinCfg);

	//UPDATAE ROUTE
	uint8_t AFIO_EXTICR_index = (EXTI_Config->EXTI_PIN.ETI_InputLineNumber/4);
	uint8_t AFIO_EXTICR_Position = (EXTI_Config->EXTI_PIN.ETI_InputLineNumber % 4) * 4;
	//clear four bit
	AFIO->EXTICR[AFIO_EXTICR_index] &= ~(0xF<<AFIO_EXTICR_Position);
	AFIO->EXTICR[AFIO_EXTICR_index] |= ((AFIO_GPIO_EXTI_MAPPING(EXTI_Config->EXTI_PIN.GPIO_PORT)&0xF)<<AFIO_EXTICR_Position);

	//UPDATE RISING AND FALLING REG
	EXTI->RTSR  &= ~( 1<< EXTI_Config->EXTI_PIN.ETI_InputLineNumber);
	EXTI->FTSR  &= ~( 1<< EXTI_Config->EXTI_PIN.ETI_InputLineNumber);


	if( EXTI_Config->Trigger_case==EXTI_Trigger_Rising)
	{
		EXTI->RTSR  |= ( 1<< EXTI_Config->EXTI_PIN.ETI_InputLineNumber);
	}
	else if( EXTI_Config->Trigger_case==EXTI_Trigger_Falling)
	{
		EXTI->FTSR  |= ( 1<< EXTI_Config->EXTI_PIN.ETI_InputLineNumber);
	}
	else if( EXTI_Config->Trigger_case==EXTI_Trigger_Rising_Falling)
	{
		EXTI->RTSR  |= ( 1<< EXTI_Config->EXTI_PIN.ETI_InputLineNumber);
		EXTI->FTSR  |= ( 1<< EXTI_Config->EXTI_PIN.ETI_InputLineNumber);
	}

	//UPDATE IRQ HANDLLING CALLBACK

	GP_IRQ_CallBack[EXTI_Config->EXTI_PIN.ETI_InputLineNumber]=EXTI_Config->P_IRQ_CallBack;

	//ENABLE /DISABLE IRQ
	if (EXTI_Config->IRQ_EN == EXTI_IRQ_EN )
	{
		EXTI->IMR |= (1<<EXTI_Config->EXTI_PIN.ETI_InputLineNumber);
		Enable_NVIC(EXTI_Config->EXTI_PIN.ETI_InputLineNumber);
	}
	else
	{
		EXTI->IMR &= ~(1<<EXTI_Config->EXTI_PIN.ETI_InputLineNumber);
		Disable_NVIC(EXTI_Config->EXTI_PIN.ETI_InputLineNumber);
	}

}


void MCAL_EXTI_GPIO_init (EXTI_PinConfig_t * EXTI_Config)
{
	Update_EXTI(EXTI_Config);


}
void MCAL_EXTI_GPIO_Deinit (void)
{
	EXTI->IMR    = 0x00000000;

	EXTI->EMR    = 0x00000000;

	EXTI->RTSR   = 0x00000000;

	EXTI->FTSR   = 0x00000000;

	EXTI->SWIER  = 0x00000000;

	EXTI->PR     = 0xFFFFFFFF;

	//DISABLE EXTI FROM NVIC TO DO SO YOU MUST GO TO THE DATA SHEET OF CORTEX-M3
	NVIC_IRQ6_EXTI0_DIS ;
	NVIC_IRQ7_EXTI1_DIS	;
	NVIC_IRQ8_EXTI2_DIS ;
	NVIC_IRQ9_EXTI3_DIS ;
	NVIC_IRQ10_EXTI4_DIS;
	NVIC_IRQ23_EXTI5_9_DIS;
	NVIC_IRQ40_EXTI10_15_DIS;
}

void MCAL_EXTI_GPIO_Update (EXTI_PinConfig_t * EXTI_Config)
{
	Update_EXTI(EXTI_Config);
}



void EXTI0_IRQHandler(void)
{
	//Clear by writing 1 in EXTI_PR
	EXTI->PR |= (1<<0);
	GP_IRQ_CallBack[0]();
}

void EXTI1_IRQHandler(void)
{
	EXTI->PR |= (1<<1);
	GP_IRQ_CallBack[1]();
}
void EXTI2_IRQHandler(void)
{
	EXTI->PR |= (1<<2);
	GP_IRQ_CallBack[2]();
}
void EXTI3_IRQHandler(void)
{
	EXTI->PR |= (1<<3);
	GP_IRQ_CallBack[3]();
}
void EXTI4_IRQHandler(void)
{
	EXTI->PR |= (1<<4);
	GP_IRQ_CallBack[4]();
}
void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR & 1<<5){ EXTI->PR |= (1<<5)   ;   GP_IRQ_CallBack[5]() ; }
	if(EXTI->PR & 1<<6){ EXTI->PR |= (1<<6)   ;   GP_IRQ_CallBack[6]() ; }
	if(EXTI->PR & 1<<7){ EXTI->PR |= (1<<7)   ;   GP_IRQ_CallBack[7]() ; }
	if(EXTI->PR & 1<<8){ EXTI->PR |= (1<<8)   ;   GP_IRQ_CallBack[8]() ; }
	if(EXTI->PR & 1<<9){ EXTI->PR |= (1<<9)   ;   GP_IRQ_CallBack[9]() ; }
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR & 1<<10){ EXTI->PR |= (1<<10)   ;   GP_IRQ_CallBack[10]() ; }
	if(EXTI->PR & 1<<11){ EXTI->PR |= (1<<11)   ;   GP_IRQ_CallBack[11]() ; }
	if(EXTI->PR & 1<<12){ EXTI->PR |= (1<<12)   ;   GP_IRQ_CallBack[12]() ; }
	if(EXTI->PR & 1<<13){ EXTI->PR |= (1<<13)   ;   GP_IRQ_CallBack[13]() ; }
	if(EXTI->PR & 1<<14){ EXTI->PR |= (1<<14)   ;   GP_IRQ_CallBack[14]() ; }
	if(EXTI->PR & 1<<15){ EXTI->PR |= (1<<15)   ;   GP_IRQ_CallBack[15]() ; }
}

