#include <mcu/drivers/port/port.h>
#include <mcu/drivers/gpio/gpio.h>
#include <mcu/drivers/tpm/tpm.h>
#include <mcu/drivers/adc/adc.h>
#include <mcu/common.h>
#include "stdio.h"

/* Canal 13
 * Modo saída única: PTB13 (ADC0_SE13)*/
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

	MCG->C1 |= MCG_C1_IRCLKEN_MASK; // habilitar o Slow IRC para os periféricos

	/*Define como fonte de clock do contador o Slow IRC que gera 32,768 kHz.*/
	TPM_SetCounterClkSrc( TPM0, TPM_CNT_CLOCK_IRC );

	/*Inicializa TPM0, dividindo clock de entrada por 128, resultando em
	* um clock do contador de 32,768/128 = 256 Hz.
	* Dessa forma, a frequência de fim de contagem é de
	* 256 Hz/tpmModulo = 1 Hz  */
	TPM_Init( TPM0, tpmModulo, TPM_PRESCALER_DIV_128 );
	TPM_EnableIRQ( TPM0 );

	/*
     * Os valores padrões, caso apenas ADC_Init seja chamado, antes de
     * inicilizar um canal são:
     * Tensão de referência = Vref;
     * clock de entrada interna = Bus;
     * divisor do clock interno = 1;
     * resolução = 8 Bit;
     * modo amostragem longa = desabilitado;
     * modo alta velocidade = desabilitado;
     * modo baixa potência = desabilitado;
     * modo comparação = desabilitado;
     * conversão continua = desabilitado.
     */
	ADC_Init( ADC0 );
	ADC_SetResolution( ADC0, ADC_RESOLUTION_12_BIT );
	ADC_EnableHardwareTrigger( ADC0, ADC_TPM0_OVERFLOW );
	NVIC_EnableIRQ(ADC0_IRQn); /* Habilita interrupção pelo NVIC. */

    /* Resultados da conversão serão impressos em console serial. */
	printf("\r\nADC temporizador - exemplo.\r\n");

    /* Realiza a calibração do do ADC. */
    if ( ADC_DoAutoCalibration( ADC0 ) == SYSTEM_STATUS_SUCCESS )
    {
    	printf( "ADC_DoAutoCalibration() Pronto.\r\n" );
    }
    else
    {
    	/* Calibração pode falhar devido a diversos fatores:
    	 * ver manual do KL05Z, pag. 438, bit CALF.
    	*/
    	printf( "ADC_DoAutoCalibration() Falhou.\r\n" );
    }

    ADC_SetChConfig( ADC0, ADC_CHANNEL, true );

    g_AdcInterruptCounter = 0U;
	TPM_InitCounter( TPM0 ); /*Inicializa registrador contador*/
    for ( ; ; )
    {
		/* Espere pelo fim da conversão quando "ADC0_IRQHandler" irá
		 setar g_AdcConversionDoneFlag*/
		while (!g_AdcConversionDoneFlag)
		{
		}
		g_AdcConversionDoneFlag = false;

		printf("Valor do ADC: %d\t\t", g_AdcConversionValue);
		printf("Contagem de interrupcoes: %d\r\n", g_AdcInterruptCounter);
    }
}
