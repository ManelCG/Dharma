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

//Destructors
bool dharma_session_destroy_from_id(uint32_t id);
bool dharma_session_destroy(D_Session *s);
bool dharma_sessions_destroy_all();

//Getters & setters
D_Session *dharma_session_get_session_from_id(uint32_t id);
uint32_t dharma_session_get_nsessions();
const D_Session **dharma_session_get_sessions();

uint32_t dharma_session_get_width(D_Session *s);
uint32_t dharma_session_get_height(D_Session *s);
uint32_t dharma_session_get_bpp(D_Session *s);
uint32_t dharma_session_get_Bpp(D_Session *s);

D_Image *dharma_session_get_layer(D_Session *s, uint32_t layer);

const char *dharma_session_get_filename(D_Session *s);
bool dharma_session_set_filename(D_Session *s, const char *name);

bool dharma_session_is_image_valid(D_Session *s, D_Image *im);

//Layer vector manipulation
bool dharma_session_add_layer_from_image(D_Session *s, D_Image *im);
bool dharma_session_add_layer(D_Session *s);
bool dharma_session_remove_layer(D_Session *s, uint32_t index);

//Debug functions
void dharma_sessions_print_all();

#endif //_DHARMA_SESSION_H_
