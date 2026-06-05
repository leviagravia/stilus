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

static GtkWidget *airpad_dialog_create_text_label(const char *text)
{
    GtkWidget *label = gtk_label_new(text);
    gtk_label_set_xalign(GTK_LABEL(label), 0.0);
    gtk_label_set_yalign(GTK_LABEL(label), 0.0);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_selectable(GTK_LABEL(label), TRUE);
    return label;
}

static GtkWidget *airpad_dialog_create_text_dialog(GtkWidget *parent, const char *title, const char *text)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        title,
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Close",
        GTK_RESPONSE_CLOSE,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 480, 360);
    gtk_window_set_resizable(GTK_WINDOW(dialog), TRUE);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_container_set_border_width(GTK_CONTAINER(scrolled_window), 18);
    gtk_box_pack_start(GTK_BOX(content_area), scrolled_window, TRUE, TRUE, 0);

    GtkWidget *label = airpad_dialog_create_text_label(text);
    gtk_container_add(GTK_CONTAINER(scrolled_window), label);

    return dialog;
}

static GdkPixbuf *airpad_dialog_load_stilus_icon(void)
{
    GError *error = NULL;
    GdkPixbuf *icon = gdk_pixbuf_new_from_file_at_size("icon.svg", 96, 96, &error);

    if (!icon)
    {
        g_clear_error(&error);
        icon = gdk_pixbuf_new_from_file_at_size("/usr/share/icons/hicolor/scalable/apps/stilus.svg", 96, 96, &error);
    }

    if (error)
        g_error_free(error);

    return icon;
}

static void airpad_dialog_credits(GtkWidget *parent)
{
    static const char *credits =
        "Created by Luciano Squizzato\n"
        "\n"
        "Stilus icon by Luciano Squizzato\n"
        "\n"
        "Stilus originates from the AirPad project by Nikola Hadžić.\n"
        "\n"
        "The Stilus project preserves attribution and licensing information in accordance with the GNU General Public License.";

    GtkWidget *dialog = airpad_dialog_create_text_dialog(parent, "Credits", credits);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// About dialog.
void airpad_dialog_about(GtkWidget *widget, GtkWidget *parent)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "About Stilus",
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Credits",
        GTK_RESPONSE_APPLY,
        "Close",
        GTK_RESPONSE_CLOSE,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 440, 360);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_set_border_width(GTK_CONTAINER(box), 20);
    gtk_box_pack_start(GTK_BOX(content_area), box, TRUE, TRUE, 0);

    GdkPixbuf *icon = airpad_dialog_load_stilus_icon();
    if (icon)
    {
        GtkWidget *image = gtk_image_new_from_pixbuf(icon);
        gtk_widget_set_halign(image, GTK_ALIGN_CENTER);
        gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);
        g_object_unref(icon);
    }

    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span size=\"xx-large\" weight=\"bold\">Stilus</span>");
    gtk_label_set_xalign(GTK_LABEL(title), 0.5);
    gtk_box_pack_start(GTK_BOX(box), title, FALSE, FALSE, 0);

    GtkWidget *label = gtk_label_new(
        "Version " AIRPAD_INFO_VERS "\n"
        "\n"
        "Created by " AIRPAD_INFO_AUTH "\n"
        "\n"
        "A lightweight text editor designed for writers\n"
        "\n"
        "Website"
    );
    gtk_label_set_xalign(GTK_LABEL(label), 0.5);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_label_set_selectable(GTK_LABEL(label), TRUE);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    GtkWidget *website = gtk_link_button_new_with_label(AIRPAD_INFO_SITE, AIRPAD_INFO_SITE);
    gtk_widget_set_halign(website, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), website, FALSE, FALSE, 0);

    GtkWidget *license_label = gtk_label_new("License");
    gtk_label_set_xalign(GTK_LABEL(license_label), 0.5);
    gtk_label_set_justify(GTK_LABEL(license_label), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(box), license_label, FALSE, FALSE, 0);

    GtkWidget *license = gtk_link_button_new_with_label(
        "https://www.gnu.org/licenses/gpl-3.0.html",
        "GNU General Public License v3 or later"
    );
    gtk_widget_set_halign(license, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), license, FALSE, FALSE, 0);

    gtk_widget_show_all(dialog);

    int response;
    while ((response = gtk_dialog_run(GTK_DIALOG(dialog))) == GTK_RESPONSE_APPLY)
        airpad_dialog_credits(parent);

    gtk_widget_destroy(dialog);
}

void airpad_dialog_writing_principles(GtkWidget *widget, GtkWidget *parent)
{
    static const char *text =
        "Text First\n"
        "Text creation is the primary purpose of Stilus.\n"
        "\n"
        "Writing Before Formatting\n"
        "Text creation has priority over document presentation.\n"
        "\n"
        "UTF-8 First\n"
        "Documents are stored and handled as UTF-8 text.\n"
        "\n"
        "Reliable File Handling\n"
        "Opening and saving documents must be predictable and safe.\n"
        "\n"
        "Keyboard Friendly\n"
        "Common actions should always be accessible from the keyboard.\n"
        "\n"
        "Fast Startup\n"
        "The editor should remain responsive and lightweight.\n"
        "\n"
        "Long-form Friendly\n"
        "The editor should remain comfortable for articles, essays, research and book-length writing.\n"
        "\n"
        "Distraction Free\n"
        "The interface should support concentration and long-form writing.\n"
        "\n"
        "Simplicity Before Complexity\n"
        "New functionality should only be added when it clearly improves the writing experience.";

    GtkWidget *dialog = airpad_dialog_create_text_dialog(parent, "Writing Principles", text);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void airpad_dialog_keyboard_shortcuts(GtkWidget *widget, GtkWidget *parent)
{
    static const char *text =
        "Ctrl+N          New\n"
        "Ctrl+O          Open\n"
        "Ctrl+S          Save\n"
        "Ctrl+Shift+S    Save As\n"
        "Ctrl+Z          Undo\n"
        "Ctrl+Y          Redo\n"
        "Ctrl+F          Find\n"
        "Ctrl+H          Replace";

    GtkWidget *dialog = airpad_dialog_create_text_dialog(parent, "Keyboard Shortcuts", text);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void airpad_dialog_writing_workflow(GtkWidget *widget, GtkWidget *parent)
{
    static const char *text =
        "Create\n"
        "Start a new document.\n"
        "\n"
        "Write\n"
        "Focus on text creation.\n"
        "\n"
        "Revise\n"
        "Review, reorganize and improve the text.\n"
        "\n"
        "Save\n"
        "Preserve work safely and predictably.\n"
        "\n"
        "Publish\n"
        "Export or use the text in external publishing workflows.\n"
        "\n"
        "Stilus focuses on writing and revision.\n"
        "\n"
        "Document layout, desktop publishing and presentation design are outside the primary scope of the project.";

    GtkWidget *dialog = airpad_dialog_create_text_dialog(parent, "Writing Workflow", text);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
