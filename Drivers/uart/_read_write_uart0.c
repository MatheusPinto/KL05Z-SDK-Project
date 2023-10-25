#include "uart0.h"

#define UART0_WRITE_STDOUT
#define UART0_READ_STDIN

#ifdef UART0_WRITE_STDOUT
int _write(int fd, const void *buf, size_t count) {
	size_t charCnt;

	(void) fd; /* Parameter is not used, suppress unused argument warning */

	for ( charCnt = 0; charCnt < count; ++charCnt )
	{
		/* Wait until UART is ready for saving a next character into the transmit buffer */
		while ( !UART0_IsTxAvailable() );

		if ( *(uint8_t*) buf == '\n' )
		{
			/* Send '\r'(0x0D) before each '\n'(0x0A). */
			/* Save a character into the transmit buffer of the UART0 device */
			UART0_Write(0x0DU);
			/* Wait until UART is ready for saving a next character into the transmit buffer */
			while (!UART0_IsTxAvailable());
		}
		/* Save a character into the transmit buffer of the UART0 device */
		UART0_Write( (unsigned char) *(uint8_t*) buf );
		(uint8_t*) buf++; /* Increase buffer pointer */
	}

	return charCnt;
}
#endif

#ifdef UART0_READ_STDIN
int _read(int fd, const void *buf, size_t count) {
	size_t charCnt = 0;
	uint8_t temp;
	(void) fd; /* Parameter is not used, suppress unused argument warning */

	UART0_CleanRxBuffer();

	//for ( charCnt = 0; charCnt < count; --charCnt )
	//while ( charCnt < count )
	while ( 1 )
	{
/*		if ( UART0_IsRxAvailable() )
		{
			 Clear error flags
			UART0_CleanRxBuffer();
			if (CharCnt != 0x00)
			{  No, at least one char received?
				break;  Yes, return received char(s)
			}
			else
			{  Wait until a char is received
				while ( !UART0_IsRxAvailable() );
			}
		}*/


		while ( !UART0_IsRxAvailable() );
		charCnt++; /* Increase char counter */

		/* Save character received by UARTx device into the receive buffer */
		temp =  UART0_Read();
		*(uint8_t*) buf = temp;

		/* Stop reading if CR (Ox0D) character is received */
		if (*(uint8_t*) buf == '\r') /* New line character (CR) received ? */
		{
			*(uint8_t*) buf = '\n'; /* Yes, convert LF to '\n' char. */
			break; /* Stop loop and return received char(s) */
		}
		(uint8_t*) buf++; /* Increase buffer pointer */
	}

	return charCnt;
}
#endif
