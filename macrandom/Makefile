# macrandom Generate a random mac address
# Copyright (C) 2020  Mathias Schmitt
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

COMPILER := gcc
ERROR_FLAGS := -Wall \
	-Werror \
	-Wextra \
	-pedantic \
	-Wshadow \
	-Wdouble-promotion \
	-fno-common \
	-Wconversion
FORMAT_FLAGS := -Wformat=2 -Wundef
OPTIMIZATION_FLAGS := -O3
FLAGS := ${ERROR_FLAGS} ${FORMAT_FLAGS} ${OPTIMIZATION_FLAGS}

OUTPUT_DIR := out

PROG_NAME := macrandom
SOURCES := main.c

LIBREAD_OUI_PATH := ../libreadoui

MAN_DIR := man
MAN := ${PROG_NAME}.1
INSTALL_DIR := /usr/local/bin
INSTALL_MAN_DIR := /usr/local/share/man/man1
INSTALL_MAN_DIR_FR := /usr/local/share/man/fr/man1

default: all

.PHONY: all
all: macrandom

.PHONY: macrandom
macrandom: ${SOURCES}
	@mkdir -p ${OUTPUT_DIR}
	@${COMPILER} ${SOURCES} -I${LIBREAD_OUI_PATH} ${LIBREAD_OUI_PATH}/libreadoui.a ${FLAGS} -o ${OUTPUT_DIR}/${PROG_NAME}

.PHONY: install
install:

.PHONY: uninstall
uninstall: uninstall_man
	@rm ${DESTDIR}${INSTALL_DIR}/${PROG_NAME}

.PHONY: install_man
install_man:
	@mkdir -p ${DESTDIR}${INSTALL_MAN_DIR}
	@cp ${MAN_DIR}/${MAN} ${DESTDIR}${INSTALL_MAN_DIR}/${MAN}

	@mkdir -p ${DESTDIR}${INSTALL_MAN_DIR_FR}
	@cp ${MAN_DIR}/fr/${MAN} ${DESTDIR}${INSTALL_MAN_DIR_FR}/${MAN}

.PHONY: uninstall_man
uninstall_man:
	@rm ${DESTDIR}${INSTALL_MAN_DIR}/${MAN}
	@rm ${DESTDIR}${INSTALL_MAN_DIR_FR}/${MAN}

.PHONY: clean
clean:
	@rm -rf ${OUTPUT_DIR}

.PHONY: help
help:
	@echo "Use one of the following targets:"
	@echo "  help     Print this help message"
	@echo "  all      Build maclookup"
	@echo "  clean    Clean output from previous build"
	@echo "  install  Install maclookup on your system"
