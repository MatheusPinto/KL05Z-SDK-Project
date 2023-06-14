#include <mcu/drivers/port/port.h>
#include <mcu/drivers/gpio/gpio.h>
#include <libraries/delay/delay.h>

int main(void)
{
	PORT_Init( PORTB );
	GPIO_InitOutputPin( GPIOB, 9, 1 );
	Delay_Init();

    for (;;)
    {
    	Delay_Waitms(1000);
    	GPIO_TogglePin( GPIOB, 9 );
    }

    return 0;
}
