/*
 * Copyright (C) 2018-2020 Nikola Hadžić
 *
 * This file is part of Airpad.
 *
 * Airpad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Airpad is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Airpad.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef AIRPAD_WINDOW_H
#define AIRPAD_WINDOW_H
#include <gtk/gtk.h>
#include "const.h"

gboolean airpad_window_window_state(GtkWidget *widget, GdkEventWindowState *event, const struct AirpadDataWindow *data_window);
void airpad_window_focus_in(GtkWidget *widget, GdkEvent *event, const struct AirpadDataWindow *data_window);
void airpad_window_copy_clipboard(GtkWidget *widget, const struct AirpadDataWindow *data_window);
void airpad_window_cut_clipboard(GtkWidget *widget, const struct AirpadDataWindow *data_window);
void airpad_window_create(struct AirpadDataWindow *data_window, GtkApplication *application, const struct AirpadDataOptions *data_options);
void airpad_window_set_title(const struct AirpadDataWindow *data_window, GFile *current_file);
void airpad_window_set_title_modified(const struct AirpadDataWindow *data_window, gboolean modified);
void airpad_window_update_status_bar(GtkTextBuffer *text_buffer, const struct AirpadDataWindow *data_window);
void airpad_window_text_buffer_mark_set(GtkTextBuffer *text_buffer, GtkTextIter *location, GtkTextMark *mark, const struct AirpadDataWindow *data_window);
void airpad_window_set_font(const struct AirpadDataWindow *data_window, const char *font);
void airpad_window_set_text_wrap_mode(const struct AirpadDataWindow *data_window, GtkWrapMode wrap_mode);
void airpad_window_set_overlay_scrolling(const struct AirpadDataWindow *data_window, gboolean state);
void airpad_window_set_scrollbar_policy(const struct AirpadDataWindow *data_window, GtkPolicyType policy);

#endif
