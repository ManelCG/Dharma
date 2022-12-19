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

GdkPixbuf *gui_templates_pixbuf_from_session(D_Session *s, uint32_t da_width, uint32_t da_height, float *newcx, float *newcy){
  GdkPixbuf *pixbuf = NULL;
  GdkPixbuf *pixbuf_scaled = NULL;
  GdkPixbuf *pixbuf_cropped = NULL;

  D_Image *im = dharma_session_get_layer(s, 0);
  uint8_t *data = dharma_image_get_data(im);
  uint32_t width, height, bpp, stride;
  float cropw, croph, scalew, scaleh;
  int32_t cropx, cropy;
  uint32_t centerx, centery;
  float scale = dharma_session_get_scale(s);

  dharma_session_get_center(s, &centerx, &centery);

  width = dharma_image_get_width(im);
  height = dharma_image_get_height(im);
  bpp = dharma_image_get_bpp(im);
  stride = width * bpp / 8;

  uint32_t postscale_width = width * scale;
  uint32_t postscale_height = height * scale;

  if (postscale_width <= 0 || postscale_height <= 0){
    return NULL;
  }

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

  cropw = (postscale_width > da_width?   (float) da_width/scale  : (float) postscale_width/scale);
  croph = (postscale_height > da_height? (float) da_height/scale : (float) postscale_height/scale);

  if (cropw <= 1){
    cropw = 1;
    scale = da_width/cropw;
  }
  if (croph <= 1){
    croph = 1;
    scale = da_height/croph;
  }


  cropx = (postscale_width < da_width? 0 : centerx - cropw/2);
  cropy = (postscale_height < da_height? 0 : centery - croph/2);

  if (cropx < 0){
    cropx = 0;
  }
  if (cropy < 0){
    cropy = 0;
  }
  if (cropx + cropw > width){
    cropx = width - cropw;
  }
  if (cropy + croph > height){
    cropy = height - croph;
  }

  pixbuf_cropped = gdk_pixbuf_new_subpixbuf(pixbuf, cropx, cropy, cropw, croph);

  scalew = cropw * scale;
  scaleh = croph * scale;

  if (cropw == 1 && croph == 1){
    pixbuf_scaled = gdk_pixbuf_scale_simple(pixbuf_cropped, da_width, da_height, GDK_INTERP_NEAREST);
  } else {
    if (scale >= 1){
      pixbuf_scaled = gdk_pixbuf_scale_simple(pixbuf_cropped, scalew, scaleh, GDK_INTERP_NEAREST);
    } else {
      pixbuf_scaled = gdk_pixbuf_scale_simple(pixbuf_cropped, scalew, scaleh, GDK_INTERP_BILINEAR);
    }
  }

  *newcx = (((float) centerx) * ((float) scalew / (float) width));
  *newcy = (((float) centery) * ((float) scaleh / (float) height));

  g_object_unref(pixbuf);
  g_object_unref(pixbuf_cropped);
  return pixbuf_scaled;
}

gboolean on_window_draw(GtkWidget *da, GdkEvent *event, gpointer data){
  (void) event;
  D_Session *session = (D_Session *) data;

  float im_centerx, im_centery;
  uint32_t da_centerx, da_centery;
  uint32_t pixbuf_h, pixbuf_w;

  uint32_t draww, drawh;
  float drawx, drawy;

  GtkAllocation *alloc = g_new(GtkAllocation, 1);
  gtk_widget_get_allocation(da, alloc);
  da_centerx = alloc->width/2;
  da_centery = alloc->height/2;

  cairo_t *cr;
  cr = gdk_cairo_create(gtk_widget_get_window(da));


  GdkPixbuf *pixbuf;
  pixbuf = gui_templates_pixbuf_from_session(session, alloc->width, alloc->height, &im_centerx, &im_centery);

  if (pixbuf == NULL){
    return false;
  }

  pixbuf_w = gdk_pixbuf_get_width(pixbuf);
  pixbuf_h = gdk_pixbuf_get_height(pixbuf);

  draww = pixbuf_w > (uint32_t) alloc->width?  (uint32_t) alloc->width : pixbuf_w;
  drawh = pixbuf_h > (uint32_t) alloc->height? (uint32_t) alloc->height : pixbuf_h;

  drawx = pixbuf_w > (uint32_t) alloc->width? 0 : (float) da_centerx - (float) draww/2;
  drawy = pixbuf_w > (uint32_t) alloc->width? 0 : (float) da_centery - (float) drawh/2;

  drawx += ((float) pixbuf_w/2 - (float) im_centerx);
  drawy += ((float) pixbuf_h/2 - (float) im_centery);

  gdk_cairo_set_source_pixbuf(cr, pixbuf, drawx, drawy);
  cairo_rectangle(cr, drawx, drawy, draww, drawh + drawh);
  cairo_fill(cr);

  cairo_destroy(cr);
  g_free(alloc);
  g_object_unref(pixbuf);

  return true;
}

GtkWidget *gui_templates_get_canvas_from_session(D_Session *s){
  GtkWidget *da;

  da = gtk_drawing_area_new();
  g_signal_connect(da, "draw", G_CALLBACK(on_window_draw), (gpointer) s);

  return da;
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
  (void) widget;
  D_Session *s = (D_Session *) data;
  GtkWidget *box = dharma_session_get_gtk_box(s);
  float curr_scale = dharma_session_get_scale(s);
  dharma_session_set_scale(s, curr_scale / 1.25);
  gui_templates_clear_container(box);
  gui_templates_pack_box_with_session(s, box);
  gtk_widget_show_all(box);
}
void gui_templates_zoomin_button_handler(GtkWidget *widget, gpointer data){
  (void) widget;
  D_Session *s = (D_Session *) data;
  GtkWidget *box = dharma_session_get_gtk_box(s);
  float curr_scale = dharma_session_get_scale(s);
  dharma_session_set_scale(s, curr_scale * 1.25);
  gui_templates_clear_container(box);
  gui_templates_pack_box_with_session(s, box);
  gtk_widget_show_all(box);
}
