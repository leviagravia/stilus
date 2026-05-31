/*
 * Copyright (C) 2020-2022 Nikola Hadžić
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

#ifndef AIRPAD_AUX_H
#define AIRPAD_AUX_H
#include <gtk/gtk.h>
#include "const.h"

void airpad_aux_position_cursor_at_open(GtkTextBuffer *text_buffer);
gboolean airpad_aux_save_prompt(const struct AirpadDataWindow *data_window, struct AirpadDataFile *data_file, const char *default_encoding, gboolean append_newline);

#endif
