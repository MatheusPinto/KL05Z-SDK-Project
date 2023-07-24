# Synth Module

The Synth module is a software component designed to provide wave generation functionality on a hardware platform. It allows users to generate and control waveforms for audio synthesis or other applications requiring waveform generation. This readme provides an overview of the module's features, specifications, and usage guidelines.

## Features

The Synth module offers the following features:

1. Waveform Generation: The module can generate various waveforms, including square, sine, triangle, and more, based on user configurations.

2. Hardware Abstraction: The module provides a hardware abstraction layer through the synthAdapterInterface_t structure, allowing easy integration with different hardware adapters.

3. Dynamic or Static Object Creation: The module supports both dynamic and static object creation. Users can choose between allocating synth instances dynamically in the heap or creating them statically.

4. Play and Stop Controls: Users can play and stop the configured waveform using the provided functions.

5. Frequency and Duty Control: The module allows users to set the frequency and duty cycle of the waveform using the provided functions.

6. Volume Control: Users can set the master volume of the generated waveform, ranging from 0 to 100.

## Specifications

The Synth module has the following specifications:

- Programming Language: C/C++
- Platform Compatibility: MCU-based platforms
- Dependencies: mcu/common.h
- Interface: Header files (synth.h and synth_services.h)
- Supported Waveforms: Square, Sine, Triangle, etc.
- Supported Hardware Adapters: GPIO Adapter (additional adapters can be added)

## Usage

To use the Synth module, follow these steps:

1. Include the necessary header files in your project:
```c
#include "synth.h"
#include "synth_services.h"
```

2. Configure the hardware adapter to be used with the Synth module. The module currently supports the GPIO adapter by default.

3. Initialize the Synth module by calling the `SYNTH_Init()` function, passing the hardware adapter as a parameter. This function returns a pointer to the synthHandle_t structure, which serves as the handle for subsequent operations.

4. Use the provided functions to control the Synth module:

- `SYNTH_Play()`: Play the configured waveform.
- `SYNTH_Stop()`: Stop the waveform playback.
- `SYNTH_SetFrequency()`: Set the frequency of the waveform.
- `SYNTH_SetVolume()`: Set the master volume of the waveform.

5. Customize the module as per your requirements by modifying the provided configuration structure, `synthConfig_t`, within the synthHandle_t handle.

## Example

Here's an example code snippet demonstrating the basic usage of the Synth module:

```c
#include "synth.h"
#include "synth_services.h"

// Create a handle for the Synth module
synthHandle_t* synthHandle;

int main() {
    // Initialize the Synth module with a hardware adapter
    synthHandle = SYNTH_Init(/* hardware adapter */);

    // Play the configured waveform
    SYNTH_Play(synthHandle);

    // Set the frequency and volume of the waveform
    SYNTH_SetFrequency(synthHandle, /* frequency value */);
    SYNTH_SetVolume(synthHandle, /* volume value */);

    // Stop the waveform playback
    SYNTH_Stop(synthHandle);

    return 0;
}
```

Please note that you need to replace `/* hardware adapter */`, `/* frequency value */`, and `/* volume value */` with the appropriate values and adapt the code to match your specific hardware platform.
