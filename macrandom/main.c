/*
    macrandom Generate a random mac address
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

#define OUI "oui.txt"
#define OUI_PATH "/usr/local/share/maclookup/"
#define BUF_SIZE 512

static void usage(void)
{
	printf(
		"Usage: maclookup [OPTIONS] [MAC-ADDRESS]\n"
		"Display the manufacturer of a network interface\n"
		"  -h  --help       display this help message and exit\n"
		"  -v  --version    output version information and exit\n");
}

static void version(void)
{
	printf(
	"macrandom 1.0.0\n"
	"\n"
	"Copyright (C) 2020 Mathias Schmitt\n"
	"License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n"
	"This is free software, and you are welcome to change and redistribute it.\n"
	"This program comes with ABSOLUTELY NO WARRANTY.\n");
}

static void print_error(void)
{
	printf("Error:\n"
		"  Failed to open/read database file: \"%s\".\n"
		"  Try 'maclookup -u' to download the latest version from "
		"the iee.\n",
		strerror(errno));
}

int main(void)
{
	int ret;
	FILE *oui;
	char oui[BUF_SIZE];

	oui = fopen(filename, "r");
	if (!oui) {
		print_error();
		return errno;
	}

	return 0;
}