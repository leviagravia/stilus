/*
 * Copyright (C) 2019-2022 Nikola Hadžić
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

// Sets sensitivity of the "Copy", "Cut" and "Delete" menu items according to
// whether some text is selected.
void airpad_edit_has_selection(GObject *object, GParamSpec *pspec, const struct AirpadDataMenuBar *data_menu_bar)
{
    const gboolean has_selection = gtk_text_buffer_get_has_selection(GTK_TEXT_BUFFER(object));

    gtk_widget_set_sensitive(data_menu_bar->menu_item_edit_copy, has_selection);
    gtk_widget_set_sensitive(data_menu_bar->menu_item_edit_cut, has_selection);
    gtk_widget_set_sensitive(data_menu_bar->menu_item_edit_delete, has_selection);
}

// Scrolls the text view when the user pastes text.
void airpad_edit_paste_done(GtkTextBuffer *text_buffer, GtkClipboard *clipboard, GtkTextView *text_view)
{
    gtk_text_view_scroll_mark_onscreen(text_view, gtk_text_buffer_get_insert(text_buffer));
}

// Initializes sensitivity of the clipboard widgets.
void airpad_edit_init(const struct AirpadDataWindow *data_window)
{
    // These elements are always disabled when the program starts,
    // since no text can be selected initially.
    gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_edit_copy, FALSE);
    gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_edit_cut, FALSE);
    gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_edit_delete, FALSE);

    // Sensitivity of the "Paste" menu item depends on whether the clipboard is empty or not.
    // Not needed since "focus-in-event" of the top-level window already takes care of this.
    // gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_edit_paste, gtk_clipboard_wait_is_text_available(gtk_clipboard_get(GDK_SELECTION_CLIPBOARD)));
}

// Copies the selected text in the text buffer to the clipboard.
void airpad_edit_copy(GtkWidget *widget, const struct AirpadDataWindow *data_window)
{
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);

    gtk_text_buffer_copy_clipboard(gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_window->data_text_view->text_view)), clipboard);
    gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_edit_paste, gtk_clipboard_wait_is_text_available(clipboard));
}

// Pastes the clipboard contents into the text buffer.
void airpad_edit_paste(GtkWidget *widget, GtkTextBuffer *text_buffer)
{
    gtk_text_buffer_paste_clipboard(text_buffer, gtk_clipboard_get(GDK_SELECTION_CLIPBOARD), NULL, TRUE);
}

// Pastes clipboard text into the text buffer as plain text.
void airpad_edit_paste_as_plain_text(GtkWidget *widget, GtkTextBuffer *text_buffer)
{
    (void) widget;

    char *text = gtk_clipboard_wait_for_text(gtk_clipboard_get(GDK_SELECTION_CLIPBOARD));
    if (!text)
        return;

    gtk_text_buffer_insert_at_cursor(text_buffer, text, -1);
    g_free(text);
}

// Cuts the selected text and copies it to the clipboard.
void airpad_edit_cut(GtkWidget *widget, const struct AirpadDataWindow *data_window)
{
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);

    gtk_text_buffer_cut_clipboard(gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_window->data_text_view->text_view)), clipboard, TRUE);
    gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_edit_paste, gtk_clipboard_wait_is_text_available(clipboard));
}

// Deletes the selected text from the text buffer.
void airpad_edit_delete(GtkWidget *widget, GtkTextBuffer *text_buffer)
{
    gtk_text_buffer_delete_selection(text_buffer, FALSE, TRUE);
}

// Selects the entire text buffer.
void airpad_edit_select_all(GtkWidget *widget, GtkTextBuffer *text_buffer)
{
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);
    gtk_text_buffer_select_range(text_buffer, &start, &end);
}

// Inserts the current local date and time at the cursor position.
void airpad_edit_insert_date_time(GtkWidget *widget, GtkTextBuffer *text_buffer)
{
    (void) widget;

    GDateTime *now = g_date_time_new_now_local();
    if (!now)
        return;

    char *date_time = g_date_time_format(now, "%Y-%m-%d %H:%M");
    g_date_time_unref(now);

    if (!date_time)
        return;

    gtk_text_buffer_insert_at_cursor(text_buffer, date_time, -1);
    g_free(date_time);
}
