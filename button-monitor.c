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
#include <signal.h>

/*
 * Command line utility for reading on board button presses
 * on FriendlyARM Mini210s boards.
 * Interfaces with linux-3.0.8/drivers/char/mini210_buttons.c
 */

#define DEVICE "/dev/buttons"
#define KEY_MASK_3 	0x00010000
#define KEY_MASK_2	0x00100000
#define KEY_MASK_1	0x01000000
#define KEY_MASK_0	0x10000000

volatile int execute;

// Used to trap ctrl-c interrupt
void trap( int signal )
{
	execute = 0;
}

int main( int argc, char *argv[] )
{

	int fd;
	char ret[8];
	int button;
	ssize_t size;

	// Check cmdline args count or for help.
	if ( argc != 1)
	{
		printf( "Usage: button-monitor\n" );
		exit( 1 );
	}

	// Open buttons device.
	fd = open( DEVICE, O_RDONLY );

	// Check if opened.
	if ( fd == -1 )
	{
		printf( "Error: Could not open %s\n", DEVICE );
		exit( 1 );
	}

	// Set up signal trap.
	signal(SIGINT, &trap);
	execute = 1;

	while ( execute )
	{
		// Read button device.
		size = read( fd, ret, sizeof( ret ) );
		button = strtol( ret, NULL, 16 );

		// Check against masks.
		if ( button & KEY_MASK_0)
		{
			printf( "0\n" );
		}
		else if ( button & KEY_MASK_1)
		{
			printf( "1\n" );
		}
		else if ( button & KEY_MASK_2)
		{
			printf( "2\n" );
		}
		else if ( button & KEY_MASK_3)
		{
			printf( "3\n" );
		}

	}

	// Close file handle and exit.
	close( fd );
	exit( 0 );
}
