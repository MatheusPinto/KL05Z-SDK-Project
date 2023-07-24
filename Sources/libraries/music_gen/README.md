# Music Gen Module

The Music Gen module is a software component designed to generate music procedurally on a hardware platform. It allows users to create dynamic and evolving musical compositions by generating random sequences of notes. This readme provides an overview of the module's features, specifications, and usage guidelines.

## Features

The Music Gen module offers the following features:

1. Procedural Music Generation: The module generates music dynamically by creating random sequences of notes based on user-defined configurations.

2. Note Configuration: Each note in the generated music has a frequency, duration, and volume specified by the `musicNote_t` structure.

3. Mode Configuration: Users can select a musical mode (such as major, minor, etc.) and define custom steps within the mode using the `musicGenModes_t` structure.

4. Looping or One-Time Playback: Users can choose between playing the generated music in a continuous loop or playing it once.

5. Integration with Synth Module: The Music Gen module integrates with the Synth module (from the "libraries/synth/synth.h" library) to generate actual audio output based on the generated music.

6. Dynamic or Static Object Creation: The module supports both dynamic and static object creation. Users can choose between allocating music gen instances dynamically in the heap or creating them statically.

## Specifications

The Music Gen module has the following specifications:

- Programming Language: C/C++
- Platform Compatibility: MCU-based platforms
- Dependencies: mcu/common.h, Synth module (synth.h)
- Interface: Header file (music_gen.h)
- Maximum Number of Notes: 100 (defined as MUSIC_GEN_MAX_NOTES)
- Supported Musical Modes: Customizable mode with up to 7 steps

## Usage

To use the Music Gen module, follow these steps:

1. Include the necessary header files in your project:
```c
#include "music_gen.h"
#include "libraries/synth/synth.h"
```

2. Ensure that you have already initialized and configured the Synth module since it is required for audio generation.

3. Create a handle for the Music Gen module by calling the `MUSIC_GEN_Init()` function and passing the Synth handle as a parameter. This function returns a pointer to the musicGenHandle_t structure, which serves as the handle for subsequent operations.

4. Generate music by calling the `MUSIC_GEN_Generate()` function, providing the musicGenHandle_t handle, a seed value for the random number generator, and a loop mode (1 for continuous loop, 0 for one-time playback).

5. Control the music playback using the following functions:

- `MUSIC_GEN_Play()`: Start playing the generated music.
- `MUSIC_GEN_Stop()`: Stop the music playback.

6. Call the `MUSIC_GEN_Poll()` function periodically and provide the musicGenHandle_t handle and the elapsed time since the last poll in milliseconds.

## Example

Here's an example code snippet demonstrating the basic usage of the Music Gen module:

```c
#include "music_gen.h"
#include "libraries/synth/synth.h"

// Create handles for the Music Gen and Synth modules
musicGenHandle_t* musicGenHandle;
synthHandle_t* synthHandle;

int main() {
    // Initialize and configure the Synth module
    synthHandle = SYNTH_Init(/* hardware adapter */);

    // Initialize the Music Gen module with the Synth handle
    musicGenHandle = MUSIC_GEN_Init(synthHandle);

    // Generate music with a specific seed and loop mode
    MUSIC_GEN_Generate(musicGenHandle, MUSIC_GEN_DEFAULT_SEED, /* loop mode */);

    // Start playing the generated music
   

    MUSIC_GEN_Play(musicGenHandle);

    // ...
    while (true)
    {
        // Apply some delay

        // Poll the Music Gen module periodically
        MUSIC_GEN_Poll(musicGenHandle, /* elapsed time */);
    }

    // Stop the music playback
    MUSIC_GEN_Stop(musicGenHandle);

    return 0;
}
```

Please note that you need to replace `/* hardware adapter */` and `/* loop mode */` with the appropriate values and adapt the code to match your specific hardware platform.

## Conclusion

The Music Gen module provides a flexible and creative solution for procedural music generation on MCU-based platforms. By utilizing the module's functions and configurations, developers can generate dynamic musical compositions and integrate them with audio output using the Synth module.