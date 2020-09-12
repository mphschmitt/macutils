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
#include <stdlib.h>
#include <signal.h>

#include "signals.h"

int (*cleanup)(char const *, char const *) = NULL;
const char *arg1 = NULL;
const char *arg2 = NULL;

static void print_error(int sig)
{
	printf("\nReceived interrupt: %s: %d\n"
		"  Stop downloading latest database version from the iee.\n"
		"  Restore databse to last version.\n",
		sig == SIGINT ? "SIGINT" : sig == SIGQUIT ? "SIGQUIT" :
		sig == SIGTERM ? "SIGTERM" : "", sig);
}

static void handler_sigterm(int sig)
{
	if (cleanup && arg1 && arg2) {
		print_error(sig);
		cleanup(arg1, arg2);
	}
	exit(0);
}

static void handler_sigint(int sig)
{
	if (cleanup && arg1 && arg2) {
		print_error(sig);
		cleanup(arg1, arg2);
	}
	exit(0);
}

static void handler_sigquit(int sig)
{
	if (cleanup && arg1 && arg2) {
		print_error(sig);
		cleanup(arg1, arg2);
	}
	exit(0);
}

void set_cleanup_cb(int cleanup_cb(char const *, char const *),
		char const * arg_1, char const *arg_2)
{
	cleanup = cleanup_cb;
	arg1 = arg_1;
	arg2 = arg_2;
}

void signal_handler(void)
{
	struct sigaction term_action = {0};
	struct sigaction int_action = {0};
	struct sigaction quit_action = {0};

	term_action.sa_handler = &handler_sigterm;
	int_action.sa_handler = &handler_sigint;
	quit_action.sa_handler = &handler_sigquit;

	sigaction(SIGTERM, &term_action, NULL);
	sigaction(SIGINT, &int_action, NULL);
	sigaction(SIGQUIT, &quit_action, NULL);
}

#include "signals.h"
