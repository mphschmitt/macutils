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
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "libreadoui.h"

#define OUI "oui.txt"
#define OUI_PATH "/usr/local/share/maclookup/"
#define BUF_SIZE 512
#define MANUFACTURER_DIGITS 8

extern char * libreadoui_line;

enum arguments {
	ARGS_V = 0X01 /* Display version */
};

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

static char check_arguments(int argc, char *argv[])
{
	int opt;
	char args = 0;
	struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"version", no_argument, 0, 'v'},
		{0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, "hv", long_options, NULL)) != -1) {
		switch (opt) {
		case 'v':
			if (optind < argc) {
				usage();
				return -EINVAL;
			}
			args |= ARGS_V;
			version();
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

	if (optind != argc) {
		usage();
		return -EINVAL;
	}

	return args;
}

static int get_random_manufacturer(int nb_of_manufacturers)
{
	time_t now;

	now = time(NULL);
	if (now < 0) {
		printf("Error: %s\n", strerror(errno));
		return -errno;
	}

	srand((unsigned int)now);

	return rand() % nb_of_manufacturers;
}

static int get_number_of_manufacturers(FILE * oui)
{
	int ret;
	int nb;

	nb = 0;

	ret = libreadoui_skip_header(oui);
	if (ret)
		return ret;

	while (true) {
		nb += 1;
		ret = libreadoui_get_next_manufacturer(oui);
		if (ret <= 0)
			break;
	}

	return nb;
}

static int goto_manufacturer(FILE * oui, int manufacturer_pos,
		int manufacturer_choice)
{
	int ret;
	int nb;

	nb = 0;
	ret = 0;

	ret = libreadoui_skip_header(oui);
	if (ret < 0)
		return ret;

	while (nb < manufacturer_pos && nb != manufacturer_choice) {
		nb += 1;
		ret = libreadoui_get_next_manufacturer(oui);
		if (ret <= 0)
			break;
	}

	return 0;
}

static int extact_prefix(char mac[BUF_SIZE])
{
	int i;
	
	if (!mac)
		return -EINVAL;

	for (i = 0; i < MANUFACTURER_DIGITS; ++i)
		mac[i] = libreadoui_line[i];

	return 0;
}

static int fill_random(char mac[BUF_SIZE])
{
	int i;
	int pos;
	int nb;
	time_t now;
	char hexa[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',
		'B', 'C', 'D', 'E', 'F'};

	if (!mac)
		return -EINVAL;

	pos = MANUFACTURER_DIGITS;
	now = time(NULL);
	if (now < 0) {
		printf("Error: %s\n", strerror(errno));
		return -errno;
	}

	srand((unsigned int)now);

	for (i = 0; i < 3; ++i) {
		mac[pos] = '-';
		pos += 1;

		nb = rand() % (int)sizeof(hexa);
		mac[pos] = hexa[nb];
		pos += 1;

		nb = rand() % (int)sizeof(hexa);
		mac[pos] = hexa[nb];
		pos += 1;
	}

	return 0;
}

int main(int argc, char * argv[])
{
	char args = 0;
	int ret = 0;
	int nb_of_manufacturers;
	int manufacturer_choice;
	char mac[BUF_SIZE] = {0};
	FILE * oui;

	args = check_arguments(argc, argv);
	if (args < 0)
		return -EINVAL;

	if (args & ARGS_V)
		goto end;

	oui = fopen(OUI_PATH OUI, "r");
	if (!oui) {
		libreadoui_print_error();
		return errno;
	}

	nb_of_manufacturers = get_number_of_manufacturers(oui);
	rewind(oui);

	manufacturer_choice = get_random_manufacturer(nb_of_manufacturers);
	ret = goto_manufacturer(oui, nb_of_manufacturers, manufacturer_choice);
	if (ret)
		goto end;
	ret = extact_prefix(mac);
	if (ret)
		goto end;
	ret = fill_random(mac);
	if (ret)
		goto end;
	printf("-------------------------------------------------\n\n");
	printf("Randomly generated mac address: %s\n\n", mac);
	printf("-------------------------------------------------\n\n");
	printf("Manufacturer informations:\n\n");
	libreadoui_print_manufacturer(oui);

end:
	libreadoui_line_free();

	if (oui) {
		ret = fclose(oui);
		if (ret) {
			printf("%s\n", strerror(errno));
			return -errno;
		}
	}

	return ret;
}
