
# libreadoui Read the iee's oui.txt file
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
FLAGS := ${MATH_FLAGS} ${ERROR_FLAGS} ${FORMAT_FLAGS} ${OPTIMIZATION_FLAGS}

OUTPUT_DIR := out
SRC := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, ${SRC})
OBJS := $(addprefix out/, ${OBJS})
LIB_NAME := libreadoui.a

.PHONY: all
all: libreadoui

${OBJS}: ${SRC}
	@mkdir -p ${OUTPUT_DIR}
	@${COMPILER} ${SRC} ${FLAGS} -c -o $@

.PHONY: libreadoui
libreadoui: ${OBJS}
	@ar rcs ${LIB_NAME} ${OBJS}

.PHONY: clean
clean:
	@rm -rf ${OUTPUT_DIR}
	@rm ${LIB_NAME}
