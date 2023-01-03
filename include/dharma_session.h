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

#include <stdbool.h>

#include <dharma_image.h>

#ifndef __DHARMA_SESSION_H_
#define __DHARMA_SESSION_H_

typedef struct D_Session D_Session;

//Constructors
D_Session *dharma_session_new(uint32_t w, uint32_t h, uint32_t bpp);
D_Session *dharma_session_new_from_data(uint8_t *data, uint32_t w, uint32_t h, uint32_t bpp);

//Destructors
bool dharma_session_destroy_from_id(uint32_t id);
bool dharma_session_destroy(D_Session *s);
bool dharma_sessions_destroy_all();

//Getters & setters
D_Session *dharma_session_get_session_from_id(uint32_t id);
uint32_t dharma_session_get_nsessions();
const D_Session **dharma_session_get_sessions();

void dharma_session_set_spanx(D_Session *s, float span);
void dharma_session_set_spany(D_Session *s, float span);
float dharma_session_get_spanx(D_Session *s);
float dharma_session_get_spany(D_Session *s);

void dharma_session_set_hadj(D_Session *s, void *);
void dharma_session_set_vadj(D_Session *s, void *);
void *dharma_session_get_hadj(D_Session *s);
void *dharma_session_get_vadj(D_Session *s);

float dharma_session_get_scale(D_Session *s);
bool dharma_session_set_scale(D_Session *s, float scale);

uint32_t dharma_session_get_width(D_Session *s);
uint32_t dharma_session_get_height(D_Session *s);
uint32_t dharma_session_get_bpp(D_Session *s);
uint32_t dharma_session_get_Bpp(D_Session *s);

void dharma_session_get_center(D_Session *s, uint32_t *x, uint32_t *y);
bool dharma_session_set_center(D_Session *s, uint32_t x, uint32_t y);
bool dharma_session_offset_center(D_Session *s, int32_t x, int32_t y);
bool dharma_session_set_centerx(D_Session *s, uint32_t c);
bool dharma_session_set_centery(D_Session *s, uint32_t c);
uint32_t dharma_session_get_centerx(D_Session *s);
uint32_t dharma_session_get_centery(D_Session *s);
bool dharma_session_offset_centerx(D_Session *s, uint32_t o);
bool dharma_session_offset_centery(D_Session *s, uint32_t o);

void dharma_session_set_gtk_image(D_Session *s, void *image);
void *dharma_session_get_gtk_image(D_Session *s);
void dharma_session_set_gtk_box(D_Session *s, void *box);
void *dharma_session_get_gtk_box(D_Session *s);
void dharma_session_set_gtk_da(D_Session *s, void *da);
void *dharma_session_get_gtk_da(D_Session *s);

D_Image *dharma_session_get_layer(D_Session *s, uint32_t layer);
uint32_t dharma_session_get_nlayers(D_Session *s);

const char *dharma_session_get_path(D_Session *s);
bool dharma_session_set_path(D_Session *s, const char *name);
const char *dharma_session_get_filename(D_Session *s);
bool dharma_session_set_filename(D_Session *s, const char *name);

void dharma_session_set_cropw(D_Session *s, uint32_t cropw);
void dharma_session_set_croph(D_Session *s, uint32_t croph);
uint32_t dharma_session_get_cropw(D_Session *s);
uint32_t dharma_session_get_croph(D_Session *s);

bool dharma_session_is_image_valid(D_Session *s, D_Image *im);

D_Session *dharma_session_get_selected_session();
D_Image  *dharma_session_get_selected_layer(D_Session *s);
uint32_t dharma_session_get_selected_layer_index(D_Session *s);
bool dharma_session_set_selected_layer(D_Session *d, uint32_t index);

D_Image *dharma_session_get_layer_sum(D_Session *s);
bool dharma_session_update_layer_sum(D_Session *s, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

//Layer vector manipulation
bool dharma_session_add_layer_from_image(D_Session *s, D_Image *im);
bool dharma_session_add_layer(D_Session *s);
bool dharma_session_remove_layer(D_Session *s, uint32_t index);

bool dharma_session_swap_layers(D_Session *s, uint32_t a, uint32_t b);
bool dharma_session_slide_layer(D_Session *s, uint32_t src, uint32_t dest);

//Session image operations
bool dharma_session_rotate_clockwise(D_Session *s);
bool dharma_session_rotate_anticlockwise(D_Session *s);
bool dharma_session_rotate_180(D_Session *s);

bool dharma_session_flip_horizontally(D_Session *s);
bool dharma_session_flip_vertically(D_Session *s);

//Debug functions
void dharma_sessions_print_all();

#endif //_DHARMA_SESSION_H_
