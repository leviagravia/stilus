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

static GKeyFile *key_file;

// Returns the filename of the configuration file.
// Caller is responsible for freeing.
static inline char *airpad_options_get_config_filename()
{
    char *config_filename_base = g_filename_from_utf8(AIRPAD_EXEC_NAME ".ini", -1, NULL, NULL, NULL);
    char *config_filename = g_build_filename(g_get_user_config_dir(), config_filename_base, NULL);
    g_free(config_filename_base);

    return config_filename;
}

// Updates the "Text Wrap" option.
void airpad_options_update_text_wrap(GtkWidget *widget, const struct AirpadDataApplication *data_application)
{
    if (widget == data_application->data_window->data_menu_bar->menu_item_options_text_wrap_none)
        data_application->data_options->text_wrap = GTK_WRAP_NONE;
    else if (widget == data_application->data_window->data_menu_bar->menu_item_options_text_wrap_on_char)
        data_application->data_options->text_wrap = GTK_WRAP_CHAR;
    else if (widget == data_application->data_window->data_menu_bar->menu_item_options_text_wrap_on_word)
        data_application->data_options->text_wrap = GTK_WRAP_WORD;
    else if (widget == data_application->data_window->data_menu_bar->menu_item_options_text_wrap_on_char_and_word)
        data_application->data_options->text_wrap = GTK_WRAP_WORD_CHAR;

    airpad_window_set_text_wrap_mode(data_application->data_window, data_application->data_options->text_wrap);
}

// Updates the "Overlay Scrollbars" option.
void airpad_options_update_overlay_scrolling(GtkWidget *widget, const struct AirpadDataApplication *data_application)
{
    data_application->data_options->overlay_scrolling = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
    airpad_window_set_overlay_scrolling(data_application->data_window, data_application->data_options->overlay_scrolling);
}

// Updates the "Scrollbar Policy" option.
void airpad_options_update_scrollbar_policy(GtkWidget *widget, const struct AirpadDataApplication *data_application)
{
    if (widget == data_application->data_window->data_menu_bar->menu_item_options_scrollbar_policy_auto)
        data_application->data_options->scrollbar_policy = GTK_POLICY_AUTOMATIC;
    else if (widget == data_application->data_window->data_menu_bar->menu_item_options_scrollbar_policy_always)
        data_application->data_options->scrollbar_policy = GTK_POLICY_ALWAYS;
    else if (widget == data_application->data_window->data_menu_bar->menu_item_options_scrollbar_policy_never)
        data_application->data_options->scrollbar_policy = GTK_POLICY_NEVER;

    airpad_window_set_scrollbar_policy(data_application->data_window, data_application->data_options->scrollbar_policy);
}

// Creates the keyfile object and loads the configuration file.
void airpad_options_open()
{
    key_file = g_key_file_new();

    char *config_filename = airpad_options_get_config_filename();

    GError *error = NULL;
    if (!g_key_file_load_from_file(key_file, config_filename, G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS, &error))
    {
        g_printerr(_("Configuration file could not be loaded: %s\n"), error->message);
        g_error_free(error);
    }

    g_free(config_filename);
}

// Saves the current options to the configuration file and frees the keyfile object.
void airpad_options_close()
{
    char *config_filename = airpad_options_get_config_filename();

    GError *error = NULL;
    if (!g_key_file_save_to_file(key_file, config_filename, &error))
    {
        g_printerr(_("Could not save configuration file: %s\n"), error->message);
        g_error_free(error);
    }

    g_free(config_filename);
    g_key_file_free(key_file);
}

// Sets the `data_options` fields to the data stored in the keyfile.
void airpad_options_read(struct AirpadDataOptions *data_options)
{
#define AIRPAD_OPTIONS_READ_VALUE(type, value, key, def)  value = g_key_file_has_key(key_file, "general", key, NULL) ? g_key_file_get_ ## type(key_file, "general", key, NULL) : def;

    AIRPAD_OPTIONS_READ_VALUE(string, data_options->font, "font", NULL)
    AIRPAD_OPTIONS_READ_VALUE(boolean, data_options->overlay_scrolling, "overlay-scrolling", TRUE)
    AIRPAD_OPTIONS_READ_VALUE(integer, data_options->text_wrap, "text-wrap", GTK_WRAP_NONE)
    AIRPAD_OPTIONS_READ_VALUE(integer, data_options->scrollbar_policy, "scrollbar-policy", GTK_POLICY_AUTOMATIC)
    AIRPAD_OPTIONS_READ_VALUE(integer, data_options->word_goal, "word-goal", 0)

#undef AIRPAD_OPTIONS_READ_VALUE
}

// Writes the data stored in `data_options` to the keyfile.
void airpad_options_write(const struct AirpadDataOptions *data_options)
{
#define AIRPAD_OPTIONS_WRITE_VALUE(setter, cond, value, key)    if (cond)                                   \
                                                                    setter(key_file, "general", key, value);

    AIRPAD_OPTIONS_WRITE_VALUE(g_key_file_set_string, data_options->font, data_options->font, "font")
    AIRPAD_OPTIONS_WRITE_VALUE(g_key_file_set_boolean, TRUE, data_options->overlay_scrolling, "overlay-scrolling")
    AIRPAD_OPTIONS_WRITE_VALUE(g_key_file_set_integer, TRUE, data_options->text_wrap, "text-wrap")
    AIRPAD_OPTIONS_WRITE_VALUE(g_key_file_set_integer, TRUE, data_options->scrollbar_policy, "scrollbar-policy")
    AIRPAD_OPTIONS_WRITE_VALUE(g_key_file_set_integer, TRUE, data_options->word_goal, "word-goal")

#undef AIRPAD_OPTIONS_WRITE_VALUE
}
