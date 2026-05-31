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
#include <string.h>
#include "const.h"
#include "dialog.h"

// Positions the cursor at the start of the file.
// Use when a file is opened.
void airpad_aux_position_cursor_at_open(GtkTextBuffer *text_buffer)
{
    GtkTextIter start;
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_place_cursor(text_buffer, &start);
}

// Prompts the user whether would he like to save the modified file and presents him with
// a file chooser dialog if the file is not already associated with a file on the filesystem.
// `data_file` will be updated appropriately if the user did not cancel the operation,
// otherwise it will not be modified.
// Returns TRUE if the user did not cancel the operation, FALSE otherwise.
gboolean airpad_aux_save_prompt(const struct AirpadDataWindow *data_window, struct AirpadDataFile *data_file, const char *default_encoding, gboolean append_newline)
{
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_window->data_text_view->text_view));

    // Prompt the user whether to save the file if it was modified.
    if (gtk_text_buffer_get_modified(text_buffer))
    {
        switch (airpad_dialog_confirm(data_window->window))
        {
            case GTK_RESPONSE_DELETE_EVENT:
            case GTK_RESPONSE_CANCEL:
                return FALSE;
            case GTK_RESPONSE_YES:
            {
                // Display a save dialog to the user if this file does not exist on the filesystem.
                if (!data_file->file)
                    if (!airpad_dialog_save(data_window->window, default_encoding, data_file))
                        return FALSE;

                GError *error = NULL;

                // Get the text buffer contents.
                GtkTextIter start, end;
                gtk_text_buffer_get_start_iter(text_buffer, &start);
                gtk_text_buffer_get_end_iter(text_buffer, &end);
                char *contents = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

                // Convert the text buffer contents to the chosen file encoding.
                if (g_strcmp0(data_file->encoding, "UTF-8"))
                {
                    char * const contents_copy = contents;
                    contents = g_convert(contents, -1, data_file->encoding, "UTF-8", NULL, NULL, &error);
                    g_free(contents_copy);

                    if (!contents)
                    {
                        char *filename = g_filename_to_utf8(g_file_peek_path(data_file->file), -1, NULL, NULL, NULL);
                        airpad_dialog_error(data_window->window, AIRPAD_ERROR_TYPE_TEXT_CONVERSION_ERROR, FALSE, filename, error->message);
                        g_free(filename);

                        g_error_free(error);

                        return FALSE;
                    }
                }

                error = NULL;

                const size_t contents_length = strlen(contents);

                // Get stream for overwriting, optionally creating the file if it does not exist.
                GFileOutputStream *output_stream = g_file_replace(data_file->file, NULL, FALSE, G_FILE_CREATE_NONE, NULL, &error);
                if (!output_stream)
                {
                    char *filename = g_filename_to_utf8(g_file_peek_path(data_file->file), -1, NULL, NULL, NULL);
                    airpad_dialog_error(data_window->window, AIRPAD_ERROR_TYPE_FILE_WRITE_FAILURE, FALSE, filename, error->message);
                    g_free(filename);

                    g_free(contents);

                    g_error_free(error);

                    return FALSE;
                }

                error = NULL;

                // Write the text buffer contents to the file.
                if (g_output_stream_write(G_OUTPUT_STREAM(output_stream), contents, contents_length, NULL, &error) == -1)
                {
                    char *filename = g_filename_to_utf8(g_file_peek_path(data_file->file), -1, NULL, NULL, NULL);
                    airpad_dialog_error(data_window->window, AIRPAD_ERROR_TYPE_FILE_WRITE_FAILURE, FALSE, filename, error->message);
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
                        airpad_dialog_error(data_window->window, AIRPAD_ERROR_TYPE_FILE_WRITE_FAILURE, FALSE, filename, error->message);
                        g_free(filename);

                        if (output_stream)
                            g_object_unref(output_stream);

                        g_free(contents);

                        g_error_free(error);

                        return FALSE;
                    }

                    g_object_unref(output_stream);
                }

                g_free(contents);
            }
            break;
        }
    }

    return TRUE;
}
