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

//Gui elements constructors
GtkWidget *gui_templates_get_mainscreen_menubar();
GtkWidget *gui_templates_get_sessions_notebook();
GtkWidget *gui_templates_get_welcome_screen_box();
GtkWidget *gui_templates_get_canvas_from_session(D_Session *s);
GtkWidget *gui_templates_get_viewmode_toolbar(D_Session *s);

GdkPixbuf *gui_templates_pixbuf_from_session(D_Session *s, uint32_t daw, uint32_t dah, float *newcx, float *newcy);
GtkWidget *gui_templates_update_gtkimage(D_Session *s);

//Helper functions
void gui_templates_clear_container(GtkWidget *container);

//Handlers
void gui_templates_zoomin_button_handler(GtkWidget *w, gpointer d);
void gui_templates_zoomout_button_handler(GtkWidget *w, gpointer d);

#endif //__GUI_TEMPLATES_H_
