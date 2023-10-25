/**
 * @file port.c
 * @version 1.0
 * @date 24/05/2023
 * @brief File with PORT module implementations for the Kinetis KL25 family.
 * @author Matheus Leitzke Pinto
 */

#include "port.h"

/*!
 * @addtogroup port driver
 * @{
 */

/*******************************************************************************
 * Code
 ******************************************************************************/

/**********************************************************************************/
void PORT_Init(PORT_Type* base)
{
	SYSTEM_ASSERT(base);

	unsigned int portIndex =  ( (unsigned int)((unsigned int)base - (unsigned int)PORTA ) / 0x1000U ) + 9;

	SIM->SCGC5 |= 1 << portIndex;
}

/**********************************************************************************/
void PORT_EnableIRQ(PORT_Type *base, uint8_t pin, portIRQ_t irq)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( pin < 32 );

	base->PCR[pin] &= ~PORT_PCR_IRQC_MASK; /*Limpa configurações anteriores*/
	base->PCR[pin] |= PORT_PCR_IRQC(irq);
}

/**********************************************************************************/
uint32_t PORT_GetIRQFlag(PORT_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( pin < 32 );

	return base->ISFR & ((uint32_t)1 << pin);
}

/**********************************************************************************/
void PORT_ClearIRQFlag(PORT_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( pin < 32 );

	base->ISFR = ( (uint32_t) 1 << pin );
}

/**********************************************************************************/
void PORT_EnablePull(PORT_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( pin < 32 );

	base->PCR[pin] |= PORT_PCR_PE_MASK;
}

/**********************************************************************************/
void PORT_EnableHighStrength(PORT_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT(pin < 32);

	base->PCR[pin] |= PORT_PCR_DSE_MASK;
}

/**********************************************************************************/
void PORT_SetMux(PORT_Type *base, uint8_t pin, portMux_t mux)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( pin < 32 );

	base->PCR[pin] &= ~PORT_PCR_MUX_MASK; /*Limpa configurações anteriores*/
	base->PCR[pin] |= PORT_PCR_MUX(mux);
}

/*! @}*/
