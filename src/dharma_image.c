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
#include <dharma_math.h>
#include <dharma_session.h>

/*********************
 *
 * Struct deffinitions
 *
 *********************/

typedef struct D_Image {
  uint32_t w;
  uint32_t h;
  uint32_t bpp;

  char *name;
  D_Session *owner_session;

  uint8_t *data;
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

  im->name = NULL;

  return im;
}

//Creates a new image from data
D_Image *dharma_image_new_from_data(uint8_t *data, uint32_t w, uint32_t h, uint32_t bpp){
  D_Image *im = malloc(sizeof(D_Image));

  im->w = w;
  im->h = h;
  im->bpp = bpp;

  im->data = data;

  im->name = NULL;

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

  if (im->name != NULL){
    free(im->name);
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

const char *dharma_image_get_name(D_Image *im){
  if (im->name == NULL){
    return "Unnamed image";
  } else {
    return im->name;
  }
}

uint8_t *dharma_image_get_data(D_Image *im){
  return im->data;
}
const uint8_t *dharma_image_get_data_cnt(D_Image *im){
  return (const uint8_t *) im->data;
}
uint8_t *dharma_image_get_pixel(D_Image *im, uint32_t x, uint32_t y){
  return &(im->data[im->bpp * (im->w * y + x)/8]);
}
const uint8_t *dharma_image_get_pixel_cnt(D_Image *im, uint32_t x, uint32_t y){
  return (const uint8_t *) &(im->data[im->bpp * (im->w * y + x)/8]);
}

//Setters
bool dharma_image_set_name(D_Image *im, const char *name){
  if (name == NULL){
    return false;
  }

  if (im->name != NULL){
    free(im->name);
  }

  im->name = malloc(sizeof(name[0]) * (strlen(name) + 1));
  strcpy(im->name, name);
  return true;
}

bool dharma_image_set_pixel_from_uint64(D_Image *im, uint32_t x, uint32_t y, uint64_t color){
  uint32_t Bpp = im->bpp / 8;
  uint8_t color_array[Bpp];

  if (!color_matches_bpp(color, im->bpp)){
    return false;
  }

  color_uint64_to_1Barray(color, color_array, Bpp);

  memcpy(&im->data[(y*im->w + x) * Bpp], color_array, Bpp);
  return true;
}
bool dharma_image_set_pixel_from_1Barray(D_Image *im, uint32_t x, uint32_t y, const uint8_t *array){
  uint32_t Bpp = im->bpp / 8;

  memcpy(&im->data[(y*im->w + x) * Bpp], array, Bpp);
  return true;
}
bool dharma_image_set_pixel_from_2Barray(D_Image *im, uint32_t x, uint32_t y, const uint16_t *array){
  uint32_t Bpp = im->bpp / 8;

  memcpy(&im->data[(y*im->w + x) * Bpp], array, Bpp / 2);
  return true;
}

/*********************
 *
 * Image Operations
 *
 *********************/


bool dharma_image_fill_canvas(D_Image *im, uint64_t color){
  uint32_t i;
  uint32_t Bpp = im->bpp/8;
  uint8_t color_array[Bpp];

  if (!color_matches_bpp(color, im->bpp)){
    return false;
  }

  color_uint64_to_1Barray(color, color_array, Bpp);

  for (i = 0; i < im->w * im->h; i++){
    memcpy(&im->data[i*Bpp], color_array, Bpp);
  }

  return true;
}

bool dharma_image_rotate_clockwise(D_Image *im){
  uint8_t *data = calloc(im->w * im->h * im->bpp, 1);
  uint32_t aux;
  uint32_t i, j, k;
  uint32_t Bpp = im->bpp / 8;

  for (j = 0; j < im->w; j++){
  for (i = 0; i < im->h; i++){
    for (k = 0; k < Bpp; k++){
      data[(j*im->h + (im->h-i-1))*Bpp + k] = im->data[(i*im->w + j)*Bpp + k];
    }
  }
  }

  free(im->data);
  im->data = data;

  aux = im->h;
  im->h = im->w;
  im->w = aux;

  return true;
}
bool dharma_image_rotate_anticlockwise(D_Image *im){
  uint8_t *data = calloc(im->w * im->h * im->bpp, 1);
  uint32_t aux;
  uint32_t i, j, k;
  uint32_t Bpp = im->bpp / 8;

  for (j = 0; j < im->w; j++){
  for (i = 0; i < im->h; i++){
    for (k = 0; k < Bpp; k++){
      data[((im->w -j -1)*im->h + i)*Bpp + k] = im->data[(i*im->w + j)*Bpp + k];
    }
  }
  }

  free(im->data);
  im->data = data;

  aux = im->h;
  im->h = im->w;
  im->w = aux;

  return true;
}
bool dharma_image_rotate_180(D_Image *im){
  uint8_t *data = calloc(im->w * im->h * im->bpp, 1);
  uint32_t i, k;
  uint32_t Bpp = im->bpp / 8;

  for (i = 0; i < im->w * im->h; i++){
    for (k = 0; k < Bpp; k++){
      data[((im->w * im->h)-i -1)*Bpp + k] = im->data[i*Bpp + k];
    }
  }

  free(im->data);
  im->data = data;
  return true;
}

bool dharma_image_flip_horizontally(D_Image *im){
  uint8_t *data = calloc(im->w * im->h * im->bpp, 1);
  uint32_t i, j, k;
  uint32_t Bpp = im->bpp / 8;

  for (j = 0; j < im->w; j++){
  for (i = 0; i < im->h; i++){
    for (k = 0; k < Bpp; k++){
      data[(i*im->w + (im->w - j -1))*Bpp + k] = im->data[(i*im->w + j)*Bpp + k];
    }
  }
  }

  free(im->data);
  im->data = data;

  return true;
}
bool dharma_image_flip_vertically(D_Image *im){
  uint8_t *data = calloc(im->w * im->h * im->bpp, 1);
  uint32_t i, j, k;
  uint32_t Bpp = im->bpp / 8;

  for (j = 0; j < im->w; j++){
  for (i = 0; i < im->h; i++){
    for (k = 0; k < Bpp; k++){
      data[((im->h -i -1)*im->w + j)*Bpp + k] = im->data[(i*im->w + j)*Bpp + k];
    }
  }
  }

  free(im->data);
  im->data = data;

  return true;
}

void *dharma_image_get_owner_session(D_Image *im){
  return im->owner_session;
}
void dharma_image_set_owner_session(D_Image *im, void *s){
  im->owner_session = s;
}

//Helpers
void dharma_image_set_default_name(D_Image *im, uint32_t n){
  char *layer = "Layer ";
  char buffer[strlen(layer) + 16];
  snprintf(buffer, strlen(layer) + 15, "%s %d", layer, n);

  dharma_image_set_name(im, buffer);
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
  uint8_t color_array[Bpp];

  for (i = 0; i < im->w * im->h; i++){
    color_uint64_to_1Barray(i, color_array, Bpp);
    memcpy(&im->data[i*Bpp], color_array, Bpp);
  }

  return true;
}

bool dharma_image_fill_canvas_random(D_Image *im){
  uint32_t i;
  uint32_t Bpp = im->bpp/8;
  uint8_t color_array[Bpp];

  uint64_t max_color = dharma_math_uint_pow(2, im->bpp);

  printf("%ld\n", max_color);

  for (i = 0; i < im->w * im->h; i++){
    color_uint64_to_1Barray(rand() % max_color, color_array, Bpp);
    memcpy(&im->data[i*Bpp], color_array, Bpp);
  }

  return true;
}
