/**
 * @file gpio.h
 * @version 1.1
 * @date 25/10/2023
 * @brief File with GPIO module implementations for the Kinetis KL25 family.
 * @author Matheus Leitzke Pinto
 */

#ifndef GPIO_DRV_H_
#define GPIO_DRV_H_

#include <common.h>
#include <Drivers/port/port.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @addtogroup gpio driver
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Set the specified pin of the GPIO module to logic high (1).
 * @param base Pointer to the GPIO module's base register.
 * @param pin Pin number to set.
 */
static inline void GPIO_SetPin(GPIO_Type *base, uint8_t pin);

/**
 * @brief Clear the specified pin of the GPIO module to logic low (0).
 * @param base Pointer to the GPIO module's base register.
 * @param pin Pin number to clear.
 */
static inline void GPIO_ClearPin(GPIO_Type *base, uint8_t pin);

/**
 * @brief Toggle the specified pin of the GPIO module.
 * @param base Pointer to the GPIO module's base register.
 * @param pin Pin number to toggle.
 */
static inline void GPIO_TogglePin(GPIO_Type *base, uint8_t pin);

/**
 * @brief Write a logical value to the specified pin of the GPIO module.
 * @param base Pointer to the GPIO module's base register.
 * @param pin Pin number to write to.
 * @param value Logical value (0 or 1) to write.
 */
static inline void GPIO_WritePin(GPIO_Type *base, uint8_t pin, uint8_t value);

/**
 * @brief Set multiple pins of the GPIO module using a mask.
 * @param base Pointer to the GPIO module's base register.
 * @param mask Bitmask specifying the pins to set.
 */
static inline void GPIO_SetPinsMask(GPIO_Type *base, uint32_t mask);

/**
 * @brief Clear multiple pins of the GPIO module using a mask.
 * @param base Pointer to the GPIO module's base register.
 * @param mask Bitmask specifying the pins to clear.
 */
static inline void GPIO_ClearPinsMask(GPIO_Type *base, uint32_t mask);

/**
 * @brief Read the logical value of a specific pin of the GPIO module.
 * @param base Pointer to the GPIO module's base register.
 * @param pin Pin number to read.
 * @return 0 for logic low, 1 for logic high.
 */
static inline uint8_t GPIO_ReadPin(GPIO_Type *base, uint8_t pin);

/**
 * @brief Initialize a GPIO module pin as an input.
 * @param base Pointer to the GPIO module's base register.
 * @param pin Pin number to initialize as input.
 */
void GPIO_InitInputPin(GPIO_Type *base, uint8_t pin);

/**
 * @brief Initialize a GPIO module pin as an output.
 * @param base Pointer to the GPIO module's base register.
 * @param pin Pin number to initialize as output.
 * @param initVal Initial logical value (0 or non-zero).
 */
void GPIO_InitOutputPin(GPIO_Type *base, uint8_t pin, uint8_t initVal);


/*******************************************************************************
 * Code
 ******************************************************************************/

/**********************************************************************************/
static inline void GPIO_SetPin(GPIO_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( pin < 32 );

	base->PSOR = (1 << pin);
}

/**********************************************************************************/
static inline void GPIO_ClearPin(GPIO_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( pin < 32 );

	base->PCOR = (1 << pin);
}

/**********************************************************************************/
static inline void GPIO_TogglePin(GPIO_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( pin < 32 );

	base->PTOR = (1 << pin);
}

/**********************************************************************************/
static inline uint8_t GPIO_ReadPin(GPIO_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( pin < 32 );

	return (uint8_t)((base->PDIR & (((uint32_t)1) << pin)) != 0);
}

/**********************************************************************************/
static inline void GPIO_WritePin(GPIO_Type *base, uint8_t pin, uint8_t value)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( pin < 32 );

	if(pin)
	{
		base->PDOR |= 1 << pin;
	}
	else
	{
		base->PDOR &= ~(1 << pin);
	}
}

/**********************************************************************************/
static inline void GPIO_SetPinsMask(GPIO_Type *base, uint32_t mask)
{
	SYSTEM_ASSERT(base);

	base->PSOR = mask;
}

/**********************************************************************************/
static inline void GPIO_ClearPinsMask(GPIO_Type *base, uint32_t mask)
{
	SYSTEM_ASSERT(base);

	base->PCOR = mask;
}

/*! @}*/

#if defined(__cplusplus)
}
#endif

#endif /* GPIO_DRV_H_ */

