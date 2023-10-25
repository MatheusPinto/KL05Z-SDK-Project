#include <Drivers/port/port.h>
#include <Drivers/gpio/gpio.h>
#include <Drivers/tpm/tpm.h>
#include <Drivers/adc/adc.h>
#include <common.h>
#include "stdio.h"

/* Canal 13
 * Modo sa�da �nica: PTB13 (ADC0_SE13)*/
#define ADC_CHANNEL 13U
#define ADC_PORT PORTB
#define ADC_PORT_PIN 13

volatile bool g_AdcConversionDoneFlag = false;
volatile uint32_t g_AdcConversionValue;
volatile uint32_t g_AdcInterruptCounter;

void ADC0_IRQHandler(void)
{
	/* Read conversion result to clear the conversion completed flag. */
	g_AdcConversionValue = ADC_GetChConversionValue( ADC0 );

	++g_AdcInterruptCounter;
	g_AdcConversionDoneFlag = true;
}

int main(void)
{
	const uint16_t tpmModulo = 256U;

	PORT_Init( ADC_PORT );
	PORT_SetMux( ADC_PORT, ADC_PORT_PIN, PORT_MUX_ALT0 );

	MCG->C1 |= MCG_C1_IRCLKEN_MASK; // habilitar o Slow IRC para os perif�ricos

	/*Define como fonte de clock do contador o Slow IRC que gera 32,768 kHz.*/
	TPM_SetCounterClkSrc( TPM0, TPM_CNT_CLOCK_IRC );

	/*Inicializa TPM0, dividindo clock de entrada por 128, resultando em
	* um clock do contador de 32,768/128 = 256 Hz.
	* Dessa forma, a frequ�ncia de fim de contagem � de
	* 256 Hz/tpmModulo = 1 Hz  */
	TPM_Init( TPM0, tpmModulo, TPM_PRESCALER_DIV_128 );
	TPM_EnableIRQ( TPM0 );

	/*
     * Os valores padr�es, caso apenas ADC_Init seja chamado, antes de
     * inicilizar um canal s�o:
     * Tens�o de refer�ncia = Vref;
     * clock de entrada interna = Bus;
     * divisor do clock interno = 1;
     * resolu��o = 8 Bit;
     * modo amostragem longa = desabilitado;
     * modo alta velocidade = desabilitado;
     * modo baixa pot�ncia = desabilitado;
     * modo compara��o = desabilitado;
     * convers�o continua = desabilitado.
     */
	ADC_Init( ADC0 );
	ADC_SetResolution( ADC0, ADC_RESOLUTION_12_BIT );
	ADC_EnableHardwareTrigger( ADC0, ADC_TPM0_OVERFLOW );
	NVIC_EnableIRQ(ADC0_IRQn); /* Habilita interrup��o pelo NVIC. */

    /* Resultados da convers�o ser�o impressos em console serial. */
	printf("\r\nADC temporizador - exemplo.\r\n");

    /* Realiza a calibra��o do do ADC. */
    if ( ADC_DoAutoCalibration( ADC0 ) == SYSTEM_STATUS_SUCCESS )
    {
    	printf( "ADC_DoAutoCalibration() Pronto.\r\n" );
    }
    else
    {
    	/* Calibra��o pode falhar devido a diversos fatores:
    	 * ver manual do KL05Z, pag. 438, bit CALF.
    	*/
    	printf( "ADC_DoAutoCalibration() Falhou.\r\n" );
    }

    ADC_SetChConfig( ADC0, ADC_CHANNEL, true );

    g_AdcInterruptCounter = 0U;
	TPM_InitCounter( TPM0 ); /*Inicializa registrador contador*/
    for ( ; ; )
    {
		/* Espere pelo fim da convers�o quando "ADC0_IRQHandler" ir�
		 setar g_AdcConversionDoneFlag*/
		while (!g_AdcConversionDoneFlag)
		{
		}
		g_AdcConversionDoneFlag = false;

		printf("Valor do ADC: %d\t\t", g_AdcConversionValue);
		printf("Contagem de interrupcoes: %d\r\n", g_AdcInterruptCounter);
    }
}
