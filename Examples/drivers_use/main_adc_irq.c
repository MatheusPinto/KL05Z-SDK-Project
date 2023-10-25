#include <Drivers/port/port.h>
#include <Drivers/gpio/gpio.h>
#include <Drivers/adc/adc.h>
#include <common.h>
#include <Libraries/delay/delay.h>
#include <stdio.h>

/* Canal 13
 * Modo saída única: PTB13 (ADC0_SE13)*/
#define ADC_CHANNEL 13U
#define ADC_PORT PORTB
#define ADC_PORT_PIN 13

/* Definições do botão */
#define BUTTON_PORT PORTA
#define BUTTON_GPIO GPIOA
#define BUTTON_GPIO_PIN 9

volatile bool g_AdcConversionDoneFlag = false;
volatile uint32_t g_AdcConversionValue;
volatile uint32_t g_AdcInterruptCounter;


void ADC0_IRQHandler(void)
{
    g_AdcConversionDoneFlag = true;
    /* Read conversion result to clear the conversion completed flag. */
    g_AdcConversionValue = ADC_GetChConversionValue( ADC0 );
    g_AdcInterruptCounter++;
}

int main(void)
{
	PORT_Init( ADC_PORT );
	PORT_Init( BUTTON_PORT );

	PORT_SetMux( ADC_PORT, ADC_PORT_PIN, PORT_MUX_ALT0 );
	PORT_SetMux( BUTTON_PORT, BUTTON_GPIO_PIN, PORT_MUX_AS_GPIO );

	PORT_EnablePull( BUTTON_PORT, BUTTON_GPIO_PIN );
	GPIO_InitInputPin( BUTTON_GPIO, BUTTON_GPIO_PIN );

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

	Delay_Init();

    /* Resultados da conversão serão impressos em console serial. */
	printf("\r\nADC interrupcao - exemplo.\r\n");

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

	NVIC_EnableIRQ( ADC0_IRQn ); /* Habilita interrupção pelo NVIC. */

    printf( "Pressione no botão para retornar o valor lido do ADC...\r\n" );

    g_AdcInterruptCounter = 0U;
    for ( ; ; )
    {
        //getchar();
    	Delay_Waitms(500);
    	while ( GPIO_ReadPin( BUTTON_GPIO, BUTTON_GPIO_PIN ) == 1 );

        ADC_SetChConfig( ADC0, ADC_CHANNEL, true );

    	/* Espere pelo fim da conversão quando "ADC0_IRQHandler" irá
    	   setar adcConversionDoneFlag*/
    	while (!g_AdcConversionDoneFlag)
    	{

    	}

    	printf("Valor ADC: %d\r\n", g_AdcConversionValue);
    	printf("Contagem de interrupcoes: %d\r\n", g_AdcInterruptCounter);
    }
}
