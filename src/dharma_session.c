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

#include <dharma_session.h>
#include <dharma_image.h>
#include <stdlib.h>

typedef struct D_Session {
  uint32_t w;
  uint32_t h;
  uint32_t bpp;

  uint32_t nlayers;
  D_Image **layers;
} D_Session;

D_Session *dharma_session_new(uint32_t w, uint32_t h, uint32_t bpp){
  D_Session *s = malloc(sizeof(D_Session));

  s->w = w;
  s->h = h;
  s->bpp = bpp;

  D_Image *im = dharma_image_new_blank(w, h, bpp);
  s->layers = malloc(sizeof(D_Image *));
  s->layers[0] = im;
  s->nlayers = 1;

  return s;
}

bool dharma_session_is_image_valid(D_Session *s, D_Image *im){
  if (dharma_image_get_width(im) != s->w){
    return false;
  }
  if (dharma_image_get_height(im) != s->h){
    return false;
  }
  if (dharma_image_get_bpp(im) != s->bpp){
    return false;
  }

  return true;
}

bool dharma_session_add_layer_from_image(D_Session *s, D_Image *im){
  if (!dharma_session_is_image_valid(s, im)){
    return false;
  }

  s->nlayers++;
  s->layers = realloc(s->layers, sizeof(D_Image *) * s->nlayers);

  s->layers[s->nlayers-1] = im;

  return true;
}

bool dharma_session_add_layer(D_Session *s){
  D_Image *im = dharma_image_new_empty(s->w, s->h, s->bpp);
  return dharma_session_add_layer_from_image(s, im);
}
