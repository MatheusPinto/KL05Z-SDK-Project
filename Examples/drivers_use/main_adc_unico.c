#include <Drivers/port/port.h>
#include <Drivers/gpio/gpio.h>
#include <Drivers/adc/adc.h>
#include <common.h>
#include <Libraries/delay/delay.h>
#include <stdio.h>

/* Canal 13
 * Modo sa�da �nica: PTB13 (ADC0_SE13)*/
#define ADC_CHANNEL 13U
#define ADC_PORT PORTB
#define ADC_PORT_PIN 13

/* Defini��es do bot�o */
#define BUTTON_PORT PORTA
#define BUTTON_GPIO GPIOA
#define BUTTON_GPIO_PIN 9

int main(void)
{
	PORT_Init( ADC_PORT );
	PORT_Init( BUTTON_PORT );

	PORT_SetMux( ADC_PORT, ADC_PORT_PIN, PORT_MUX_ALT0 );
	PORT_SetMux( BUTTON_PORT, BUTTON_GPIO_PIN, PORT_MUX_AS_GPIO );

	PORT_EnablePull( BUTTON_PORT, BUTTON_GPIO_PIN );
	GPIO_InitInputPin( BUTTON_GPIO, BUTTON_GPIO_PIN );

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
	ADC_SetHardwareAverage( ADC0, ADC_HARDWARE_AVG_COUNT_8 );

	Delay_Init();

    /* Resultados da convers�o ser�o impressos em console serial. */
	printf("\r\nADC unico - exemplo.\r\n");

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

    printf( "Pressione no bot�o para retornar o valor lido do ADC...\r\n" );


    for ( ; ; )
    {
        //getchar();
    	Delay_Waitms(500);
    	while ( GPIO_ReadPin( BUTTON_GPIO, BUTTON_GPIO_PIN ) == 1 );

        /*
         * Quando estiver no modo de disparo por software, cada convers�o deve ser feita chamando
         * a fun��o "ADC_SetChConfig()", que ir� enviar o comando de convers�o e
         * executar a convers�o. Para uma convers�o em outro canal, basta mudar o campo "ADC_CHANNEL"
         * e chamar novamente o "ADC_SetChConfig()".
         */
        ADC_SetChConfig( ADC0, ADC_CHANNEL, false );

        // Espere pelo fim da convers�o e depois pegue o valor.
        while ( !ADC_IsConversionDone( ADC0 ) )
        {
        }

        // O valor pego aqui ser� de 12 bits: 0 � 4095
        printf( "Valor ADC: %d\r\n", ADC_GetChConversionValue( ADC0 ) );
    }
}
