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
#include <dharma_defines.h>

/*******************
 *
 * DRAWING FUNCTIONS
 *
 ******************/

void draw_main_window(GtkWidget *window, gpointer data){
  (void) data;
  gui_templates_clear_container(window);

  GtkWidget *main_vbox; //Contains the menubar and the rest of the widgets

  GtkWidget *menu_menubar;
  GtkWidget *notebook_sessions;

  notebook_sessions = gui_templates_get_sessions_notebook();

  main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  menu_menubar = gui_templates_get_mainscreen_menubar();
  gtk_box_pack_start(GTK_BOX(main_vbox), menu_menubar, false, false, 0);

  gtk_box_pack_start(GTK_BOX(main_vbox), notebook_sessions, true, true, 0);

  gtk_container_add(GTK_CONTAINER(window), main_vbox);
  gtk_widget_show_all(window);
}


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

void gui_templates_new_file_window(GtkWidget *w_unused, gpointer data){
  (void) w_unused;
  GtkWidget *window_root = gtk_widget_get_toplevel((GtkWidget *) data);

  GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, (GtkMessageType) GTK_DIALOG_DESTROY_WITH_PARENT, GTK_BUTTONS_OK_CANCEL, "Create new file");
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog), 0);

  GtkWidget *dialog_button_cancel = gtk_dialog_get_widget_for_response(GTK_DIALOG(dialog), GTK_RESPONSE_CANCEL);
  gtk_button_set_label(GTK_BUTTON(dialog_button_cancel), "Cancel");
  { GtkWidget *icon = gtk_image_new_from_icon_name("window-close", GTK_ICON_SIZE_MENU);
    gtk_button_set_image(GTK_BUTTON(dialog_button_cancel), icon); }
  gtk_button_set_always_show_image(GTK_BUTTON(dialog_button_cancel), true);

  GtkWidget *dialog_button_ok = gtk_dialog_get_widget_for_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
  gtk_button_set_label(GTK_BUTTON(dialog_button_ok), "Accept");
  { GtkWidget *icon = gtk_image_new_from_icon_name("emblem-ok-symbolic", GTK_ICON_SIZE_MENU);
    gtk_button_set_image(GTK_BUTTON(dialog_button_ok), icon); }
  gtk_button_set_always_show_image(GTK_BUTTON(dialog_button_ok), true);

  GtkWidget *main_vbox;
  GtkWidget *geometry_vbox;

  GtkWidget *entry_width, *entry_height, *combo_bpp;
  entry_width = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry_width), "Width");

  entry_height = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry_height), "Height");

  combo_bpp = gtk_combo_box_text_new();
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_bpp), "8");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_bpp), "24");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_bpp), "32");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_bpp), "64");

  geometry_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(geometry_vbox), entry_width, true, true, 0);
  gtk_box_pack_start(GTK_BOX(geometry_vbox), entry_height, true, true, 0);
  gtk_box_pack_start(GTK_BOX(geometry_vbox), combo_bpp, true, true, 0);

  main_vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  gtk_box_pack_start(GTK_BOX(main_vbox), geometry_vbox, false, false, 0);

  gtk_widget_show_all(dialog);

  int response = gtk_dialog_run(GTK_DIALOG(dialog));
  if (response == GTK_RESPONSE_OK){
    const char *width = gtk_entry_get_text(GTK_ENTRY(entry_width));
    const char *height = gtk_entry_get_text(GTK_ENTRY(entry_height));
    const char *bpp = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_bpp));

    D_Session *s = dharma_session_new(atoi(width), atoi(height), atoi(bpp));
    dharma_sessions_print_all();

    draw_main_window(window_root, NULL);

    gui_templates_destroy(dialog, dialog);
  } else {
    gui_templates_destroy(dialog, dialog);
    return;
  }
}

GtkWidget *gui_templates_get_welcome_screen_box(){
  GtkWidget *frame;
  GtkWidget *main_vbox;
  GtkWidget *main_hbox;
  GtkWidget *frame_subvbox;

  GtkWidget *label_welcome;

  GtkWidget *button_new_file;
  GtkWidget *button_open_file;

  label_welcome = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(label_welcome), "<span size=\"x-large\">Welcome to Dharma!</span>");

  button_new_file = gtk_button_new_with_label("New file");
  g_signal_connect(button_new_file, "activate", G_CALLBACK(gui_templates_new_file_window), (gpointer) button_new_file);
  g_signal_connect(button_new_file, "pressed", G_CALLBACK(gui_templates_new_file_window), (gpointer) button_new_file);
  button_open_file = gtk_button_new_with_label("Open file");

  frame_subvbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(frame_subvbox), label_welcome, true, true, 0);
  gtk_box_pack_start(GTK_BOX(frame_subvbox), button_new_file, true, true, 0);
  gtk_box_pack_start(GTK_BOX(frame_subvbox), button_open_file, true, true, 0);

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(frame), frame_subvbox);

  main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(main_hbox), frame, true, false, 0);

  main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(main_vbox), main_hbox, true, false, 0);


  return main_vbox;
}

GtkWidget *gui_templates_get_viewmode_toolbar(D_Session *s){
  GtkWidget *main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

  char scale[16];
  snprintf(scale, 15, "%.2f%%", dharma_session_get_scale(s)*100);

  GtkWidget *zoomin_button = gtk_button_new_with_label("+");
  g_signal_connect(zoomin_button, "pressed", G_CALLBACK(gui_templates_zoomin_button_handler), (gpointer) s);
  GtkWidget *zoomout_button = gtk_button_new_with_label("-");
  g_signal_connect(zoomout_button, "pressed", G_CALLBACK(gui_templates_zoomout_button_handler), (gpointer) s);

  GtkWidget *zoom_entry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(zoom_entry), scale);
  gtk_entry_set_width_chars(GTK_ENTRY(zoom_entry), 16);

  gtk_box_pack_end(GTK_BOX(main_hbox), zoom_entry, false, false, 0);
  gtk_box_pack_end(GTK_BOX(main_hbox), zoomin_button, false, false, 0);
  gtk_box_pack_end(GTK_BOX(main_hbox), zoomout_button, false, false, 0);

  return main_hbox;
}

GdkPixbuf *gui_templates_pixbuf_from_session(D_Session *s, uint32_t da_width, uint32_t da_height, float *newcx, float *newcy){
  GdkPixbuf *pixbuf = NULL;
  GdkPixbuf *pixbuf_scaled = NULL;
  GdkPixbuf *pixbuf_cropped = NULL;


  //STEP 1: GENERATE FULL PIXBUF
  D_Image *im = dharma_session_get_layer(s, 0);
  uint8_t *data = dharma_image_get_data(im);

  float scale;
  uint32_t width, height, bpp, stride;
  uint32_t centerx, centery;
  width = dharma_image_get_width(im);
  height = dharma_image_get_height(im);
  bpp = dharma_image_get_bpp(im);
  stride = width * bpp / 8;
  scale = dharma_session_get_scale(s);
  dharma_session_get_center(s, &centerx, &centery);

  //Generate pixbuf
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


  //STEP 2: CROP PIXBUF BEFORE SCALING

  float cropw, croph;
  float cropx, cropy;

  GtkAdjustment *vadj, *hadj;

  //I know my DA's size and my image's size.
  cropw = MIN(width,  ((float) da_width  / scale) +2);
  croph = MIN(height, ((float) da_height / scale) +2);

  //Information for drawing scrollbars
  dharma_session_set_spanx(s, cropw);
  dharma_session_set_spany(s, croph);
  hadj = (GtkAdjustment *) dharma_session_get_hadj(s);
  gtk_adjustment_set_page_size(hadj, cropw);
  gtk_adjustment_set_value(hadj, MIN(MAX(centerx - cropw/2, 0), width));
  vadj = (GtkAdjustment *) dharma_session_get_vadj(s);
  gtk_adjustment_set_page_size(vadj, croph);
  gtk_adjustment_set_value(vadj, MIN(MAX(centery - croph/2, 0), height));

  //Start cropping image leaving center in the middle
  cropx = (float) centerx - cropw/2;
  cropy = (float) centery - croph/2;

  //Keep cropx bounded by 0..width-cropw
  cropx = MAX(cropx, 0);
  cropy = MAX(cropy, 0);
  cropx = MIN(cropx, (float) width - cropw);
  cropy = MIN(cropy, (float) height - croph);

  pixbuf_cropped = gdk_pixbuf_new_subpixbuf(pixbuf, cropx, cropy, cropw, croph);

  //STEP 3: SCALE PIXBUF
  //Sizes after scaling and cropping
  float scalew, scaleh;
  scalew = cropw * scale;
  scaleh = croph * scale;

  //Zoom is too little, can't draw less than 1 pixel in any dimension
  if (scalew < 1 || scaleh < 1){
    g_object_unref(pixbuf);
    g_object_unref(pixbuf_cropped);
    return NULL;
  }

  if (scale >= 1){
    pixbuf_scaled = gdk_pixbuf_scale_simple(pixbuf_cropped, scalew, scaleh, GDK_INTERP_NEAREST);
  } else {
    pixbuf_scaled = gdk_pixbuf_scale_simple(pixbuf_cropped, scalew, scaleh, GDK_INTERP_BILINEAR);
  }

  *newcx = ((float) centerx - cropx) * scale;
  *newcy = ((float) centery - cropy) * scale;

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

  draww = pixbuf_w;
  drawh = pixbuf_h;

  drawx = (float) da_centerx - im_centerx;
  drawy = (float) da_centery - im_centery;

  // drawx += ((float) pixbuf_w/2 - (float) im_centerx);
  // drawy += ((float) pixbuf_h/2 - (float) im_centery);

  gdk_cairo_set_source_pixbuf(cr, pixbuf, drawx, drawy);
  cairo_rectangle(cr, drawx, drawy, draww, drawh);
  cairo_fill(cr);

  cairo_destroy(cr);
  g_free(alloc);
  g_object_unref(pixbuf);

  return true;
}

GtkWidget *gui_templates_get_canvas_from_session(D_Session *s){
  GtkWidget *da;
  GtkWidget *event_box;

  da = gtk_drawing_area_new();
  g_signal_connect(da, "draw", G_CALLBACK(on_window_draw), (gpointer) s);

  event_box = gtk_event_box_new();
  g_signal_connect(G_OBJECT(event_box), "motion-notify-event",  G_CALLBACK(canvas_mouse_handler), (gpointer) s);
  g_signal_connect(G_OBJECT(event_box), "button-press-event",   G_CALLBACK(canvas_mouse_handler), (gpointer) s);
  g_signal_connect(G_OBJECT(event_box), "button-release-event", G_CALLBACK(canvas_mouse_handler), (gpointer) s);
  gtk_container_add(GTK_CONTAINER(event_box), da);

  return event_box;
}

void gui_templates_pack_box_with_session(D_Session *session, GtkWidget *session_vbox){
  GtkWidget *session_viewmode_toolbar;
  GtkWidget *session_canvas;

  GtkWidget *hbox;

  GtkWidget *vertical_scrollbar;
  GtkWidget *horizontal_scrollbar;

  GtkAdjustment *vertical_adjustment;
  GtkAdjustment *horizontal_adjustment;

  hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  uint32_t w, h, cx, cy;
  float spanx, spany;

  w = dharma_session_get_width(session);
  h = dharma_session_get_height(session);
  dharma_session_get_center(session, &cx, &cy);
  spanx = dharma_session_get_spanx(session);
  spany = dharma_session_get_spany(session);

  session_canvas = gui_templates_get_canvas_from_session(session);
  gtk_box_pack_start(GTK_BOX(hbox), session_canvas, true, true, 0);

  vertical_adjustment = gtk_adjustment_new(cy, 0, h, 1, h, spany);
  vertical_scrollbar = gtk_scrollbar_new(GTK_ORIENTATION_VERTICAL, vertical_adjustment);
  gtk_box_pack_start(GTK_BOX(hbox), vertical_scrollbar, false, false, 0);

  gtk_box_pack_start(GTK_BOX(session_vbox), hbox, true, true, 0);

  horizontal_adjustment = gtk_adjustment_new(cx, 0, w, 1, w, spanx);
  horizontal_scrollbar = gtk_scrollbar_new(GTK_ORIENTATION_HORIZONTAL, horizontal_adjustment);
  gtk_box_pack_start(GTK_BOX(session_vbox), horizontal_scrollbar, false, false, 0);

  dharma_session_set_vadj(session, vertical_adjustment);
  dharma_session_set_hadj(session, horizontal_adjustment);

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

void gui_templates_destroy(GtkWidget *w, gpointer data){
  (void) w;
  GtkWidget *window = (GtkWidget *) data;
  gui_templates_clear_container(window);
  gtk_widget_destroy(window);
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
void canvas_mouse_handler(GtkWidget *event_box, GdkEventButton *event, gpointer data){
  double evb_height, evb_width;
  D_Session *s = (D_Session *) data;
  (void) event_box;

  evb_height = gtk_widget_get_allocated_height(event_box);
  evb_width = gtk_widget_get_allocated_height(event_box);

  (void) evb_height;
  (void) evb_width;
  (void) s;

  printf("%f, %f\n", event->x, event->y);
}
