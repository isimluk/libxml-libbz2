/*
 * Copyright 2014 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors:
 *     Šimon Lukašík
 *
 * Descriptions:
 *     Dumps bzip2ed XML to the standard output
 */

#include <stdlib.h>
#include <bzlib.h>

#define BUFF_SIZE 10

libbz2_open(const char *filename)
{
	FILE* f;
	BZFILE* b;
	int nBuf;
	char buf[BUFF_SIZE];
	int bzerror;
	int nWritten;

	f = fopen (filename, "r" );
	if ( !f ) {
		abort();
	}
	b = BZ2_bzReadOpen ( &bzerror, f, 0, 0, NULL, 0 );
	if ( bzerror != BZ_OK ) {
		BZ2_bzReadClose ( &bzerror, b );
		abort();
	}

	bzerror = BZ_OK;
	while ( bzerror == BZ_OK) {
		nBuf = BZ2_bzRead ( &bzerror, b, buf, BUFF_SIZE );
		buf[nBuf] = '\0';
		if ( bzerror == BZ_OK || bzerror == BZ_STREAM_END) {
			printf ("%s", buf);
		}
	}
	if ( bzerror != BZ_STREAM_END ) {
		BZ2_bzReadClose ( &bzerror, b );
		abort();
	} else {
		BZ2_bzReadClose ( &bzerror, b );
	}
}

int main()
{
	libbz2_open("test/simple.xml.bz2");
}

