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
#include <stdbool.h>

#include "libreadoui.h"

char * libreadoui_line = NULL;
size_t libreadoui_buff_size = 0;

void libreadoui_line_free(void)
{
	if (libreadoui_line)
		free(libreadoui_line);
	libreadoui_line = NULL;
}

void libreadoui_print_error(void)
{
	printf("Error:\n"
		"  Failed to open/read database file: \"%s\".\n"
		"  Try 'maclookup -u' to download the latest version from "
		"the iee.\n",
		strerror(errno));
}

int libreadoui_skip_header(FILE *ptr)
{
	ssize_t read;

	if (!ptr)
		return -EINVAL;

	while (1) {
		read = getline(&libreadoui_line, &libreadoui_buff_size, ptr);
		if (read == -1) {
			if (errno) {
				libreadoui_print_error();
				return -errno;
			}
			break;
		}

		if (libreadoui_line && libreadoui_line[0] == '\r' &&
				libreadoui_line[1] == '\n')
			break;

	}

	/* Read last empty line to position file pointer on first line of
	 * next manufacturer entry */
	read = getline(&libreadoui_line, &libreadoui_buff_size, ptr);
	if (read == -1) {
		if (errno) {
			libreadoui_print_error();
			return -errno;
		}
	}

	return 0;
}

int libreadoui_print_manufacturer(FILE *ptr)
{
	ssize_t read;

	if (!ptr)
		return -EINVAL;

	printf("%s", libreadoui_line);
	while (true) {
		read = getline(&libreadoui_line, &libreadoui_buff_size, ptr);
		if (read == -1) {
			if (errno) {
				libreadoui_print_error();
				return -errno;
			}
			return 0;
		}

		if (libreadoui_line && libreadoui_line[0] == '\r' &&
				libreadoui_line[1] == '\n') {
			break;
		}

		printf("%s", libreadoui_line);

	}

	return (int)read;
}

int libreadoui_get_next_manufacturer(FILE *ptr)
{
	bool is_first_line;
	ssize_t read;

	errno = 0;
	is_first_line = false;

	if (!ptr)
		return -EINVAL;

	while (1) {
		read = getline(&libreadoui_line, &libreadoui_buff_size, ptr);
		if (read == -1) {
			if (errno) {
				libreadoui_print_error();
				return -errno;
			}
			return 0;
		}

		if (is_first_line)
			break;

		if (libreadoui_line && libreadoui_line[0] == '\r' &&
				libreadoui_line[1] == '\n') {
			is_first_line = true;
		}
	}

	return (int)read;
}

