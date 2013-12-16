/*
 * Mini210s Board Demo Utils
 * (c) 2013 Jason Pruitt (jrspruitt@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

/*
 * Command line utility for reading on board potentiometer
 * on FriendlyARM Mini210s boards.
 * Interfaces with linux-3.0.8/drivers/char/mini210_adc.c
 */

#define DEVICE "/dev/adc"

int main( int argc, char *argv[] )
{

	int fd;
	// Size as defined in mini210_adc.c
	char ret[20]; 
	ssize_t size;

	// Check cmdline args count or for help.
	if ( argc != 1)
	{
		printf( "Usage: adc-read\n" );
		exit( 1 );
	}

	// Open leds device.
	fd = open( DEVICE, O_RDONLY );

	// Check if opened.
	if ( fd == -1 )
	{
		printf( "Error: Could not open %s\n", DEVICE );
		exit( 1 );
	}

	// Read ADC from device.
	size = read( fd, ret, sizeof( ret ) );

	printf( "%.*s", size, ret );

	// Close file handle and exit.
	close( fd );

	exit( 0 );
}
