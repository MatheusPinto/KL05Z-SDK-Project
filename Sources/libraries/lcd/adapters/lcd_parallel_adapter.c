
/** std */
#include <stddef.h>

/* Self Header */
#include "lcd_parallel_adapter.h"

/** Utils */
#include "libraries/delay/delay.h"

#ifndef LCD_DISABLE_PARALLEL_ADAPTER

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MCU_PortSet(portPinsRegister, portPinMask) portPinsRegister->PSOR = portPinMask;

#define MCU_PortClear(portPinsRegister, portPinMask) portPinsRegister->PCOR = portPinMask;

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

/*!< Structure that holds information when using parallel connection with GPIO */
typedef struct
{
	/*!< Adapter interface implementation */
	lcdAdapterInterface_t interface;

	/*!< The pins for data/commands transaction.*/
#ifdef LCD_8_BIT_MODE
	lcdPin_t data[8]; /*!< data[7] -> DB7, data[6] -> DB6, ...*/
#else
	lcdPin_t data[4]; /*!< data[3] -> DB7, data[2] -> DB6, ...*/
#endif

	/*!< Register select pin.*/
	lcdPin_t rs;

	/*!< Enable pin.*/
	lcdPin_t en;
} lcdParallelHardwareAdapter_t;

/*******************************************************************************
 * Locals
 ******************************************************************************/

#ifdef LCD_STATIC_OBJECTS_CREATION

/*!< The static list of parallel adapter structures that is used by the API */
static lcdParallelHardwareAdapter_t g_lcdParallelAdapterList[LCD_MAX_STATIC_OBJECTS];
/*!< Amount of static configs already allocated */
static uint8_t g_staticParallelAdaptersCreated;

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
void LCD_ParallelWriteBits(
	lcdHandle_t* handle, uint8_t value, uint8_t is_expanded, uint8_t mode
);

/**
 * @brief Internal function to allocate a given adapter
 * 
 * @return lcdParallelHardwareAdapter_t* Created adapter
 */
static lcdParallelHardwareAdapter_t* AllocAdapter();

/**
 * @brief Generates a enable pulse to latch the LCD
 * 
 * @param handle LCD handler object instance
 */
void EnablePulseParallel(lcdHandle_t* handle);

/**
 * @brief Sets reset bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelSetRs(lcdHandle_t* handle);

/**
 * @brief Clear reset bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelClrRs(lcdHandle_t* handle);

/**
 * @brief Sets enable bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelSetEn(lcdHandle_t* handle);

/**
 * @brief Clear reset bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelClrEn(lcdHandle_t* handle);

/*******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * @brief Creates a Parallel hardware adaptor configuration object.
 *
 * @return - Created I2C LCD hardware configuration object based on provided
 *					 parameters.
 */
lcdAdapter_t LCD_CreateParallelAdapter(
#ifdef LCD_8_BIT_MODE
	lcdPin_t data[8],
#else
	lcdPin_t data[4],
#endif
	lcdPin_t *rsPin, lcdPin_t *enPin
)
{
	lcdParallelHardwareAdapter_t *adapter = AllocAdapter();

	if (!adapter) return NULL;

	adapter->interface.type = LCD_PARALLEL_HARD_ADAPTER;

	adapter->interface.setRs = LCD_ParallelSetRs;
	adapter->interface.setEn = LCD_ParallelSetEn;
	adapter->interface.clrRs = LCD_ParallelClrRs;
	adapter->interface.clrEn = LCD_ParallelClrEn;
	adapter->interface.write = LCD_ParallelWriteBits;

#ifdef LCD_8_BIT_MODE
	uint8_t limit = 8;
#else
	uint8_t limit = 4;
#endif

	for (int i = 0; i < limit; ++i)
	{
		adapter->data[i].portRegister = data[i].portRegister;
		adapter->data[i].pinMask = data[i].pinMask;
	}

	adapter->en.pinMask = enPin->pinMask;
	adapter->en.portRegister = enPin->portRegister;
	adapter->rs.pinMask = rsPin->pinMask;
	adapter->rs.portRegister = rsPin->portRegister;

	return adapter;
}

/**
 * @brief Dispatches a given byte of data to display
 *
 * @param handle - LCD handler object instance
 * @param value - Byte to be sent to LCD display
 * @param is_expanded - a boolean value used only in I2C adapter
 * @param mode -
 * 				  \a LCD_COMMAND_MODE - write a command
 * 				  \a LCD_DATA_MODE - write a character
 *
 */
void LCD_ParallelWriteBits(
	lcdHandle_t* handle, uint8_t value, uint8_t is_expanded, uint8_t mode
)
{
	/** Reinterprets the adapter */
	lcdParallelHardwareAdapter_t *adapter = (lcdParallelHardwareAdapter_t*)(handle->config->adapter);

	/** Set or Clear RS based on mode */
	if (mode)
	{
		LCD_ParallelSetRs(handle);
	}
	else
	{
		LCD_ParallelClrRs(handle);
	}

	/** Dispatches the data */
#ifdef LCD_4_BIT_MODE
	for (int i = 0; i < 4; ++i)
	{
		if((value >> 4 >> i) & 0x01)
		{
			MCU_PortSet(adapter->data[i].portRegister, adapter->data[i].pinMask);
		}
		else
		{
			MCU_PortClear(adapter->data[i].portRegister, adapter->data[i].pinMask);
		}
	}
	EnablePulseParallel(handle);

	for (int i = 0; i < 4; ++i)
	{
		if((value >> i) & 0x01)
		{
			MCU_PortSet(adapter->data[i].portRegister, adapter->data[i].pinMask);
		}
		else
		{
			MCU_PortClear(adapter->data[i].portRegister, adapter->data[i].pinMask);
		}
	}
	EnablePulseParallel(handle);

#else
	for (int i = 0; i < 8; ++i)
	{
		if((value >> i) & 0x01)
		{
			MCU_PortSet(adapter->data[i].portRegister, adapter->data[i].pinMask);
		}
		else
		{
			MCU_PortClear(adapter->data[i].portRegister, adapter->data[i].pinMask);
		}
	}
	EnablePulseParallel(handle);
#endif
}

/**
 * @brief Generates a enable pulse to latch the LCD
 * 
 * @param handle LCD handler object instance
 */
void EnablePulseParallel(lcdHandle_t* handle)
{
	lcdParallelHardwareAdapter_t* adapter = (lcdParallelHardwareAdapter_t*)(handle->config->adapter);

	MCU_PortClear(adapter->en.portRegister, adapter->en.pinMask);
	Waitus(1);
	MCU_PortSet(adapter->en.portRegister, adapter->en.pinMask);
	Waitus(1);
	MCU_PortClear(adapter->en.portRegister, adapter->en.pinMask);
	Waitus(100);
}

/**
 * @brief Sets reset bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelSetRs(lcdHandle_t* handle)
{
	lcdParallelHardwareAdapter_t* adapter = (lcdParallelHardwareAdapter_t*)(handle->config->adapter);
	MCU_PortSet(adapter->rs.portRegister, adapter->rs.pinMask)
}

/**
 * @brief Clear reset bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelClrRs(lcdHandle_t* handle)
{
	lcdParallelHardwareAdapter_t* adapter = (lcdParallelHardwareAdapter_t*)(handle->config->adapter);
	MCU_PortClear(adapter->rs.portRegister, adapter->rs.pinMask)
}

/**
 * @brief Sets enable bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelSetEn(lcdHandle_t* handle)
{
	lcdParallelHardwareAdapter_t* adapter = (lcdParallelHardwareAdapter_t*)(handle->config->adapter);
	MCU_PortSet(adapter->en.portRegister, adapter->en.pinMask)
}

/**
 * @brief Clear reset bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelClrEn(lcdHandle_t* handle)
{
	lcdParallelHardwareAdapter_t* adapter = (lcdParallelHardwareAdapter_t*)(handle->config->adapter);
	MCU_PortClear(adapter->en.portRegister, adapter->en.pinMask)
}

/**
 * @brief Internal function to allocate a given adapter
 * 
 * @return lcdParallelHardwareAdapter_t* Created adapter
 */
lcdParallelHardwareAdapter_t* AllocAdapter()
{
	lcdParallelHardwareAdapter_t* objectCreated = NULL;
#ifdef LCD_STATIC_OBJECTS_CREATION
	if(g_staticParallelAdaptersCreated < LCD_MAX_STATIC_OBJECTS)
	{
		objectCreated = (void*)&g_lcdParallelAdapterList[g_staticParallelAdaptersCreated++];
	}
#else
	objectCreated = embUtil_Malloc(sizeof(lcdParallelHardwareAdapter_t));
#endif
	return objectCreated;
}

/**
 * @brief Destroys a given LCD parallel adapter
 * 
 * @param adapter Adapter to be destroyed
 */
void LCD_FreeAdapterParallel(lcdAdapter_t adapter)
{
#ifdef LCD_STATIC_OBJECTS_CREATION
	if(g_staticParallelAdaptersCreated)
		--g_staticParallelAdaptersCreated;
#else
	embUtil_Free(obj);
#endif
	adapter = NULL;
}

#endif
