/*
 * Copyright (C) 2018-2022 Nikola Hadžić
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

#include <gtk/gtk.h>
#include "const.h"

// Initializes the state of the search elements.
void airpad_search_init(const struct AirpadDataWindow *data_window)
{
    gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_search_find_next, FALSE);
    gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_search_find_prev, FALSE);
}
