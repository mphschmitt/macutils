/*
    libreadoui Read the iee's oui.txt file
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

#ifndef _LIBREADOUI_H_
#define _LIBREADOUI_H_

/*
 * libreaoui Read a oui.txt file.
 *
 * It offers 2 global varibales: libreadoui_line and libreadoui_buff_size.
 * Both are necessary to read the file, and are completly handled by the 
 * library. The user should not be concerned with these values, and should
 * not modify them.
 *
 * libreaoui_line is a pointer to the current line in the file.
 *
 * libreadoui_buff_size is the current size of the buffer containing the line.
 * libreadoui uses getline. Since getline may realloc the memory for each
 * new line, it is thus necessary to store the current size of the buffer to
 * avoid leaks.
 *
 * When done reading the file, the user should free the memory by calling
 * libreadoui_line_free.
 *
 */

/**
 * @brief Skip the first lines from the file.
 *
 * These lines are a header that describes the format of the file.
 * They are of no use to maclookup.
 */
int libreadoui_skip_header(FILE *ptr);

/**
 * @brief Move the file pointer to the start of the next manufacturer
 */
int libreadoui_get_next_manufacturer(FILE *ptr);

/**
 * @brief Print the manufacturer's informations.
 */
int libreadoui_print_manufacturer(FILE *ptr);

void libreadoui_print_error(void);

void libreadoui_line_free(void);

#endif /* _LIBREADOUI_H_ */

