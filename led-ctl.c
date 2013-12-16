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
 * Command line utility for controlling LEDs on FriendlyARM Mini210s boards.
 * Interfaces with linux-3.0.8/drivers/char/mini210_leds.c
 */

#define DEVICE "/dev/leds"

int main( int argc, char *argv[] )
{
	int fd;
	int cmd;
	int led;
	int all;
	int i;

	// Check cmdline args count or for help.
	if ( argc != 3 || strcmp( argv[1], "-h" ) == 0 || \
					strcmp( argv[1], "-help" ) == 0 )
	{
		printf( "Usage: led-ctl <led [0-3|all]> <on [1|0]>\n" );
		exit( 1 );
	}

	// If led arg is all, set all and led to arbitrary int.
	// Else convert led arg to int
	if ( strcmp( argv[1], "all" ) == 0 )
	{
		led = 0;
		all = 1;
	}
	else
	{
		led = atoi(argv[1]);
	}

	// Convert cmdline arg cmd (1|0) to int.
	cmd = atoi(argv[2]);

	if ( led > 4 || led < 0 )
	{
		printf( "LED 0-3 only." );
		exit( 1 );
	}	

	// Constrain command numbers to 1 or 0.
	if ( cmd > 1 )
	{
		cmd = 1;
	}
	else if ( cmd < 0 )
	{
		cmd = 0;
	}

	// Open leds device.
	fd = open( DEVICE, O_RDONLY );

	// Check if opened.
	if ( fd == -1 )
	{
		printf( "Error: Could not open %s\n", DEVICE );
		exit( 1 );
	}

	if ( all )
	{
		// Loop through all leds setting to cmd.
		for ( i = 0; i < 4; i++ )
		{
			// Set led on or off.
			ioctl( fd, cmd, i );
		}
	}
	else
	{
		// Set led on or off.
		ioctl( fd, cmd, led);
	}

	// Close file handle and exit.
	close( fd );
	exit( 0 );
}
