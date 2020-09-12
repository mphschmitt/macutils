/*
    libreadoui Read the iee's oui.txt file
    Copyright (C) 2020  Mathias Schmitt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void libreadoui_print_error(void)
{
	printf("Error:\n"
		"  Failed to open/read database file: \"%s\".\n"
		"  Try 'maclookup -u' to download the latest version from "
		"the iee.\n",
		strerror(errno));
}

/**
 * @brief Skip the first lines from the file.
 *
 * These lines are a header that describes the format of the file.
 * They are of no use to maclookup.
 *
 * @param oui The file to read
 */
int libreadoui_skip_header(FILE *oui)
{
	ssize_t read;
	size_t buff_size;
	char *line = NULL;

	while ((read = getline(&line, &buff_size, oui))) {
		if (read == -1) {
			if (errno) {
				libreadoui_print_error();
				return -errno;
			}
			break;
		}

		if (line && line[0] == '\r' && line[1] == '\n')
			break;

		if (line) {
			free(line);
			line = NULL;
		}
	}

	if (line) {
		free(line);
		line = NULL;
	}

	return 0;
}

