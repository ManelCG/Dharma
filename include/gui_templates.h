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

#include <gtk/gtk.h>

#include <dharma_session.h>

#ifndef __GUI_TEMPLATES_H_
#define __GUI_TEMPLATES_H_

void draw_main_window(GtkWidget *window, gpointer data);

//SPAWNERS
void gui_templates_spawn_layers_window();

//Gui elements constructors
GtkWidget *gui_templates_get_mainscreen_menubar();
GtkWidget *gui_templates_get_sessions_notebook();
GtkWidget *gui_templates_get_welcome_screen_box();
GtkWidget *gui_templates_get_canvas_from_session(D_Session *s);
GtkWidget *gui_templates_get_viewmode_toolbar(D_Session *s);

GtkWidget *gui_templates_get_layers_window_box(D_Session *s);

GtkWidget *gui_templates_get_window_toggling_toolbar();

GdkPixbuf *gui_templates_pixbuf_from_image(D_Image *im);
GdkPixbuf *gui_templates_pixbuf_from_session(D_Session *s, uint32_t daw, uint32_t dah, float *newcx, float *newcy);

void gui_templates_new_file_window(GtkWidget *w_unused, gpointer gp_unused);

//Wrappers
D_Session *gui_templates_open_file();

//Helper functions
void gui_templates_clear_container(GtkWidget *container);
void gui_templates_destroy(GtkWidget *w, gpointer data);

void gui_templates_update_layers_window(D_Session *s);
void gui_templates_update_session_and_redraw(D_Session *s);


void gui_templates_set_window_layers(GtkWidget *w);

//Handlers
void gui_templates_notebook_page_reordered_handler(GtkNotebook* self,GtkWidget* child, uint32_t page_num, gpointer data);

void gui_templates_close_session_handler(GtkWidget *w, gpointer d);

void gui_templates_toggle_window_layers_visible(GtkWidget *w, gpointer d);
void gui_templates_toggle_window_layers_visible_button_handler(GtkWidget *w, gpointer d);

void gui_templates_notebook_switch_page_handler(GtkWidget *notebook, GtkWidget *page, uint32_t pagenum, gpointer data);

void gui_templates_new_layer_button_handler(GtkWidget *w, gpointer d);
void gui_templates_remove_layer_button_handler(GtkWidget *w, gpointer d);

void gui_templates_move_layer_up_button_handler(GtkWidget *w, gpointer d);
void gui_templates_move_layer_down_button_handler(GtkWidget *w, gpointer d);

void gui_templates_select_layer_handler(GtkWidget *w, gpointer d);
void gui_templates_select_layer_handler_buttonpress(GtkWidget *w, GdkEventButton *ev, gpointer d);

void gui_templates_toggle_layer_visibility_handler(GtkWidget *w, gpointer d);

void gui_templates_zoomin_button_handler(GtkWidget *w, gpointer d);
void gui_templates_zoomout_button_handler(GtkWidget *w, gpointer d);

void gui_templates_flip_image_horizontally_handler(GtkWidget *w, gpointer d);
void gui_templates_flip_image_vertically_handler(GtkWidget *w, gpointer d);
void gui_templates_flip_layer_horizontally_handler(GtkWidget *w, gpointer d);
void gui_templates_flip_layer_vertically_handler(GtkWidget *w, gpointer d);

void gui_templates_rotate_image_clockwise_handler(GtkWidget *w, gpointer d);
void gui_templates_rotate_image_anticlockwise_handler(GtkWidget *w, gpointer d);
void gui_templates_rotate_image_180_handler(GtkWidget *w, gpointer d);
void gui_templates_rotate_layer_clockwise_handler(GtkWidget *w, gpointer d);
void gui_templates_rotate_layer_anticlockwise_handler(GtkWidget *w, gpointer d);
void gui_templates_rotate_layer_180_handler(GtkWidget *w, gpointer d);

void gui_templates_canvas_vertical_adjustment_value_changed_handler(GtkAdjustment *adj, gpointer d);
void gui_templates_canvas_horizontal_adjustment_value_changed_handler(GtkAdjustment *adj, gpointer d);

void canvas_mouse_handler(GtkWidget *event_box, GdkEventButton *event, gpointer data);

#endif //__GUI_TEMPLATES_H_
