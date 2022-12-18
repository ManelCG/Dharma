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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dharma_image.h>
#include <dharma_color.h>

/*********************
 *
 * Struct deffinitions
 *
 *********************/

typedef struct D_Image {
  uint32_t w;
  uint32_t h;
  uint32_t bpp;

  unsigned char *data;
} D_Image;

/***************
 *
 * Constructors
 *
 ***************/

//Creates a new image with an empty canvas
D_Image *dharma_image_new_empty(uint32_t w, uint32_t h, uint32_t bpp){
  D_Image *im = malloc(sizeof(D_Image));

  im->w = w;
  im->h = h;
  im->bpp = bpp;

  im->data = calloc(w * h * bpp, 1);

  return im;
}

//Creates a new image with a white canvas.
D_Image *dharma_image_new_blank(uint32_t w, uint32_t h, uint32_t bpp){
  D_Image *im = dharma_image_new_empty(w, h, bpp);

  switch(bpp){
    case 8:
      dharma_image_fill_canvas(im, 0xFF);
      break;
    case 24:
      dharma_image_fill_canvas(im, 0xFFFFFF);
      break;
    case 32:
      dharma_image_fill_canvas(im, 0xFFFFFFFF);
      break;
    case 64:
      dharma_image_fill_canvas(im, 0xFFFFFFFFFFFFFFFF);
      break;
  }

  return im;
}

//Destructors
bool dharma_image_destroy(D_Image *im){
  if (im == NULL){
    return false;
  }
  if (im->data != NULL){
    free(im->data);
  }

  free(im);
  return true;
}

//Getters
uint32_t dharma_image_get_width(D_Image *im){
  return im->w;
}
uint32_t dharma_image_get_height(D_Image *im){
  return im->h;
}
uint32_t dharma_image_get_bpp(D_Image *im){
  return im->bpp;
}
uint32_t dharma_image_get_Bpp(D_Image *im){
  return im->bpp / 8;
}

unsigned char *dharma_image_get_data(D_Image *im){
  return im->data;
}
const unsigned char *dharma_image_get_data_cnt(D_Image *im){
  return (const unsigned char *) im->data;
}
unsigned char *dharma_image_get_pixel(D_Image *im, uint32_t x, uint32_t y){
  return &(im->data[im->bpp * (im->w * y + x)/8]);
}
const unsigned char *dharma_image_get_pixel_cnt(D_Image *im, uint32_t x, uint32_t y){
  return (const unsigned char *) &(im->data[im->bpp * (im->w * y + x)/8]);
}

bool dharma_image_set_pixel_from_uint64(D_Image *im, uint32_t x, uint32_t y, uint64_t color){
  uint32_t Bpp = im->bpp / 8;
  unsigned char color_array[Bpp];

  if (!color_matches_bpp(color, im->bpp)){
    return false;
  }

  color_uint64_to_1Barray(color, color_array, Bpp);

  memcpy(&im->data[(y*im->w + x) * Bpp], color_array, Bpp);
  return true;
}
bool dharma_image_set_pixel_from_array(D_Image *im, uint32_t x, uint32_t y, const unsigned char *array){
  uint32_t Bpp = im->bpp / 8;

  memcpy(&im->data[(y*im->w + x) * Bpp], array, Bpp);
  return true;
}

bool dharma_image_fill_canvas(D_Image *im, uint64_t color){
  uint32_t i;
  uint32_t Bpp = im->bpp/8;
  unsigned char color_array[Bpp];

  if (!color_matches_bpp(color, im->bpp)){
    return false;
  }

  color_uint64_to_1Barray(color, color_array, Bpp);

  for (i = 0; i < im->w * im->h; i++){
    memcpy(&im->data[i*Bpp], color_array, Bpp);
  }

  return true;
}

//Debug tools
void dharma_image_print(D_Image *im){
  uint32_t Bpp = im->bpp / 8;
  uint32_t i, j, k;

  for (i = 0; i < im->h; i++){
  for (j = 0; j < im->w; j++){
    for (k = 0; k < Bpp; k++){
      printf("%0*x", 2, im->data[(i*im->w + j)*Bpp + k]);
    }
    printf(" ");
  }
  printf("\n");
  }
}
bool dharma_image_fill_canvas_sequential(D_Image *im){
  uint32_t i;
  uint32_t Bpp = im->bpp/8;
  unsigned char color_array[Bpp];

  for (i = 0; i < im->w * im->h; i++){
    color_uint64_to_1Barray(i, color_array, Bpp);
    memcpy(&im->data[i*Bpp], color_array, Bpp);
  }

  return true;
}

