
#ifndef SYNTH_H_
#define SYNTH_H_

/** General config */
#include <common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup synth
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* !< Defines if synth instances will be created statically.
 * If commented, synth instances will be allocated dynamically in heap.
 */
#define SYNTH_STATIC_OBJECTS_CREATION
#ifdef SYNTH_STATIC_OBJECTS_CREATION
	/*!< The number of object instances that will be created statically.*/
	#define SYNTH_MAX_STATIC_OBJECTS 1
#endif

/*******************************************************************************
 * Types
 ******************************************************************************/

/*!< Hardware configuration structure describing current hardware connections */
typedef void* synthAdapter_t;

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*!< Defines all possible hardware adapters when creating a new synth */
typedef enum
{
	SYNTH_GPIO_ADAPTER,
} synthHardwareAdapters_t;


/*******************************************************************************
 * Structures
 ******************************************************************************/

/*!
 * @brief Synth configuration structure
 *
 * This structure holds the configuration settings for the synth module.
 */
typedef struct
{
	/*!< The LCD bus. */
	synthAdapter_t *adapter;
} synthConfig_t;

/*!
 * @brief synth handle structure used internally
 */
typedef struct
{
	synthConfig_t *config;
} synthHandle_t;

/**
 * @brief Structure that holds the LCD bus information
 */
typedef struct
{
	/*!< Hardware adapter type being used */
	synthHardwareAdapters_t type;

	/*!< Play configured wave */
	void (*play)(synthHandle_t* handle);

	/*!< Stop configured wave */
	void (*stop)(synthHandle_t* handle);

	/*!< Set frequency */
	void (*setFrequency)(synthHandle_t* handle, uint16_t frequency);

	/*!< Set duty */
	void (*setDuty)(synthHandle_t* handle, uint8_t duty);
} synthAdapterInterface_t;

/*******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * @brief Initializes the synth module
 * 
 * @param adapter Hardware adapter to be used
 * @return synthHandle_t* New synth handle
 */
synthHandle_t* SYNTH_Init(synthAdapter_t *adapter);

#ifdef __cplusplus
}  /* extern "C" */
#endif

/*! @}*/

#endif /* SYNTH_H_ */
