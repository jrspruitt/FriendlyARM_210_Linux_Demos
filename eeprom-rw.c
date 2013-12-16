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
 * Command line utility for reading and writing to the EEPROM
 * on FriendlyARM Mini210s boards.
 */

#define EEPROM "/sys/bus/i2c/devices/0-0050/eeprom"

int main( int argc, char *argv[] )
{
	int fd;
	char buf[256];
	int length;
	int position; 
	ssize_t size;

	// Check cmdline args count or for help.
	if ( argc < 3 || strcmp( argv[1], "-h" ) == 0 || \
					strcmp( argv[1], "-help" ) == 0 )
	{
		printf( "Usage: eeprom-ctl <read|write> <position> <read:length|write:\"text[256]\"> \n" );
		exit( 1 );
	}

	/* TODO: Check input better. */	

	position = atoi(argv[2]);

	// Open eeprom.
	fd = open( EEPROM, O_RDWR );

	// Check if opened.
	if ( fd == -1 )
	{
		printf( "Error: Could not open %s\n", EEPROM );
		exit( 1 );
	}

	// Seek to position.
	lseek( fd, position, SEEK_SET );


	// If write requested, write data, else read.
	if ( strcmp( argv[1], "write" ) == 0 )
	{
		write( fd, argv[3], strlen( argv[3] ) );
	}
	else
	{
		length = atoi( argv[3] );
		size = read( fd, buf, length );
		printf( "%.*s\n", size, buf );
	}

	// Close file handle and exit.
	close( fd );
	exit( 0 );
}
