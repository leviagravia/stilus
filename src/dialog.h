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

#ifndef AIRPAD_DIALOG_H
#define AIRPAD_DIALOG_H
#include <gtk/gtk.h>
#include "const.h"

int airpad_dialog_error(GtkWidget *parent, enum AirpadErrorType error_type, gboolean warning, ...);
int airpad_dialog_confirm(GtkWidget *parent);
void airpad_dialog_no_matches_found(GtkWidget *parent);
GFile *airpad_dialog_open(GtkWidget *parent, const char *default_encoding, struct AirpadDataFile *data_file);
GFile *airpad_dialog_save(GtkWidget *parent, const char *default_encoding, struct AirpadDataFile *data_file);
void airpad_dialog_about(GtkWidget *widget, GtkWidget *parent);
void airpad_dialog_writing_principles(GtkWidget *widget, GtkWidget *parent);
void airpad_dialog_keyboard_shortcuts(GtkWidget *widget, GtkWidget *parent);
void airpad_dialog_writing_workflow(GtkWidget *widget, GtkWidget *parent);

#endif
