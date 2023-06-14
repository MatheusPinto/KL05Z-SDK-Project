#include <mcu/drivers/port/port.h>
#include <mcu/drivers/uart/uart.h>

#define MAX_BUFFER_SIZE 40

int main(void) {

	PORT_Init( PORTB );
	PORT_SetMux( PORTB, 1, PORT_MUX_ALT2 );
	PORT_SetMux( PORTB, 2, PORT_MUX_ALT2 );

	UART_SetClkSrc( UART0, UART_CLOCK_FLL );
	UART_Init( UART0, 115200, UART_TX_RX_ENABLE, UART_NO_PARITY, UART_ONE_STOP_BIT );

	int i = 0;
	char buffer[MAX_BUFFER_SIZE];
	for (;;)
	{
		/*Lê bytes um à um até chegar em fim de string ('\n')*/
		while ( !UART_IsRxAvailable( UART0 ) );
		buffer[i] = UART_Read( UART0 );

		if ( buffer[i] == '\n' )
		{
			/*Reenvia a string que foi recebida (echo)*/
			for ( i = 0; buffer[i] != '\n'; ++i )
			{
				UART_Write( UART0, buffer[i] );
				while ( !UART_IsTxAvailable( UART0 ) );
			}
			UART_Write( UART0, buffer[i] );
			while ( !UART_IsTxAvailable( UART0 ) );
			i = 0; /*Zera contador para leitura da próxima string no buffer*/
		}
		else
		{
			++i;
		}
	}

}
