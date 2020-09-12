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

#include <curl/curl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "signals.h"

#define HOST "http://standards-oui.ieee.org/oui/oui.txt"
#define BUF_SIZE 512
#define BUF_SIZE_BIG 2048

static void curl_error(char const *str_error)
{
	printf("Error:\n"
		"  %s.\n"
		"  Aborting update...\n", str_error);
}

/*
 * See https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
 * It was possible to pass NULL to CURLOPT_WRITEFUNCTION and pass a FILE* to
 * CURLOPT_WRITEDATA but whatever...
 *
 * @return As requested by libcurl, the number of bytes taken care of is
 * returned
 */
static size_t write_to_file(char *ptr, size_t size, size_t nmemb,
	void *userdata)
{
	int fd;
	ssize_t handled_bytes;

	fd = *(int *)userdata;
	handled_bytes = write(fd, ptr, nmemb);
	if (handled_bytes < 0) {
		printf("CURL: %s\n", strerror(errno));
		return nmemb + size; /* different from nmemb, so it is an error */
	}

	return (size_t)handled_bytes;
}

static int curl_set_options(CURL *curl, int fd_oui)
{
	int res;

	res = 0;

	res = curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
	if (res) {
		curl_error(curl_easy_strerror(res));
		goto out;
	}

	res = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
	if (res) {
		curl_error(curl_easy_strerror(res));
		goto out;
	}

	res = curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
	if (res) {
		curl_error(curl_easy_strerror(res));
		goto out;
	}

	res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_to_file);
	if (res) {
		curl_error(curl_easy_strerror(res));
		goto out;
	}

	res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &fd_oui);
	if (res) {
		curl_error(curl_easy_strerror(res));
		goto out;
	}

	res = curl_easy_setopt(curl, CURLOPT_URL, HOST);
	if (res) {
		curl_error(curl_easy_strerror(res));
		goto out;
	}

out:
	return res;
}

int restore_backup(char const * file, char const * backup)
{
	/* Restore file from its copy */
	if (rename(backup, file)) {
		printf("Error:\n"
			"Failed to restore file %s from %s -> %s\n"
			"Aborting...\n",
			file, backup, strerror(errno));
	}

	return errno;
}

int update(char const *path, char const *file)
{
	CURL *curl;
	CURLcode res;
	int fd_oui, fd_oui_save;
	int mode;
	char oui[BUF_SIZE];
	char oui_save[BUF_SIZE];

	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	memset(oui, '\0', sizeof(oui));
	memset(oui_save, '\0', sizeof(oui_save));

	/* Open oui.txt */
	/* We know this to work because we now the lengths */
	strcat(oui, path);
	strcat(oui, file);

	fd_oui = open(oui, O_RDWR);
	if (fd_oui < 0) {
		printf("%s -> %s\n", oui, strerror(errno));
		return errno;
	}

	/* Open oui_save.txt */
	/* We know this to work because we now the lengths */
	strcat(oui_save, path);
	strcat(oui_save, "oui_save.txt");

	/* Remove old oui_save file if necessary */
	remove(oui_save);

	fd_oui_save = open(oui_save, O_RDWR | O_CREAT, mode);
	if (fd_oui_save < 0) {
		printf("%s -> %s\n", oui_save, strerror(errno));
		goto oui_out;
	}

	/* copy previous file, in case something goes wrong */
	while (true) {
		ssize_t bytes_read;
		ssize_t bytes_written;
		char buffer[BUF_SIZE_BIG];

		memset(buffer, '\0', sizeof(buffer));

		bytes_read = read(fd_oui, buffer, sizeof(buffer));
		if (bytes_read < 0) {
			printf("%s\n", strerror(errno));
			goto oui_save_out;
		} else if (bytes_read == 0) {
			break;
		}

		bytes_written = write(fd_oui_save, buffer, strlen(buffer));
		if (bytes_written < 0) {
			printf("%s\n", strerror(errno));
			goto oui_save_out;
		}
	}

	/* Must be called at least once */
	res = curl_global_init(CURL_GLOBAL_ALL);
	if (res) {
		curl_error(curl_easy_strerror(res));
		goto oui_save_out;
	}

	curl = curl_easy_init();
	if (!curl) {
		curl_error(curl_easy_strerror(CURLE_FAILED_INIT));
		goto global_out;
	}

	/* Set options */
	if (curl_set_options(curl, fd_oui))
		goto easy_out;

	/* Specify how to restore backup, since it is now needed */
	set_cleanup_cb(&restore_backup, oui, oui_save);

	/* Remove old oui.txt file */
	if (remove(oui)) {
		printf("Error:\n"
			"  Failed to remove file %s -> %s\n"
			"  Aborting...\n", oui, strerror(errno));
		goto oui_save_out;
	}

	/* Close the deleted file */
	close(fd_oui);

	/* And create a new one! */
	fd_oui = open(oui, O_RDWR | O_CREAT, mode);
	if (fd_oui < 0) {
		printf("%s -> %s\n", oui, strerror(errno));
		return errno;
	}

	/* Perform the request */
	res = curl_easy_perform(curl);
	if (res) {
		curl_error(curl_easy_strerror(res));

		/* Restore oui.txt from its copy */
		if (rename(oui_save, oui)) {
			printf("Error:\n"
				"  Failed to rename file %s -> %s\n"
				"  Aborting...\n", oui_save, strerror(errno));
		}

		goto easy_out;
	}

easy_out:
	curl_easy_cleanup(curl);

global_out:
	curl_global_cleanup();

oui_save_out:
	if (fd_oui_save) {
		remove(oui_save);
		close(fd_oui_save);
	}

oui_out:
	if (fd_oui)
		close(fd_oui);

	return 0;
}
