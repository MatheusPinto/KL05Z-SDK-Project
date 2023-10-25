/**
 * @file	lcd_parallel_adapter.h
 * @author  JOAO MARIO CARNIELETTO IZOTON LAGO <joao.mario.lago@hotmail.com>
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

#ifndef LCD_HD44780_I2C_HARDWARE_ADAPTER_H_
#define LCD_HD44780_I2C_HARDWARE_ADAPTER_H_

/* LCD definitions includes */
#include "../lcd.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup lcd-adapters
 * @{
 */

#ifndef LCD_DISABLE_I2C_ADAPTER

/**
 * @brief Creates a I2C hardware adaptor configuration object.
 *
 * @param base - I2C base memory map.
 * @param slave_addr - LCD slave address.
 *
 * @return - Created I2C LCD hardware configuration object based on provided
 *           parameters.
 */
lcdAdapter_t LCD_CreateI2CAdapter(
	I2C_Type *base, uint8_t slave_addr
);

/**
 * @brief Destroys a given LCD I2C adapter
 * 
 * @param adapter Adapter to be destroyed
 */
void LCD_FreeAdapterI2C(lcdAdapter_t adapter);

#endif

#ifdef __cplusplus
}  /* extern "C" */
#endif

/*! @}*/

#endif /* !LCD_HD44780_I2C_HARDWARE_ADAPTER_H_ */
