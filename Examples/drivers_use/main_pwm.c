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
	 * Dessa forma, a frequ�ncia do PWM � de 5,242880 MHz/tpmPwmModulo */
	TPM_Init(TPM0, tpmPwmModulo, TPM_PRESCALER_DIV_4);

	PORT_Init(PORTB);
	PORT_SetMux(PORTB, 9, PORT_MUX_ALT2);

	/* O canal que controla o pino PTB9 � o TPM0_Ch2
	 * Ir� ser utilizado o PWM alinhado a borda, com o duty cycle em n�vel alto*/
	TPM_InitChannel(TPM0, 2, TPM_EDGE_PWM_MODE, TPM_PWM_HIGH_TRUE_CONFIG);
	TPM_SetChMatch(TPM0, 2, 0); /*Configura��o do duty cycle (valor de match)*/

	/*Inicializa registrador contador que ir� contar de 0 a tpmPwmModulo em
	 * 5,242880 MHz.*/
	TPM_InitCounter(TPM0);

	Delay_Init();

	uint16_t updatedDutycycle = 0;
	bool brightnessUp = true;
    for ( ; ; )
    {
    	/* Atrasa para ver a mudan�a de brilho no LED. */
        Delay_Waitus( 50 );
        if ( true == brightnessUp )
        {
            /* Aumenta o duty cycle at� alcan�ar o valor limite. */
            if ( tpmPwmModulo == ++updatedDutycycle )
            {
                brightnessUp = false;
            }
        }
        else
        {
        	/* Diminui o duty cycle at� alcan�ar o valor limite. */
            if ( 0U == --updatedDutycycle )
            {
                brightnessUp = true;
            }
        }

        TPM_SetChMatch( TPM0, 2, updatedDutycycle ); /*Configura��o do duty cycle (valor de match)*/
    }

    return 0;
}
