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

#include <stdio.h>
#include <stdlib.h>

#include <dharma_session.h>
#include <dharma_image.h>

//Structure deffinitions
typedef struct D_Session {
  uint32_t ID;

  uint32_t w;
  uint32_t h;
  uint32_t bpp;

  uint32_t nlayers;
  D_Image **layers;
} D_Session;

//Global variables
uint32_t nsessions = 0;
D_Session **sessions = NULL;

//Constructors
D_Session *dharma_session_new(uint32_t w, uint32_t h, uint32_t bpp){
  D_Session *s = malloc(sizeof(D_Session));

  s->w = w;
  s->h = h;
  s->bpp = bpp;

  D_Image *im = dharma_image_new_blank(w, h, bpp);
  s->layers = malloc(sizeof(D_Image *));
  s->layers[0] = im;
  s->nlayers = 1;

  s->ID = nsessions;

  if (sessions == NULL){
    sessions = malloc(sizeof(D_Session *));
  } else {
    sessions = realloc(sessions, sizeof(D_Session *) * (nsessions + 1));
  }
  sessions[nsessions] = s;
  nsessions++;

  return s;
}

//Destructors
bool dharma_session_destroy_from_id(uint32_t id){
  uint32_t i;
  D_Session *s = sessions[id];
  D_Session **new_sessions;

  if (id >= nsessions){
    return false;
  }

  for (i = id+1; i < nsessions; i++){
    sessions[i-1] = sessions[i];
  }
  nsessions--;

  for (i = 0; i < s->nlayers; i++){
    dharma_image_destroy(s->layers[i]);
  }
  free(s->layers);
  free(s);

  if (nsessions == 0){
    free(sessions);
    sessions = NULL;
    return true;
  }

  new_sessions = malloc(sizeof(D_Session *) * nsessions);
  for (i = 0; i < nsessions; i++){
    new_sessions[i] = sessions[i];
    new_sessions[i]->ID = i;
  }
  free(sessions);
  sessions = new_sessions;

  return true;
}
bool dharma_session_destroy(D_Session *s){
  return dharma_session_destroy_from_id(s->ID);
}

bool dharma_sessions_destroy_all(){
  while (nsessions > 0){
    if (!dharma_session_destroy_from_id(0)){
      return false;
    }
  }
  return true;
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
  if (s->layers != NULL){
    s->layers = realloc(s->layers, sizeof(D_Image *) * s->nlayers);
  } else {
    s->layers = malloc(sizeof(D_Image *) * s->nlayers);
  }

  s->layers[s->nlayers-1] = im;

  return true;
}

bool dharma_session_remove_layer(D_Session *s, uint32_t index){
  uint32_t i;
  D_Image **new_layers;
  D_Image *im;

  if (index >= s->nlayers){
    return false;
  }

  im = s->layers[index];

  for (i = index+1; i < s->nlayers; i++){
    s->layers[i-1] = s->layers[i];
  }
  s->nlayers--;

  dharma_image_destroy(im);

  if (s->nlayers == 0){
    free(s->layers);
    s->layers = NULL;
    return true;
  }

  new_layers = malloc(sizeof(D_Image *) * s->nlayers);
  for (i = 0; i < s->nlayers; i++){
    new_layers[i] = s->layers[i];
  }
  free(s->layers);
  s->layers = new_layers;
  return true;
}

bool dharma_session_add_layer(D_Session *s){
  D_Image *im = dharma_image_new_empty(s->w, s->h, s->bpp);
  return dharma_session_add_layer_from_image(s, im);
}

//Debug functions
void dharma_sessions_print_all(){
  uint32_t i;
  D_Session *s;
  if (sessions != NULL){
    for (i = 0; i < nsessions; i++){
      s = sessions[i];
      printf("Session %d: %dx%d with depth %d and %d layers\n", s->ID, s->w, s->h, s->bpp, s->nlayers);
    }
  } else {
    printf("No sessions open\n");
  }
}