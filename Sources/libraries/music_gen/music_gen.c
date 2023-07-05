
/** Self header */
#include "music_gen.h"

/** Synth */
#include "libraries/synth/services/services.h"

/** Std */
#include <stdlib.h>

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*!< Music gen step types */
enum
{
	MUSIC_GEN_STEP_HALF,
	MUSIC_GEN_STEP_WHOLE,
	MUSIC_GEN_STEP_QUARTER,
};

/*!< Music modes supported by music gen */
enum
{
	MUSIC_GEN_MAJOR_MODE,
	MUSIC_GEN_MINOR_MODE,
	MUSIC_GEN_IONIAN_MODE,
	MUSIC_GEN_DORIAN_MODE,
	MUSIC_GEN_PHRYGIAN_MODE,
	MUSIC_GEN_LYDIAN_MODE,
	MUSIC_GEN_MIXOLYDIAN_MODE,
	MUSIC_GEN_AEOLIAN_MODE,
	MUSIC_GEN_LOCRIAN_MODE,
};

/*!< Roots supported by music gen */
enum
{
	MUSIC_GEN_ROOT_C = 0,
	MUSIC_GEN_ROOT_D_BEMOL,
	MUSIC_GEN_ROOT_D,
	MUSIC_GEN_ROOT_E_BEMOL,
	MUSIC_GEN_ROOT_E,
	MUSIC_GEN_ROOT_F,
	MUSIC_GEN_ROOT_F_SHARP,
	MUSIC_GEN_ROOT_G,
	MUSIC_GEN_ROOT_A_BEMOL,
	MUSIC_GEN_ROOT_A,
	MUSIC_GEN_ROOT_B_BEMOL,
};

/*!< Enumeration to identify objects creation.*/
enum
{
	MUSIC_GEN_OBJECT_IS_HANDLE,
	MUSIC_GEN_OBJECT_IS_CONFIG
};

/*******************************************************************************
 * Locals
 ******************************************************************************/

/*!< Array of notes */
static musicNote_t notes[MUSIC_GEN_MAX_NOTES] = {};

/**
 * Be careful to make sure steps have maximum of 7 elements.
 */
const static musicGenModes_t music_modes[] = {
	{
		.mode = MUSIC_GEN_MAJOR_MODE,
		.steps = {
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
		}
	},
	{
		.mode = MUSIC_GEN_MINOR_MODE,
		.steps = {
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
		}
	},
	{
		.mode = MUSIC_GEN_IONIAN_MODE,
		.steps = {
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
		}
	},
	{
		.mode = MUSIC_GEN_DORIAN_MODE,
		.steps = {
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
		}
	},
	{
		.mode = MUSIC_GEN_PHRYGIAN_MODE,
		.steps = {
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
		}
	},
	{
		.mode = MUSIC_GEN_LYDIAN_MODE,
		.steps = {
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
		}
	},
	{
		.mode = MUSIC_GEN_MIXOLYDIAN_MODE,
		.steps = {
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
		}
	},
	{
		.mode = MUSIC_GEN_AEOLIAN_MODE,
		.steps = {
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
		}
	},
	{
		.mode = MUSIC_GEN_LOCRIAN_MODE,
		.steps = {
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_HALF,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
			MUSIC_GEN_STEP_WHOLE,
		}
	},
};

/*!< The list of notes that can be played by the music gen module.*/
const static uint16_t notes_frequency[] = {
	163, 173, 183, 194, 206, 218, 231, 245, 259, 275, 291, 308,
	327, 346, 367, 388, 412, 436, 462, 490, 519, 550, 582, 617,
	654, 693, 734, 777, 824, 873, 925, 980, 1038, 1100, 1165, 1234,
	1308, 1385, 1468, 1555, 1648, 1746, 1850, 1960, 2076, 2200, 2330, 2469,
	2616, 2771, 2936, 3111, 3296, 3492, 3700, 3920, 4153, 4400, 4661, 4938,
	5232, 5543, 5873, 6222, 6592, 6984, 7400, 7839, 8306, 8800, 9323, 9877,
	10465, 11087, 11746, 12445, 13185, 13969, 14799, 15679, 16612, 17600, 18646, 19755,
	20930, 22174, 23493, 24890, 26370, 27938, 29599, 31359, 33224, 35200, 37293, 39510
};

#ifdef MUSIC_GEN_STATIC_OBJECTS_CREATION

/*!< The static list of configuration structures that is returned to the MUSIC GEN API.*/
static musicGenConfig_t g_musicGenConfigList[MUSIC_GEN_MAX_STATIC_OBJECTS];

/*!< The static list of handle structures that is returned to the MUSIC GEN API.*/
static musicGenHandle_t g_musicGenHandleList[MUSIC_GEN_MAX_STATIC_OBJECTS];

/*!< The number of configuration and handle structures created using the MUSIC GEN API.*/
static uint8_t g_staticConfigsCreated;
static uint8_t g_staticHandlesCreated;

#endif

/*******************************************************************************
 * Forward declarations
 ******************************************************************************/

/**
 * @brief Create an specific object used by an MUSIC GEN instance.
 *
 * @param objectType - \a MUSIC_GEN_OBJECT_IS_HANDLE, if want to create a MUSIC GEN handle;
 * 					   \a MUSIC_GEN_OBJECT_IS_CONFIG, if want to create a MUSIC GEN configuration structure.
 *
 */
static void* MusicGenCreateObject(uint8_t objectType);

/**
 * @brief Destroy an specific object used by an MUSIC GEN instance.
 * 
 * @param objectType - \a MUSIC_GEN_OBJECT_IS_HANDLE, if want to destroy a MUSIC GEN handle;
 * 					   \a MUSIC_GEN_OBJECT_IS_CONFIG, if want to destroy a MUSIC GEN configuration structure.
 * @param object Object to be destroyed.
 */
void MusicGenDestroyObject(uint8_t objectType, void *object);

/**
 * @brief Select a key and a mode to be used by the MUSIC GEN module.
 * 
 * @param handle MUSIC GEN handle.
 */
void MUSIC_GEN_SelectKey(musicGenHandle_t *handle);

/**
 * @brief Generate a given scale from the selected key and mode
 * 
 * @param handle MUSIC GEN handle.
 */
void MUSIC_GEN_GenerateScale(musicGenHandle_t *handle);

/**
 * @brief Generates steps from key and mode
 * 
 * @param handle MUSIC GEN handle.
 */
void MUSIC_GEN_GenerateSteps(musicGenHandle_t *handle, uint8_t *steps);

/*******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * @brief Create an specific object used by an LCD instance.
 *
 * @param objectType - \a MUSIC_GEN_OBJECT_IS_HANDLE, if want to create a LCD handle;
 * 					   \a MUSIC_GEN_OBJECT_IS_CONFIG, if want to create a LCD configuration structure.
 *
 */
static void* MusicGenCreateObject(uint8_t objectType)
{
	void* objectCreated = NULL;
#ifdef MUSIC_GEN_STATIC_OBJECTS_CREATION
	switch (objectType)
	{
	case MUSIC_GEN_OBJECT_IS_HANDLE:
		if(g_staticHandlesCreated < MUSIC_GEN_MAX_STATIC_OBJECTS)
		{
			objectCreated = (void*)&g_musicGenHandleList[g_staticHandlesCreated++];
		}
		break;
	case MUSIC_GEN_OBJECT_IS_CONFIG:
		if(g_staticConfigsCreated < MUSIC_GEN_MAX_STATIC_OBJECTS)
		{
			objectCreated = (void*)&g_musicGenConfigList[g_staticConfigsCreated++];
		}
		break;
	}
#else
	switch (objectType)
	{
	case LCD_OBJECT_IS_HANDLE:
		objectCreated = embUtil_Malloc(sizeof(musicGenHandle_t));
		break;
	case LCD_OBJECT_IS_CONFIG:
		objectCreated = embUtil_Malloc(sizeof(musicGenConfig_t));
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
void MusicGenDestroyObject(uint8_t objectType, void *object)
{
#ifdef MUSIC_GEN_STATIC_OBJECTS_CREATION
	switch (objectType)
	{
	case MUSIC_GEN_OBJECT_IS_HANDLE:
		if(g_staticHandlesCreated)
			--g_staticHandlesCreated;
		break;
	case MUSIC_GEN_OBJECT_IS_CONFIG:
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
 * @brief Initializes the music gen module
 * 
 * @param synth Synth handle
 * @return musicGenHandle_t* New handle for music generator
 */
musicGenHandle_t* MUSIC_GEN_Init(synthHandle_t *synth)
{
	musicGenHandle_t *handle = (musicGenHandle_t*)MusicGenCreateObject(MUSIC_GEN_OBJECT_IS_CONFIG);
	musicGenConfig_t *config = (musicGenConfig_t*)MusicGenCreateObject(MUSIC_GEN_OBJECT_IS_HANDLE);

	/** TODO add free */
	if(!handle || !config) return NULL;

	/** Config setup */
	config->seed = 0;
	config->loop = 0;
	config->isPlaying = 0;
	config->note_index = 0;
	config->notes_number = 0;

	handle->synthHandle = synth;
	handle->config = config;

	return handle;
}

/**
 * @brief Generates a new music
 * 
 * @param handle MusicGen handle
 * @param seed Seed for the random number generator
 * @param loop 1 to keep music in loop mode, 0 to play only once
 */
void MUSIC_GEN_Generate(musicGenHandle_t *handle, uint8_t seed, uint8_t loop)
{
	/** Initializes random number generator */
	srand(seed);

	handle->config->seed = seed;
	handle->config->loop = loop;
	handle->config->notes_number = MUSIC_GEN_MAX_NOTES;

	/** Generates the music */
	MUSIC_GEN_SelectKey(handle);
	MUSIC_GEN_GenerateScale(handle);
}

/**
 * @brief Stops the music gen module
 * 
 * @param handle MusicGen handle
 */
void MUSIC_GEN_Stop(musicGenHandle_t *handle)
{
	/** Stop event polling */
	handle->config->isPlaying = 0;
	/** Stop synth */
	SYNTH_Stop(handle->synthHandle);
}

/**
 * @brief Plays the music gen module
 * 
 * @param handle MusicGen handle
 */
void MUSIC_GEN_Play(musicGenHandle_t *handle)
{
	/** Resume event polling */
	handle->config->isPlaying = 1;
	/** Play synth */
	SYNTH_Play(handle->synthHandle);
}

/**
 * @brief Polls the music gen module
 * 
 * @param handle MusicGen handle
 * @param dt_ms Amount of time passed in ms
 */
void MUSIC_GEN_Poll(musicGenHandle_t *handle, uint16_t dt_ms)
{
	/** Check if is playing */
	if (!handle->config->isPlaying) return;

	/** Increments total time elapsed in current note */
	handle->config->elapsed_time += dt_ms;

	/** Get current note */
	musicNote_t *currentNote = &notes[handle->config->note_index];

	/** If current note playing time is complete */
	if (handle->config->elapsed_time > currentNote->duration)
	{
		/** Reset elapsed timer */
		handle->config->elapsed_time = 0;

		if (handle->config->note_index >= (handle->config->notes_number - 1))
		{
			/** If last note was played */
			if (handle->config->loop)
			{
				/** If loop is enabled, restart from first note */
				handle->config->note_index = 0;
			}
			else
			{
				/** If loop is disabled, stop playing */
				MUSIC_GEN_Stop(handle);
				return;
			}
		}
		else
		{
			/** Increments note pointer */
			handle->config->note_index++;
		}

		/** Get new note */
		currentNote = &notes[handle->config->note_index];

		/** Play new note */
		SYNTH_SetFrequency(handle->synthHandle, (uint16_t)(currentNote->frequency / 10));
		SYNTH_SetVolume(handle->synthHandle, currentNote->volume);
	}
}

/**
 * @brief Select a key and a mode to be used by the MUSIC GEN module.
 * 
 * @param handle MUSIC GEN handle.
 */
void MUSIC_GEN_SelectKey(musicGenHandle_t *handle)
{
	/** Get a random mode from modes array */
	handle->config->mode = music_modes[rand() % (sizeof(music_modes) / sizeof(music_modes[0]))];
	/** Get a random root */
	handle->config->root = rand() % 11;
}

/**
 * @brief Generates steps from key and mode
 * 
 * @param handle MUSIC GEN handle.
 */
void MUSIC_GEN_GenerateSteps(musicGenHandle_t *handle, uint8_t *steps)
{
	for (uint8_t i = 0; i < 7; ++i)
	{
		uint8_t key = handle->config->mode.steps[i];
		if (key == MUSIC_GEN_STEP_WHOLE)
		{
			steps[i] = i + 2;
		}
		else if (key == MUSIC_GEN_STEP_QUARTER)
		{
			steps[i] = i + 3;
		}
		else
		{
			steps[i] = i + 1;
		}
	}
}

/**
 * @brief Generate a given scale from the selected key and mode
 * 
 * @param handle MUSIC GEN handle.
 */
void MUSIC_GEN_GenerateScale(musicGenHandle_t *handle)
{
	/** Generate steps */
	uint8_t steps[7];
	MUSIC_GEN_GenerateSteps(handle, steps);

	/** (max_octave - min_octave) * 8 */
	uint8_t last_note = MUSIC_GEN_MAX_NOTES;

	uint8_t start = handle->config->root;

	for (uint8_t i = 0; i < last_note; ++i)
	{
		uint8_t index = start + steps[i % 7] + (rand() % 75) * 12;

		if (index > sizeof(notes_frequency) / sizeof(notes_frequency[0]))
		{
			index = 0;
		}

		uint16_t note = (uint16_t)notes_frequency[index];
		uint16_t duration = (uint16_t)(100 + (uint16_t)((uint16_t)rand() % 1000));
		uint8_t volume = (uint8_t)(50 + (uint8_t)((uint8_t)rand() % 50));
	
		notes[i].frequency = note;
		notes[i].duration = duration;
		notes[i].volume = volume;
	}
}
