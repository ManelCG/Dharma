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

bool color_matches_stride(uint64_t color, uint32_t stride){
  //Stride bigger than 8 overflows uint64_t
  if (stride > 8){
    return false;
  }

  //Max color acceptable is 2^(stride*8)
  if (color > dharma_math_uint_pow(2, stride*8)){
      return false;
  }

  return true;
}
