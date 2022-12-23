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
#include <string.h>

#include <dharma_session.h>
#include <dharma_image.h>

//Structure deffinitions
typedef struct D_Session {
  uint32_t ID;

  char *filename;

  uint32_t w;
  uint32_t h;
  uint32_t bpp;

  uint32_t centerx;
  uint32_t centery;

  float spanX;
  float spanY;

  float scale;

  void *gtk_image;
  void *gtk_box;

  void *gtk_hadj;
  void *gtk_vadj;

  uint32_t nlayers;
  D_Image **layers;
} D_Session;

//Global variables
uint32_t nsessions = 0;
D_Session **sessions = NULL;

//Constructors
D_Session *dharma_session_new(uint32_t w, uint32_t h, uint32_t bpp){
  D_Session *s = malloc(sizeof(D_Session));

  s->filename = NULL;
  s->w = w;
  s->h = h;
  s->bpp = bpp;

  s->spanX = w;
  s->spanY = h;

  s->centerx = w/2;
  s->centery = h/2;

  s->gtk_image = NULL;
  s->gtk_box = NULL;

  s->gtk_hadj = NULL;
  s->gtk_vadj = NULL;

  //All new sessions have a white canvas by default
  D_Image *im = dharma_image_new_blank(w, h, bpp);
  s->layers = malloc(sizeof(D_Image *));
  s->layers[0] = im;
  s->nlayers = 1;

  s->scale = 1;

  s->ID = nsessions;

  //Allocate space for a new session in sessions array and add it
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

  //Is ID valid?
  if (id >= nsessions){
    return false;
  }

  //Move all sessions in array to the left
  for (i = id+1; i < nsessions; i++){
    sessions[i-1] = sessions[i];
  }
  nsessions--;

  //Destroy all layers in session
  for (i = 0; i < s->nlayers; i++){
    dharma_image_destroy(s->layers[i]);
  }

  //Free all session struct members
  if (s->filename != NULL){
    free((char *) s->filename);
  }
  free(s->layers);
  free(s);

  //If s was the last session, set sessions array as NULL
  if (nsessions == 0){
    free(sessions);
    sessions = NULL;
    return true;
  }

  //Reallocate sessions array and copy all members
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

/*********
 *
 * GETTERS
 *
 *********/

void *dharma_session_get_hadj(D_Session *s){
  return s->gtk_hadj;
}
void *dharma_session_get_vadj(D_Session *s){
  return s->gtk_vadj;
}
float dharma_session_get_spanx(D_Session *s){
  return s->spanX;
}
float dharma_session_get_spany(D_Session *s){
  return s->spanY;
}
void dharma_session_get_center(D_Session *s, uint32_t *x, uint32_t *y){
  *x = s->centerx;
  *y = s->centery;
}
void *dharma_session_get_gtk_box(D_Session *s){
  return s->gtk_box;
}
void *dharma_session_get_gtk_image(D_Session *s){
  return s->gtk_image;
}
D_Session *dharma_session_get_session_from_id(uint32_t id){
  if (id >= nsessions){
    return NULL;
  }
  return sessions[id];
}
uint32_t dharma_session_get_nsessions(){
  return nsessions;
}
const D_Session **dharma_session_get_sessions(){
  return (const D_Session **) sessions;
}

uint32_t dharma_session_get_width(D_Session *s){
  return s->w;
}
uint32_t dharma_session_get_height(D_Session *s){
  return s->h;
}
uint32_t dharma_session_get_bpp(D_Session *s){
  return s->bpp;
}
uint32_t dharma_session_get_Bpp(D_Session *s){
  return s->bpp/8;
}

D_Image *dharma_session_get_layer(D_Session *s, uint32_t layer){
  if (layer >= s->nlayers){
    return NULL;
  }

  return s->layers[layer];
}

float dharma_session_get_scale(D_Session *s){
  return s->scale;
}

//Gets the file name, or a placeholder if filename is NULL
const char *dharma_session_get_filename(D_Session *s){
  if (s->filename == NULL){
    return "Unnamed file";
  } else {
    return s->filename;
  }
}

/*********
 *
 * SETTERS
 *
 *********/

//Sets the filename for the session. If an old filename already existed, it is safely freed.
bool dharma_session_set_filename(D_Session *s, const char *name){
  if (name == NULL){
    return false;
  }

  if (s->filename != NULL){
    free(s->filename);
  }

  s->filename = malloc(name[0] * (strlen(name) + 1));
  strcpy(s->filename, name);
  return true;
}

void dharma_session_set_hadj(D_Session *s, void *adj){
  s->gtk_hadj = adj;
}
void dharma_session_set_vadj(D_Session *s, void *adj){
  s->gtk_vadj = adj;
}
void dharma_session_set_gtk_box(D_Session *s, void *box){
  s->gtk_box = box;
}
void dharma_session_set_gtk_image(D_Session *s, void *image){
  s->gtk_image = image;
}

void dharma_session_set_spanx(D_Session *s, float span){
  s->spanX = span;
}
void dharma_session_set_spany(D_Session *s, float span){
  s->spanY = span;
}

bool dharma_session_set_scale(D_Session *s, float scale){
  if (scale <= 0){
    return false;
  }

  s->scale = scale;
  return true;
}
bool dharma_session_set_center(D_Session *s, uint32_t x, uint32_t y){
  if (x >= s->w || y >= s->h){
    return false;
  }

  s->centerx = x;
  s->centery = y;
  return true;
}
bool dharma_session_offset_center(D_Session *s, int32_t x, int32_t y){
  if (x + (int32_t) s->centerx < 0 || x + s->centerx >= s->w ||
      y + (int32_t) s->centery < 0 || y + s->centery >= s->h){
    return false;
  }

  s->centerx = x + s->centerx;
  s->centery = y + s->centery;

  return true;
}

/********************
 *
 * LAYER MANIPULATION
 *
 ********************/

//Adds a new layer to D_Session s, which is D_Image im.
bool dharma_session_add_layer_from_image(D_Session *s, D_Image *im){
  //Check if the image is valid for the session
  if (!dharma_session_is_image_valid(s, im)){
    return false;
  }

  //Allocate space for new image and add it.
  s->nlayers++;
  if (s->layers != NULL){
    s->layers = realloc(s->layers, sizeof(D_Image *) * s->nlayers);
  } else {
    s->layers = malloc(sizeof(D_Image *) * s->nlayers);
  }

  s->layers[s->nlayers-1] = im;

  return true;
}
//Creates a new empty image and adds it as layer to D_Session s.
bool dharma_session_add_layer(D_Session *s){
  D_Image *im = dharma_image_new_empty(s->w, s->h, s->bpp);
  return dharma_session_add_layer_from_image(s, im);
}

//Removes the layer [index] from the session D_Session s.
bool dharma_session_remove_layer(D_Session *s, uint32_t index){
  uint32_t i;
  D_Image **new_layers;
  D_Image *im;

  //Check if index overflows s->layers
  if (index >= s->nlayers){
    return false;
  }

  //Save pointer to the layer to be destroyed
  im = s->layers[index];

  //Offset to the left all layers after our target
  for (i = index+1; i < s->nlayers; i++){
    s->layers[i-1] = s->layers[i];
  }
  s->nlayers--;

  //Destroy all memory from the target layer
  dharma_image_destroy(im);

  //If it was the last layer in the session, set layer vector as NULL
  if (s->nlayers == 0){
    free(s->layers);
    s->layers = NULL;
    return true;
  }

  //Reallocate new vector for layers and copy the old one.
  new_layers = malloc(sizeof(D_Image *) * s->nlayers);
  for (i = 0; i < s->nlayers; i++){
    new_layers[i] = s->layers[i];
  }
  free(s->layers);
  s->layers = new_layers;
  return true;
}

/***********************
 *
 * CHECKS AND ASSERTIONS
 *
 ***********************/

//Checks if D_Image im can be a valid layer for D_Session s.
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


/*****************
 *
 * DEBUG FUNCTIONS
 *
 *****************/

void dharma_sessions_print_all(){
  uint32_t i;
  D_Session *s;
  if (sessions != NULL){
    for (i = 0; i < nsessions; i++){
      s = sessions[i];
      printf("Session %d [%s]: %dx%d with depth %d and %d layers centered in %dx%d\n", s->ID, dharma_session_get_filename(s), s->w, s->h, s->bpp, s->nlayers, s->centerx, s->centery);
    }
  } else {
    printf("No sessions open\n");
  }
}
