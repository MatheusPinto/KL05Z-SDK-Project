
#ifndef MUSIC_GEN_H_
#define MUSIC_GEN_H_

/** General config */
#include <common.h>

/** Synth */
#include "Libraries/synth/synth.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup music_gen
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MUSIC_GEN_DEFAULT_SEED 0x00

/** Must be multiple of  */
#define MUSIC_GEN_MAX_NOTES 100

/* !< Defines if music gen instances will be created statically.
 * If commented, music gen instances will be allocated dynamically in heap.
 */
#define MUSIC_GEN_STATIC_OBJECTS_CREATION
#ifdef MUSIC_GEN_STATIC_OBJECTS_CREATION
	/*!< The number of object instances that will be created statically.*/
	#define MUSIC_GEN_MAX_STATIC_OBJECTS 1
#endif

/*******************************************************************************
 * Structures
 ******************************************************************************/

/*!
 * @brief Structure that holds a given note configuration
 */
typedef struct
{
	/*!< Frequency of the note */
	uint16_t frequency;

	/*!< Duration of the note in ms */
	uint16_t duration;

	/*!< Volume of the note */
	uint8_t volume;
} musicNote_t;

/*!
 * @brief Structure that holds a given mode configuration
 */
typedef struct
{
	/*!< Mode type */
	uint8_t mode;

	/*!< Mode steps */
	uint8_t steps[7];
} musicGenModes_t;

/*!
 * @brief Music gen configuration structure
 *
 * This structure holds the configuration settings for the music gen module.
 */
typedef struct
{
	/*!< Seed for the random number generator */
	uint8_t seed;

	/*!< Selected root */
	uint8_t root;

	/*!< Selected mode */
	musicGenModes_t mode;

	/*!< 1 to keep music in loop mode, 0 to play only once */
	uint8_t loop;

	/*!< 1 if music is playing, 0 otherwise */
	uint8_t isPlaying;

	/*!< Number of notes in the music */
	uint16_t notes_number;

	/*!< Current note being played */
	uint16_t note_index;

	/*!< Time elapsed in current note */
	uint16_t elapsed_time;
} musicGenConfig_t;

/*!
 * @brief Music gen handle structure used internally
 */
typedef struct
{
	/*!< Music gen configuration */
	musicGenConfig_t *config;

	/*!< Synth handle */
	synthHandle_t *synthHandle;
} musicGenHandle_t;

/*******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * @brief Initializes the music gen module
 * 
 * @param synth Synth handle
 * @return musicGenHandle_t* New handle for music generator
 */
musicGenHandle_t* MUSIC_GEN_Init(synthHandle_t *synth);

/**
 * @brief Generates a new music
 * 
 * @param handle MusicGen handle
 * @param seed Seed for the random number generator
 * @param loop 1 to keep music in loop mode, 0 to play only once
 */
void MUSIC_GEN_Generate(musicGenHandle_t *handle, uint8_t seed, uint8_t loop);

/**
 * @brief Stops the music gen module
 * 
 * @param handle MusicGen handle
 */
void MUSIC_GEN_Stop(musicGenHandle_t *handle);

/**
 * @brief Plays the music gen module
 * 
 * @param handle MusicGen handle
 */
void MUSIC_GEN_Play(musicGenHandle_t *handle);

/**
 * @brief Polls the music gen module
 * 
 * @param handle MusicGen handle
 * @param dt_ms Amount of time passed in ms
 */
void MUSIC_GEN_Poll(musicGenHandle_t *handle, uint16_t dt_ms);

#ifdef __cplusplus
}  /* extern "C" */
#endif

/*! @}*/

#endif /* MUSIC_GEN_H_ */
