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
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "checker.h"
#include "manufacturer.h"
#include "file.h"
#include "signals.h"

#define OUI "oui.txt"
#define OUI_PATH "/usr/local/share/maclookup/"

enum arguments {
	ARGS_U = 0X01, /* Update the database of manufacturers */
	ARGS_V = 0X02, /* Display version */
	ARGS_S = 0X04 /* Read input from stdin */
};

static void usage(void)
{
	printf(
		"Usage: maclookup [-s] [-huv MAC-ADDRESS]\n"
		"Display the manufacturer of a network interface\n"
		"  -h  --help       display this help message and exit\n"
		"  -u  --update     update the database of manufacturers\n"
		"  -v  --version    output version information and exit\n"
		"  -s  --stdin      read input from the stdin\n");
}

static void version(void)
{
	printf(
	"maclookup 1.0.0\n"
	"\n"
	"Copyright (C) 2020 Mathias Schmitt\n"
	"License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n"
	"This is free software, and you are welcome to change and redistribute it.\n"
	"This program comes with ABSOLUTELY NO WARRANTY.\n");
}

static char check_arguments(int argc, char *argv[], char ** mac_address)
{
	int opt;
	char args = 0;
	struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"update", no_argument, 0, 'u'},
		{"version", no_argument, 0, 'v'},
		{"stdin", no_argument, 0, 's'},
		{0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, "huvs", long_options, NULL)) != -1) {
		switch (opt) {
		case 'u':
			args |= ARGS_U;
			break;
		case 'v':
			if (optind < argc) {
				usage();
				return -EINVAL;
			}
			args |= ARGS_V;
			version();
			return -EINVAL;
		case 's':
			args |= ARGS_S;
			break;
		case 'h':
		case '?':
			usage();
			return -EINVAL;
		default:
			break;
		}
	}

	if (args & ARGS_V)
		return 0;

	if (optind + 1 == argc) {
		if (!mac_address_is_valid(argv[1])) {
			printf("Invalid mac address: %s\n", argv[1]);
			return -EINVAL;
		}
		*mac_address = strndup(argv[1], strlen(argv[1]));
	} else if (optind < argc) {
		printf("Invalid argument: %s\n", argv[1]);
		usage();
		return -EINVAL;
	} else if (optind == argc && !(args & ARGS_U) && !(args & ARGS_S)) {
		printf("Missing argument: mac address\n");
		usage();
		return -EINVAL;
	}

	return args;
}

static int read_input(char **mac_address)
{
	char buffer[MAC_LENGTH];
	ssize_t ret = 0;

	ret = read(STDIN_FILENO, buffer, sizeof(buffer));
	if (ret < 0) {
		printf("%s\n", strerror(errno));
		return -EINVAL;
	} else if (ret != MAC_LENGTH) {
		printf("Invalid input: %s\n", buffer);
		return -EINVAL;
	}

	if (!mac_address_is_valid(buffer)) {
		printf("invalid mac address: %s\n", buffer);
		return -EINVAL;
	}
	*mac_address = strndup(buffer, strlen(buffer));

	return 0;
}

int main(int argc, char *argv[])
{
	char *mac_address = NULL;
	char args = 0;
	int res = 0;

	args = check_arguments(argc, argv, &mac_address);
	if (args < 0) {
		res = -EINVAL;
		goto end;
	}

	if (args & ARGS_V)
		goto end;

	/* Restore the backup if exist during update */
	/* That way we avoid to use an incomplete file */
	signal_handler();

	if (args & ARGS_S) {
		res = read_input(&mac_address);
		if (res < 0) {
			usage();
			goto end;
		}
	}

	if (args & ARGS_U) {
		update(OUI_PATH, OUI);
		goto end;
	} else if (!mac_address) {
		res = -EINVAL;
		goto end;
	}

	find_manufacturer(mac_address, OUI_PATH OUI);

end:
	free(mac_address);
	mac_address = NULL;

	return res;
}
