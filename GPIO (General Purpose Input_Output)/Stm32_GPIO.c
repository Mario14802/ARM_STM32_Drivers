/*
 * Stm32_GPIO_driver.c
 *
 *  Created on: Aug 24, 2023
 *      Author: MARIO
 */


#include "Stm32_GPIO.h"

uint8_t GET_CRLH_POSITION(uint16_t PINNumber)
{
	switch (PINNumber)
	{
	case GPIO_PIN_0:
		return 0;
		break;

	case GPIO_PIN_1:
		return 4;
		break;

	case GPIO_PIN_2:
		return 8;
		break;

	case GPIO_PIN_3:
		return 12;
		break;

	case GPIO_PIN_4:
		return 16;
		break;

	case GPIO_PIN_5:
		return 20;
		break;

	case GPIO_PIN_6:
		return 24;
		break;

	case GPIO_PIN_7:
		return 28;
		break;

	case GPIO_PIN_8:
		return 0;
		break;

	case GPIO_PIN_9:
		return 4;
		break;

	case GPIO_PIN_10:
		return 8;
		break;

	case GPIO_PIN_11:
		return 12;
		break;

	case GPIO_PIN_12:
		return 16;
		break;

	case GPIO_PIN_13:
		return 20;
		break;

	case GPIO_PIN_14:
		return 24;
		break;

	case GPIO_PIN_15:
		return 28;
		break;


	}
	return 0;
}

void MCAL_GPIO_init(GPIO_Type_Def* GPIOx, GPIO_PinConfig* PinConfig) {
	// Determine the configuration register based on pin number
	volatile uint32_t* configregister = (PinConfig->GPIO_PinNumber < GPIO_PIN_8) ? &GPIOx->CRL : &GPIOx->CRH;

	// Clear the configuration bits for the pin
	(*configregister) &= ~(0xf << GET_CRLH_POSITION(PinConfig->GPIO_PinNumber));

	// Set pin configuration based on mode
	uint8_t Pin_Config = 0;

	switch (PinConfig->GPIO_MODE) {
	case GPIO_MODE_OUTPUT_AF_OD:
	case GPIO_MODE_OUTPUT_AF_PP:
	case GPIO_MODE_OUTPUT_OD:
	case GPIO_MODE_OUTPUT_PP:
		// Output mode configurations
		Pin_Config = (((PinConfig->GPIO_MODE - 4) << 2) | (PinConfig->GPIO_OUTPUT_SPEED)) & 0x0f;
		break;

	case GPIO_MODE_ANALOGE:
		// Analog input mode configuration
		Pin_Config = (PinConfig->GPIO_MODE << 2) & 0x0f;
		break;


	case GPIO_MODE_INPUT_PD:
		Pin_Config = ((GPIO_MODE_INPUT_PD << 2) | 0b1000) & 0x0f;
		GPIOx->ODR &= ~(PinConfig->GPIO_PinNumber);
		break;

	case GPIO_MODE_INPUT_PU:
		Pin_Config = ((GPIO_MODE_INPUT_PU << 2) | 0b1000) & 0x0f;
		GPIOx->ODR |= PinConfig->GPIO_PinNumber;
		break;

	case GPIO_MODE_INPUT_FLO:
		// Input mode configurations with pull-down/up or floating
		Pin_Config = ((GPIO_MODE_INPUT_FLO << 2) | 0b0100) & 0x0f;
		break;

	default:
		// Handle unsupported modes or additional custom configurations
		break;
	}

	// Apply the pin configuration to the register
	(*configregister) |= (Pin_Config << GET_CRLH_POSITION(PinConfig->GPIO_PinNumber));
}
void MCAL_GPIO_Deinit(GPIO_Type_Def* GPIOx )

{
	if(GPIOx == GPIOA)
	{
		RCC->APB2RSTR |= (1<<2);
		RCC->APB2RSTR &= ~(1<<2);
	}

	if(GPIOx == GPIOB)
	{
		RCC->APB2RSTR |= (1<<3);
		RCC->APB2RSTR &= ~(1<<3);
	}
	if(GPIOx == GPIOC)
	{
		RCC->APB2RSTR |= (1<<4);
		RCC->APB2RSTR &= ~(1<<4);
	}
	if(GPIOx == GPIOD)
	{
		RCC->APB2RSTR |= (1<<5);
		RCC->APB2RSTR &= ~(1<<5);
	}
	if(GPIOx == GPIOE)
	{
		RCC->APB2RSTR |= (1<<6);
		RCC->APB2RSTR &= ~(1<<6);
	}


}


uint8_t MCAL_GPIO_Read_Pin  (GPIO_Type_Def* GPIOx ,uint16_t PinNumber)

{
	uint8_t  bitstatus;
	if (((GPIOx->IDR)& PinNumber )!= (uint32_t) GPIO_PIN_RESET)
	{
		bitstatus = GPIO_PIN_SET;
	}
	else
	{
		bitstatus = GPIO_PIN_RESET;
	}
	return bitstatus;
}





uint16_t MCAL_GPIO_Read_Port(GPIO_Type_Def* GPIOx)
{
	uint8_t  port_value;
	port_value = (uint16_t) (GPIOx->IDR);
	return port_value;

}


//WRITE APIS
void MCAL_GPIO_WritePin(GPIO_Type_Def* GPIOx ,uint16_t PinNumber,uint16_t Value)
{

	if (Value == GPIO_PIN_SET)
	{
		GPIOx->ODR |= (1<<PinNumber);
	}
	else
	{
		GPIOx->ODR &= ~(1<<PinNumber);
	}
}


void MCAL_GPIO_WritePinBSRR(GPIO_Type_Def* GPIOx ,uint16_t PinNumber,uint16_t Value)
{

	if ( Value == GPIO_PIN_RESET)
	{
		GPIOx->BSRR = (uint32_t)PinNumber;


	}
	else
	{
		GPIOx->BRR = (uint32_t)PinNumber;

	}

}

void MCAL_GPIO_WritePort(GPIO_Type_Def* GPIOx ,uint16_t value)
{
	GPIOx->ODR |=(uint32_t)value;
}

void MCAL_GPIO_TogglePin(GPIO_Type_Def* GPIOx,uint16_t PinNumber)
{
	GPIOx->ODR ^= PinNumber;
}

uint8_t MCAL_GPIO_LockPin(GPIO_Type_Def* GPIOx,uint16_t PinNumber)
{
	volatile uint32_t tmp = 1<<16;
	//set the lcky
	tmp |= PinNumber;

	GPIOx->LCKR = tmp;

	GPIOx->LCKR = PinNumber;

	GPIOx->LCKR = tmp;

	tmp = GPIOx->LCKR;

	if ((uint32_t)(GPIOx->LCKR & 1<<16 ))
	{
		return GPIO_RETURN_LOCK_ENABLED;
	}
	else
	{
		return GPIO_RETURN_LOCK_ERROR;
	}



}
