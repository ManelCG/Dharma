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

#include <dharma_image.h>
#include <dharma_defines.h>
#include <dharma_math.h>
#include <dharma_color.h>

int main(int argc, char *argv[]){
  gtk_init(&argc, &argv);

  GtkWidget *window_root = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window_root), PROGRAMNAME);
  gtk_widget_set_name(window_root, DHARMA_MAINWINDOW_WIDGET_NAME);
  g_signal_connect(window_root, "destroy", G_CALLBACK(gtk_main_quit), (gpointer) window_root);
  gtk_window_set_position(GTK_WINDOW(window_root), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window_root), 0);
  gtk_window_set_default_size(GTK_WINDOW(window_root), 1200, 800);

  gtk_widget_show_all(window_root);
  gtk_main();
  return 0;
}
