#include "Drivers/port/port.h"
#include "Drivers/gpio/gpio.h"
#include "Libraries/delay/delay.h"

int main(void)
{
	PORT_Init( PORTB );
	GPIO_InitOutputPin( GPIOB, 9, 1 );
	PORT_SetMux( PORTB, 9, PORT_MUX_AS_GPIO );
	Delay_Init();

    for (;;)
    {
    	Delay_Waitms(1000);
    	GPIO_TogglePin( GPIOB, 9 );
    }

    return 0;
}
