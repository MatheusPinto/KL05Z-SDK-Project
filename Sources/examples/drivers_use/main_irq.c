#include <mcu/drivers/port/port.h>
#include <mcu/drivers/gpio/gpio.h>
#include <libraries/delay/delay.h>

/* Ver página 147 e 148 do manual de referência do KL05 para saber quais
 * pinos possuem IRQ. */

void PORTA_IRQHandler( void )
{
	if ( PORT_GetIRQFlag( PORTA, 10 ) )// PTA10 causou interrupção?
    {
        //Limpa a flag de ISR do PTA10
        PORT_ClearIRQFlag( PORTA, 10 );
        //Inverte valor lógico de PTB9
        GPIO_TogglePin( GPIOB, 9 );
    }
}

void NMI_Handler(void)
{
	for ( int i = 0; ; ++i )
	{
		__asm ( "nop" );
		++i;
	}
}

void HardFault_Handler(void)
{
	for ( int i = 0; ; ++i )
	{
		__asm ( "nop" );
		++i;
	}
}

int main(void)
{
	PORT_Init( PORTA );
	PORT_Init( PORTB );

	GPIO_InitOutputPin( GPIOB, 9, 1 );
	GPIO_InitInputPin( GPIOA, 10 );
	PORT_EnablePull( PORTA, 10 );
	PORT_EnableIRQ( PORTA, 10, PORT_IRQ_ON_FALLING_EDGE );

    NVIC_EnableIRQ( PORTA_IRQn );

    for ( ; ; )
    {

    }

    return 0;
}
