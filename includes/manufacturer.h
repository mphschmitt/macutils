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

#ifndef _MANUFACTURER_H_
#define _MANUFACTURER_H_

/**
 * @brief Find the manufacturer in the file passed as an argument
 *
 * @param mac The mac address to checl
 * @param filename The name of the file to use 
 */
int find_manufacturer(char const *mac, char const *filename);

#endif /* _MANUFACTURER_H_ */

