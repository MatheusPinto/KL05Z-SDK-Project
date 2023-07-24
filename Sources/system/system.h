#include <stdint.h>
#include "hardware_init.h"
#include "software_init.h"
#include "os/os.h"

//#define __OS_PROJECT

#ifdef __OS_PROJECT
#define SYSTEM_MALLOC( x ) OS_Heap_Alloc( x )
#define SYSTEM_FREE( x )
#else
#include <stdlib.h>
#define SYSTEM_MALLOC( x ) malloc( x )
#define SYSTEM_FREE( x ) free( x )
#endif
void System_Init();
