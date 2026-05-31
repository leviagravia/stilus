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

#ifndef AIRPAD_FILE_H
#define AIRPAD_FILE_H
#include <gtk/gtk.h>
#include "const.h"

void airpad_file_text_buffer_modified(GtkTextBuffer *text_buffer, const struct AirpadDataWindow *data_window);
void airpad_file_init(const struct AirpadDataWindow *data_window);
void airpad_file_new(GtkWidget *widget, const struct AirpadDataApplication *data_application);
void airpad_file_open(GtkWidget *widget, const struct AirpadDataApplication *data_application);
void airpad_file_save(GtkWidget *widget, const struct AirpadDataApplication *data_application);
void airpad_file_save_as(GtkWidget *widget, const struct AirpadDataApplication *data_application);
void airpad_file_close(GtkWidget *widget, const struct AirpadDataApplication *data_application);

#endif
