#include <Drivers/port/port.h>
#include <Drivers/uart/uart0.h>

#define MAX_BUFFER_SIZE 40

int main(void) {

	PORT_Init( PORTB );
	PORT_SetMux( PORTB, 1, PORT_MUX_ALT2 );
	PORT_SetMux( PORTB, 2, PORT_MUX_ALT2 );

	UART0_SetClkSrc( UART0_CLOCK_FLL );
	UART0_Init( 115200, UART0_TX_RX_ENABLE, UART0_NO_PARITY, UART0_ONE_STOP_BIT );

	int i = 0;
	char buffer[MAX_BUFFER_SIZE];

	for ( ; ; )
	{
		/*Lê bytes um à um até chegar em fim de string ('\n')*/
		while ( !UART0_IsRxAvailable( ) );
		buffer[i] = UART0_Read( );

		if ( buffer[i] == '\n' )
		{
			/*Reenvia a string que foi recebida (echo)*/
			for ( i = 0; buffer[i] != '\n'; ++i )
			{
				UART0_Write( buffer[i] );
				while ( !UART0_IsTxAvailable() );
			}
			UART0_Write( buffer[i] );
			while ( !UART0_IsTxAvailable() );
			i = 0; /*Zera contador para leitura da próxima string no buffer*/
		}
		else
		{
			++i;
		}
	}

}
