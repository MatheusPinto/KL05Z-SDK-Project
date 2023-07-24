[![en](https://img.shields.io/badge/lang-en-red.svg)](/README.md)
[![pt-br](https://img.shields.io/badge/lang-pt--br-green.svg)](/README.pt-br.md)


# Project with SDK for KL05Z

This project was created in Kinetis Design Studio (KDS) / Eclipse from NXP to assist in the development of applications with microcontrollers, especially for the Kinetis KL05Z. This SDK (Software Development Kit) was designed to be used with any family of microcontrollers from any manufacturer, requiring only specific modifications in some files.

## Directories

The project has a predefined structure for standardized and simplified use of the SDK.

The SDK is structured within the `Sources` folder. The directories above it are specific to the microcontroller configuration in KDS.

- `mcu`: Contains the API and specific implementations for the used microcontroller.
  - `drivers`: Implements the microcontroller peripheral drivers.
  - `common.h`: File with common definitions for the drivers.

- `libraries`: Generic libraries to assist in development.

- `system`: Module with files related to system configurations. Some notable directories within this module include:
  - `os`: Contains a generic Operating System (OS) API. It is recommended to port specific OS functions (e.g., FreeRTOS, Zephyr, TinyOS, etc.) into this software layer.
  - `system.h`: Header file with the definition of the `System_Init` function, which should be called at the beginning of the `main` function.
  - `hardware_init.h/.c`: Should contain the user-provided implementation of the system hardware initialization code (internal/external peripherals and clock system). The initialization should be triggered by the `System_Hardware_Init` function, which will be invoked within `System_Init`.
  - `software_init.h/.c`: Should contain the user-provided implementation of the software library initialization code (e.g., Delay, random number seed generation, etc.). The initialization should be triggered by the `System_Software_Init` function, which will be invoked within `System_Init`.

- `app_tasks`: Implementation of application tasks. Each task should be implemented as a separate C function, with its prototype available in a header file.

- `events`: Functions that are called due to internal or external events of the microcontroller.
  - `isr`: Contains the microcontroller's interrupt service routines (ISRs).
  - `os`: Contains the system's event handling functions for the used operating system.

- `fsm`: If the firmware is bare-metal (without an OS), it is recommended to develop using a Finite State Machine (FSM) model. This directory contains an FSM model for system implementation.

- `examples`: Contains example codes with a `main` function. Other files in this directory should be disabled during compilation to avoid multiple definitions of `main`.

## Coding Convention
In code not developed by third parties, there is a defined coding convention.

#### Functions
All functions begin with the suggestive name of the module to which they belong, starting with an uppercase letter. This name is followed by an underscore `_` and the specific function name, starting with an uppercase letter.

```c
  <Module Name>_FunctionName();
```

Examples:

```c
  Delay_WaitMS();
  GPIO_SetPin();
  Util_ClearMask();
  LCD_Print();
```

For complex modules with multiple sub-modules (e.g., OS), the following convention is allowed:

```c
  <Module Name>_<Submodule Name>_FunctionName();
```

Examples:

```c
  OS_Mutex_Take();
  OS_Task_Create();
  OS_Heap_Alloc();
  OS_Sched_Start();
```

#### Variables
All variables start with a lowercase letter. If they consist of multiple words, each subsequent word starts with an uppercase letter.

Examples:

```c
  myFlag;
  x;
  dataIn;
  lastWokenTime;
```

- Local scope variables (declared within functions) should be named as described above.
- Global variables, whether static or external, should have their names preceded by `g_`.
- Constant variables should have their names preceded by an underscore `_`.

#### Defined Types
Types defined using `typedef` follow the same variable naming convention, but end with `_t`. It is also advised to include the module name as the first word in the type definition.

Examples:

```c
  myType_t;
  uartParity_t;
  adcStatusFlag_t;
  tpmPrescalerValues_t;
```

#### Macros and Enumerated Values
All letters should be in uppercase, with words separated by underscores `_`. It is also advised to include the module name as the first word in the macro or enumeration definition.

Examples:

```c
  #define MY_DEFINITION
  #define BUFFER_MAX_LENGTH

  typedef enum{
    UART_ONE_STOP_BIT = 0U, 
    UART_TWO_STOP_BIT 
  }uartStopBitNum_t;
```
