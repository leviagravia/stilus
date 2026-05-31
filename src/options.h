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

#ifndef AIRPAD_OPTIONS_H
#define AIRPAD_OPTIONS_H
#include <gtk/gtk.h>
#include "const.h"

void airpad_options_update_append_newline(GtkWidget *widget, gboolean *option);
void airpad_options_update_overlay_scrolling(GtkWidget *widget, const struct AirpadDataApplication *data_application);
void airpad_options_update_text_wrap(GtkWidget *widget, const struct AirpadDataApplication *data_application);
void airpad_options_update_scrollbar_policy(GtkWidget *widget, const struct AirpadDataApplication *data_application);
void airpad_options_open();
void airpad_options_close();
void airpad_options_read(struct AirpadDataOptions *options);
void airpad_options_write(const struct AirpadDataOptions *options);

#endif
