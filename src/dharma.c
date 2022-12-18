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

#include <dharma_image.h>
#include <dharma_defines.h>
#include <dharma_math.h>
#include <dharma_color.h>
#include <dharma_session.h>

void draw_main_window(GtkWidget *window, gpointer data){
  gui_templates_clear_container(window);

  GtkWidget *main_vbox; //Contains the menubar and the rest of the widgets

  GtkWidget *menu_menubar;

  main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  menu_menubar = gui_templates_get_mainscreen_menubar();
  gtk_box_pack_start(GTK_BOX(main_vbox), menu_menubar, false, false, 0);

  gtk_container_add(GTK_CONTAINER(window), main_vbox);
  gtk_widget_show_all(window);
}

int main(int argc, char *argv[]){
  gtk_init(&argc, &argv);

  D_Session *s = dharma_session_new(4, 16, 64);
  D_Image *im = dharma_image_new_empty(4, 16, 64);
  printf("%s\n", dharma_session_add_layer(s) == true? "Success" : "Failure");
  printf("%s\n", dharma_session_add_layer_from_image(s, im) == true? "Success" : "Failure");

  dharma_session_set_filename(s, "test.png");

  dharma_session_remove_layer(s, 0);
  dharma_session_remove_layer(s, 0);
  dharma_session_remove_layer(s, 0);

  printf("%s\n", dharma_session_add_layer(s) == true? "Success" : "Failure");
  printf("%s\n", dharma_session_add_layer(s) == true? "Success" : "Failure");
  printf("%s\n", dharma_session_add_layer(s) == true? "Success" : "Failure");

  dharma_session_new(1920, 1080, 32);
  dharma_session_new(1280, 720, 32);
  dharma_session_new(1, 2, 32);
  dharma_session_new(420, 69, 32);
  dharma_session_new(438, 793, 32);

  dharma_sessions_print_all();

  dharma_sessions_destroy_all();

  dharma_sessions_print_all();

  GtkWidget *window_root = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window_root), PROGRAMNAME);
  gtk_widget_set_name(window_root, DHARMA_MAINWINDOW_WIDGET_NAME);
  g_signal_connect(window_root, "destroy", G_CALLBACK(gtk_main_quit), (gpointer) window_root);
  gtk_window_set_position(GTK_WINDOW(window_root), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window_root), 0);
  gtk_window_set_default_size(GTK_WINDOW(window_root), 1200, 800);

  draw_main_window(window_root, NULL);
  gtk_main();
  return 0;
}
