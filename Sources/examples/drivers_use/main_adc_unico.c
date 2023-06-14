#include <mcu/drivers/port/port.h>
#include <mcu/drivers/gpio/gpio.h>
#include <mcu/drivers/adc/adc.h>
#include <mcu/common.h>
#include <libraries/delay/delay.h>
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

int main(void)
{
	PORT_Init( ADC_PORT );
	PORT_Init( BUTTON_PORT );

	PORT_SetMux( ADC_PORT, ADC_PORT_PIN, PORT_MUX_ALT0 );

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
	printf("\r\nADC unico - exemplo.\r\n");

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

    printf( "Pressione no botão para retornar o valor lido do ADC...\r\n" );


    for ( ; ; )
    {
        //getchar();
    	Delay_Waitms(500);
    	while ( GPIO_ReadPin( BUTTON_GPIO, BUTTON_GPIO_PIN ) == 1 );

        /*
         * Quando estiver no modo de disparo por software, cada conversão deve ser feita chamando
         * a função "ADC_SetChConfig()", que irá enviar o comando de conversão e
         * executar a conversão. Para uma conversão em outro canal, basta mudar o campo "ADC_CHANNEL"
         * e chamar novamente o "ADC_SetChConfig()".
         */
        ADC_SetChConfig( ADC0, ADC_CHANNEL, false );

        // Espere pelo fim da conversão e depois pegue o valor.
        while ( !ADC_IsConversionDone( ADC0 ) )
        {
        }

        // O valor pego aqui será de 12 bits: 0 à 4095
        printf( "Valor ADC: %d\r\n", ADC_GetChConversionValue( ADC0 ) );
    }
}
