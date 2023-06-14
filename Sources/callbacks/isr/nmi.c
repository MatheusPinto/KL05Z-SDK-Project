/*
 * nmi.c
 *
 *  Created on: 13/06/2023
 *      Author: mathe
 */


void NMI_Handler(void)
{
	for ( int i = 0; ; ++i )
	{
		__asm ( "nop" );
		++i;
	}
}


