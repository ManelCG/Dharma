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

#include <dharma_color.h>
#include <dharma_math.h>
#include <math.h>

bool color_matches_Bpp(uint64_t color, uint32_t Bpp){
  return color_matches_bpp(color, Bpp*8);
}

bool color_matches_bpp(uint64_t color, uint32_t bpp){
  //Allow for 8Bpp max
  if (bpp > 64){
    return false;
  }

  if (log2f(color) > bpp){
    return false;
  }

  return true;
}

bool color_uint64_to_1Barray(uint64_t color, unsigned char *array, uint32_t Bpp){
  uint32_t i;
  uint64_t color_offset;

  if (! color_matches_Bpp(color, Bpp)){
    return false;
  }

  color_offset = color;
  for (i = 0; i < Bpp; i++){
    array[Bpp-i-1] = color_offset & 0xFF;
    color_offset >>= 8;
  }

  return true;
}

uint64_t color_1Barray_to_uint64(const unsigned char *array, uint32_t Bpp){
  uint32_t i;
  uint64_t color = 0;

  for (i = 0; i < Bpp; i++){
    color <<= 8;
    color += array[i];
  }

  return color;
}
