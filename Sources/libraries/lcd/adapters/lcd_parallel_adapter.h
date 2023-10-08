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
 * @param reset - Reset pin.
 * @param enable - Enable pin.
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
	lcdPin_t *reset, lcdPin_t *enable
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
