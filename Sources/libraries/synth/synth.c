
/** Self header */
#include "synth.h"
#include "services/services.h"

/** Utils */
#include "libraries/delay/delay.h"

/** STD */
#include <stddef.h>

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*!< Enumeration to identify objects creation.*/
enum
{
	SYNTH_OBJECT_IS_HANDLE,
	SYNTH_OBJECT_IS_CONFIG
};

/*******************************************************************************
 * Locals
 ******************************************************************************/

#ifdef SYNTH_STATIC_OBJECTS_CREATION

/*!< The static list of configuration structures that is returned to the SYNTH API.*/
static synthConfig_t g_synthConfigList[SYNTH_MAX_STATIC_OBJECTS];

/*!< The static list of handle structures that is returned to the SYNTH API.*/
static synthHandle_t g_synthHandleList[SYNTH_MAX_STATIC_OBJECTS];

/*!< The number of configuration and handle structures created using the SYNTH API.*/
static uint8_t g_staticConfigsCreated;
static uint8_t g_staticHandlesCreated;

#endif

/**
 * @brief Create an specific object used by an SYNTH instance.
 *
 * @param objectType - \a SYNTH_OBJECT_IS_HANDLE, if want to create a SYNTH handle;
 * 					   \a SYNTH_OBJECT_IS_CONFIG, if want to create a SYNTH configuration structure.
 *
 */
static void* SynthCreateObject(uint8_t objectType);

/**
 * @brief Destroy an specific object used by an SYNTH instance.
 * 
 * @param objectType - \a SYNTH_OBJECT_IS_HANDLE, if want to destroy a SYNTH handle;
 * 					   \a SYNTH_OBJECT_IS_CONFIG, if want to destroy a SYNTH configuration structure.
 * @param object Object to be destroyed.
 */
void SynthDestroyObject(uint8_t objectType, void *object);

/*******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * @brief Create an specific object used by an LCD instance.
 *
 * @param objectType - \a SYNTH_OBJECT_IS_HANDLE, if want to create a LCD handle;
 * 					   \a SYNTH_OBJECT_IS_CONFIG, if want to create a LCD configuration structure.
 *
 */
static void* SynthCreateObject(uint8_t objectType)
{
	void* objectCreated = NULL;
#ifdef SYNTH_STATIC_OBJECTS_CREATION
	switch (objectType)
	{
	case SYNTH_OBJECT_IS_HANDLE:
		if(g_staticHandlesCreated < SYNTH_MAX_STATIC_OBJECTS)
		{
			objectCreated = (void*)&g_synthHandleList[g_staticHandlesCreated++];
		}
		break;
	case SYNTH_OBJECT_IS_CONFIG:
		if(g_staticConfigsCreated < SYNTH_MAX_STATIC_OBJECTS)
		{
			objectCreated = (void*)&g_synthConfigList[g_staticConfigsCreated++];
		}
		break;
	}
#else
	switch (objectType)
	{
	case LCD_OBJECT_IS_HANDLE:
		objectCreated = embUtil_Malloc(sizeof(synthHandle_t));
		break;
	case LCD_OBJECT_IS_CONFIG:
		objectCreated = embUtil_Malloc(sizeof(synthConfig_t));
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
void SynthDestroyObject(uint8_t objectType, void *object)
{
#ifdef SYNTH_STATIC_OBJECTS_CREATION
	switch (objectType)
	{
	case SYNTH_OBJECT_IS_HANDLE:
		if(g_staticHandlesCreated)
			--g_staticHandlesCreated;
		break;
	case SYNTH_OBJECT_IS_CONFIG:
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
 * @brief Initializes the synth module
 * 
 * @param adapter Hardware adapter to be used
 * @return synthHandle_t* New synth handle
 */
synthHandle_t* SYNTH_Init(synthAdapter_t *adapter)
{
	synthHandle_t *handle = (synthHandle_t*)SynthCreateObject(SYNTH_OBJECT_IS_HANDLE);
	synthConfig_t *config = (synthConfig_t*)SynthCreateObject(SYNTH_OBJECT_IS_CONFIG);

	/** Reinterpret as hardware config interface */
	synthAdapter_t *adapter_interface = (synthAdapter_t*)adapter;

	/** TODO add free */
	if(!handle || !config) return NULL;

	/** Config setup */
	config->adapter = adapter;
	handle->config = config;

	return handle;
}

