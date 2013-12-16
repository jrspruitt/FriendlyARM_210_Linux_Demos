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
 * Command line utility for controlling buzzer on FriendlyARM Mini210s boards.
 * Interfaces with linux-3.0.8/drivers/char/mini210_pwm.c
 */

#define DEVICE	"/dev/pwm"

int main( int argc, char *argv[] )
{
	int fd;
	int cmd;
	long freq;

	// Check cmdline args count or for help.
	if ( argc < 2 || strcmp( argv[1], "-h" ) == 0 || \
					strcmp( argv[1], "-help" ) == 0 )
	{
		printf( "Usage: buzzer-ctl <on [1|0]> [freq num]\n" );
		exit( 1 );
	}


	// Convert cmdline args to ints.
	cmd = atoi(argv[1]);

	// Check if freq arg supplied.
	if ( argc > 2 )
	{
		freq = atol(argv[2]);
	}
	else
	{
		freq = 1000;
	}

	// Check freq is greater than 0.
	if ( freq < 0 )
	{
		printf( "Freq greater than 0 only." );
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

	// Open buzzer device and set commands.
	fd = open( DEVICE, O_RDONLY );

	// Check if opened.
	if ( fd == -1 )
	{
		printf( "Error: Could not open %s\n", DEVICE );
		exit( 1 );
	}

	// Set buzzer on or off with frequency.
	ioctl( fd, cmd, freq);

	// Close file handle and exit.
	close( fd );
	exit( 0 );
}
