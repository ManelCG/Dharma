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

#include <stdio.h>

#include <gui_templates.h>

#include <file_io.h>

#include <dharma_image.h>
#include <dharma_defines.h>
#include <dharma_math.h>
#include <dharma_color.h>
#include <dharma_session.h>

int main(int argc, char *argv[]){
  gtk_init(&argc, &argv);

  D_Session *s;
  D_Image *im, *firstlayer;

  s = dharma_session_new(200, 500, 32);
  firstlayer = dharma_session_get_layer(s, 0);
  dharma_image_fill_canvas(firstlayer, 0xFF000080);
  im = dharma_image_new_empty(200, 500, 32);
  dharma_image_fill_canvas(im, 0x00FF0080);
  dharma_session_add_layer_from_image(s, im);
  dharma_session_update_layer_sum(s, 0, 0, dharma_session_get_width(s), dharma_session_get_height(s));
  dharma_session_set_filename(s, "Red.png");

  s = dharma_session_new(1920, 1080, 32);
  dharma_session_update_layer_sum(s, 0, 0, dharma_session_get_width(s), dharma_session_get_height(s));
  dharma_session_set_filename(s, "1920x1080");

  // s = dharma_session_new(1280, 720, 24);
  // dharma_session_set_filename(s, "1280x720");
  // dharma_session_update_layer_sum(s, 0, 0, dharma_session_get_width(s), dharma_session_get_height(s));

  // s = dharma_session_new(500, 500, 24);
  // im = dharma_session_get_layer(s, 0);
  // dharma_image_fill_canvas_random(im);
  // dharma_session_update_layer_sum(s, 0, 0, dharma_session_get_width(s), dharma_session_get_height(s));
  // dharma_session_set_filename(s, "Random");

  // s = dharma_session_new(420, 69, 24);
  // im = dharma_session_get_layer(s, 0);
  // dharma_image_fill_canvas_sequential(im);
  // dharma_session_update_layer_sum(s, 0, 0, dharma_session_get_width(s), dharma_session_get_height(s));
  // dharma_session_set_filename(s, "Sequential");

  // s = dharma_session_new(438, 793, 32);
  // dharma_session_update_layer_sum(s, 0, 0, dharma_session_get_width(s), dharma_session_get_height(s));
  // dharma_session_set_filename(s, "438x793");

  // dharma_sessions_destroy_all();

  dharma_sessions_print_all();

  GtkWidget *window_root = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window_root), PROGRAMNAME);
  gtk_widget_set_name(window_root, DHARMA_MAINWINDOW_WIDGET_NAME);
  g_signal_connect(window_root, "destroy", G_CALLBACK(gtk_main_quit), (gpointer) window_root);
  gtk_window_set_position(GTK_WINDOW(window_root), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window_root), 0);
  gtk_window_set_default_size(GTK_WINDOW(window_root), 1200, 800);
  draw_main_window(window_root, NULL);
  gtk_widget_show_all(window_root);

  GtkWidget *window_layers = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window_layers), DHARMA_LAYERS_WINDOW_NAME);
  gtk_widget_set_name(window_layers, DHARMA_LAYERS_WINDOW_WIDGET_NAME);
  g_signal_connect(window_layers, "destroy", G_CALLBACK(gui_templates_destroy), (gpointer) window_layers);
  gtk_window_set_position(GTK_WINDOW(window_layers), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window_layers), 5);
  gtk_window_set_default_size(GTK_WINDOW(window_layers), 280, 300);
  gtk_container_add(GTK_CONTAINER(window_layers), gui_templates_get_layers_window_box(dharma_session_get_session_from_id(0)));
  gui_templates_set_window_layers(window_layers);
  gtk_widget_show_all(window_layers);

  gtk_main();
  return 0;
}
