/**
 * @file port.h
 * @version 1.0
 * @date 24/05/2023
 * @brief File with PORT module implementations for the Kinetis KL25 family.
 * @author Matheus Leitzke Pinto
 */

#ifndef PORT_DRV_H_
#define PORT_DRV_H_

#include <common.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @addtogroup port driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * @brief Possible alternatives for pin multiplexing in PORT modules.
 */
typedef enum{
	PORT_MUX_ALT0 		= 0x0u,
	PORT_MUX_ALT1 		= 0x1u,
	PORT_MUX_ALT2 		= 0x2u,
	PORT_MUX_ALT3 		= 0x3u,
	PORT_MUX_DISABLE 	= PORT_MUX_ALT0,
	PORT_MUX_AS_GPIO    = PORT_MUX_ALT1
}portMux_t;

/**
 * @brief Possible alternatives for pin interrupts in PORT modules.
 */
typedef enum{
	PORT_IRQ_DISABLE 		 	 = 0x0U,
	PORT_IRQ_DMA_ON_RISING_EDGE  = 0x1U,
	PORT_IRQ_DMA_ON_FALLING_EDGE = 0x2U,
	PORT_IRQ_DMA_ON_EITHER_EDGE  = 0x3U,
	PORT_IRQ_ON_lOW_LEVEL		 = 0x8U,
	PORT_IRQ_ON_RISING_EDGE 	 = 0x9U,
	PORT_IRQ_ON_FALLING_EDGE 	 = 0xAU,
	PORT_IRQ_ON_EITHER_EDGE 	 = 0xBU,
	PORT_IRQ_ON_HIGH_LEVEL 	 	 = 0xCU
}portIRQ_t;


/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Initialize a PORT module before use.
 * @param base Pointer to the PORT module's base register.
 */
void PORT_Init(PORT_Type *base);

/**
 * @brief Enable interrupt generation for a PORT module pin.
 * @param base Pointer to the PORT module's base register.
 * @param pin Pin number to enable interrupts on.
 * @param irq One of the alternatives for pin interrupts.
 */
void PORT_EnableIRQ(PORT_Type *base, uint8_t pin, portIRQ_t irq);

/**
 * @brief Check if the interrupt flag for a PORT pin is set.
 * @param base Pointer to the PORT module's base register.
 * @param pin Pin number to check.
 * @return 0 if the flag is not set, non-zero if the flag is set.
 */
uint32_t PORT_GetIRQFlag(PORT_Type *base, uint8_t pin);

/**
 * @brief Clear the interrupt flag for a PORT pin.
 * @param base Pointer to the PORT module's base register.
 * @param pin Pin number to clear the flag.
 */
void PORT_ClearIRQFlag(PORT_Type *base, uint8_t pin);

/**
 * @brief Set the multiplexing configuration for a PORTB pin.
 * @param base Pointer to the PORT module's base register.
 * @param pin Pin number to configure.
 * @param mux One of the alternatives for pin multiplexing.
 */
void PORT_SetMux(PORT_Type *base, uint8_t pin, portMux_t mux);

/**
 * @brief Enable pull-up on a PORT pin.
 * @param base Pointer to the PORT module's base register.
 * @param pin Pin number to enable pull-up on.
 */
void PORT_EnablePull(PORT_Type *base, uint8_t pin);

/**
 * @brief Enable high-strength current on a PORT pin.
 * @param base Pointer to the PORT module's base register.
 * @param pin Pin number to enable high-strength current on.
 */
void PORT_EnableHighStrength(PORT_Type *base, uint8_t pin);

/*! @}*/

#if defined(__cplusplus)
}
#endif

#endif /* PORT_DRV_H_ */
