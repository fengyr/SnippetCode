#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] )
{
	int i;
	
	for( i = 0; i < argc; i++ )
	{
		fprintf( stderr, "argv[%d] = %s\r\n", i, argv[i] );
	}
	for( i = 0; ; i++)
	{
		fprintf( stderr, "I am running %d...\r\n", i );
		sleep( 1 );
	}

	return 0;
}
