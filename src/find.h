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

#ifndef AIRPAD_IN_SEARCH_H
#error "This file cannot be included by itself, include search.h"
#endif

#ifndef AIRPAD_FIND_H
#define AIRPAD_FIND_H
#include <gtk/gtk.h>
#include "const.h"

void airpad_find_find(GtkWidget *widget, const struct AirpadDataApplication *data_application);
void airpad_find_find_next(GtkWidget *widget, const struct AirpadDataApplication *data_application);
void airpad_find_find_prev(GtkWidget *widget, const struct AirpadDataApplication *data_application);

#endif
