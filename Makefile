# maclookup Find the manufacturer of a network interface
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
MATH_FLAGS := -lm
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
FLAGS := ${MATH_FLAGS} ${ERROR_FLAGS} ${FORMAT_FLAGS} ${OPTIMIZATION_FLAGS}

OUTPUT_DIR := out

PROG_NAME := maclookup
SRC := main.c \
	checker.c \
	manufacturer.c \
	file.c
SOURCES := $(addprefix src/, ${SRC})

INCLUDES := includes
LDLIBS := -lcurl

MAN_DIR := man
MAN := ${PROG_NAME}.1
INSTALL_DIR := /usr/local/bin
INSTALL_MAN_DIR := /usr/local/share/man/man1
INSTALL_MAN_DIR_FR := /usr/local/share/man/fr/man1

DATABASE_DIR := /usr/local/share/${PROG_NAME}

.PHONY: all
all: maclookup

.PHONY: maclookup
maclookup: ${SOURCES}
	@mkdir -p ${OUTPUT_DIR}
	${COMPILER} ${SOURCES} -I${INCLUDES} ${LDLIBS} ${FLAGS} -o ${OUTPUT_DIR}/${PROG_NAME}

.PHONY: install
install: ${OUTPUT_DIR}/${PROG_NAME} install_man download_data
	@mkdir -p ${DESTDIR}${INSTALL_DIR}
	@cp $< ${DESTDIR}${INSTALL_DIR}/${PROG_NAME}

.PHONY: uninstall
uninstall: uninstall_man
	@rm ${DESTDIR}${INSTALL_DIR}/${PROG_NAME}

.PHONY: install_man
install_man:
	@mkdir -p ${DESTDIR}${INSTALL_MAN_DIR}
	@cp ${MAN_DIR}/${MAN} ${DESTDIR}${INSTALL_MAN_DIR}/${MAN}

	@mkdir -p ${DESTDIR}${INSTALL_MAN_DIR_FR}
	@cp ${MAN_DIR}/fr/${MAN} ${DESTDIR}${INSTALL_MAN_DIR_FR}/${MAN}

	# Fedora
	# sudo dnf install libcurl-devel

.PHONY: uninstall_man
uninstall_man:
	rm ${DESTDIR}${INSTALL_MAN_DIR}/${MAN}
	rm ${DESTDIR}${INSTALL_MAN_DIR_FR}/${MAN}

.PHONY: download_data
download_data:
	@mkdir -p ${DATABASE_DIR}
	@bash -c 'curl --connect-timeout 10 http://standards-oui.ieee.org/oui.txt > ${DATABASE_DIR}/oui.txt'

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
