/*
    maclookup Find the manufacturer of a network interface
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
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

#include "checker.h"
#include "manufacturer.h"

#define MANUFACTURER_DIGITS 8

static void print_error(void)
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
static int skip_header(FILE *oui)
{
	ssize_t read;
	size_t buff_size;
	char *line = NULL;

	while ((read = getline(&line, &buff_size, oui))) {
		if (read == -1) {
			if (errno) {
				print_error();
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

static int compare_mac_addresses(char *mac1, char *mac2)
{
	int i = 0;

	for (i = 0; i < MANUFACTURER_DIGITS; ++i) {
		if (!isxdigit(mac1[i]))
			continue;

		if (!chars_are_equivalent(mac1[i], mac2[i]))
			break;
	}

	return i == MANUFACTURER_DIGITS;
}

static int find_mac_address(FILE *oui, char *mac)
{
	ssize_t read;
	size_t buff_size;
	char *line = NULL;
	bool is_first_line = true;
	bool last_entry = false;

	while ((read = getline(&line, &buff_size, oui))) {
		if (read == -1) {
			if (errno) {
				print_error();
				return -errno;
			}
			return 0;
		}

		if (line && line[0] == '\r' && line[1] == '\n') {
			if (last_entry)
				break;
			is_first_line = true;
			continue;
		}

		if (is_first_line) {
			is_first_line = false;

			if (compare_mac_addresses(line, mac))
				last_entry = true;
		}

		/* If the manufacturer has been found, display its informations then
		 * break */
		if (last_entry)
			printf("%s", line);

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

int find_manufacturer(char * mac, char const * filename)
{
	FILE *oui;
	int ret;

	oui = fopen(filename, "r");
	if (!oui) {
		print_error();
		return -errno;
	}

	ret = skip_header(oui);
	if (ret)
		goto end;

	ret = find_mac_address(oui, mac);
	if (ret)
		goto end;

end:
	ret = fclose(oui);
	if (ret) {
		printf("%s\n", strerror(errno));
		return -errno;
	}

	return 0;
}

