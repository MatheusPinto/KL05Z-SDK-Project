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
