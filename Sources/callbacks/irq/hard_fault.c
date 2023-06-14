void HardFault_Handler(void)
{
	for ( int i = 0; ; ++i )
	{
		__asm ( "nop" );
		++i;
	}
}
