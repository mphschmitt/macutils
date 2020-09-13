# macutils Utilities to manipulate mac addresses
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

MACLOOKUP_PATH := ./maclookup
MACRANDOM_PATH := ./macrandom
LIBREADOUI_PATH := ./libreadoui

default: all

.PHONY: libreadoui
libreadoui:
	@make -C ${LIBREADOUI_PATH}

.PHONY: maclookup
maclookup: libreadoui
	@make -C ${MACLOOKUP_PATH}

.PHONY: macrandom
macrandom: libreadoui
	@make -C ${MACRANDOM_PATH}

.PHONY: all
all: libreadoui maclookup macrandom

.PHONY: install
install:
	@make install -C ${MACLOOKUP_PATH}
	@make install -C ${MACRANDOM_PATH}

.PHONY: uninstall
uninstall:
	@make uninstall -C ${MACLOOKUP_PATH}
	@make uninstall -C ${MACRANDOM_PATH}

.PHONY: clean
clean:
	@make clean -C ${MACLOOKUP_PATH}
	@make clean -C ${MACRANDOM_PATH}
	@make clean -C ${LIBREADOUI_PATH}

.PHONY: help
help:
	@echo "Use one of the following targets:"
	@echo "  help     Print this help message"
	@echo "  all      Build macutils programs"
	@echo "  clean    Clean output from previous build"
	@echo "  install  Install macutils programs on your system"
