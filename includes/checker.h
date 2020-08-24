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

#ifndef _CHECKER_H_
#define _CHECKER_H_

/**
 * @brief Case insensitive comparison of two characters
 *
 * @param c1 The first character
 * @param c2 The second character
 * @return True if the character are equivalent, false otherwise
 */
bool chars_are_equivalent(char c1, char c2);

/**
 * @brief Check if a mac address is valid.
 *
 * A mac address is well formated if it is made of 17 hexadecimal digits
 * separated with either -, : or a space
 *
 * @param mac The mac address to check
 * @return True if the mac address is well-formated, false otherwise
 */
bool mac_address_is_valid(char * mac);

#endif /* _CHECKER_H_ */
