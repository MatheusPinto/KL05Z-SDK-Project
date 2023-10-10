/**
 * @file	lcd_parallel_adapter.h
 * @author  Matheus Leitzke Pinto <matheus.pinto@ifsc.edu.br>
 * @version 1.0
 * @date    2023
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The HD44780 hardware parallel interface adapter driver.
 */

#ifndef LCD_HD44780_PARALLEL_HARDWARE_ADAPTER_H_
#define LCD_HD44780_PARALLEL_HARDWARE_ADAPTER_H_

/* LCD definitions includes */
#include "../lcd.h"

/* Types */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup lcd-adapters
 * @{
 */

#ifndef LCD_DISABLE_PARALLEL_ADAPTER

/*******************************************************************************
 * Structures
 ******************************************************************************/

/*!< Structure that holds the necessary LCD pin information when using GPIO. */
typedef struct
{
	/*!< Port register attached to this pin */
	GPIO_Type* portRegister;

	/*!< Pin mask */
	uint32_t pinMask;
} lcdPin_t;

/*******************************************************************************
 * Forward declarations
 ******************************************************************************/

/**
 * @brief Creates a Parallel hardware adaptor configuration object.
 *
 * @param cols - Number of columns that LCD contains.
 * @param lines - Number of lines that LCD contains.
 * @param char_size - LCD character size.
 * @param data - Data pin array.
 * @param rsPin - The RS pin.
 * @param enPin - The Enable pin.
 *
 * @return - Created I2C LCD hardware configuration object based on provided
 *           parameters.
 */
lcdAdapter_t LCD_CreateParallelAdapter(
#ifdef LCD_8_BIT_MODE
	lcdPin_t data[8],
#else
	lcdPin_t data[4],
#endif
	lcdPin_t *rsPin, lcdPin_t *enPin
);

/**
 * @brief Destroys a given LCD parallel adapter
 * 
 * @param adapter Adapter to be destroyed
 */
void LCD_FreeAdapterParallel(lcdAdapter_t adapter);

#endif

#ifdef __cplusplus
}  /* extern "C" */
#endif

/*! @}*/

#endif /* !LCD_HD44780_PARALLEL_HARDWARE_ADAPTER_H_ */
