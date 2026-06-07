/*
 * Copyright (C) 2019-2020 Nikola Hadžić
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

#ifndef AIRPAD_CLIPBOARD_H
#define AIRPAD_CLIPBOARD_H
#include <gtk/gtk.h>
#include "const.h"

void airpad_edit_has_selection(GObject *object, GParamSpec *pspec, const struct AirpadDataWindow *data_window);
void airpad_edit_paste_done(GtkTextBuffer *text_buffer, GtkClipboard *clipboard, GtkTextView *text_view);
void airpad_edit_init(const struct AirpadDataWindow *data_window);
void airpad_edit_copy(GtkWidget *widget, const struct AirpadDataWindow *data_window);
void airpad_edit_paste(GtkWidget *widget, GtkTextBuffer *text_buffer);
void airpad_edit_cut(GtkWidget *widget, const struct AirpadDataWindow *data_window);
void airpad_edit_delete(GtkWidget *widget, GtkTextBuffer *text_buffer);
void airpad_edit_select_all(GtkWidget *widget, GtkTextBuffer *text_buffer);
void airpad_edit_insert_date_time(GtkWidget *widget, GtkTextBuffer *text_buffer);

#endif
