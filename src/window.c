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
#include <string.h>
#include <gtk/gtk.h>
#include "const.h"

#define AIRPAD_WINDOW_TITLE     AIRPAD_INFO_NAME
#define AIRPAD_WINDOW_WIDTH     640
#define AIRPAD_WINDOW_HEIGHT    480

// Creates the menubar for the window.
static void airpad_window_menu_bar_create(struct AirpadDataMenuBar *data_menu_bar, const struct AirpadDataOptions *data_options)
{
    // Create the menubar itself.
    data_menu_bar->menu_bar = gtk_menu_bar_new();

    // Create the menus.
    data_menu_bar->menu_file = gtk_menu_new();
    data_menu_bar->menu_edit = gtk_menu_new();
    data_menu_bar->menu_search = gtk_menu_new();
    data_menu_bar->menu_view = gtk_menu_new();
    data_menu_bar->menu_options = gtk_menu_new();
    data_menu_bar->menu_options_text_wrap = gtk_menu_new();
    data_menu_bar->menu_options_scrollbar_policy = gtk_menu_new();
    data_menu_bar->menu_help = gtk_menu_new();

    /* MENU ITEMS */

    GSList *radio_group;    // Holds radio groups while building the interface.

    // Submenu buttons.
    data_menu_bar->menu_item_file = gtk_menu_item_new_with_mnemonic(_("_File"));
    data_menu_bar->menu_item_edit = gtk_menu_item_new_with_mnemonic(_("_Edit"));
    data_menu_bar->menu_item_search = gtk_menu_item_new_with_mnemonic(_("_Search"));
    data_menu_bar->menu_item_view = gtk_menu_item_new_with_mnemonic(_("_View"));
    data_menu_bar->menu_item_options = gtk_menu_item_new_with_mnemonic(_("_Options"));
    data_menu_bar->menu_item_help = gtk_menu_item_new_with_mnemonic(_("_Help"));

    // "File" buttons.
    data_menu_bar->menu_item_file_new = gtk_menu_item_new_with_mnemonic(_("_New"));
    data_menu_bar->menu_item_file_open = gtk_menu_item_new_with_mnemonic(_("_Open\u2026"));
    data_menu_bar->menu_item_file_save = gtk_menu_item_new_with_mnemonic(_("_Save"));
    data_menu_bar->menu_item_file_save_as = gtk_menu_item_new_with_mnemonic(_("Save _As\u2026"));
    data_menu_bar->menu_item_file_close = gtk_menu_item_new_with_mnemonic(_("_Close"));

    // "Edit" buttons.
    data_menu_bar->menu_item_edit_copy = gtk_menu_item_new_with_mnemonic(_("_Copy"));
    data_menu_bar->menu_item_edit_paste = gtk_menu_item_new_with_mnemonic(_("_Paste"));
    data_menu_bar->menu_item_edit_cut = gtk_menu_item_new_with_mnemonic(_("C_ut"));
    data_menu_bar->menu_item_edit_delete = gtk_menu_item_new_with_mnemonic(_("_Delete"));
    data_menu_bar->menu_item_edit_select_all = gtk_menu_item_new_with_mnemonic(_("_Select All"));
    data_menu_bar->menu_item_edit_undo = gtk_menu_item_new_with_mnemonic(_("_Undo"));
    data_menu_bar->menu_item_edit_redo = gtk_menu_item_new_with_mnemonic(_("_Redo"));

    // "Search" buttons.
    data_menu_bar->menu_item_search_find = gtk_menu_item_new_with_mnemonic(_("_Find\u2026"));
    data_menu_bar->menu_item_search_find_next = gtk_menu_item_new_with_mnemonic(_("Find _Next"));
    data_menu_bar->menu_item_search_find_prev = gtk_menu_item_new_with_mnemonic(_("Find _Previous"));
    data_menu_bar->menu_item_search_replace = gtk_menu_item_new_with_mnemonic(_("_Replace\u2026"));

    // "View" buttons.
    data_menu_bar->menu_item_view_fullscreen = gtk_check_menu_item_new_with_mnemonic(_("_Fullscreen"));

    // "Options" buttons.
    data_menu_bar->menu_item_options_font = gtk_menu_item_new_with_mnemonic(_("_Font\u2026"));
    data_menu_bar->menu_item_options_append_newline = gtk_check_menu_item_new_with_mnemonic(_("Append _Newline"));
    data_menu_bar->menu_item_options_text_wrap = gtk_menu_item_new_with_mnemonic(_("Text _Wrap"));
    data_menu_bar->menu_item_options_text_wrap_none = gtk_radio_menu_item_new_with_mnemonic(NULL, _("_None"));
    radio_group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(data_menu_bar->menu_item_options_text_wrap_none));
    data_menu_bar->menu_item_options_text_wrap_on_char = gtk_radio_menu_item_new_with_mnemonic(radio_group, _("On _Character"));
    data_menu_bar->menu_item_options_text_wrap_on_word = gtk_radio_menu_item_new_with_mnemonic(radio_group, _("On _Word"));
    data_menu_bar->menu_item_options_text_wrap_on_char_and_word = gtk_radio_menu_item_new_with_mnemonic(radio_group, _("On Character _And Word"));
    data_menu_bar->menu_item_options_overlay_scrolling = gtk_check_menu_item_new_with_mnemonic(_("_Overlay Scrolling"));
    data_menu_bar->menu_item_options_scrollbar_policy = gtk_menu_item_new_with_mnemonic(_("Scrollbar _Policy"));
    data_menu_bar->menu_item_options_scrollbar_policy_auto = gtk_radio_menu_item_new_with_mnemonic(NULL, _("_Automatic"));
    radio_group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(data_menu_bar->menu_item_options_scrollbar_policy_auto));
    data_menu_bar->menu_item_options_scrollbar_policy_always = gtk_radio_menu_item_new_with_mnemonic(radio_group, _("A_lways"));
    data_menu_bar->menu_item_options_scrollbar_policy_never = gtk_radio_menu_item_new_with_mnemonic(radio_group, _("_Never"));

    // "Help" buttons.
    data_menu_bar->menu_item_help_about = gtk_menu_item_new_with_mnemonic(_("_About"));

    // Attach the menus to items.
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(data_menu_bar->menu_item_file), data_menu_bar->menu_file);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(data_menu_bar->menu_item_edit), data_menu_bar->menu_edit);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(data_menu_bar->menu_item_search), data_menu_bar->menu_search);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(data_menu_bar->menu_item_view), data_menu_bar->menu_view);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(data_menu_bar->menu_item_options), data_menu_bar->menu_options);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(data_menu_bar->menu_item_options_text_wrap), data_menu_bar->menu_options_text_wrap);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(data_menu_bar->menu_item_options_scrollbar_policy), data_menu_bar->menu_options_scrollbar_policy);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(data_menu_bar->menu_item_help), data_menu_bar->menu_help);

    // Attach the items to menus.
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_file), data_menu_bar->menu_item_file_new);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_file), data_menu_bar->menu_item_file_open);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_file), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_file), data_menu_bar->menu_item_file_save);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_file), data_menu_bar->menu_item_file_save_as);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_file), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_file), data_menu_bar->menu_item_file_close);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_edit), data_menu_bar->menu_item_edit_copy);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_edit), data_menu_bar->menu_item_edit_paste);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_edit), data_menu_bar->menu_item_edit_cut);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_edit), data_menu_bar->menu_item_edit_delete);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_edit), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_edit), data_menu_bar->menu_item_edit_select_all);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_edit), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_edit), data_menu_bar->menu_item_edit_undo);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_edit), data_menu_bar->menu_item_edit_redo);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_search), data_menu_bar->menu_item_search_find);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_search), data_menu_bar->menu_item_search_find_next);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_search), data_menu_bar->menu_item_search_find_prev);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_search), data_menu_bar->menu_item_search_replace);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_view), data_menu_bar->menu_item_view_fullscreen);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_options), data_menu_bar->menu_item_options_font);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_options), data_menu_bar->menu_item_options_append_newline);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_options), data_menu_bar->menu_item_options_text_wrap);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_options_text_wrap), data_menu_bar->menu_item_options_text_wrap_none);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_options_text_wrap), data_menu_bar->menu_item_options_text_wrap_on_char);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_options_text_wrap), data_menu_bar->menu_item_options_text_wrap_on_word);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_options_text_wrap), data_menu_bar->menu_item_options_text_wrap_on_char_and_word);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_options), data_menu_bar->menu_item_options_overlay_scrolling);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_options), data_menu_bar->menu_item_options_scrollbar_policy);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_options_scrollbar_policy), data_menu_bar->menu_item_options_scrollbar_policy_auto);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_options_scrollbar_policy), data_menu_bar->menu_item_options_scrollbar_policy_always);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_options_scrollbar_policy), data_menu_bar->menu_item_options_scrollbar_policy_never);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_help), data_menu_bar->menu_item_help_about);

    // Attach the menus to the menubar.
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_bar), data_menu_bar->menu_item_file);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_bar), data_menu_bar->menu_item_edit);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_bar), data_menu_bar->menu_item_search);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_bar), data_menu_bar->menu_item_view);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_bar), data_menu_bar->menu_item_options);
    gtk_menu_shell_append(GTK_MENU_SHELL(data_menu_bar->menu_bar), data_menu_bar->menu_item_help);

    // Set initial menu item states.
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data_menu_bar->menu_item_options_append_newline), data_options->append_newline);
    switch (data_options->text_wrap)
    {
        case GTK_WRAP_NONE:
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data_menu_bar->menu_item_options_text_wrap_none), TRUE);
            break;
        case GTK_WRAP_CHAR:
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data_menu_bar->menu_item_options_text_wrap_on_char), TRUE);
            break;
        case GTK_WRAP_WORD:
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data_menu_bar->menu_item_options_text_wrap_on_word), TRUE);
            break;
        case GTK_WRAP_WORD_CHAR:
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data_menu_bar->menu_item_options_text_wrap_on_char_and_word), TRUE);
            break;
    }
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data_menu_bar->menu_item_options_overlay_scrolling), data_options->overlay_scrolling);
    switch (data_options->scrollbar_policy)
    {
        case GTK_POLICY_AUTOMATIC:
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data_menu_bar->menu_item_options_scrollbar_policy_auto), TRUE);
            break;
        case GTK_POLICY_ALWAYS:
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data_menu_bar->menu_item_options_scrollbar_policy_always), TRUE);
            break;
        case GTK_POLICY_NEVER:
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data_menu_bar->menu_item_options_scrollbar_policy_never), TRUE);
            break;
        default:
            break;
    }
}

// Creates the text view for the window.
static void airpad_window_text_view_create(struct AirpadDataTextView *data_text_view)
{
    // Build the text view elements...
    data_text_view->text_view = gtk_text_view_new();
    data_text_view->scrolled_window = gtk_scrolled_window_new(NULL, NULL);

    // ...and add them together.
    gtk_container_add(GTK_CONTAINER(data_text_view->scrolled_window), data_text_view->text_view);
}

// Adds accelerators to the window.
static void airpad_window_add_accelerators(const struct AirpadDataWindow *data_window)
{
    // Create the accelerator group and add it to the window.
    GtkAccelGroup *accel_group = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(data_window->window), accel_group);

    // Set accelerators on the menu items.
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_file_new, "activate", accel_group, GDK_KEY_N, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_file_open, "activate", accel_group, GDK_KEY_O, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_file_save, "activate", accel_group, GDK_KEY_S, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_file_close, "activate", accel_group, GDK_KEY_Q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_edit_copy, "activate", accel_group, GDK_KEY_C, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_edit_paste, "activate", accel_group, GDK_KEY_V, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_edit_cut, "activate", accel_group, GDK_KEY_X, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_edit_select_all, "activate", accel_group, GDK_KEY_A, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_edit_undo, "activate", accel_group, GDK_KEY_Z, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_edit_redo, "activate", accel_group, GDK_KEY_Z, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_search_find, "activate", accel_group, GDK_KEY_F, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_search_find_next, "activate", accel_group, GDK_KEY_G, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_search_find_prev, "activate", accel_group, GDK_KEY_G, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_search_replace, "activate", accel_group, GDK_KEY_R, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(data_window->data_menu_bar->menu_item_view_fullscreen, "activate", accel_group, GDK_KEY_F11, 0, GTK_ACCEL_VISIBLE);
}

// Loads and sets the window icon.
static void airpad_window_set_icon(struct AirpadDataWindow *data_window)
{
    if ((data_window->icon = gtk_icon_theme_load_icon(gtk_icon_theme_get_default(), AIRPAD_EXEC_NAME, 48, GTK_ICON_LOOKUP_FORCE_SIZE, NULL)))
        gtk_window_set_icon(GTK_WINDOW(data_window->window), data_window->icon);
}

// Updates the state of the window elements when the window state changes.
gboolean airpad_window_window_state(GtkWidget *widget, GdkEventWindowState *event, const struct AirpadDataWindow *data_window)
{
    if (event->changed_mask & GDK_WINDOW_STATE_FULLSCREEN)
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data_window->data_menu_bar->menu_item_view_fullscreen), event->new_window_state & GDK_WINDOW_STATE_FULLSCREEN);

    return FALSE;
}

// Updates the state of the window elements when the window becomes focused.
void airpad_window_focus_in(GtkWidget *widget, GdkEvent *event, const struct AirpadDataWindow *data_window)
{
    gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_edit_paste, gtk_clipboard_wait_is_text_available(gtk_clipboard_get(GDK_SELECTION_CLIPBOARD)));
}

// Updates the state of the window when a text is copied.
void airpad_window_copy_clipboard(GtkWidget *widget, const struct AirpadDataWindow *data_window)
{
    gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_edit_paste, gtk_clipboard_wait_is_text_available(gtk_clipboard_get(GDK_SELECTION_CLIPBOARD)));
}

// Updates the state of the window when a text is cut.
void airpad_window_cut_clipboard(GtkWidget *widget, const struct AirpadDataWindow *data_window)
{
    gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_edit_paste, gtk_clipboard_wait_is_text_available(gtk_clipboard_get(GDK_SELECTION_CLIPBOARD)));
}

// Creates the window, its elements and attaches them all together.
void airpad_window_create(struct AirpadDataWindow *data_window, GtkApplication *application, const struct AirpadDataOptions *data_options)
{
    // Create the window itself.
    data_window->window = gtk_application_window_new(application);
    gtk_window_set_default_size(GTK_WINDOW(data_window->window), AIRPAD_WINDOW_WIDTH, AIRPAD_WINDOW_HEIGHT);

    // Set the window icon.
    airpad_window_set_icon(data_window);

    // Create the window elements.
    airpad_window_menu_bar_create(data_window->data_menu_bar, data_options);
    airpad_window_text_view_create(data_window->data_text_view);
    data_window->status_bar = gtk_label_new("Unsaved | Writable | IT | 0 words | Ln 1 Col 1 | UTF-8 | TXT | 0 KB");

    // Attach the window elements to the contents grid.
    data_window->content = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(data_window->content), data_window->data_menu_bar->menu_bar, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(data_window->content), data_window->data_text_view->scrolled_window, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(data_window->content), data_window->status_bar, 0, 2, 1, 1);

    // Add the window accelerators.
    airpad_window_add_accelerators(data_window);

    // Set the expansion properties of the elements.
    gtk_widget_set_hexpand(data_window->data_menu_bar->menu_bar, TRUE);
    gtk_widget_set_hexpand(data_window->data_text_view->text_view, TRUE);
    gtk_widget_set_vexpand(data_window->data_text_view->text_view, TRUE);

    // Attach the grid to the window.
    gtk_container_add(GTK_CONTAINER(data_window->window), data_window->content);
}

// Accepts the current file as the argument, and if it is non-NULL, sets the window title to
// its path, otherwise to AIRPAD_WINDOW_TITLE.
void airpad_window_set_title(const struct AirpadDataWindow *data_window, GFile *current_file)
{
    if (current_file)
    {
        GError *error = NULL;
        char *filename = g_filename_to_utf8(g_file_peek_path(current_file), -1, NULL, NULL, &error);

        if (!filename)
        {
            g_printerr(_("Conversion of filename \"%s\" to UTF-8 failed: %s\n"), g_file_peek_path(current_file), error->message);
            g_error_free(error);

            gtk_window_set_title(GTK_WINDOW(data_window->window), _("Unknown filename"));
        }
        else
        {
            gtk_window_set_title(GTK_WINDOW(data_window->window), filename);
            g_free(filename);
        }
    }
    else
        gtk_window_set_title(GTK_WINDOW(data_window->window), AIRPAD_WINDOW_TITLE);
}

static unsigned int airpad_window_count_words(GtkTextBuffer *text_buffer)
{
    GtkTextIter start;
    GtkTextIter end;
    gtk_text_buffer_get_bounds(text_buffer, &start, &end);

    char *text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    unsigned int words = 0;
    gboolean inside_word = FALSE;

    for (const char *p = text; p && *p; p = g_utf8_next_char(p))
    {
        gunichar ch = g_utf8_get_char(p);

        if (g_unichar_isspace(ch))
        {
            inside_word = FALSE;
        }
        else if (!inside_word)
        {
            inside_word = TRUE;
            ++words;
        }
    }

    g_free(text);
    return words;
}

static char *airpad_window_format_text_size(GtkTextBuffer *text_buffer)
{
    GtkTextIter start;
    GtkTextIter end;
    gtk_text_buffer_get_bounds(text_buffer, &start, &end);

    char *text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);
    const gsize bytes = strlen(text);
    const unsigned int kb = bytes == 0 ? 0 : (unsigned int)((bytes + 1023) / 1024);

    char *result = g_strdup_printf("%u KB", kb);
    g_free(text);

    return result;
}


void airpad_window_update_status_bar(GtkTextBuffer *text_buffer, const struct AirpadDataWindow *data_window)
{
    const gboolean modified = gtk_text_buffer_get_modified(text_buffer);
    const unsigned int words = airpad_window_count_words(text_buffer);

    GtkTextIter cursor;
    GtkTextMark *insert_mark = gtk_text_buffer_get_insert(text_buffer);
    gtk_text_buffer_get_iter_at_mark(text_buffer, &cursor, insert_mark);

    const int line = gtk_text_iter_get_line(&cursor) + 1;
    const int column = gtk_text_iter_get_line_offset(&cursor) + 1;

    char *text_size = airpad_window_format_text_size(text_buffer);

    char *status = g_strdup_printf(
        "%s | Writable | IT | %u %s | Ln %d Col %d | UTF-8 | TXT | %s",
        modified ? "Unsaved" : "Saved",
        words,
        words == 1 ? "word" : "words",
        line,
        column,
        text_size
    );

    g_free(text_size);

    gtk_label_set_text(GTK_LABEL(data_window->status_bar), status);
    g_free(status);
}



void airpad_window_text_buffer_mark_set(GtkTextBuffer *text_buffer, GtkTextIter *location, GtkTextMark *mark, const struct AirpadDataWindow *data_window)
{
    (void)location;

    if (mark != gtk_text_buffer_get_insert(text_buffer))
        return;

    airpad_window_update_status_bar(text_buffer, data_window);
}


// Shows modified document state explicitly in the window title.
void airpad_window_set_title_modified(const struct AirpadDataWindow *data_window, gboolean modified)
{
    const char *title = gtk_window_get_title(GTK_WINDOW(data_window->window));

    if (!title)
        return;

    const char *suffix = " [Unsaved]";
    const size_t suffix_len = strlen(suffix);
    const size_t title_len = strlen(title);

    const gboolean has_suffix = title_len >= suffix_len && strcmp(title + title_len - suffix_len, suffix) == 0;

    if (modified && !has_suffix)
    {
        char *new_title = g_strconcat(title, suffix, NULL);
        gtk_window_set_title(GTK_WINDOW(data_window->window), new_title);
        g_free(new_title);
    }
    else if (!modified && has_suffix)
    {
        char *new_title = g_strndup(title, title_len - suffix_len);
        gtk_window_set_title(GTK_WINDOW(data_window->window), new_title);
        g_free(new_title);
    }
}

// Overrides the font of the text view.
void airpad_window_set_font(const struct AirpadDataWindow *data_window, const char *font)
{
    PangoFontDescription *font_desc = pango_font_description_from_string(font);
    gtk_widget_override_font(data_window->data_text_view->text_view, font_desc);
    pango_font_description_free(font_desc);
}

// Turns on/off overlaying during scrolling.
void airpad_window_set_overlay_scrolling(const struct AirpadDataWindow *data_window, gboolean state)
{
    gtk_scrolled_window_set_overlay_scrolling(GTK_SCROLLED_WINDOW(data_window->data_text_view->scrolled_window), state);
}

// Sets the text wrapping mode.
void airpad_window_set_text_wrap_mode(const struct AirpadDataWindow *data_window, GtkWrapMode wrap_mode)
{
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(data_window->data_text_view->text_view), wrap_mode);
}

// Sets the scrollbar policy.
void airpad_window_set_scrollbar_policy(const struct AirpadDataWindow *data_window, GtkPolicyType policy)
{
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(data_window->data_text_view->scrolled_window), policy, policy);
}
