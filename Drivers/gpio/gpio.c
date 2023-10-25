/**
 * @file gpio.c
 * @version 1.1
 * @date 25/10/2023
 * @brief File with GPIO module implementations for the Kinetis KL25 family.
 * @author Matheus Leitzke Pinto
 */

#include "gpio.h"

/*!
 * @addtogroup gpio driver
 * @{
 */

/*******************************************************************************
 * Code
 ******************************************************************************/

/**********************************************************************************/
void GPIO_InitInputPin(GPIO_Type *base, uint8_t pin)
{
	base->PDDR &= ~(1 << pin);
}

/**********************************************************************************/
void GPIO_InitOutputPin(GPIO_Type *base, uint8_t pin, uint8_t initVal)
{
	   base->PDDR |= (1 << pin);
	   if(initVal == 0)
	   {
		   base->PCOR = 1 << pin;
	   }
	   else
	   {
		   base->PSOR = 1 << pin;
	   }
}

/*! @}*/
