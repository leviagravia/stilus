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

#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <stdarg.h>
#include "const.h"

// Displays the error message to the user.
// User response is returned.
int airpad_dialog_error(GtkWidget *parent, enum AirpadErrorType error_type, gboolean warning, ...)
{
    // Create a dialog and set its message according to the passed error type.
    GtkWidget *dialog;

    va_list args;
    va_start(args, warning);

    switch (error_type)
    {
        case AIRPAD_ERROR_TYPE_FILE_READ_FAILURE:
        {
            const char *filename = va_arg(args, char *);
            const char *detail = va_arg(args, char *);

            if (filename)
                dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("Reading from file \"%s\" failed: %s"), filename, detail);
            else
                dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("Reading from file (name unknown) failed: %s"), detail);
        }
        break;
        case AIRPAD_ERROR_TYPE_FILE_WRITE_FAILURE:
        {
            const char *filename = va_arg(args, char *);
            const char *detail = va_arg(args, char *);

            if (filename)
                dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("Writing to file \"%s\" failed: %s"), filename, detail);
            else
                dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("Writing to file (name unknown) failed: %s"), detail);
        }
        break;
        case AIRPAD_ERROR_TYPE_TEXT_INVALID_UTF8:
        {
            const char *filename = va_arg(args, char *);

            if (filename)
                dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("File \"%s\" does not have valid UTF-8 content."), filename);
            else
                dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("File (name unknown) does not have valid UTF-8 content."));
        }
        break;
        case AIRPAD_ERROR_TYPE_TEXT_CONVERSION_ERROR:
        {
            const char *filename = va_arg(args, char *);
            const char *detail = va_arg(args, char *);

            if (filename)
                dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("Converting contents of file \"%s\" failed: %s"), filename, detail);
            else
                dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("Converting contents of file (name unknown) failed: %s"), detail);
        }
        break;
    }

    va_end(args);

    // Configure the dialog.
    gtk_window_set_title(GTK_WINDOW(dialog), warning ? _("Warning") : _("Error"));

    // Add an image to the dialog.
    GtkWidget *dialog_image = gtk_image_new_from_icon_name(warning ? "dialog-warning" : "dialog-error", GTK_ICON_SIZE_DIALOG);
    gtk_message_dialog_set_image(GTK_MESSAGE_DIALOG(dialog), dialog_image);
    gtk_widget_show(dialog_image);

    // Run the dialog.
    const int response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    // Return the user response.
    return response;
}

// Informs the user that the search operation has found no matches.
void airpad_dialog_no_matches_found(GtkWidget *parent)
{
    // Create the dialog.
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, _("No matches found."));

    // Set its properties.
    gtk_window_set_title(GTK_WINDOW(dialog), _("No matches"));

    // Run it.
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Displays a question dialog asking the user whether would he like to
// save the changes that he has made to the file.
// Returns the user response.
int airpad_dialog_confirm(GtkWidget *parent)
{
    // Create a dialog.
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_NONE, _("The file was modified. Do you want to save it?"));
    gtk_window_set_title(GTK_WINDOW(dialog), _("Save changes?"));
    gtk_dialog_add_buttons(GTK_DIALOG(dialog), _("Yes"), GTK_RESPONSE_YES, _("No"), GTK_RESPONSE_NO, _("Cancel"), GTK_RESPONSE_CANCEL, NULL);
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_YES);

    // Run the dialog and get the user response.
    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    // Return the user response.
    return response;
}

// Displays a dialog that allows the user to select which file to open.
// `data_file` will be updated appropriately if the user completed the
// operation: `file` will be set to the selected file and `encoding` to
// the chosen encoding.
// Returns TRUE if the user completed the operation and FALSE otherwise.
gboolean airpad_dialog_open(GtkWidget *parent, const char *default_encoding, struct AirpadDataFile *data_file)
{
    // Create a file chooser dialog.
    GtkWidget *dialog = gtk_file_chooser_dialog_new(_("Open file"), GTK_WINDOW(parent), GTK_FILE_CHOOSER_ACTION_OPEN, _("Cancel"), GTK_RESPONSE_CANCEL, _("Open"), GTK_RESPONSE_ACCEPT, NULL);

    /* FILTERS */

    // Text filter.
    GtkFileFilter *filter_text = gtk_file_filter_new();
    gtk_file_filter_add_mime_type(filter_text, "text/*");
    gtk_file_filter_set_name(filter_text, _("Text files"));

    // Any filter.
    GtkFileFilter *filter_any = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter_any, "*");
    gtk_file_filter_set_name(filter_any, _("All files"));

    // Add filters to the file chooser.
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_text);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_any);

    /* ENCODING OPTIONS */

    // Create the encoding widgets.
    GtkWidget *encoding_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *encoding_label = gtk_label_new(_("Encoding:"));
    GtkWidget *encoding_entry = gtk_entry_new();

    // Configure the entry.
    gtk_entry_set_input_hints(GTK_ENTRY(encoding_entry), GTK_INPUT_HINT_NO_EMOJI);

    // Compose the encoding widgets.
    gtk_box_pack_start(GTK_BOX(encoding_box), encoding_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(encoding_box), encoding_entry, FALSE, FALSE, 0);

    // Add the encoding widget to the file chooser.
    gtk_file_chooser_set_extra_widget(GTK_FILE_CHOOSER(dialog), encoding_box);

    // Display the encoding widget.
    gtk_widget_show_all(encoding_box);

    // Set default encoding.
    gtk_entry_set_text(GTK_ENTRY(encoding_entry), default_encoding);

    /* ACTIVATE */

    // Run the dialog and get the user response.
    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_DELETE_EVENT || response == GTK_RESPONSE_CANCEL)   // Operation cancelled.
    {
        gtk_widget_destroy(dialog);
        return FALSE;
    }

    // Save reference to the newly chosen file. 
    data_file->file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog));
    data_file->encoding = g_strdup(gtk_entry_get_text(GTK_ENTRY(encoding_entry)));

    // Destroy the dialog.
    gtk_widget_destroy(dialog);

    return TRUE;
}

// Displays a save dialog to the user that allows creating folders.
// Initial file chooser option values will be supplied from `data_file`,
// if available. `data_file` will be updated appropriately if the user
// completed the operation: `file` will be set to the selected file and
// `encoding` to the chosen encoding.
// Returns TRUE if the user completed the operation and FALSE otherwise.
gboolean airpad_dialog_save(GtkWidget *parent, const char *default_encoding, struct AirpadDataFile *data_file)
{
    // Create a file chooser dialog.
    GtkWidget *dialog = gtk_file_chooser_dialog_new(_("Save file"), GTK_WINDOW(parent), GTK_FILE_CHOOSER_ACTION_SAVE, _("Cancel"), GTK_RESPONSE_CANCEL, _("Save"), GTK_RESPONSE_ACCEPT, NULL);
    gtk_file_chooser_set_show_hidden(GTK_FILE_CHOOSER(dialog), FALSE);
    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
    gtk_file_chooser_set_create_folders(GTK_FILE_CHOOSER(dialog), TRUE);

    /* FILTERS */

    // Text filter.
    GtkFileFilter *filter_text = gtk_file_filter_new();
    gtk_file_filter_add_mime_type(filter_text, "text/*");
    gtk_file_filter_set_name(filter_text, _("Text files"));

    // Any filter.
    GtkFileFilter *filter_any = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter_any, "*");
    gtk_file_filter_set_name(filter_any, _("All files"));

    // Add filters to the file chooser.
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_text);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_any);

    /* ENCODING OPTIONS */

    // Create the encoding widgets.
    GtkWidget *encoding_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *encoding_label = gtk_label_new(_("Encoding:"));
    GtkWidget *encoding_entry = gtk_entry_new();

    // Configure the entry.
    gtk_entry_set_input_hints(GTK_ENTRY(encoding_entry), GTK_INPUT_HINT_NO_EMOJI);

    // Compose the encoding widgets.
    gtk_box_pack_start(GTK_BOX(encoding_box), encoding_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(encoding_box), encoding_entry, FALSE, FALSE, 0);

    // Add the encoding widget to the file chooser.
    gtk_file_chooser_set_extra_widget(GTK_FILE_CHOOSER(dialog), encoding_box);

    // Display the encoding widget.
    gtk_widget_show_all(encoding_box);

    // Set the initial filename and encoding.
    if (data_file->file)
    {
        gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog), g_file_peek_path(data_file->file));
        gtk_entry_set_text(GTK_ENTRY(encoding_entry), data_file->encoding);
    }
    else
    {
        gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), _("Untitled"));
        gtk_entry_set_text(GTK_ENTRY(encoding_entry), default_encoding);
    }

    /* ACTIVATE */

    // Run the dialog and get the user response.
    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_DELETE_EVENT || response == GTK_RESPONSE_CANCEL)   // Operation cancelled.
    {
        gtk_widget_destroy(dialog);
        return FALSE;
    }

    // Save reference to the newly chosen file. 
    data_file->file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog));
    data_file->encoding = g_strdup(gtk_entry_get_text(GTK_ENTRY(encoding_entry)));

    // Destroy the dialog.
    gtk_widget_destroy(dialog);

    return TRUE;
}

// About dialog.
void airpad_dialog_about(GtkWidget *widget, GtkWidget *parent)
{
    static const char *authors[] =  {
                                        AIRPAD_INFO_AUTH " <" AIRPAD_INFO_AUTH_EMAIL ">",
                                        NULL
                                    };

    static const char *artists[] =  {
                                        AIRPAD_INFO_AUTH " <" AIRPAD_INFO_AUTH_EMAIL ">",
                                        NULL
                                    };

    GdkPixbuf *icon = gtk_icon_theme_load_icon(gtk_icon_theme_get_default(), AIRPAD_EXEC_NAME, 128, GTK_ICON_LOOKUP_FORCE_SIZE, NULL);

    // Create an about dialog.
    GtkWidget *dialog = gtk_about_dialog_new();
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent));
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_title(GTK_WINDOW(dialog), _("About"));
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), AIRPAD_INFO_VERS);
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Copyright (C) " AIRPAD_INFO_YEAR " " AIRPAD_INFO_AUTH);
    gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog), AIRPAD_INFO_LICE_TYPE);
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), AIRPAD_INFO_SITE);
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(dialog), authors);
    gtk_about_dialog_set_artists(GTK_ABOUT_DIALOG(dialog), artists);
    gtk_about_dialog_set_translator_credits(GTK_ABOUT_DIALOG(dialog), _("translator-credits"));
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), icon);

    // Display the dialog.
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    if (icon)
        g_object_unref(icon);
}
