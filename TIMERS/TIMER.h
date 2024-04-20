/*
 * TIMER.h
 *
 *  Created on: Apr 11, 2024
 *      Author: MARIO
 */

#ifndef MCAL_TIMER_TIMER_H_
#define MCAL_TIMER_TIMER_H_

#endif /* MCAL_TIMER_TIMER_H_ */

//includes
#include "../Stm32f103C8.h"

//-----------------------------
//User type definitions (structures)
//-----------------------------

typedef struct
{
	uint32_t timerNumber; // Pointer to the TIM peripheral (e.g., TIM1, TIM2, ...)
    uint32_t prescaler;   // Timer prescaler value
    uint32_t ClockDiv;
    uint32_t AutoReload;      // Auto-reload value (timer period)
    uint32_t interruptEnable; // Flag to enable/disable timer interrupts
    uint32_t IRQ; // Flag to enable/disable timer interrupts
    void (*callback)(void);  // Pointer to the user-defined interrupt callback function
} Timer_Config;

//FAST PWM MODE
typedef struct
{
	uint32_t Channel;//channel
	uint32_t mode;
	uint32_t Polarity;//edge selector
	//uint32_t EN_event;//enable or disable event
	//uint32_t Alignment; //select which align if it is center or edge or non
	uint32_t Duty_Cycle;
}Timer_pwm_Mode;

/////////////////// configS////////////////////////////////////

//TIMER CONFIG
#define TIMER_1						1
#define TIMER_2						2
#define TIMER_3						3
#define TIMER_4						4
#define TIMER_5						5
#define TIMER_6						6
#define TIMER_7						7
#define TIMER_8						8
#define TIMER_9						9
#define TIMER_10					10
#define TIMER_11					11
#define TIMER_12					12
#define TIMER_13					13
#define TIMER_14					14


#define TIMER_EN	1
#define TIMER_DIS	0


//timer modes
#define TIMER_MODE_Down_COUNTER		0
#define TIMER_MODE_UP_COUNTER		1
#define TIMER_MODE_PWM				2
#define TIMER_MODE_INPUT_CAPTURE	3

//CLOCK CONFIG

#define CK			0b00
#define	CK_2			0b01
#define	CK_4			0b10

//INT ENABLE

#define INT_EN   1
#define INT_DIS  0

//IRQ
#define TIM2_IRQ 	0
#define TIM3_IRQ 	1
#define TIM4_IRQ 	2
#define TIM5_IRQ 	3

#define TIM6_IRQ 	4
#define TIM7_IRQ 	5

#define TIM9_IRQ 	6
#define TIM10_IRQ 	7
#define TIM11_IRQ 	8
#define TIM12_IRQ	9
#define TIM13_IRQ 	10
#define TIM14_IRQ 	11

//CHANNEL config
#define Channel_1                   1
#define Channel_2	 				2
#define Channel_3	 				3
#define Channel_4	 				4

//Channel modes
#define PWM_Mode_1					0  //Simply  speed increase by increasing duty cycle
#define PWM_Mode_2					1  //Simply  speed Decrease by decreasing duty cycle
//POLARITY
#define ACTIVE_HIGH     0
#define ACTIVE_LOW      1


//////////////////////////////////////////FUNCTIONS/////////////////////////////////////////////
void TIMER_init(TIMER_TypeDef* TIMx, Timer_Config* Config);
void TIMER_up_down_counting_mode(TIMER_TypeDef* TIMX, uint32_t Mode);
void TIMER_PWM(TIMER_TypeDef* TIMX,Timer_pwm_Mode*PWM );
void TIMER_update_PWM(TIMER_TypeDef* TIMX, Timer_pwm_Mode* PWM,uint16_t dutyCycle);


