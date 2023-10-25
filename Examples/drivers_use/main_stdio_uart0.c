#include <Drivers/port/port.h>
#include <Drivers/uart/uart0.h>
#include <stdio.h>

/*
 * If stdio use debugger subsytem, you must pass "rdimon.specs" parameter to linker and exclude "nosys.specs" if included.
 * Otherwise, if you want to use specific mcu peripheral as stdio (e.g.: UART, USB, etc.), maintain "nosys.specs" and
 * do not include "rdimon.specs". Moreover, you must implement "_write" and "_read", in the last case.
 *
 * Note that for J-Link debbuger, nano lib and gcc used, the scanf doesnt work in debbug mode.
 *
 * */

#define MAX_BUFFER_SIZE 40

int main(void) {

	PORT_Init( PORTB );
	PORT_SetMux( PORTB, 1, PORT_MUX_ALT2 );
	PORT_SetMux( PORTB, 2, PORT_MUX_ALT2 );

	UART0_SetClkSrc( UART0_CLOCK_FLL );
	UART0_Init( 115200, UART0_TX_RX_ENABLE, UART0_NO_PARITY, UART0_ONE_STOP_BIT );

	char buffer[MAX_BUFFER_SIZE];
	int i = 0;
	int x;

	printf("Insert a integer number: ");
	fflush( stdout );
	scanf("%d", &x);

	for ( ; ; )
	{
		fgets (buffer, MAX_BUFFER_SIZE, stdin);
		for ( i = 0; buffer[i] != '\n'; i++)
		{
			printf("%c", buffer[i]);
		}
		printf(" %d\n", x);
	}
}
