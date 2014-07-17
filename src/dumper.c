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
#include <libxml/xmlreader.h>
#include <libxml/tree.h>
#include <bzlib.h>

struct bz2_file {
	BZFILE *file;
};

struct bz2_file *bz2_open(const char *filename)
{
	struct bz2_file *b = NULL;
	FILE* f;
	int bzerror;

	f = fopen (filename, "r" );
	if (f) {
		b = malloc(sizeof(struct bz2_file));
		b->file = BZ2_bzReadOpen(&bzerror, f, 0, 0, NULL, 0);
		if (bzerror != BZ_OK) {
			BZ2_bzReadClose(&bzerror, b->file);
			free(b);
			b = NULL;
		}
	}
	return b;
}

//xmlInputReadCallback
int bz2_read(void *bzfile, char *buffer, int len)
{
	int bzerror;
	int size = BZ2_bzRead(&bzerror, ((struct bz2_file *)bzfile)->file, buffer, len);
	if (bzerror == BZ_OK || bzerror == BZ_STREAM_END)
		return size;
	else
		return -1;
}

// xmlInputCloseCallback
int bz2_close(void *bzfile)
{
	int bzerror;
	BZ2_bzReadClose(&bzerror, ((struct bz2_file *)bzfile)->file);
	return bzerror == BZ_OK ? 0 : -1;
}

int main()
{
	xmlInitParser();
	struct bz2_file *bzfile = bz2_open("test/simple.xml.bz2");
	if (bzfile) {
		xmlDocPtr doc = xmlReadIO(bz2_read, bz2_close, bzfile, "url", NULL, XML_PARSE_PEDANTIC);
		xmlNodePtr root = xmlDocGetRootElement(doc);
		printf("%s\n", xmlNodeGetContent(root));
	}
	xmlCleanupParser();
}

