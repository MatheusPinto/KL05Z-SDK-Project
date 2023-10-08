
#ifndef LCD_HD44780_H_
#define LCD_HD44780_H_

/** General config */
#include <mcu/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup lcd
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!< HD44780 control commands.*/
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_RETURN_HOME 0x02
#define LCD_ENTRY_MODE_SET 0x04
#define LCD_DISPLAY_CONTROL 0x08
#define LCD_CURSOR_SHIFT 0x10
#define LCD_FUNCTION_SET 0x20
#define LCD_SET_CG_RAM_ADDR 0x40
#define LCD_SET_DD_RAM_ADDR 0x80

/*!< Flags for display entry mode.*/
#define LCD_ENTRY_RIGHT 0x00
#define LCD_ENTRY_LEFT 0x02
#define LCD_ENTRY_SHIFT_INCREMENT 0x01
#define LCD_ENTRY_SHIFT_DECREMENT 0x00

/*!< Flags for display on/off control.*/
#define LCD_DISPLAY_ON 0x04
#define LCD_DISPLAY_OFF 0x00
#define LCD_CURSOR_ON 0x02
#define LCD_CURSOR_OFF 0x00
#define LCD_BLINK_ON 0x01
#define LCD_BLINK_OFF 0x00

/*!< Flags for display/cursor shift.*/
#define LCD_DISPLAY_MOVE 0x08
#define LCD_CURSOR_MOVE 0x00
#define LCD_MOVE_RIGHT 0x04
#define LCD_MOVE_LEFT 0x00

/*!< Flags for function set.*/
#define LCD_4_BIT_MODE 0x00
#ifndef LCD_4_BIT_MODE
	#define LCD_8_BIT_MODE 0x10
#endif
#define LCD_2_LINE 0x08
#define LCD_1_LINE 0x00
#define LCD_5x10_DOTS 0x04
#define LCD_5x8_DOTS 0x00

/*!< Flags for backlight control */
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

/* !< Defines if LCD instances will be created statically.
 *    If commented, LCD instances will be allocated dynamically in heap.
 */
#define LCD_STATIC_OBJECTS_CREATION
#ifdef LCD_STATIC_OBJECTS_CREATION
	#define LCD_MAX_STATIC_OBJECTS 1 /*!< The number of object instances that will be created statically.*/
#endif

/*!< Uncomment this macro if want to use reentrant access of API.*/
#define LCD_REENTRANT_ACCESS

/*******************************************************************************
 * Types
 ******************************************************************************/

/*!< Hardware configuration structure describing current display hardware connections */
typedef void* lcdAdapter_t;

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*!< Defines all possible hardware adapters when creating a new lcd */
typedef enum
{
	LCD_PARALLEL_HARD_ADAPTER,
	LCD_I2C_HARD_ADAPTER
} lcdHardwareAdapters_t;

/*******************************************************************************
 * Structures
 ******************************************************************************/

/*!
 * @brief LCD manager configuration structure
 *
 * This structure holds the configuration settings for the LCD module.
 */
typedef struct
{
	/*!< The display columns number. */
	uint8_t cols;

	/*!< The display lines number. */
	uint8_t lines;

	/*!< The display character size in pixels. */
	uint8_t char_size;

	/*!< The LCD bus. */
	lcdAdapter_t adapter;
} lcdConfig_t;

/*!
 * @brief LCD handle structure used internally
 */
typedef struct
{
	/*!< The pointer to the configuration structure passed by the user.*/
	lcdConfig_t *config;

	/*!< The display function command that will be send to the LCD controller.*/
	uint8_t display_function;

	/*!< The display control command that will be send to the LCD controller.*/
	uint8_t display_control;

	/*!< The display mode command that will be send to the LCD controller.*/
	uint8_t display_mode;

	/*!< The row offset.Get(12);s list determined by the number of LCD rows and columns.*/
	uint8_t row_offsets[4];

#ifdef __FREERTOS_H
#ifdef LCD_REENTRANT_ACCESS
	/*!< The mutex used for mutual exclusion in API calls.*/
	xSemaphoreHandle lcdAccessMutex;
#endif /* LCD_REENTRANT_ACCESS */
#endif /* __FREERTOS_H */
} lcdHandle_t;

/**
 * @brief Structure that holds the LCD bus information
 */
typedef struct
{
	/*!< Hardware adapter type being used */
	lcdHardwareAdapters_t type;

	/*!< Mid level command callback */
	void (*write)(lcdHandle_t* handle, uint8_t value, uint8_t is_expanded, uint8_t mode);

	/*!< Bus set reset callback */
	void (*setRs)(lcdHandle_t* handle);

	/*!< Bus clear reset callback */
	void (*clrRs)(lcdHandle_t* handle);

	/*!< Bus set enable callback */
	void (*setEn)(lcdHandle_t* handle);

	/*!< Bus clear enable callback */
	void (*clrEn)(lcdHandle_t* handle);
} lcdAdapterInterface_t;

/*******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * @brief Initialize the LCD module.
 *
 * @param adapter - Hardware adapter for this handle
 * @param cols - Display columns number
 * @param lines - Display lines number
 * @param char_size - Display character size
 *
 * @return - The specific LCD module handle that must be passed
 *           to the API for communication or;
 *         - NULL, if was not possible to create the handle.
 */
lcdHandle_t* LCD_Init(
	lcdAdapter_t adapter, uint8_t cols, uint8_t lines, uint8_t char_size
);

/**
 * @brief Send a command to the HD44780 controller.
 *
 * This function is used used inside API. It should
 * be avoided in favor of other API functions. The user
 * must call this function only if it is necessary to
 * make another kind of communication not covered to
 * the API.
 *
 * @param handle - the specific LCD handle.
 * @param value  - the command value.
 */
void LCD_Command(lcdHandle_t *handle, uint8_t value);

/**
 * @brief Send a data to the HD44780 controller.
 *
 * This function is used used inside API. It should
 * be avoided in favor of other API functions. The user
 * must call this function only if it is necessary to
 * make another kind of communication not covered to
 * the API.
 *
 * @param handle - the specific LCD handle.
 * @param value  - the data value.
 */
void LCD_Write(lcdHandle_t *handle, uint8_t value);

#ifdef __cplusplus
}  /* extern "C" */
#endif

/*! @}*/

#endif /* LCD_HD44780_H_ */
