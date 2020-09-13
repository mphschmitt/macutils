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
#include "libreadoui.h"

#define MANUFACTURER_DIGITS 8

extern char * libreadoui_line;
extern size_t buff_size;

static int compare_mac_addresses(char *mac1, char *mac2)
{
	int i;

	if (!mac1 || !mac2)
		return -EINVAL;

	for (i = 0; i < MANUFACTURER_DIGITS; ++i) {
		if (!isxdigit(mac2[i]))
			continue;

		if (!chars_are_equivalent(mac1[i], mac2[i]))
			break;
	}

	return i == MANUFACTURER_DIGITS;
}

static int find_mac_address(FILE *oui, char *mac)
{
	int ret;

	ret = 0;

	/* Check first manufacturer, after the skipped headers */
	if (compare_mac_addresses(libreadoui_line, mac)) {
		libreadoui_print_manufacturer(oui);
		return 0;
	}

	/* Check all the other manufacturers */
	while (true) {
		ret = libreadoui_get_next_manufacturer(oui);
		if (ret <= 0)
			break;

		if (compare_mac_addresses(libreadoui_line, mac)) {
			libreadoui_print_manufacturer(oui);
			break;
		}
	}

	return ret;
}

int find_manufacturer(char * mac, char const * filename)
{
	FILE *oui;
	int ret;

	libreadoui_line = NULL;

	oui = fopen(filename, "r");
	if (!oui) {
		libreadoui_print_error();
		return -errno;
	}

	ret = libreadoui_skip_header(oui);
	if (ret)
		goto end;

	ret = find_mac_address(oui, mac);
	if (ret)
		goto end;

end:
	libreadoui_line_free();

	ret = fclose(oui);
	if (ret) {
		printf("%s\n", strerror(errno));
		return -errno;
	}

	return 0;
}

