
/* Self Header */
#include "synth_gpio_adapter.h"

/** TPM */
#include "mcu/drivers/tpm/tpm.h"

/** STD */
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup synth-adapters
 * @{
 */

#ifndef SYNTH_DISABLE_GPIO_ADAPTER

/*******************************************************************************
 * Structures
 ******************************************************************************/

/*!< Structure that holds information when using GPIO module */
typedef struct
{
	/*!< Adapter interface implementation */
	synthAdapterInterface_t interface;

	/*!< Frequency of the waveform*/
	uint16_t frequency;
	/*!< Duty cycle of the waveform*/
	uint8_t duty;

	/*!< Base memory mapping for timer module used by synth */
	TPM_Type *base;

	/** Channel to be used */
	uint8_t channel;
} synthGPIOHardwareAdapter_t;


/*******************************************************************************
 * Locals
 ******************************************************************************/

#ifdef SYNTH_STATIC_OBJECTS_CREATION

/*!< The static list of parallel adapter structures that is used by the API */
static synthGPIOHardwareAdapter_t g_synthGPIOAdapterList[SYNTH_MAX_STATIC_OBJECTS];

static uint8_t g_staticGPIOAdaptersCreated;

#endif

/*******************************************************************************
 * Forward Declarations
 ******************************************************************************/

/**
 * @brief Internal function to allocate a given adapter
 * 
 * @return synthGPIOHardwareAdapter_t* Created adapter
 */
static synthGPIOHardwareAdapter_t* AllocAdapter();

/**
 * @brief Creates a GPIO hardware adaptor configuration object.
 * 
 * @param base Base memory mapping for timer module used by synth
 * @param channel Channel to be used
 * @return synthAdapter_t 
 */
synthAdapter_t SYNTH_CreateGPIOAdapter(TPM_Type *base, uint8_t channel);

/**
 * @brief Destroys a given GPIO synth adapter
 * 
 * @param adapter Adapter to be destroyed
 */
void SYNTH_FreeGPIOAdapter(synthAdapter_t adapter);

/**
 * @brief Play configured wave
 * 
 * @param handle Synth handle
 */
void SYNTH_play(synthHandle_t* handle);

/**
 * @brief Stop configured wave
 * 
 * @param handle 
 */
void SYNTH_stop(synthHandle_t* handle);

/**
 * @brief Set frequency of the wave
 * 
 * @param handle 
 * @param frequency 
 */
void SYNTH_setFrequency(synthHandle_t* handle, uint16_t frequency);

/**
 * @brief Set duty of wave
 * 
 * @param handle 
 * @param duty
 */
void SYNTH_setDuty(synthHandle_t* handle, uint8_t duty);

/*******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * @brief Creates a GPIO hardware adaptor configuration object.
 * 
 * @param base Base memory mapping for timer module used by synth
 * @param channel Channel to be used
 * @return synthAdapter_t 
 */
synthAdapter_t SYNTH_CreateGPIOAdapter(TPM_Type *base, uint8_t channel)
{
	synthGPIOHardwareAdapter_t *adapter = AllocAdapter();

	if (!adapter) return NULL;

	adapter->base = base;
	adapter->channel = channel;
	adapter->duty = 0;
	adapter->frequency = 0;

	adapter->interface.type = SYNTH_GPIO_ADAPTER;

	adapter->interface.play = SYNTH_play;
	adapter->interface.stop = SYNTH_stop;
	adapter->interface.setFrequency = SYNTH_setFrequency;
	adapter->interface.setDuty = SYNTH_setDuty;

	/** Set clock src */
	TPM_SetCounterClkSrc(TPM0, TPM_CNT_CLOCK_FLL);

	/** Init timer */
	TPM_Init(TPM0, 65535U, TPM_PRESCALER_DIV_1);

	/** Init channel */
	TPM_InitChannel(TPM0, channel, TPM_EDGE_PWM_MODE, TPM_PWM_HIGH_TRUE_CONFIG);

	/* Configures the PWM */
	TPM_SetChMatch(TPM0, channel, 32767);

	/** Init timer */
	TPM_InitCounter(TPM0);

	return adapter;
}

/**
 * @brief Play configured wave
 * 
 * @param handle Synth handle
 */
void SYNTH_play(synthHandle_t* handle)
{
	synthGPIOHardwareAdapter_t* adapter = (synthGPIOHardwareAdapter_t*)(handle->config->adapter);

	/* Set waveform frequency */
	SYNTH_setFrequency(handle, adapter->frequency);

	/* Set waveform duty */
	SYNTH_setDuty(handle, adapter->duty);
}

/**
 * @brief Stop configured wave
 * 
 * @param handle 
 */
void SYNTH_stop(synthHandle_t* handle)
{
	/* Disable waveform */
	SYNTH_setDuty(handle, 0);
}

/**
 * @brief Set frequency of the wave
 * 
 * @param handle 
 * @param frequency 
 */
void SYNTH_setFrequency(synthHandle_t* handle, uint16_t frequency)
{
	synthGPIOHardwareAdapter_t* adapter = (synthGPIOHardwareAdapter_t*)(handle->config->adapter);

	TPM_SetFrequency(adapter->base, frequency, adapter->channel);
}

/**
 * @brief Set duty of wave
 * 
 * @param handle 
 * @param duty
 */
void SYNTH_setDuty(synthHandle_t* handle, uint8_t duty)
{
	synthGPIOHardwareAdapter_t* adapter = (synthGPIOHardwareAdapter_t*)(handle->config->adapter);

	uint16_t module = TPM_GetModulo(adapter->base);

	TPM_SetChMatch(adapter->base, adapter->channel, (uint16_t)((module * duty) / 255));
}

/**
 * @brief Internal function to allocate a given adapter
 * 
 * @return synthGPIOHardwareAdapter_t* Created adapter
 */
static synthGPIOHardwareAdapter_t* AllocAdapter()
{
	synthGPIOHardwareAdapter_t* objectCreated = NULL;
#ifdef SYNTH_STATIC_OBJECTS_CREATION
	if(g_staticGPIOAdaptersCreated < SYNTH_MAX_STATIC_OBJECTS)
	{
		objectCreated = (void*)&g_synthGPIOAdapterList[g_staticGPIOAdaptersCreated++];
	}
#else
	objectCreated = embUtil_Malloc(sizeof(lcdParallelHardwareAdapter_t));
#endif
	return objectCreated;
}

/**
 * @brief Destroys a given GPIO synth adapter
 * 
 * @param adapter Adapter to be destroyed
 */
void SYNTH_FreeGPIOAdapter(synthAdapter_t adapter)
{
#ifdef SYNTH_STATIC_OBJECTS_CREATION
	if(g_staticGPIOAdaptersCreated)
		--g_staticGPIOAdaptersCreated;
#else
	embUtil_Free(obj);
#endif
	adapter = NULL;
}

#ifdef __cplusplus
}  /* extern "C" */
#endif

/*! @}*/

#endif /* !SYNTH_DISABLE_GPIO_ADAPTER */
