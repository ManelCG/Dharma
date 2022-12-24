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

#include <file_io.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


D_Session *file_io_open_file(const char *filename){
  int32_t x, y, n;
  uint8_t *data = stbi_load(filename, &x, &y, &n, 0);

  char buffer[strlen(filename) + 1];
  char *tok = strrchr(filename, '/');
  if (tok != NULL){
    strcpy(buffer, tok+1);
  } else {
    strcpy(buffer, filename);
  }

  D_Session *s = dharma_session_new_from_data(data, x, y, n*8);

  dharma_session_set_path(s, filename);
  dharma_session_set_filename(s, buffer);


  return s;
}
