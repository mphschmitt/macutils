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

#define MAC_LENGTH 17

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static bool is_separator(char c)
{
	static char previous_separator = '\0';
	char separators[] = {':', '-', ' '};
	bool ret = false;
	int i;

	for (i = 0; i < 3; ++i) {
		if (c == separators[i]) {
			if (previous_separator != '\0' && c != previous_separator) {
				printf("Inconsistent use of '%c' and '%c' as a separator\n", c,
					previous_separator);
				break;
			}
			previous_separator = c;
			ret = true;
			break;
		}
	}

	return ret;
}

bool chars_are_equivalent(char c1, char c2)
{
	int delta;

	delta = 'a' - 'A';

	if (c1 == c2)
		return true;

	if (isdigit(c1) && isdigit(c2))
		return c1 == c2;

	if (isalpha(c1) && isalpha(c2))
		if (abs(c1 - c2) == delta)
			return true;

	return false;
}

bool mac_address_is_valid(char * mac)
{
	size_t length, i;
	int counter = 0;
	bool res = true;

	length = strlen(mac);
	if (length != MAC_LENGTH)
		return false;

	for (i = 0; i < length; ++i) {
		if (counter == 2) {
			counter = 0;
			if (!is_separator(mac[i])) {
				res = false;
				break;
			}
		} else if (isxdigit(mac[i])) {
			counter += 1;
		} else {
			res = false;
			break;
		}
	}

	return res;
}

