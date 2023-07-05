#ifndef SYNTH_SERVICES_H_
#define SYNTH_SERVICES_H_

/* SYNTH */
#include "../synth.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup synth
 * @{
 */

/**
 * @brief Play a certain configured note in handle
 *
 * @param handle - the specific SYNTH handle.
 */
void SYNTH_Play(synthHandle_t *handle);

/**
 * @brief Play a certain configured note in handle
 *
 * @param handle - the specific SYNTH handle.
 */
void SYNTH_Stop(synthHandle_t *handle);

/**
 * @brief Play a certain configured note in handle
 *
 * @param handle - the specific SYNTH handle.
 * @param frequency - Frequency to be used in wave generation.
 */
void SYNTH_SetFrequency(synthHandle_t *handle, uint16_t frequency);

/**
 * @brief Play a certain configured note in handle
 *
 * @param handle - the specific SYNTH handle.
 * @param volume - The master volume to be used in wave generation, between 0 and 100.
 */
void SYNTH_SetVolume(synthHandle_t *handle, uint8_t volume);


#ifdef __cplusplus
}  /* extern "C" */
#endif

/*! @}*/

#endif /* !SYNTH_SERVICES_H_ */
