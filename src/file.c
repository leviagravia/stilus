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
#include "aux.h"
#include "window.h"
#include "dialog.h"
#include "undo.h"

// Writes the given text buffer to the given file.
static gboolean airpad_file_write(GtkWidget *parent, GtkTextBuffer *text_buffer, const struct AirpadDataFile *data_file, gboolean append_newline)
{
    // Get the text buffer contents.
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);
    char *contents = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    GError *error = NULL;

    // Convert the text buffer contents to the chosen file encoding.
    if (g_strcmp0(data_file->encoding, "UTF-8"))
    {
        char * const contents_copy = contents;
        contents = g_convert(contents, -1, data_file->encoding, "UTF-8", NULL, NULL, &error);
        g_free(contents_copy);

        if (!contents)
        {
            char *filename = g_filename_to_utf8(g_file_peek_path(data_file->file), -1, NULL, NULL, NULL);
            airpad_dialog_error(parent, AIRPAD_ERROR_TYPE_TEXT_CONVERSION_ERROR, FALSE, filename, error->message);
            g_free(filename);

            g_error_free(error);

            return FALSE;
        }
    }

    error = NULL;

    // Create a backup of the previous on-disk version before replacing it.
    if (g_file_query_exists(data_file->file, NULL))
    {
        char *path = g_file_get_path(data_file->file);

        if (path)
        {
            char *backup_path = g_strconcat(path, ".bak", NULL);
            GFile *backup_file = g_file_new_for_path(backup_path);
            GError *backup_error = NULL;

            if (!g_file_copy(data_file->file, backup_file, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, &backup_error))
                g_clear_error(&backup_error);

            g_object_unref(backup_file);
            g_free(backup_path);
            g_free(path);
        }
    }

    // Get stream for overwriting, optionally creating the file if it does not exist.
    GFileOutputStream *output_stream = g_file_replace(data_file->file, NULL, FALSE, G_FILE_CREATE_NONE, NULL, &error);
    if (!output_stream)
    {
        char *filename = g_filename_to_utf8(g_file_peek_path(data_file->file), -1, NULL, NULL, NULL);
        airpad_dialog_error(parent, AIRPAD_ERROR_TYPE_FILE_WRITE_FAILURE, FALSE, filename, error->message);
        g_free(filename);

        g_free(contents);

        g_error_free(error);

        return FALSE;
    }

    error = NULL;

    const size_t contents_length = strlen(contents);

    // Write the text buffer contents to the file.
    if (g_output_stream_write(G_OUTPUT_STREAM(output_stream), contents, contents_length, NULL, &error) == -1)
    {
        char *filename = g_filename_to_utf8(g_file_peek_path(data_file->file), -1, NULL, NULL, NULL);
        airpad_dialog_error(parent, AIRPAD_ERROR_TYPE_FILE_WRITE_FAILURE, FALSE, filename, error->message);
        g_free(filename);

        g_object_unref(output_stream);
        g_free(contents);

        g_error_free(error);

        return FALSE;
    }

    g_object_unref(output_stream);

    // If the last character in the buffer is not a newline, add it.
    if (append_newline && contents[contents_length - 1] != '\n')
    {
        error = NULL;

        if (!(output_stream = g_file_append_to(data_file->file, G_FILE_CREATE_NONE, NULL, &error)) || g_output_stream_write(G_OUTPUT_STREAM(output_stream), "\n", 1, NULL, &error) == -1)
        {
            char *filename = g_filename_to_utf8(g_file_peek_path(data_file->file), -1, NULL, NULL, NULL);
            airpad_dialog_error(parent, AIRPAD_ERROR_TYPE_FILE_WRITE_FAILURE, FALSE, filename, error->message);
            g_free(filename);

            g_free(contents);

            if (output_stream)
                g_object_unref(output_stream);

            g_error_free(error);

            return FALSE;
        }

        g_object_unref(output_stream);
    }

    g_free(contents);

    return TRUE;
}

// Activated when the text buffer was modified to update the window state.
void airpad_file_text_buffer_modified(GtkTextBuffer *text_buffer, const struct AirpadDataWindow *data_window)
{
    const gboolean modified = gtk_text_buffer_get_modified(text_buffer);

    gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_file_save, modified);
    airpad_window_set_title_modified(data_window, modified);

    airpad_window_update_status_bar(text_buffer, data_window);
}

// Initializes the state of the file widgets.
void airpad_file_init(const struct AirpadDataWindow *data_window)
{
    gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_file_save, FALSE);
}

// Clears the text buffer.
// If the current file was modified the user is also prompted to save it.
// If he accepts but the file is not associated with any file on the filesystem,
// a save dialog is also presented.
void airpad_file_new(GtkWidget *widget, const struct AirpadDataApplication *data_application)
{
    // Prompt the user to save the file.
    if (!airpad_aux_save_prompt(data_application->data_window, data_application->data_file, data_application->data_arguments->file_encoding, data_application->data_options->append_newline))
        return;

    // Get the text buffer.
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view));

    // Forget the current file.
    if (data_application->data_file->file)
    {
        g_object_unref(data_application->data_file->file);
        g_free(data_application->data_file->encoding);
        data_application->data_file->file = NULL;
    }

    // Block the functions that run when the text buffer's modified bit is flipped.
    g_signal_handler_block(text_buffer, data_application->data_signals->text_buffer_modified);
    g_signal_handler_block(text_buffer, data_application->data_signals->text_buffer_insert_text);
    g_signal_handler_block(text_buffer, data_application->data_signals->text_buffer_delete_range);

    // Empty the text buffer.
    const gboolean modified = gtk_text_buffer_get_modified(text_buffer);
    gtk_text_buffer_set_text(text_buffer, "", 0);
    gtk_text_buffer_set_modified(text_buffer, modified);

    // Change the window title.
    airpad_window_set_title(data_application->data_window, NULL);

    // Reset the undo handling.
    airpad_undo_reset();
    airpad_undo_init(data_application->data_window);

    // Unblock the functions that run when the text buffer's modified bit is flipped.
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_delete_range);
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_insert_text);
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_modified);

    // Update the window state and mark the text buffer as not modified.
    gtk_text_buffer_set_modified(text_buffer, FALSE);
}

// Displays a file chooser dialog to the user that lets him pick a file to open.
// If the current file was modified the user is also prompted to save it.
// If he accepts but the file is not associated with any file on the filesystem,
// a save dialog is also presented.
void airpad_file_open(GtkWidget *widget, const struct AirpadDataApplication *data_application)
{
    // Prompt the user to save the file.
    if (!airpad_aux_save_prompt(data_application->data_window, data_application->data_file, data_application->data_arguments->file_encoding, data_application->data_options->append_newline))
        return;

    // Get the text buffer.
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view));

    // Block the functions that run when the text buffer's modified bit is flipped.
    g_signal_handler_block(text_buffer, data_application->data_signals->text_buffer_modified);
    g_signal_handler_block(text_buffer, data_application->data_signals->text_buffer_insert_text);
    g_signal_handler_block(text_buffer, data_application->data_signals->text_buffer_delete_range);

    // Do not forget the current file data because we will need to revert to it in case of an error.
    GFile * const current_file = data_application->data_file->file;
    char * const current_encoding = data_application->data_file->encoding;

    // Display an open file chooser dialog.
    if (!airpad_dialog_open(data_application->data_window->window, data_application->data_arguments->file_encoding, data_application->data_file))
        goto done;

    // Replace the text buffer contents with the contents of the newly opened file.
    char *contents;
    gsize length;
    GError *error = NULL;

    if (!g_file_load_contents(data_application->data_file->file, NULL, &contents, &length, NULL, &error))   // Error while reading.
    {
        char *filename = g_filename_to_utf8(g_file_peek_path(data_application->data_file->file), -1, NULL, NULL, NULL);
        airpad_dialog_error(data_application->data_window->window, AIRPAD_ERROR_TYPE_FILE_READ_FAILURE, FALSE, filename, error->message);
        g_free(filename);

        g_object_unref(data_application->data_file->file);
        g_free(data_application->data_file->encoding);
        data_application->data_file->file = current_file;
        data_application->data_file->encoding = current_encoding;

        g_error_free(error);

        goto done;
    }

    if (g_strcmp0(data_application->data_file->encoding, "UTF-8"))
    {
        error = NULL;

        char * const contents_copy = contents;
        contents = g_convert(contents, length, "UTF-8", data_application->data_file->encoding, NULL, &length, &error);
        g_free(contents_copy);

        if (!contents)
        {
            char *filename = g_filename_to_utf8(g_file_peek_path(data_application->data_file->file), -1, NULL, NULL, NULL);
            airpad_dialog_error(data_application->data_window->window, AIRPAD_ERROR_TYPE_TEXT_CONVERSION_ERROR, FALSE, filename, error->message);
            g_free(filename);

            g_object_unref(data_application->data_file->file);
            g_free(data_application->data_file->encoding);
            data_application->data_file->file = current_file;
            data_application->data_file->encoding = current_encoding;

            g_error_free(error);

            goto done;
        }
    }
    else if (!g_utf8_validate(contents, length, NULL))  // Invalid UTF-8.
    {
        char *filename = g_filename_to_utf8(g_file_peek_path(data_application->data_file->file), -1, NULL, NULL, NULL);
        airpad_dialog_error(data_application->data_window->window, AIRPAD_ERROR_TYPE_TEXT_INVALID_UTF8, FALSE, filename);
        g_free(filename);

        g_object_unref(data_application->data_file->file);
        g_free(data_application->data_file->encoding);
        data_application->data_file->file = current_file;
        data_application->data_file->encoding = current_encoding;

        g_free(contents);

        goto done;
    }

    // Forget the current file.
    if (current_file)
    {
        g_object_unref(current_file);
        g_free(current_encoding);
    }

    // Set the text buffer's contents to the loaded text buffer.
    const gboolean modified = gtk_text_buffer_get_modified(text_buffer);
    gtk_text_buffer_set_text(text_buffer, contents, length);
    gtk_text_buffer_set_modified(text_buffer, modified);

    g_free(contents);
    
    // Change the window title.
    airpad_window_set_title(data_application->data_window, data_application->data_file->file);

    // Reset the undo handling.
    airpad_undo_reset();
    airpad_undo_init(data_application->data_window);

    // Unblock the functions that run when the text buffer's modified bit is flipped.
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_delete_range);
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_insert_text);
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_modified);

    // Update the window state and mark the text buffer as not modified.
    gtk_text_buffer_set_modified(text_buffer, FALSE);

    // Properly position the cursor.
    airpad_aux_position_cursor_at_open(text_buffer);

    return;

done:
    // Unblock the functions that run when the text buffer's modified bit is flipped.
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_delete_range);
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_insert_text);
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_modified);
}

// If the file is associated with a file on the filesystem, text buffer contents
// are just silently written down. Otherwise a save dialog is presented to the user
// that lets him choose the save location and name for the file. If he accepts,
// the file data is updated appropriately.
void airpad_file_save(GtkWidget *widget, const struct AirpadDataApplication *data_application)
{
    // Ask the user to name the file if it already does not exist on the disk.
    if (!data_application->data_file->file)
        if (!airpad_dialog_save(data_application->data_window->window, data_application->data_arguments->file_encoding, data_application->data_file))
            return;

    // Get the text buffer.
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view));

    // Write the file.
    const gboolean success = airpad_file_write(data_application->data_window->window, text_buffer, data_application->data_file, data_application->data_options->append_newline);

    // Change the window title.
    airpad_window_set_title(data_application->data_window, data_application->data_file->file);

    // Update the window state and mark the text buffer as not modified if the saving succeeded, and as modified otherwise.
    if (success)
        airpad_undo_save(text_buffer);
    gtk_text_buffer_set_modified(text_buffer, !success);
}

// Presents a save dialog to the user that lets him pick a file to save the current text
// buffer to. If the text buffer is already associated with a file on the filesystem,
// its values are substituted in place of the default values for file attributes.
void airpad_file_save_as(GtkWidget *widget, const struct AirpadDataApplication *data_application)
{
    // Save the current file reference.
    GFile * const current_file = data_application->data_file->file;
    char * const current_encoding = data_application->data_file->encoding;

    // Ask the user to name the file and replace the current file with the newly chosen one.
    if (!airpad_dialog_save(data_application->data_window->window, data_application->data_arguments->file_encoding, data_application->data_file))
        return;

    // Get the text buffer.
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view));

    // Write the file.
    if (airpad_file_write(data_application->data_window->window, text_buffer, data_application->data_file, data_application->data_options->append_newline) && g_file_equal(current_file, data_application->data_file->file))
    {
        // Destroy the reference to the previous file.
        if (current_file)
        {
            g_object_unref(current_file);
            g_free(current_encoding);
        }

        // Update the window state and mark the text buffer as not modified.
        airpad_undo_save(text_buffer);
        gtk_text_buffer_set_modified(text_buffer, FALSE);
    }
    else    // Saving operation either did not succeed or it was applied to a different file, restore the current file reference.
    {
        g_object_unref(data_application->data_file->file);
        g_free(data_application->data_file->encoding);
        data_application->data_file->file = current_file;
        data_application->data_file->encoding = current_encoding;
    }
}

// Terminates the application.
// If the current file was modified the user is also prompted to save the file.
// If he accepts but the file is not associated with any file on the filesystem,
// a save dialog is also presented.
void airpad_file_close(GtkWidget *widget, const struct AirpadDataApplication *data_application)
{
    // Prompt the user to save the file.
    if (!airpad_aux_save_prompt(data_application->data_window, data_application->data_file, data_application->data_arguments->file_encoding, data_application->data_options->append_newline))
        return;

    g_application_quit(G_APPLICATION(data_application->application));
}
