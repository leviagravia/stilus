/*
 * Copyright (C) 2020 Nikola Hadžić
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

#ifndef AIRPAD_UNDO_H
#define AIRPAD_UNDO_H
#include <gtk/gtk.h>
#include "const.h"

void airpad_undo_mark_unit_next();
void airpad_undo_mark_unit_prev();
void airpad_undo_save(GtkTextBuffer *text_buffer);
void airpad_undo_flush();
void airpad_undo_init(const struct AirpadDataWindow *data_window);
void airpad_undo_reset();
void airpad_undo_add_on_insert(GtkTextBuffer *text_buffer, GtkTextIter *position, char *new_text, int new_text_length, const struct AirpadDataMenuBar *data_menu_bar);
void airpad_undo_add_on_delete(GtkTextBuffer *text_buffer, GtkTextIter *start, GtkTextIter *end, const struct AirpadDataMenuBar *data_menu_bar);
void airpad_undo_action_undo(GtkWidget *widget, const struct AirpadDataApplication *data_application);
void airpad_undo_action_redo(GtkWidget *widget, const struct AirpadDataApplication *data_application);
void airpad_undo_full_start(GtkTextBuffer *text_buffer, const struct AirpadDataSignals *data_signals);
void airpad_undo_full_end(GtkTextBuffer *text_buffer, const struct AirpadDataSignals *data_signals, const struct AirpadDataWindow *data_window);

#endif
