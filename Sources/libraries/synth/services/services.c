
/* Self header */
#include "services.h"


/*******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * @brief Play a certain configured note in handle
 *
 * @param handle - the specific SYNTH handle.
 */
void SYNTH_Play(synthHandle_t *handle)
{
	/** Reinterpret as hardware config interface */
	synthAdapterInterface_t *adapter = (synthAdapterInterface_t*)handle->config->adapter;
	
	adapter->play(handle);
}

/**
 * @brief Play a certain configured note in handle
 *
 * @param handle - the specific SYNTH handle.
 */
void SYNTH_Stop(synthHandle_t *handle)
{
	/** Reinterpret as hardware config interface */
	synthAdapterInterface_t *adapter = (synthAdapterInterface_t*)handle->config->adapter;
	
	adapter->stop(handle);
}

/**
 * @brief Play a certain configured note in handle
 *
 * @param handle - the specific SYNTH handle.
 * @param frequency - Frequency to be used in wave generation.
 */
void SYNTH_SetFrequency(synthHandle_t *handle, uint16_t frequency)
{
	/** Reinterpret as hardware config interface */
	synthAdapterInterface_t *adapter = (synthAdapterInterface_t*)handle->config->adapter;
	
	adapter->setFrequency(handle, frequency);
}

/**
 * @brief Play a certain configured note in handle
 *
 * @param handle - the specific SYNTH handle.
 * @param volume - The master volume to be used in wave generation, between 0 and 100.
 */
void SYNTH_SetVolume(synthHandle_t *handle, uint8_t volume)
{
	/** Reinterpret as hardware config interface */
	synthAdapterInterface_t *adapter = (synthAdapterInterface_t*)handle->config->adapter;

	if (volume > 100)
	{
		volume = 100;
	}
	if (volume < 0)
	{
		volume = 0;
	}

	adapter->setDuty(handle, volume * 126 / 100);
}