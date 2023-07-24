
/** Self header */
#include "lcd.h"
#include "services/services.h"

/** Utils */
#include "libraries/delay/delay.h"

#ifdef __FREERTOS_H
#include "FreeRTOS.h"
#include "semphr.h"
#endif /* __FREERTOS_H */

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*!< Enumeration to identify objects creation.*/
enum
{
	LCD_OBJECT_IS_HANDLE,
	LCD_OBJECT_IS_CONFIG
};

/*******************************************************************************
 * Locals
 ******************************************************************************/

#ifdef LCD_STATIC_OBJECTS_CREATION

/*!< The static list of configuration structures that is returned to the LCD API.*/
static lcdConfig_t g_lcdConfigList[LCD_MAX_STATIC_OBJECTS];

/*!< The static list of handle structures that is returned to the LCD API.*/
static lcdHandle_t g_lcdHandleList[LCD_MAX_STATIC_OBJECTS];

/*!< The number of configuration and handle structures created using the LCD API.*/
static uint8_t g_staticConfigsCreated;
static uint8_t g_staticHandlesCreated;

#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* waiting macros */
#define Waitns(x) \
		Delay_Waitns(x)				 /* Wait x ns */
#define Waitus(x) \
		Delay_Waitus(x)				 /* Wait x us */
#define Waitms(x) \
		Delay_Waitms(x)				 /* Wait x ms */

#ifndef __FREERTOS_H
#define LcdEnterMutex(x) (void)0
#define LcdExitMutex(x) (void)0
#else
#ifdef LCD_REENTRANT_ACCESS
/* macros for mutex access and release */
#define LcdEnterMutex(x) xSemaphoreTake(x->lcdAccessMutex, portMAX_DELAY)
#define LcdExitMutex(x) xSemaphoreGive(x->lcdAccessMutex)
#endif /* LCD_REENTRANT_ACCESS */
#endif /* __FREERTOS_H */

/*******************************************************************************
 * Forward declarations
 ******************************************************************************/

/**
 * @brief Set the row offset, which is based from the LCD module used.
 *
 * @param handle - the specific LCD handle.
 *
 */
static void SetRowOffsets(lcdHandle_t* handle);

/**
 * @brief Create an specific object used by an LCD instance.
 *
 * @param objectType - \a K_LCD_OBJECT_IS_HANDLE, if want to create a LCD handle;
 * 					   \a K_LCD_OBJECT_IS_CONFIG, if want to create a LCD configuration structure.
 *
 */
static void* CreateObject(uint8_t objectType);

/**
 * @brief Destroy an specific object used by an LCD instance.
 * 
 * @param objectType - \a K_LCD_OBJECT_IS_HANDLE, if want to destroy a LCD handle;
 * 					   \a K_LCD_OBJECT_IS_CONFIG, if want to destroy a LCD configuration structure.
 * @param object Object to be destroyed.
 */
void DestroyObject(uint8_t objectType, void *object);

/*******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * @brief Create an specific object used by an LCD instance.
 *
 * @param objectType - \a K_LCD_OBJECT_IS_HANDLE, if want to create a LCD handle;
 * 					   \a K_LCD_OBJECT_IS_CONFIG, if want to create a LCD configuration structure.
 *
 */
static void* CreateObject(uint8_t objectType)
{
	void* objectCreated = NULL;
#ifdef LCD_STATIC_OBJECTS_CREATION
	switch (objectType)
	{
	case LCD_OBJECT_IS_HANDLE:
		if(g_staticHandlesCreated < LCD_MAX_STATIC_OBJECTS)
		{
			objectCreated = (void*)&g_lcdHandleList[g_staticHandlesCreated++];
		}
		break;
	case LCD_OBJECT_IS_CONFIG:
		if(g_staticConfigsCreated < LCD_MAX_STATIC_OBJECTS)
		{
			objectCreated = (void*)&g_lcdConfigList[g_staticConfigsCreated++];
		}
		break;
	}
#else
	switch (objectType)
	{
	case LCD_OBJECT_IS_HANDLE:
		objectCreated = embUtil_Malloc(sizeof(lcdHandle_t));
		break;
	case LCD_OBJECT_IS_CONFIG:
		objectCreated = embUtil_Malloc(sizeof(lcdConfig_t));
		break;
	}
#endif
	return objectCreated;
}

/**
 * @brief Destroy an specific object used by an LCD instance.
 * 
 * @param objectType - \a K_LCD_OBJECT_IS_HANDLE, if want to destroy a LCD handle;
 * 					   \a K_LCD_OBJECT_IS_CONFIG, if want to destroy a LCD configuration structure.
 * @param object Object to be destroyed.
 */
void DestroyObject(uint8_t objectType, void *object)
{
#ifdef LCD_STATIC_OBJECTS_CREATION
	switch (objectType)
	{
	case LCD_OBJECT_IS_HANDLE:
		if(g_staticHandlesCreated)
			--g_staticHandlesCreated;
		break;
	case LCD_OBJECT_IS_CONFIG:
		if(g_staticConfigsCreated)
			--g_staticConfigsCreated;
		break;
	}
#else
	embUtil_Free(obj);
#endif

	object = NULL;
}

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
	lcdAdapter_t *adapter, uint8_t cols, uint8_t lines, uint8_t char_size
)
{
	SYSTEM_ASSERT( adapter );

	lcdHandle_t *handle = (lcdHandle_t*)CreateObject(LCD_OBJECT_IS_HANDLE);
	lcdConfig_t *config = (lcdConfig_t*)CreateObject(LCD_OBJECT_IS_CONFIG);

	/** Reinterpret as hardware config interface */
	lcdAdapterInterface_t *adapter_interface = (lcdAdapterInterface_t*)adapter;

	/** TODO add free */
	if(!handle || !config) return NULL;

	/** Config setup */
	config->adapter = adapter;
	config->cols = cols;
	config->lines = lines;
	config->char_size = char_size;
	handle->config = config;

#ifdef LCD_4_BIT_MODE
	handle->display_function = LCD_4_BIT_MODE | LCD_1_LINE | LCD_5x8_DOTS;
#else
	handle->display_function = LCD_8_BIT_MODE | LCD_1_LINE | LCD_5x8_DOTS;
#endif

	if (config->lines > 1)
	{
		handle->display_function |= LCD_2_LINE;
	}

	SetRowOffsets(handle);

	// for some 1 line displays you can select a 10 pixel high font
	if ((config->char_size != LCD_5x8_DOTS) && (config->lines == 1)) {
		handle->display_function |= LCD_5x10_DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. So we'll wait 50ms.
	Waitms(50);

	// Pull both RS and R/W low to begin commands
	if (adapter_interface->type == LCD_I2C_HARD_ADAPTER)
	{
		// Reset expander and turn backlight off (Bit 8 = 1)
		adapter_interface->write(handle, LCD_BACKLIGHT, true, 0);
		Waitms(1000);
	}
	else
	{
		adapter_interface->clrRs(handle);
		adapter_interface->clrEn(handle);
	}

	//put the LCD into 4 bit or 8 bit mode
#ifdef LCD_4_BIT_MODE
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46
	// we start in 8bit mode, try to set 4 bit mode
	adapter_interface->write(handle, 0x03, false, 0);
	Waitus(4500); // wait min 4.1ms
	// second try
	adapter_interface->write(handle, 0x03, false, 0);
	Waitus(4500); // wait min 4.1ms
	// third go!
	adapter_interface->write(handle, 0x03, false, 0);
	Waitus(4500);
	// finally, set to 4-bit interface
	adapter_interface->write(handle, 0x02, false, 0);
	Waitus(150);
#else
	// this is according to the hitachi HD44780 datasheet
	// page 45 figure 23

	// Send function set command sequence
	LCD_Command((lcdHandle_t)handle, LCD_FUNCTION_SET | display_function);
	Waitus(4500); // wait min 4.1ms

	// second try
	LCD_Command((lcdHandle_t)handle, LCD_FUNCTION_SET | display_function);
	Waitus(150);

	// third go
	LCD_Command((lcdHandle_t)handle, LCD_FUNCTION_SET | display_function);
#endif
	// finally, set # lines, font size, etc.
	LCD_Command(handle, LCD_FUNCTION_SET | handle->display_function);

	// turn the display on with no cursor or blinking default
	handle->display_control = LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF;
	LCD_Display(handle);

	// clear it off
	LCD_Clear(handle);

	Waitms(10);

	// Initialize to default text direction (for romance languages)
	handle->display_mode = LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT;
	// set the entry mode
	LCD_Command(handle, LCD_ENTRY_MODE_SET | handle->display_mode);

	Waitus(400);

	return handle;
}

/**
 * @brief Set the row offset, which is based from the LCD module used.
 *
 * @param handle - the specific LCD handle.
 *
 */
static void SetRowOffsets(lcdHandle_t* handle)
{
	handle->row_offsets[0] = 0x00;
	handle->row_offsets[1] = 0x40;
	handle->row_offsets[2] = 0x00 + handle->config->cols;
	handle->row_offsets[3] = 0x40 + handle->config->cols;
}

/** MID level commands */

void LCD_Command(lcdHandle_t *handle, uint8_t value)
{
	/** Reinterpret as hardware config interface */
	lcdAdapterInterface_t *adapter = (lcdAdapterInterface_t*)handle->config->adapter;

	/** Call command for current adapter */
	adapter->write(handle, value, false, 0);
}

void LCD_Write(lcdHandle_t *handle, uint8_t value)
{
	/** Reinterpret as hardware config interface */
	lcdAdapterInterface_t *adapter = (lcdAdapterInterface_t*)handle->config->adapter;

	/** Call command for current adapter */
	adapter->write(handle, value, false, 1);
}
