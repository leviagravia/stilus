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
#include "const.h"
#include "dialog.h"

// Enables/disables the find button depending on whether there is some content in the find entry.
static void airpad_find_set_find_button_state(GtkEditable *entry, GtkWidget *button)
{
    gtk_widget_set_sensitive(button, gtk_entry_get_text_length(GTK_ENTRY(entry)));
}

// Triggers the user accepting the operation when he presses "Enter"
// while the text entry is focused.
static void airpad_find_entry_activated(GtkEntry *entry, GtkWidget *dialog)
{
    gtk_dialog_response(GTK_DIALOG(dialog), GTK_RESPONSE_ACCEPT);
}

// Causes the find entry to grab the focus and expands the find entry's selection
// bound to the entire text. This function should be called when the dialog has
// been mapped.
static gboolean airpad_find_focus_entry(GtkWidget *dialog, GdkEvent *event, GtkWidget *entry)
{
    gtk_widget_grab_focus(entry);

    return FALSE;
}

// Displays a find dialog to the user.
// Returns TRUE if the user clicked "Find" and FALSE otherwise.
static gboolean airpad_find_dialog_find(GtkWidget *parent, struct AirpadDataSearch *data_search)
{
    // Create the dialog itself.
    GtkWidget *dialog = gtk_dialog_new_with_buttons(_("Find"), GTK_WINDOW(parent), GTK_DIALOG_MODAL, _("Cancel"), GTK_RESPONSE_CANCEL, _("Find"), GTK_RESPONSE_ACCEPT, NULL);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(dialog), 5);

    // Create the dialog contents.
    GtkWidget *grid_contents = gtk_grid_new();
    GtkWidget *label_find = gtk_label_new(_("Find what:"));
    GtkWidget *entry_find = gtk_entry_new();
    GtkWidget *check_button_case_insensitive = gtk_check_button_new_with_label(_("Case insensitive"));
    GtkWidget *check_button_backward_search = gtk_check_button_new_with_label(_("Backward search"));
    GtkWidget *check_button_from_here = gtk_check_button_new_with_label(_("From here"));
    GtkWidget *check_button_search_wraps = gtk_check_button_new_with_label(_("Search wraps"));

    // Get the "Find" button.
    GtkWidget *button_find = gtk_dialog_get_widget_for_response(GTK_DIALOG(dialog), GTK_RESPONSE_ACCEPT);

    // Configure the dialog contents.
    gtk_grid_set_row_spacing(GTK_GRID(grid_contents), 2);
    gtk_grid_set_column_spacing(GTK_GRID(grid_contents), 2);
    gtk_container_set_border_width(GTK_CONTAINER(grid_contents), 5);
    if (data_search->text_find)
        gtk_entry_set_text(GTK_ENTRY(entry_find), data_search->text_find);
    else
        gtk_widget_set_sensitive(button_find, FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button_case_insensitive), data_search->case_insensitive);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button_backward_search), data_search->backward_search);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button_from_here), data_search->from_here);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button_search_wraps), data_search->search_wraps);

    // Connect the events.
    g_signal_connect_after(dialog, "map-event", G_CALLBACK(&airpad_find_focus_entry), entry_find);
    g_signal_connect_after(entry_find, "changed", G_CALLBACK(&airpad_find_set_find_button_state), button_find);
    g_signal_connect_after(entry_find, "activate", G_CALLBACK(&airpad_find_entry_activated), dialog);

    // Attach the dialog contents together.
    gtk_grid_attach(GTK_GRID(grid_contents), label_find, 0, 0, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(grid_contents), entry_find, label_find, GTK_POS_RIGHT, 2, 1);
    gtk_grid_attach_next_to(GTK_GRID(grid_contents), check_button_case_insensitive, label_find, GTK_POS_BOTTOM, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(grid_contents), check_button_backward_search, check_button_case_insensitive, GTK_POS_RIGHT, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(grid_contents), check_button_from_here, check_button_case_insensitive, GTK_POS_BOTTOM, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(grid_contents), check_button_search_wraps, check_button_from_here, GTK_POS_RIGHT, 1, 1);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), grid_contents, TRUE, TRUE, 0);

    gtk_widget_show_all(grid_contents);

    // Run the dialog.
    const int response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_DELETE_EVENT || response == GTK_RESPONSE_CANCEL)
    {
        gtk_widget_destroy(dialog);
        return FALSE;
    }

    // Release the old and save new values.
    g_free(data_search->text_find);
    data_search->text_find = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_find)));
    data_search->case_insensitive = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_case_insensitive));
    data_search->backward_search = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_backward_search));
    data_search->from_here = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_from_here));
    data_search->search_wraps = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_search_wraps));

    gtk_widget_destroy(dialog);

    return TRUE;
}

// Finds the next instance of the string according to the search options and sets the selection bound.
// If `reverse` is on, the previous instance is found.
// Returns whether there was a match.
static gboolean airpad_find_find_instance(GtkTextBuffer *text_buffer, const GtkTextIter *iter_insert, const GtkTextIter *iter_selection_bound, const struct AirpadDataSearch *data_search, GtkTextIter bounds[2], gboolean reverse, gboolean ignore_from_here)
{
    // Compute the direction and the search flags.
    const int search_flags = GTK_TEXT_SEARCH_VISIBLE_ONLY | GTK_TEXT_SEARCH_TEXT_ONLY | (data_search->case_insensitive ? GTK_TEXT_SEARCH_CASE_INSENSITIVE : 0);
    const gboolean backward = reverse ^ data_search->backward_search;

    // Determine the starting position.
    GtkTextIter iter_start;
    if (backward)
    {
        if (data_search->from_here || ignore_from_here)
            iter_start = gtk_text_iter_compare(iter_insert, iter_selection_bound) < 0 ? *iter_insert : *iter_selection_bound;
        else
            gtk_text_buffer_get_end_iter(text_buffer, &iter_start);
    }
    else
    {
        if (data_search->from_here || ignore_from_here)
            iter_start = gtk_text_iter_compare(iter_insert, iter_selection_bound) < 0 ? *iter_selection_bound : *iter_insert;
        else
            gtk_text_buffer_get_start_iter(text_buffer, &iter_start);
    }

    // Search until the beginning/end of the buffer.
    if (backward ? gtk_text_iter_backward_search(&iter_start, data_search->text_find, search_flags, &bounds[0], &bounds[1], NULL) : gtk_text_iter_forward_search(&iter_start, data_search->text_find, search_flags, &bounds[0], &bounds[1], NULL))
        return TRUE;

    // If there was no match, try searching from the opposite end of the file
    // if the search wrap is on.
    if (data_search->search_wraps)
    {
        // Determine the new starting and final positions.
        GtkTextIter iter_end = iter_start;
        if (backward)
        {
            gtk_text_iter_backward_chars(&iter_end, g_utf8_strlen(data_search->text_find, -1) - 1);
            gtk_text_buffer_get_end_iter(text_buffer, &iter_start);
        }
        else
        {
            gtk_text_iter_forward_chars(&iter_end, g_utf8_strlen(data_search->text_find, -1) - 1);
            gtk_text_buffer_get_start_iter(text_buffer, &iter_start);
        }

        // Try performing the search again.
        if (backward ? gtk_text_iter_backward_search(&iter_start, data_search->text_find, search_flags, &bounds[0], &bounds[1], &iter_end) : gtk_text_iter_forward_search(&iter_start, data_search->text_find, search_flags, &bounds[0], &bounds[1], &iter_end))
            return TRUE;
    }

    return FALSE;
}

// Displays a find dialog to the user and searches the text buffer for occurences of the entered text.
void airpad_find_find(GtkWidget *widget, const struct AirpadDataApplication *data_application)
{
    // Get the text buffer.
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view));

    // Get the "insert" and "selection_bound" marks.
    GtkTextMark *mark_insert = gtk_text_buffer_get_insert(text_buffer);
    GtkTextMark *mark_selection_bound = gtk_text_buffer_get_selection_bound(text_buffer);

    // Turn the "insert" and "selection_bound" (before they are (possibly) changed by the dialog) into iterators.
    GtkTextIter iter_insert, iter_selection_bound;
    gtk_text_buffer_get_iter_at_mark(text_buffer, &iter_insert, mark_insert);
    gtk_text_buffer_get_iter_at_mark(text_buffer, &iter_selection_bound, mark_selection_bound);

    // Display the find dialog to the user.
    if (!airpad_find_dialog_find(data_application->data_window->window, data_application->data_search))
    {
        gtk_text_buffer_select_range(text_buffer, &iter_insert, &iter_selection_bound);
        return;
    }

    // Search for the next instance of the entered string according to the search options and select and scroll to the found instance.
    GtkTextIter bounds[2];
    if (airpad_find_find_instance(text_buffer, &iter_insert, &iter_selection_bound, data_application->data_search, bounds, FALSE, FALSE))
    {
        gtk_text_buffer_select_range(text_buffer, &bounds[0], &bounds[1]);
        gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view), mark_insert, 0.0, TRUE, 0.5, 0.5);
    }
    else    // If nothing was found, inform the user.
    {
        airpad_dialog_no_matches_found(data_application->data_window->window);
        gtk_text_buffer_select_range(text_buffer, &iter_insert, &iter_selection_bound);
    }

    // Enable the "Find Next" and "Find Previous" menu items.
    gtk_widget_set_sensitive(data_application->data_window->data_menu_bar->menu_item_search_find_next, TRUE);
    gtk_widget_set_sensitive(data_application->data_window->data_menu_bar->menu_item_search_find_prev, TRUE);
}

// Finds the next instance of the string and selects it.
// Search is performed using the current search data.
void airpad_find_find_next(GtkWidget *widget, const struct AirpadDataApplication *data_application)
{
    // Get the text buffer.
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view));

    // Get the "insert" and "selection_bound" marks.
    GtkTextMark *mark_insert = gtk_text_buffer_get_insert(text_buffer);
    GtkTextMark *mark_selection_bound = gtk_text_buffer_get_selection_bound(text_buffer);

    // Turn the "insert" and "selection_bound" (before they are (possibly) changed by the dialog) into iterators.
    GtkTextIter iter_insert, iter_selection_bound;
    gtk_text_buffer_get_iter_at_mark(text_buffer, &iter_insert, mark_insert);
    gtk_text_buffer_get_iter_at_mark(text_buffer, &iter_selection_bound, mark_selection_bound);

    // Search for the next instance of the entered string according to the search options and select and scroll to the found instance.
    GtkTextIter bounds[2];
    if (airpad_find_find_instance(text_buffer, &iter_insert, &iter_selection_bound, data_application->data_search, bounds, FALSE, TRUE))
    {
        gtk_text_buffer_select_range(text_buffer, &bounds[0], &bounds[1]);
        gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view), mark_insert, 0.0, TRUE, 0.5, 0.5);
    }
    else    // If nothing was found, inform the user.
    {
        airpad_dialog_no_matches_found(data_application->data_window->window);
        gtk_text_buffer_select_range(text_buffer, &iter_insert, &iter_selection_bound);
    }
}

// Finds the previous instance of the string and selects it.
// Search is performed using the current search data, but `backward_search` property
// is reversed.
void airpad_find_find_prev(GtkWidget *widget, const struct AirpadDataApplication *data_application)
{
    // Get the text buffer.
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view));

    // Get the "insert" and "selection_bound" marks.
    GtkTextMark *mark_insert = gtk_text_buffer_get_insert(text_buffer);
    GtkTextMark *mark_selection_bound = gtk_text_buffer_get_selection_bound(text_buffer);

    // Turn the "insert" and "selection_bound" (before they are (possibly) changed by the dialog) into iterators.
    GtkTextIter iter_insert, iter_selection_bound;
    gtk_text_buffer_get_iter_at_mark(text_buffer, &iter_insert, mark_insert);
    gtk_text_buffer_get_iter_at_mark(text_buffer, &iter_selection_bound, mark_selection_bound);

    /* SEARCH */

    // Search for the next instance of the entered string according to the search options and select and scroll to the found instance.
    GtkTextIter bounds[2];
    if (airpad_find_find_instance(text_buffer, &iter_insert, &iter_selection_bound, data_application->data_search, bounds, TRUE, TRUE))
    {
        gtk_text_buffer_select_range(text_buffer, &bounds[0], &bounds[1]);
        gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view), mark_insert, 0.0, TRUE, 0.5, 0.5);
    }
    else    // If nothing was found, inform the user.
    {
        airpad_dialog_no_matches_found(data_application->data_window->window);
        gtk_text_buffer_select_range(text_buffer, &iter_insert, &iter_selection_bound);
    }
}
