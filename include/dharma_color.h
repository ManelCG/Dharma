/*
 *  Dharma
 *  Copyright (C) 2022  Manel Castillo Giménez <manelcg@protonmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <stdint.h>
#include <stdbool.h>

#ifndef __DHARMA_COLOR_H_
#define __DHARMA_COLOR_H_

//Checks if color is a valid color for the given Bpp / bpp
bool color_matches_Bpp(uint64_t color, uint32_t Bpp);
bool color_matches_bpp(uint64_t color, uint32_t bpp);

//Transform a color int to an array of bytes and backwards
bool color_uint64_to_1Barray(uint64_t color, unsigned char *array, uint32_t Bpp);
uint64_t color_1Barray_to_uint64(const unsigned char *array, uint32_t Bpp);

#endif //_DHARMA_COLOR_H_
