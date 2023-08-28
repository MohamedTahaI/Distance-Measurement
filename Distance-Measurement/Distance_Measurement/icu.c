/******************************************************************************
 *
 * Module: ICU
 *
 * File Name: icu.h
 *
 * Description: Header file for the AVR ICU driver
 *
 * Author: Ahmed Gamal
 *
 *******************************************************************************/
# include "common_macros.h"
# include "icu.h"
# include "gpio.h"
# include <avr/interrupt.h> /* For ICU ISR*/
# include <avr/io.h> /* For ICU & Timer1 Registers*/

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the callback function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;
/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR (TIMER1_CAPT_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the callBack Function when the edge is detected*/
		(*g_callBackPtr) (); /* call the function using apointer to function */
	}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description: Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3. Enable the Input Capture Interrupt.
 * 	4. Initialize Timer1 Registers
 */
void ICU_init(const ICU_ConfigType * Config_Ptr)
{
	/* Set PIN6 in PORTD as input Pin*/
	GPIO_setupPinDirection(PORTD_ID, PIN6_ID,PIN_INPUT);
	/* Timer1 normale mode */
	SET_BIT(TCCR1A,FOC1A);
	SET_BIT(TCCR1A,FOC1B);
	/* Set the ICU Clock*/
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr ->clock);
	/* Set the required edge to detect */
	TCCR1B = (TCCR1B & 0xBF) | ((Config_Ptr->edge)<<6);
	/* Timer1 initial value */
	TCNT1 = 0 ;
	/* Initial Value for the input capture register */
	ICR1 = 0 ;
	/* Enable the Input Capture interrupt to generate an interrupt when the edge is detected on ICP1/PD6 pin */
	SET_BIT(TIMSK,TICIE1);
}

/*
 * Description: Function to set the Call Back function address.
 */
void ICU_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the CallBack function */
	g_callBackPtr = a_ptr ;
}
/*
 * Description: Function to set the required edge detection.
 */
void ICU_setEdgeDetectionType(const ICU_EdgeType a_edgeType)
{
	/* Set the required edge to detect */
	TCCR1B = (TCCR1B & 0xBF) | ((a_edgeType)<<6);
}
/*
 * Description: Function to get the Timer1 Value when the input is captured
 * The value stored at Input Capture Register ICR1
 */
uint16 ICU_getInputCaptureValue(void)
{
	return ICR1 ;
}

/*
 * Description: Function to clear the Timer1 Value to start counting from ZERO
 */
void ICU_clearTimerValue(void)
{
	TCNT1 = 0 ;
}
/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void ICU_deInit(void)
{
	TCCR1A = 0 ;
	TCCR1B = 0 ;
	TCNT1 = 0;
	ICR1 = 0;
	/* Disable the Input Capture interrupt */
	CLEAR_BIT(TIMSK,TICIE1);
	/* Reset the global pointer value */
	g_callBackPtr = NULL_PTR ;
}
