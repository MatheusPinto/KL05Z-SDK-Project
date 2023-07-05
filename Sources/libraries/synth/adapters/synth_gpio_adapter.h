#ifndef SYNTH_GPIO_HARDWARE_HARDWARE_ADAPTER_H_
#define SYNTH_GPIO_HARDWARE_HARDWARE_ADAPTER_H_

/* Synth definitions includes */
#include "../synth.h"

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

#endif

#ifdef __cplusplus
}  /* extern "C" */
#endif

/*! @}*/

#endif /* !SYNTH_GPIO_HARDWARE_HARDWARE_ADAPTER_H_ */
