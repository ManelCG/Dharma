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

#ifndef __DHARMA_IMAGE_H_
#define __DHARMA_IMAGE_H_

enum {D_Image_Mono, D_Image_RGB, D_Image_ARGB};

typedef struct D_Image D_Image;

//Constructors
D_Image *dharma_image_new_empty(uint32_t w, uint32_t h, uint32_t bpp);
D_Image *dharma_image_new_blank(uint32_t w, uint32_t h, uint32_t bpp);
D_Image *dharma_image_new_from_data(uint8_t *data, uint32_t w, uint32_t h, uint32_t bpp);

//Destructors
bool dharma_image_destroy(D_Image *im);

//Getters
uint32_t dharma_image_get_width(D_Image *im);
uint32_t dharma_image_get_height(D_Image *im);
uint32_t dharma_image_get_bpp(D_Image *im);
uint32_t dharma_image_get_Bpp(D_Image *im);

bool dharma_image_set_name(D_Image *im, const char *name);
const char *dharma_image_get_name(D_Image *im);

uint8_t *dharma_image_get_data(D_Image *im);
const uint8_t *dharma_image_get_data_cnt(D_Image *im);
uint8_t *dharma_image_get_pixel(D_Image *im, uint32_t x, uint32_t y);
const uint8_t *dharma_image_get_pixel_cnt(D_Image *im, uint32_t x, uint32_t y);

bool dharma_image_set_pixel_from_uint64(D_Image *im, uint32_t x, uint32_t y, uint64_t color);
bool dharma_image_set_pixel_from_1Barray(D_Image *im, uint32_t x, uint32_t y, const uint8_t *array);
bool dharma_image_set_pixel_from_2Barray(D_Image *im, uint32_t x, uint32_t y, const uint16_t *array);

void *dharma_image_get_owner_session(D_Image *im);
void dharma_image_set_owner_session(D_Image *im, void *s);

bool dharma_image_is_visible(D_Image *im);
void dharma_image_set_visible(D_Image *im, bool v);

//Image operations
bool dharma_image_fill_canvas(D_Image *im, uint64_t color);

bool dharma_image_rotate_clockwise(D_Image *im);
bool dharma_image_rotate_anticlockwise(D_Image *im);
bool dharma_image_rotate_180(D_Image *im);

bool dharma_image_flip_horizontally(D_Image *im);
bool dharma_image_flip_vertically(D_Image *im);

//Helpers
void dharma_image_set_default_name(D_Image *im, uint32_t n);

//Debug tools
void dharma_image_print(D_Image *im);
bool dharma_image_fill_canvas_sequential(D_Image *im);
bool dharma_image_fill_canvas_random(D_Image *im);

#endif //_DHARMA_IMAGE_H_
