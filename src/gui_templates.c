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

#include <gui_templates.h>

#include <dharma_session.h>

/***************************
 *
 * GUI ELEMENTS CONSTRUCTORS
 *
 ***************************/

GtkWidget *gui_templates_get_mainscreen_menubar(){
  GtkWidget *menu_menubar;

  //File menu
  GtkWidget *menu_filemenu;
  GtkWidget *menu_fileMi;

  //Edit menu
  GtkWidget *menu_editmenu;
  GtkWidget *menu_editMi;

  //Help menu
  GtkWidget *menu_helpmenu;
  GtkWidget *menu_helpMi;

  //Menu bar
  menu_menubar = gtk_menu_bar_new();
  menu_filemenu = gtk_menu_new();
  menu_editmenu = gtk_menu_new();
  menu_helpmenu = gtk_menu_new();

  //File submenu
  menu_fileMi = gtk_menu_item_new_with_label("File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_menubar), menu_fileMi);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_fileMi), menu_filemenu);

  //Edit submenu
  menu_editMi = gtk_menu_item_new_with_label("Edit");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_menubar), menu_editMi);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_editMi), menu_editmenu);

  //Help submenu
  menu_helpMi = gtk_menu_item_new_with_label("Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_menubar), menu_helpMi);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_helpMi), menu_helpmenu);

  return menu_menubar;
}

GtkWidget *gui_templates_get_welcome_screen_box(){
  GtkWidget *main_vbox;

  GtkWidget *label_welcome;

  label_welcome = gtk_label_new("Welcome to Dharma!");

  main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(main_vbox), label_welcome, true, true, 0);

  return main_vbox;
}

GtkWidget *gui_templates_get_viewmode_toolbar(D_Session *s){
  GtkWidget *main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

  char scale[8];
  snprintf(scale, 7, "%.2f", dharma_session_get_scale(s)*100);

  GtkWidget *zoomin_button = gtk_button_new_with_label("+");
  g_signal_connect(zoomin_button, "pressed", G_CALLBACK(gui_templates_zoomin_button_handler), (gpointer) s);
  GtkWidget *zoomout_button = gtk_button_new_with_label("-");
  g_signal_connect(zoomout_button, "pressed", G_CALLBACK(gui_templates_zoomout_button_handler), (gpointer) s);

  GtkWidget *zoom_entry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(zoom_entry), scale);
  gtk_entry_set_width_chars(GTK_ENTRY(zoom_entry), 10);

  gtk_box_pack_end(GTK_BOX(main_hbox), zoom_entry, false, false, 0);
  gtk_box_pack_end(GTK_BOX(main_hbox), zoomin_button, false, false, 0);
  gtk_box_pack_end(GTK_BOX(main_hbox), zoomout_button, false, false, 0);

  return main_hbox;
}

GdkPixbuf *gui_templates_pixbuf_from_session(D_Session *s){
  GdkPixbuf *pixbuf = NULL;
  GdkPixbuf *pixbuf_scaled = NULL;

  D_Image *im = dharma_session_get_layer(s, 0);
  uint8_t *data = dharma_image_get_data(im);
  uint32_t width, height, bpp, stride;
  float scale = dharma_session_get_scale(s);

  width = dharma_image_get_width(im);
  height = dharma_image_get_height(im);
  bpp = dharma_image_get_bpp(im);
  stride = width * bpp / 8;

  switch(bpp){
    case 8:

      break;
    case 24:
      pixbuf = gdk_pixbuf_new_from_data(
        data,
        GDK_COLORSPACE_RGB,
        0,
        8,
        width, height, stride,
        NULL, NULL);
      break;
    case 32:
      pixbuf = gdk_pixbuf_new_from_data(
        data,
        GDK_COLORSPACE_RGB,
        1,
        8,
        width, height, stride,
        NULL, NULL);
      break;
    case 64:

      break;
  }

  pixbuf_scaled = gdk_pixbuf_scale_simple(pixbuf, width * scale, height * scale, GDK_INTERP_NEAREST);
  g_object_unref(pixbuf);
  return pixbuf_scaled;
}

GtkWidget *gui_templates_update_gtk_image(D_Session *s){
  GtkWidget *image = dharma_session_get_gtk_image(s);
  GdkPixbuf *pixbuf;

  // if (image != NULL){
  //   g_free(image);
  // }

  pixbuf = gui_templates_pixbuf_from_session(s);

  image = gtk_image_new_from_pixbuf(pixbuf);
  g_object_unref(pixbuf);
  dharma_session_set_gtk_image(s, image);

  return image;
}

GtkWidget *gui_templates_get_canvas_from_session(D_Session *s){
  GtkWidget *image;
  GtkWidget *scrollbox;

  image = gui_templates_update_gtk_image(s);

  scrollbox = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbox), image);

  return scrollbox;
}

void gui_templates_pack_box_with_session(D_Session *session, GtkWidget *session_vbox){
  GtkWidget *session_viewmode_toolbar;
  GtkWidget *session_canvas;

  session_canvas = gui_templates_get_canvas_from_session(session);
  gtk_box_pack_start(GTK_BOX(session_vbox), session_canvas, true, true, 0);

  {GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
     gtk_box_pack_start(GTK_BOX(session_vbox), separator, false, false, 0);}

  session_viewmode_toolbar = gui_templates_get_viewmode_toolbar(session);
  gtk_box_pack_start(GTK_BOX(session_vbox), session_viewmode_toolbar, false, false, 0);
}

GtkWidget *gui_templates_get_sessions_notebook(){
  uint32_t nsessions;
  uint32_t i;
  D_Session *session;

  GtkWidget *notebook = NULL;

  GtkWidget *session_vbox;
  GtkWidget *session_namelabel;

  nsessions = dharma_session_get_nsessions();

  if (nsessions == 0){
    return gui_templates_get_welcome_screen_box();
  }


  for (i = 0; i < nsessions; i++){
    session = dharma_session_get_session_from_id(i);

    session_namelabel = gtk_label_new(dharma_session_get_filename(session));
    session_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    gui_templates_pack_box_with_session(session, session_vbox);
    dharma_session_set_gtk_box(session, session_vbox);

    if (nsessions == 1){
      return session_vbox;
    }

    if (notebook == NULL){
      notebook = gtk_notebook_new();
    }
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), session_vbox, session_namelabel);
  }

  return notebook;
}

/******************
 *
 * HELPER FUNCTIONS
 *
 ******************/

void gui_templates_clear_container(GtkWidget *container){
  GList *children, *iter;
  children = gtk_container_get_children(GTK_CONTAINER(container));
  for (iter = children; iter != NULL; iter = g_list_next(iter)){
    gtk_widget_destroy(GTK_WIDGET(iter->data));
  }
  g_list_free(children);
}

/**********
 *
 * HANDLERS
 *
 **********/

void gui_templates_zoomout_button_handler(GtkWidget *widget, gpointer data){
  D_Session *s = (D_Session *) data;
  GtkWidget *box = dharma_session_get_gtk_box(s);
  float curr_scale = dharma_session_get_scale(s);
  dharma_session_set_scale(s, curr_scale / 1.25);
  gui_templates_clear_container(box);
  gui_templates_pack_box_with_session(s, box);
  gtk_widget_show_all(box);
}
void gui_templates_zoomin_button_handler(GtkWidget *widget, gpointer data){
  D_Session *s = (D_Session *) data;
  GtkWidget *box = dharma_session_get_gtk_box(s);
  float curr_scale = dharma_session_get_scale(s);
  dharma_session_set_scale(s, curr_scale * 1.25);
  gui_templates_clear_container(box);
  gui_templates_pack_box_with_session(s, box);
  gtk_widget_show_all(box);
}
