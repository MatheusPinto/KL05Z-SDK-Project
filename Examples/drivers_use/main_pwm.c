#include <Drivers/port/port.h>
#include <Drivers/tpm/tpm.h>
#include <Libraries/delay/delay.h>

int main(void)
{
	const uint16_t tpmPwmModulo = 65535U;

	/*Define como fonte de clock do contador o FLL que gera 20.971520 MHz.*/
	TPM_SetCounterClkSrc(TPM0, TPM_CNT_CLOCK_FLL);

	/*Inicializa TPM0, dividindo clock de entrada por 4, resultando em
	 * um clock do contador de 20,971520 MHz/4 = 5,242880 MHz.
	 * Dessa forma, a frequência do PWM é de 5,242880 MHz/tpmPwmModulo */
	TPM_Init(TPM0, tpmPwmModulo, TPM_PRESCALER_DIV_4);

	PORT_Init(PORTB);
	PORT_SetMux(PORTB, 9, PORT_MUX_ALT2);

	/* O canal que controla o pino PTB9 é o TPM0_Ch2
	 * Irá ser utilizado o PWM alinhado a borda, com o duty cycle em nível alto*/
	TPM_InitChannel(TPM0, 2, TPM_EDGE_PWM_MODE, TPM_PWM_HIGH_TRUE_CONFIG);
	TPM_SetChMatch(TPM0, 2, 0); /*Configuração do duty cycle (valor de match)*/

	/*Inicializa registrador contador que irá contar de 0 a tpmPwmModulo em
	 * 5,242880 MHz.*/
	TPM_InitCounter(TPM0);

	Delay_Init();

	uint16_t updatedDutycycle = 0;
	bool brightnessUp = true;
    for ( ; ; )
    {
    	/* Atrasa para ver a mudança de brilho no LED. */
        Delay_Waitus( 50 );
        if ( true == brightnessUp )
        {
            /* Aumenta o duty cycle até alcançar o valor limite. */
            if ( tpmPwmModulo == ++updatedDutycycle )
            {
                brightnessUp = false;
            }
        }
        else
        {
        	/* Diminui o duty cycle até alcançar o valor limite. */
            if ( 0U == --updatedDutycycle )
            {
                brightnessUp = true;
            }
        }

        TPM_SetChMatch( TPM0, 2, updatedDutycycle ); /*Configuração do duty cycle (valor de match)*/
    }

    return 0;
}
