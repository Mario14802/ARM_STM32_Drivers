/*
 * Stm32_UART.h
 *
 *  Created on: Sep 27, 2023
 *      Author: MARIO
 */

#ifndef INC_STM32_USART_H_
#define INC_STM32_USART_H_

#include "Stm32f103x6.h"
#include "Stm32_GPIO.h"
#include "Stm32_EXTI.h"
#include "Stm32_RCC.h"
#include <stdint.h>


//config struct this stuct are for the user input



typedef struct {

	uint8_t USART_MODE;

	uint32_t BaudRate ;

	uint8_t Payload_Length;

	uint8_t Parity;

	uint8_t StopBits;

	uint8_t HwFlowCtl;

	uint8_t IRQ_Enable;

	void(*P_IRQ_CallBack)(void);

}UART_Config;

//REF MACROS

//UART MODE Define
#define Uart_Mode_Rx   			   (uint32_t)(1<<2)
#define Uart_Mode_Tx               (uint32_t)(1<<3) //bit 3 TE
#define Uart_Mode_Tx_Rx     	   ((uint32_t)(1<<2 | 1<<3))

//baud rate
#define UART_BAUDRate_2400				2400
#define UART_BAUDRate_9600				9600
#define UART_BAUDRate_19200				19200
#define UART_BAUDRate_57600				57600
#define UART_BAUDRate_115200			115200
#define UART_BAUDRate_230400			230400
#define UART_BAUDRate_460800			460800
#define UART_BAUDRate_921600			921600
#define UART_BAUDRate_2250000			2250000
#define UART_BAUDRate_4500000			4500000

//payload ref

#define UART_Payload_Length_8B			(uint32_t)(0)
#define UART_Payload_Length_9B			(uint32_t)(1<<12)

//parity ref
#define UART_PARITY_NONE      ((uint32_t)(0))
#define UART_PARITY_EVEN      ((uint32_t)(1<<10))
#define UART_PARITY_ODD       ((uint32_t)(1<<10 | 1<<9))


//UART stop bits
#define UART_StopBits_half		    ((uint32_t)(1<<12))
#define UART_StopBits_1			    ((uint32_t)(0))
#define UART_StopBits_1_half		((uint32_t)(3<<12))
#define UART_StopBits_2		        ((uint32_t)(2<<12))


//UART HwFlow

#define UART_HwFlowCtl_NONE					((uint32_t)(0))
#define UART_HwFlowCtl_RTS					((uint32_t)(1<<8))
#define UART_HwFlowCtl_CTS					((uint32_t)(1<<9))
#define UART_HwFlowCtl_RTS_CTS				((uint32_t)(1<<8 | 1<<9))

//UART_IRQ ENABLE
#define UART_IRQ_ENABLE_NONE				((uint32_t)(0))
#define UART_IRQ_ENABLE_TXE					((uint32_t)(1<<7))
#define UART_IRQ_ENABLE_TC					((uint32_t)(1<<6))
#define UART_IRQ_ENABLE_RXNEIE				((uint32_t)(1<<5))
#define UART_IRQ_ENABLE_PE					((uint32_t)(1<<8))


#define USARTDIV(_PCLK_, _BAUD_)					((uint32_t)(_PCLK_ / (16 * _BAUD_)))
#define USARTDIV_MUL100(_PCLK_, _BAUD_)				((uint32_t)((25 * _PCLK_) / (4 * _BAUD_)))
#define Mantissa_MUL100(_PCLK_, _BAUD_)				((uint32_t)(USARTDIV(_PCLK_, _BAUD_) * 100))
#define Mantissa(_PCLK_, _BAUD_)					((uint32_t)(USARTDIV(_PCLK_, _BAUD_)))
#define DIV_Fraction(_PCLK_, _BAUD_)				((uint32_t)(((USARTDIV_MUL100(_PCLK_, _BAUD_) - Mantissa_MUL100(_PCLK_, _BAUD_)) * 16) / 100))
#define UART_BRR_Register(_PCLK_, _BAUD_)			(((Mantissa(_PCLK_, _BAUD_)) << 4) | ((DIV_Fraction(_PCLK_, _BAUD_)) & 0xF))




typedef enum {
    enable,
    disable
} PollingMechanism;




void MCAL_UART_init(USART_TypeDef*USARTx, UART_Config* UART_Config);
void MCAL_UART_Deinit(USART_TypeDef*USARTx);

void MCAL_UART_GPIO_SET_PINS(USART_TypeDef*USARTx);

void MCAL_UARt_SendData(USART_TypeDef*USARTx,uint16_t *pTxBuffer,PollingMechanism PollingEn);

void MCAL_UARt_RecieveData(USART_TypeDef*USARTx,uint16_t *pRxBuffer, PollingMechanism PollingEn);

void MCAL_UART_WAIT_TC(USART_TypeDef * USARTx);


#endif /* INC_STM32_USART_H_ */
