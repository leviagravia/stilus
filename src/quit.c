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

#include <gtk/gtk.h>
#include "const.h"
#include "aux.h"

// Quits the program when "delete-event" is sent.
// Dialog that asks the user would he like to save the changes is presented to him
// if the file was modified. If the user confirms, a save dialog is also presented
// in case that the current file is not associated with any file on the filesystem,
// otherwise the file is just silently written down.
gboolean airpad_quit(GtkWidget *widget, GdkEvent *event, const struct AirpadDataApplication *data_application)
{
    if (airpad_aux_save_prompt(data_application->data_window, data_application->data_file, data_application->data_arguments->file_encoding))
        g_application_quit(G_APPLICATION(data_application->application));

    return TRUE;
}
