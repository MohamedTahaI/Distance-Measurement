/******************************************************************************
 *
 * Module: Distance Measurement
 *
 * File Name: App.c
 *
 * Description: Source file for the App
 *
 * Author : Ahmed Gamal
 *
 *******************************************************************************/
# include "ultrasonic.h"
# include "icu.h"
# include "lcd.h"
# include <avr/interrupt.h> /* for ICU ISR*/
# include "common_macros.h"

int main()
{
	uint16 distance = 0;

	/* Enable I-BIT*/
	SET_BIT(SREG,7);

	LCD_init(); /* initialize LCD driver*/
	Ultrasonic_init(); /* initialize Ultrasonic driver*/
	LCD_displayString("Distance =    cm");

	while(1)
	{
		/* read the Distance*/
		distance = Ultrasonic_readDistance();
		/* Display the temperature value every time at same position */
		LCD_moveCursor(0,10);
		if(distance >= 100)
		{
			LCD_intgerToString(distance);
		}
		else
		{
			LCD_intgerToString(distance);
			/* In case the digital value is two or one digits print space in the next digit place */
			LCD_displayCharacter(' ');
		}
	}
}
