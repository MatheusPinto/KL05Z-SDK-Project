
/** std */
#include <stddef.h>

/* Common */
#include "mcu/common.h"

/* I2C */
#include "mcu/drivers/i2c/i2c.h"

/* Self Header */
#include "lcd_i2c_adapter.h"

/** Utils */
#include "libraries/delay/delay.h"

#ifndef LCD_DISABLE_I2C_ADAPTER

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Enable bit */
#define EN_BYTE 0b00000100

/* Read / Write bit */
#define RW_BYTE 0b00000010

/* Register select bit */
#define RS_BYTE 0b00000001

/* waiting macros */
#define Waitns(x) \
		Delay_Waitns(x)				 /* Wait x ns */
#define Waitus(x) \
		Delay_Waitus(x)				 /* Wait x us */
#define Waitms(x) \
		Delay_Waitms(x)				 /* Wait x ms */

/*******************************************************************************
 * Structures
 ******************************************************************************/

/*!< Structure that holds information when using I2C module */
typedef struct
{
	/*!< Adapter interface implementation */
	lcdAdapterInterface_t interface;

	/*!< Module memory map to use */
	I2C_Type *base;

	/*!< Slave address for display module */
	uint8_t slave_addr;
} lcdI2CHardwareAdapter_t;

/*******************************************************************************
 * Locals
 ******************************************************************************/

#ifdef LCD_STATIC_OBJECTS_CREATION

/*!< The static list of parallel adapter structures that is used by the API */
static lcdI2CHardwareAdapter_t g_lcdI2CAdapterList[LCD_MAX_STATIC_OBJECTS];

static uint8_t g_staticI2CAdaptersCreated;

#endif

/*******************************************************************************
 * Forward Declarations
 ******************************************************************************/

/**
 * @brief Dispatches a given byte of data to display
 *
 * @param handle - LCD handler object instance
 * @param value - Byte to be sent to LCD display
 *
 */
void LCD_I2CWriteBits(
	lcdHandle_t* handle, uint8_t value, uint8_t is_expanded, uint8_t mode
);

/**
 * @brief Generates a enable pulse to latch the LCD
 * 
 * @param handle LCD handler object instance
 */
void EnablePulseI2C(lcdHandle_t* handle, uint8_t value);

/**
 * @brief Internal function to allocate a given adapter
 * 
 * @return lcdI2CHardwareAdapter_t* Created adapter
 */
lcdI2CHardwareAdapter_t* AllocAdapter();

/*******************************************************************************
 *Functions
 ******************************************************************************/

/**
 * @brief Empty callback function
 * 
 * @param handle LCD handle instance
 */
static void callback_incinerator(lcdHandle_t* handle)
{
	/** Goodbye :) */
}

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
)
{
	/** Inits I2C Hardware */
	if (I2C_Init(base) != SYSTEM_STATUS_SUCCESS)
	{
		return NULL;
	}

	lcdI2CHardwareAdapter_t *adapter = AllocAdapter();

	if (!adapter) return NULL;

	adapter->interface.type = LCD_I2C_HARD_ADAPTER;

	adapter->interface.setRs = callback_incinerator;
	adapter->interface.setEn = callback_incinerator;
	adapter->interface.clrRs = callback_incinerator;
	adapter->interface.clrEn = callback_incinerator;
	adapter->interface.write = LCD_I2CWriteBits;

	adapter->base = base;
	adapter->slave_addr = slave_addr;

	return adapter;
}

/**
 * @brief Dispatches a given byte of data to display
 *
 * @param handle - LCD handler object instance
 * @param value - Byte to be sent to LCD display
 *
 */
void LCD_I2CWriteBits(
	lcdHandle_t* handle, uint8_t value, uint8_t is_expanded, uint8_t mode
)
{
	lcdI2CHardwareAdapter_t* adapter = (lcdI2CHardwareAdapter_t*)(handle->config->adapter);

	if (is_expanded)
	{
		I2C_WriteSlave(adapter->base, adapter->slave_addr, value);
	}
	else
	{
		uint8_t h_nibble = (value & 0xf0) | mode;
		uint8_t l_nibble = ((value << 4) & 0xf0) | mode;

		I2C_WriteSlave(adapter->base, adapter->slave_addr, h_nibble);
		EnablePulseI2C(handle, h_nibble);

		I2C_WriteSlave(adapter->base, adapter->slave_addr, l_nibble);
		EnablePulseI2C(handle, l_nibble);
	}
}

/**
 * @brief Generates a enable pulse to latch the LCD
 * 
 * @param handle LCD handler object instance
 */
void EnablePulseI2C(lcdHandle_t* handle, uint8_t value)
{
	lcdI2CHardwareAdapter_t* adapter = (lcdI2CHardwareAdapter_t*)(handle->config->adapter);

	// En high
	I2C_WriteSlave(adapter->base, adapter->slave_addr, value | EN_BYTE);
	Waitus(2);

	// En low
	I2C_WriteSlave(adapter->base, adapter->slave_addr, value | ~EN_BYTE);
	Waitus(100);
}

/**
 * @brief Internal function to allocate a given adapter
 * 
 * @return lcdI2CHardwareAdapter_t* Created adapter
 */
lcdI2CHardwareAdapter_t* AllocAdapter()
{
	lcdI2CHardwareAdapter_t* objectCreated = NULL;
#ifdef LCD_STATIC_OBJECTS_CREATION
	if(g_staticI2CAdaptersCreated < LCD_MAX_STATIC_OBJECTS)
	{
		objectCreated = (void*)&g_lcdI2CAdapterList[g_staticI2CAdaptersCreated++];
	}
#else
	objectCreated = embUtil_Malloc(sizeof(lcdParallelHardwareAdapter_t));
#endif
	return objectCreated;
}

/**
 * @brief Destroys a given LCD I2C adapter
 * 
 * @param adapter Adapter to be destroyed
 */
void LCD_FreeAdapterI2C(lcdAdapter_t adapter)
{
#ifdef LCD_STATIC_OBJECTS_CREATION
	if(g_staticI2CAdaptersCreated)
		--g_staticI2CAdaptersCreated;
#else
	embUtil_Free(obj);
#endif
	adapter = NULL;
}

#endif
