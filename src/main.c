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
#include "aux.h"
#include "window.h"
#include "dialog.h"
#include "quit.h"
#include "file.h"
#include "edit.h"
#include "undo.h"
#include "search.h"
#include "stilus_commands.h"
#include "view.h"
#include "options.h"
#include "font.h"

// Allocates the application data on the heap and returns a pointer to it.
// The entire structure is recursively initialized to NULL.
static struct AirpadDataApplication *airpad_data_application_new()
{
    struct AirpadDataApplication *data_application = g_malloc0(sizeof(struct AirpadDataApplication));
    data_application->data_arguments = g_malloc0(sizeof(struct AirpadDataArguments));
    data_application->data_options = g_malloc0(sizeof(struct AirpadDataOptions));
    data_application->data_file = g_malloc0(sizeof(struct AirpadDataFile));
    data_application->data_search = g_malloc0(sizeof(struct AirpadDataSearch));
    data_application->data_signals = g_malloc0(sizeof(struct AirpadDataSignals));
    data_application->data_window = g_malloc0(sizeof(struct AirpadDataWindow));
    data_application->data_window->data_menu_bar = g_malloc0(sizeof(struct AirpadDataMenuBar));
    data_application->data_window->data_text_view = g_malloc0(sizeof(struct AirpadDataTextView));

    return data_application;
}

// Frees the application data recursively.
// If NULL is passed this function does nothing.
static void airpad_data_application_free(struct AirpadDataApplication *data_application)
{
    if (data_application)
    {
        if (data_application->data_window)
        {
            g_free(data_application->data_window->data_text_view);
            g_free(data_application->data_window->data_menu_bar);
            g_free(data_application->data_window);
        }

        g_free(data_application->data_signals);
        g_free(data_application->data_search);
        g_free(data_application->data_file);
        g_free(data_application->data_options);
        g_free(data_application->data_arguments);
        g_free(data_application);
    }
}

// Specifies to the argument parser which options to accept.
static void airpad_prepare_argument_parser(GApplication *application, struct AirpadDataArguments *data_arguments)
{
    // Main options.
    {
        GOptionEntry entries[] =    {
                                        {
                                            "version",
                                            '\0',
                                            G_OPTION_FLAG_NONE,
                                            G_OPTION_ARG_NONE,
                                            NULL,
                                            _("Show program version"),
                                            NULL
                                        },
                                        {
                                            "license",
                                            '\0',
                                            G_OPTION_FLAG_NONE,
                                            G_OPTION_ARG_NONE,
                                            NULL,
                                            _("Show program license"),
                                            NULL
                                        },
                                        {
                                            NULL,
                                            '\0',
                                            G_OPTION_FLAG_NONE,
                                            G_OPTION_ARG_NONE,
                                            NULL,
                                            NULL,
                                            NULL
                                        }
                                    };

        g_application_add_main_option_entries(application, entries);
    }

    // File options.
    {
        GOptionEntry entries[] =    {
                                        {
                                            "encoding",
                                            '\0',
                                            G_OPTION_FLAG_NONE,
                                            G_OPTION_ARG_STRING,
                                            &(data_arguments->file_encoding),
                                            _("Assume that files are in this encoding"),
                                            _("ENCODING")
                                        },
                                        {
                                            NULL,
                                            '\0',
                                            G_OPTION_FLAG_NONE,
                                            G_OPTION_ARG_NONE,
                                            NULL,
                                            NULL,
                                            NULL
                                        }
                                    };

        GOptionGroup *group = g_option_group_new("file", _("File Options:"), _("Show file options"), NULL, NULL);
        g_option_group_add_entries(group, entries);
        g_application_add_option_group(application, group);
    }

    // Set the help menu description.
    g_application_set_option_context_parameter_string(application, _("[FILE\u2026]"));

    // Set the options to the default values.
    // Stilus defaults new/unknown text files to UTF-8.
    //
    // AirPad used g_get_codeset() here, which depends on the current
    // process locale. On systems with a non-UTF-8 or ASCII locale, the
    // Open dialog and command-line open path default to that legacy
    // encoding. UTF-8 files saved by Stilus then fail to reopen because
    // g_convert() tries to decode them using the locale codeset.
    //
    // Users can still override the encoding via the dialog/--encoding
    // option when opening legacy files.
    data_arguments->file_encoding = g_strdup("UTF-8");
}

// Loads the file contents into the text buffer, after optionally converting it to UTF-8.
// Used to load the file that was opened via command-line.
// Returns TRUE on success and FALSE on failure.
static gboolean airpad_load_contents(GtkWidget *parent, GtkTextBuffer *text_buffer, const struct AirpadDataFile *data_file)
{
    // Get the file contents.
    char *contents;
    gsize length;
    GError *error = NULL;
    if (!g_file_load_contents(data_file->file, NULL, &contents, &length, NULL, &error))
    {
        if (error->code == G_IO_ERROR_NOT_FOUND)
        {
            g_error_free(error);
            return TRUE;
        }

        char *filename = g_filename_to_utf8(g_file_peek_path(data_file->file), -1, NULL, NULL, NULL);
        airpad_dialog_error(parent, AIRPAD_ERROR_TYPE_FILE_READ_FAILURE, FALSE, filename, error->message);
        g_free(filename);

        g_error_free(error);

        return FALSE;
    }

    // If the file encoding is not UTF-8, convert it to UTF-8.
    if (g_strcmp0(data_file->encoding, "UTF-8"))
    {
        error = NULL;

        char * const contents_copy = contents;
        contents = g_convert(contents, length, "UTF-8", data_file->encoding, NULL, &length, &error);
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
    else if (!g_utf8_validate(contents, length, NULL))  // Otherwise, check whether the file contains valid UTF-8.
    {
        char *filename = g_filename_to_utf8(g_file_peek_path(data_file->file), -1, NULL, NULL, NULL);
        airpad_dialog_error(parent, AIRPAD_ERROR_TYPE_TEXT_INVALID_UTF8, FALSE, filename);
        g_free(filename);

        g_free(contents);

        return FALSE;
    }

    // Set the text buffer contents to the loaded file contents.
    gtk_text_buffer_set_text(text_buffer, contents, length);

    g_free(contents);

    // Update the text buffer state and properly position the cursor.
    gtk_text_buffer_set_modified(text_buffer, FALSE);
    airpad_aux_position_cursor_at_open(text_buffer);

    return TRUE;
}

// Connects the signal handlers to the widget events.
static void airpad_connect_signals(struct AirpadDataApplication *data_application)
{
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view));

    // Window signals.
    data_application->data_signals->window_delete_event = g_signal_connect(data_application->data_window->window, "delete-event", G_CALLBACK(&airpad_quit), data_application);
    data_application->data_signals->window_focus_in = g_signal_connect_after(data_application->data_window->window, "focus-in-event", G_CALLBACK(&airpad_window_focus_in), data_application->data_window);
    data_application->data_signals->window_state = g_signal_connect_after(data_application->data_window->window, "window-state-event", G_CALLBACK(&airpad_window_window_state), data_application->data_window);

    // Text view signals.
    data_application->data_signals->text_view_copy_clipboard = g_signal_connect_after(data_application->data_window->data_text_view->text_view, "copy-clipboard", G_CALLBACK(&airpad_window_copy_clipboard), data_application->data_window);
    data_application->data_signals->text_view_cut_clipboard = g_signal_connect_after(data_application->data_window->data_text_view->text_view, "cut-clipboard", G_CALLBACK(&airpad_window_cut_clipboard), data_application->data_window);

    // Text buffer signals.
    data_application->data_signals->text_buffer_modified = g_signal_connect_after(text_buffer, "modified-changed", G_CALLBACK(&airpad_file_text_buffer_modified), data_application->data_window);
    data_application->data_signals->text_buffer_has_selection = g_signal_connect_after(text_buffer, "notify::has-selection", G_CALLBACK(&airpad_edit_has_selection), data_application->data_window->data_menu_bar);
    data_application->data_signals->text_buffer_insert_text = g_signal_connect(text_buffer, "insert-text", G_CALLBACK(&airpad_undo_add_on_insert), data_application->data_window->data_menu_bar);
    data_application->data_signals->text_buffer_delete_range = g_signal_connect(text_buffer, "delete-range", G_CALLBACK(&airpad_undo_add_on_delete), data_application->data_window->data_menu_bar);
    g_signal_connect(text_buffer, "changed", G_CALLBACK(&airpad_window_update_status_bar), data_application->data_window);
    g_signal_connect(text_buffer, "mark-set", G_CALLBACK(&airpad_window_text_buffer_mark_set), data_application->data_window);
    data_application->data_signals->text_buffer_paste_done = g_signal_connect(text_buffer, "paste-done", G_CALLBACK(&airpad_edit_paste_done), data_application->data_window->data_text_view->text_view);

    // Menu item signals.
    data_application->data_signals->menu_item_file_new_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_file_new, "activate", G_CALLBACK(&airpad_file_new), data_application);
    data_application->data_signals->menu_item_file_open_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_file_open, "activate", G_CALLBACK(&airpad_file_open), data_application);
    data_application->data_signals->menu_item_file_save_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_file_save, "activate", G_CALLBACK(&airpad_file_save), data_application);
    data_application->data_signals->menu_item_file_save_as_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_file_save_as, "activate", G_CALLBACK(&airpad_file_save_as), data_application);
    data_application->data_signals->menu_item_file_close_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_file_close, "activate", G_CALLBACK(&airpad_file_close), data_application);
    data_application->data_signals->menu_item_navigate_go_to_line_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_navigate_go_to_line, "activate", G_CALLBACK(&stilus_cmd_navigate_go_to_line), data_application);
    data_application->data_signals->menu_item_writing_statistics_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_writing_statistics, "activate", G_CALLBACK(&stilus_cmd_writing_statistics), data_application);
    data_application->data_signals->menu_item_writing_word_goal_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_writing_word_goal, "activate", G_CALLBACK(&stilus_cmd_writing_word_goal), data_application);
    data_application->data_signals->menu_item_revise_uppercase_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_revise_uppercase, "activate", G_CALLBACK(&stilus_cmd_revise_uppercase), data_application);
    data_application->data_signals->menu_item_revise_lowercase_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_revise_lowercase, "activate", G_CALLBACK(&stilus_cmd_revise_lowercase), data_application);
    data_application->data_signals->menu_item_revise_remove_trailing_spaces_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_revise_remove_trailing_spaces, "activate", G_CALLBACK(&stilus_cmd_revise_remove_trailing_spaces), data_application);
    data_application->data_signals->menu_item_edit_copy_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_edit_copy, "activate", G_CALLBACK(&airpad_edit_copy), data_application->data_window);
    data_application->data_signals->menu_item_edit_paste_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_edit_paste, "activate", G_CALLBACK(&airpad_edit_paste), text_buffer);
    data_application->data_signals->menu_item_edit_paste_as_plain_text_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_edit_paste_as_plain_text, "activate", G_CALLBACK(&airpad_edit_paste_as_plain_text), text_buffer);
    data_application->data_signals->menu_item_edit_cut_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_edit_cut, "activate", G_CALLBACK(&airpad_edit_cut), data_application->data_window);
    data_application->data_signals->menu_item_edit_delete_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_edit_delete, "activate", G_CALLBACK(&airpad_edit_delete), text_buffer);
    data_application->data_signals->menu_item_edit_select_all_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_edit_select_all, "activate", G_CALLBACK(&airpad_edit_select_all), text_buffer);
    data_application->data_signals->menu_item_edit_insert_date_time_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_edit_insert_date_time, "activate", G_CALLBACK(&airpad_edit_insert_date_time), text_buffer);
    data_application->data_signals->menu_item_edit_undo_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_edit_undo, "activate", G_CALLBACK(&airpad_undo_action_undo), data_application);
    data_application->data_signals->menu_item_edit_redo_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_edit_redo, "activate", G_CALLBACK(&airpad_undo_action_redo), data_application);
    data_application->data_signals->menu_item_search_find_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_search_find, "activate", G_CALLBACK(&airpad_find_find), data_application);
    data_application->data_signals->menu_item_search_find_next_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_search_find_next, "activate", G_CALLBACK(&airpad_find_find_next), data_application);
    data_application->data_signals->menu_item_search_find_prev_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_search_find_prev, "activate", G_CALLBACK(&airpad_find_find_prev), data_application);
    data_application->data_signals->menu_item_search_replace_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_search_replace, "activate", G_CALLBACK(&airpad_replace_replace), data_application);
    data_application->data_signals->menu_item_view_fullscreen_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_view_fullscreen, "activate", G_CALLBACK(&airpad_view_fullscreen), data_application->data_window->window);
    data_application->data_signals->menu_item_view_always_on_top_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_view_always_on_top, "activate", G_CALLBACK(&airpad_view_always_on_top), data_application->data_window->window);
    data_application->data_signals->menu_item_options_font_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_options_font, "activate", G_CALLBACK(&airpad_font_choose_font), data_application);
    data_application->data_signals->menu_item_view_font_bigger_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_view_font_bigger, "activate", G_CALLBACK(&stilus_cmd_view_font_bigger), data_application);
    data_application->data_signals->menu_item_view_font_smaller_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_view_font_smaller, "activate", G_CALLBACK(&stilus_cmd_view_font_smaller), data_application);
    data_application->data_signals->menu_item_options_text_wrap_none_toggled = g_signal_connect_after(data_application->data_window->data_menu_bar->menu_item_options_text_wrap_none, "toggled", G_CALLBACK(&airpad_options_update_text_wrap), data_application);
    data_application->data_signals->menu_item_options_text_wrap_on_char_toggled = g_signal_connect_after(data_application->data_window->data_menu_bar->menu_item_options_text_wrap_on_char, "toggled", G_CALLBACK(&airpad_options_update_text_wrap), data_application);
    data_application->data_signals->menu_item_options_text_wrap_on_word_toggled = g_signal_connect_after(data_application->data_window->data_menu_bar->menu_item_options_text_wrap_on_word, "toggled", G_CALLBACK(&airpad_options_update_text_wrap), data_application);
    data_application->data_signals->menu_item_options_text_wrap_on_char_and_word_toggled = g_signal_connect_after(data_application->data_window->data_menu_bar->menu_item_options_text_wrap_on_char_and_word, "toggled", G_CALLBACK(&airpad_options_update_text_wrap), data_application);
    data_application->data_signals->menu_item_options_overlay_scrolling_toggled = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_options_overlay_scrolling, "toggled", G_CALLBACK(&airpad_options_update_overlay_scrolling), data_application);
    data_application->data_signals->menu_item_options_scrollbar_policy_auto_toggled = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_options_scrollbar_policy_auto, "toggled", G_CALLBACK(&airpad_options_update_scrollbar_policy), data_application);
    data_application->data_signals->menu_item_options_scrollbar_policy_always_toggled = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_options_scrollbar_policy_always, "toggled", G_CALLBACK(&airpad_options_update_scrollbar_policy), data_application);
    data_application->data_signals->menu_item_options_scrollbar_policy_never_toggled = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_options_scrollbar_policy_never, "toggled", G_CALLBACK(&airpad_options_update_scrollbar_policy), data_application);
    data_application->data_signals->menu_item_help_about_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_help_about, "activate", G_CALLBACK(&airpad_dialog_about), data_application->data_window->window);
    data_application->data_signals->menu_item_help_writing_principles_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_help_writing_principles, "activate", G_CALLBACK(&airpad_dialog_writing_principles), data_application->data_window->window);
    data_application->data_signals->menu_item_help_keyboard_shortcuts_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_help_keyboard_shortcuts, "activate", G_CALLBACK(&airpad_dialog_keyboard_shortcuts), data_application->data_window->window);
    data_application->data_signals->menu_item_help_writing_workflow_activate = g_signal_connect(data_application->data_window->data_menu_bar->menu_item_help_writing_workflow, "activate", G_CALLBACK(&airpad_dialog_writing_workflow), data_application->data_window->window);
}

// Acts on the specified options that require local handling and
// decides whether to activate the application based on them.
static int airpad_handle_local_options(GApplication *application, GVariantDict *dictionary, const struct AirpadDataArguments *data_arguments)
{
    int retval = -1;    // Assume not exiting from the application.

    if (g_variant_dict_contains(dictionary, "version"))
    {
        g_print(_("Version: %s\n"), AIRPAD_INFO_VERS);
        retval = 0;
    }

    if (g_variant_dict_contains(dictionary, "license"))
    {
        g_print(_("License: %s\n"), AIRPAD_INFO_LICE);
        retval = 0;
    }

    return retval;
}

// Sets the currently opened file to the first of the received files.
// This program is then spawned for each remaining file and each file
// is passed to exactly one of the created processes.
static void airpad_open(GApplication *application, GFile **files, gint n_files, gchar *hint, GFile **current_file)
{
    // Save the first passed file as the opened file.
    g_object_ref(*files);
    *current_file = *files;

    // For all other files spawn processes to open them, for each process exactly one file.
    char * const exec_name = n_files > 1 ? g_filename_from_utf8(AIRPAD_EXEC_NAME, -1, NULL, NULL, NULL) : NULL;
    for (gint i = 1;i < n_files;++i)
    {
        GError *error = NULL;
        char *argv[3] = {exec_name, g_file_get_path(files[i]), NULL};    // Here we prefer a macro to getting the program name dynamically.

        if (!g_spawn_async(NULL, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, &error))
        {
            g_printerr(_("Could not spawn process for file \"%s\": %s\n"), argv[1], error->message);
            g_error_free(error);
        }

        g_free(argv[1]);
    }
    g_free(exec_name);

    // Activate the application.
    g_application_activate(application);
}

// Runs when the application has been activated.
// Creates the application window, sets its initial contents and state,
// connects the event handlers to the signals and displays the window.
static void airpad_activate(GApplication *application, struct AirpadDataApplication *data_application)
{
    // Create the application window.
    airpad_window_create(data_application->data_window, data_application->application, data_application->data_options);

    // Load the file contents if the file was passed on the command-line.
    if (data_application->data_file->file)
    {
        data_application->data_file->encoding = g_strdup(data_application->data_arguments->file_encoding);

        if (!airpad_load_contents(data_application->data_window->window, gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view)), data_application->data_file))
        {
            g_object_unref(data_application->data_file->file);
            g_free(data_application->data_file->encoding);
            data_application->data_file->file = NULL;
        }
    }

    // Set the window properties.
    airpad_window_set_title(data_application->data_window, data_application->data_file->file);
    if (data_application->data_options->font)
        airpad_window_set_font(data_application->data_window, data_application->data_options->font);
    airpad_window_set_overlay_scrolling(data_application->data_window, data_application->data_options->overlay_scrolling);
    airpad_window_set_text_wrap_mode(data_application->data_window, data_application->data_options->text_wrap);
    airpad_window_set_scrollbar_policy(data_application->data_window, data_application->data_options->scrollbar_policy);

    // Connect the widget events to their respective handlers.
    airpad_connect_signals(data_application);

    // Initialization of the state of the widget elements.
    airpad_file_init(data_application->data_window);
    airpad_edit_init(data_application->data_window);
    airpad_undo_init(data_application->data_window);
    airpad_search_init(data_application->data_window);
    airpad_window_update_status_bar(gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view)), data_application->data_window);

    // Display the window to the user.
    gtk_widget_show_all(data_application->data_window->window);
}

// Sets up internationalization.
static inline void airpad_i18n(const char *locale_dir, const char *text_domain, const char *codeset)
{
    char *_locale_dir = g_filename_from_utf8(locale_dir, -1, NULL, NULL, NULL);
    char *_text_domain = g_filename_from_utf8(text_domain, -1, NULL, NULL, NULL);
    char *_codeset = g_filename_from_utf8(codeset, -1, NULL, NULL, NULL);

    bindtextdomain(_text_domain, _locale_dir);
    bind_textdomain_codeset(_text_domain, _codeset);
    textdomain(_text_domain);

    g_free(_text_domain);
    g_free(_locale_dir);
    g_free(_codeset);
}

int main(int argc, char *argv[])
{
    // Setup internationalization.
    airpad_i18n(AIRPAD_LOCALE_DIR, AIRPAD_EXEC_NAME, "UTF-8");

    // Set the application name.
    g_set_application_name(AIRPAD_INFO_NAME);

    // Allocate the application data.
    struct AirpadDataApplication *data_application = airpad_data_application_new();

    // Create the GTK application.
    data_application->application = gtk_application_new(AIRPAD_INFO_IDEN, G_APPLICATION_HANDLES_OPEN | G_APPLICATION_NON_UNIQUE);

    // Configure the application command-line.
    airpad_prepare_argument_parser(G_APPLICATION(data_application->application), data_application->data_arguments);

    // Create the keyfile object and read application options into it from the configuration file.
    airpad_options_open();
    airpad_options_read(data_application->data_options);

    // Connect the application signals to their respective handlers.
    g_signal_connect(data_application->application, "handle-local-options", G_CALLBACK(&airpad_handle_local_options), data_application->data_arguments);
    g_signal_connect(data_application->application, "open", G_CALLBACK(&airpad_open), &(data_application->data_file->file));
    g_signal_connect(data_application->application, "activate", G_CALLBACK(&airpad_activate), data_application);

    // Run the application.
    const int status = g_application_run(G_APPLICATION(data_application->application), argc, argv);
    g_object_unref(data_application->application);

    /* CLEANUP */

    // Write the current program options to the configuration file and close the keyfile object.
    airpad_options_write(data_application->data_options);
    airpad_options_close();

    // Destroy the undo buffer.
    airpad_undo_reset();

    // Free the options.
    g_free(data_application->data_options->font);

    // Destroy the search data.
    g_free(data_application->data_search->text_find);
    g_free(data_application->data_search->text_replace);

    // Release the default encoding.
    g_free(data_application->data_arguments->file_encoding);

    // Destroy the current file, if there is one.
    if (data_application->data_file->file)
    {
        g_object_unref(data_application->data_file->file);
        g_free(data_application->data_file->encoding);
    }

    // Destroy the icon copy.
    if (data_application->data_window->icon)
        g_object_unref(data_application->data_window->icon);

    // Release the application data.
    airpad_data_application_free(data_application);

    // Return the status of running this program.
    return status;
}
