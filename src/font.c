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

#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include "const.h"
#include "window.h"

// Displays a font chooser dialog to the user.
// Default values are supplied from the current option values,
// and option values are updated if the user selects a font.
void airpad_font_choose_font(GtkWidget *widget, const struct AirpadDataApplication *data_application)
{
    GtkWidget *dialog = gtk_font_chooser_dialog_new(_("Select font"), GTK_WINDOW(data_application->data_window->window));

    if (data_application->data_options->font)
        gtk_font_chooser_set_font(GTK_FONT_CHOOSER(dialog), data_application->data_options->font);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
    {
        g_free(data_application->data_options->font);
        data_application->data_options->font = gtk_font_chooser_get_font(GTK_FONT_CHOOSER(dialog));

        airpad_window_set_font(data_application->data_window, data_application->data_options->font);
    }

    gtk_widget_destroy(dialog);
}
