#include "hardware_init.h"
#include "libraries_init.h"

int main(void)
{
	Hardware_Init();
	Libraries_Init();

    for (;;)
    {
    }

    return 0;
}
