#include <Drivers/port/port.h>
#include <Drivers/gpio/gpio.h>
#include <Drivers/tpm/tpm.h>
#include <Libraries/delay/delay.h>

void TPM0_IRQHandler(void)
{
	if( TPM_GetIRQFlag( TPM0 ) )
    {
        GPIO_TogglePin( GPIOB, 9 );
    }

	TPM_ClearIRQFlag( TPM0 );
}

int main(void)
{
	const uint16_t tpmModulo = 0xBFFFU;

	// OSC0->CR |= OSC_CR_ERCLKEN_MASK; // habilitar o clock externo para os periféricos
	// MCG->C1 |= MCG_C1_IRCLKEN_MASK; // habilitar o Slow IRC para os periféricos

	/*Define como fonte de clock do contador o FLL que gera 20.971520 MHz.*/
	TPM_SetCounterClkSrc( TPM0, TPM_CNT_CLOCK_FLL );
	/*Inicializa TPM0, dividindo clock de entrada por 128, resultando em
	* um clock do contador de 20,971520 MHz/128 = 163.840 Hz.
	* Dessa forma, a frequência de fim de contagem é de 163.840 Hz/tpmPwmModulo */
	TPM_Init( TPM0, tpmModulo, TPM_PRESCALER_DIV_128 );
	TPM_EnableIRQ( TPM0 );

	PORT_Init( PORTB );
	GPIO_InitOutputPin( GPIOB, 9, 1 );
	PORT_SetMux( PORTB, 9, PORT_MUX_AS_GPIO );

	NVIC_EnableIRQ( TPM0_IRQn );

	Delay_Init( );
	TPM_InitCounter( TPM0 ); /*Inicializa registrador contador*/

    for ( ; ; )
    {

    }

    return 0;
}
