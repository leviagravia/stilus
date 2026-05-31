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
#include "undo.h"

// Enables/disables the replace button depending on whether there is some content in the find entry.
static void airpad_replace_set_replace_button_state(GtkEditable *entry, GtkWidget *button)
{
    gtk_widget_set_sensitive(button, gtk_entry_get_text_length(GTK_ENTRY(entry)));
}

// Triggers the user accepting the operation when he presses "Enter" while either of
// the text entries is focused.
static void airpad_replace_entry_activated(GtkEntry *entry, GtkWidget *dialog)
{
    gtk_dialog_response(GTK_DIALOG(dialog), GTK_RESPONSE_ACCEPT);
}

// Causes the replace entry to grab the focus and expands the replace entry's selection
// bound to the entire text. This function should be called when the dialog has
// been mapped.
static gboolean airpad_replace_focus_entry(GtkWidget *dialog, GdkEvent *event, GtkWidget *entry)
{
    gtk_widget_grab_focus(entry);

    return FALSE;
}

// Dialog that comes up when a string instance is about to get replaced.
// Allowed answers are "Yes", "No", "Cancel" and "All".
// User response is returned.
static int airpad_replace_dialog_replace_prompt(GtkWidget *parent)
{
    // Create a dialog.
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_NONE, _("Replace this instance?"));
    gtk_window_set_title(GTK_WINDOW(dialog), _("Replace?"));

    // Add the dialog responses.
    gtk_dialog_add_buttons(GTK_DIALOG(dialog), _("Yes"), GTK_RESPONSE_YES, _("No"), GTK_RESPONSE_NO, _("Cancel"), GTK_RESPONSE_CANCEL, _("All"), GTK_RESPONSE_ACCEPT, NULL);

    // Run the dialog and get the user response.
    const int response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    // Return the user response.
    return response;
}

// Displays a replace dialog to the user.
// Returns TRUE if the user clicked "Replace" and FALSE otherwise.
static gboolean airpad_replace_dialog_replace(GtkWidget *parent, struct AirpadDataSearch *data_search)
{
    // Create the dialog itself.
    GtkWidget *dialog = gtk_dialog_new_with_buttons(_("Replace"), GTK_WINDOW(parent), GTK_DIALOG_MODAL, _("Cancel"), GTK_RESPONSE_CANCEL, _("Replace"), GTK_RESPONSE_ACCEPT, NULL);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(dialog), 5);

    // Create the dialog contents.
    GtkWidget *grid_contents = gtk_grid_new();
    GtkWidget *label_find = gtk_label_new(_("Find what:"));
    GtkWidget *entry_find = gtk_entry_new();
    GtkWidget *label_replace = gtk_label_new(_("Replace with:"));
    GtkWidget *entry_replace = gtk_entry_new();
    GtkWidget *check_button_case_insensitive = gtk_check_button_new_with_label(_("Case insensitive"));
    GtkWidget *check_button_backward_search = gtk_check_button_new_with_label(_("Backward search"));
    GtkWidget *check_button_from_here = gtk_check_button_new_with_label(_("From here"));
    GtkWidget *check_button_replace_all = gtk_check_button_new_with_label(_("Replace all"));

    // Get the "Find" button.
    GtkWidget *button_replace = gtk_dialog_get_widget_for_response(GTK_DIALOG(dialog), GTK_RESPONSE_ACCEPT);

    // Configure the dialog contents.
    gtk_grid_set_row_spacing(GTK_GRID(grid_contents), 2);
    gtk_grid_set_column_spacing(GTK_GRID(grid_contents), 2);
    gtk_container_set_border_width(GTK_CONTAINER(grid_contents), 5);
    if (data_search->text_find)
        gtk_entry_set_text(GTK_ENTRY(entry_find), data_search->text_find);
    else
        gtk_widget_set_sensitive(button_replace, FALSE);
    if (data_search->text_replace)
        gtk_entry_set_text(GTK_ENTRY(entry_replace), data_search->text_replace);
    gtk_widget_set_sensitive(button_replace, (uintptr_t) data_search->text_find);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button_case_insensitive), data_search->case_insensitive);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button_backward_search), data_search->backward_search);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button_from_here), data_search->from_here);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button_replace_all), data_search->replace_all);

    // Connect the events.
    g_signal_connect_after(dialog, "map-event", G_CALLBACK(&airpad_replace_focus_entry), entry_find);
    g_signal_connect_after(entry_find, "changed", G_CALLBACK(&airpad_replace_set_replace_button_state), button_replace);
    g_signal_connect_after(entry_find, "activate", G_CALLBACK(&airpad_replace_entry_activated), dialog);
    g_signal_connect_after(entry_replace, "activate", G_CALLBACK(&airpad_replace_entry_activated), dialog);

    // Attach dialog contents together.
    gtk_grid_attach(GTK_GRID(grid_contents), label_find, 0, 0, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(grid_contents), entry_find, label_find, GTK_POS_RIGHT, 2, 1);
    gtk_grid_attach_next_to(GTK_GRID(grid_contents), label_replace, label_find, GTK_POS_BOTTOM, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(grid_contents), entry_replace, label_replace, GTK_POS_RIGHT, 2, 1);
    gtk_grid_attach_next_to(GTK_GRID(grid_contents), check_button_case_insensitive, label_replace, GTK_POS_BOTTOM, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(grid_contents), check_button_backward_search, check_button_case_insensitive, GTK_POS_RIGHT, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(grid_contents), check_button_from_here, check_button_case_insensitive, GTK_POS_BOTTOM, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(grid_contents), check_button_replace_all, check_button_from_here, GTK_POS_RIGHT, 1, 1);
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
    g_free(data_search->text_replace);
    data_search->text_find = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_find)));
    data_search->text_replace = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_replace)));
    data_search->case_insensitive = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_case_insensitive));
    data_search->backward_search = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_backward_search));
    data_search->from_here = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_from_here));
    data_search->replace_all = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_replace_all));

    gtk_widget_destroy(dialog);

    return TRUE;
}

// Does the replacing.
// Returns TRUE if an instance of the searched for string was found.
static gboolean airpad_replace_replace_instances(const struct AirpadDataApplication *data_application, const GtkTextIter *iter_insert, const GtkTextIter *iter_selection_bound)
{
    // Get the search flags.
    const int search_flags = GTK_TEXT_SEARCH_VISIBLE_ONLY | GTK_TEXT_SEARCH_TEXT_ONLY | (data_application->data_search->case_insensitive ? GTK_TEXT_SEARCH_CASE_INSENSITIVE : 0);

    // Get the text buffer.
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view));

    // `replace_all` flag, shorter version.
    gboolean replace_all = data_application->data_search->replace_all;

    // Get the starting position.
    GtkTextIter iter_start;
    if (data_application->data_search->backward_search)
    {
        if (data_application->data_search->from_here)
            iter_start = gtk_text_iter_compare(iter_insert, iter_selection_bound) < 0 ? *iter_insert : *iter_selection_bound;
        else
            gtk_text_buffer_get_end_iter(text_buffer, &iter_start);
    }
    else
    {
        if (data_application->data_search->from_here)
            iter_start = gtk_text_iter_compare(iter_insert, iter_selection_bound) < 0 ? *iter_selection_bound : *iter_insert;
        else
            gtk_text_buffer_get_start_iter(text_buffer, &iter_start);
    }

    // Get the quantity that needs to be added to/subtracted from the pointers whenever a string is replaced.
    const int replace_adjustment = g_utf8_strlen(data_application->data_search->text_replace, -1) - g_utf8_strlen(data_application->data_search->text_find, -1);

    // Holds whether there was a match.
    gboolean matched = FALSE;

    // Get the cursor position. Revert the cursor to this position if `replace_all` is/becomes true.
    int last_cursor_pos;
    g_object_get(text_buffer, "cursor-position", &last_cursor_pos, NULL);

    // If the user chose to replace all found instances, mark the entire replacement process
    // as an undo unit.
    if (replace_all)
        airpad_undo_mark_unit_next();

    // Search for strings to replace.
    for (GtkTextIter bounds[2];data_application->data_search->backward_search ? gtk_text_iter_backward_search(&iter_start, data_application->data_search->text_find, search_flags, &bounds[0], &bounds[1], NULL) : gtk_text_iter_forward_search(&iter_start, data_application->data_search->text_find, search_flags, &bounds[0], &bounds[1], NULL);)
    {
        matched = TRUE;

        if (!replace_all)
        {
            gtk_text_buffer_select_range(text_buffer, &bounds[0], &bounds[1]);
            gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view), gtk_text_buffer_get_insert(text_buffer), 0.0, TRUE, 0.5, 0.1);
            last_cursor_pos = gtk_text_iter_get_offset(&bounds[0]);

            switch (airpad_replace_dialog_replace_prompt(data_application->data_window->window))
            {
                case GTK_RESPONSE_DELETE_EVENT:
                case GTK_RESPONSE_CANCEL:
                    return TRUE;
                case GTK_RESPONSE_NO:
                    iter_start = data_application->data_search->backward_search ? bounds[0] : bounds[1];
                    continue;
                case GTK_RESPONSE_ACCEPT:
                    replace_all = TRUE;
                case GTK_RESPONSE_YES:
                    // If the replacement has been confirmed, mark this replace operation as a unit.
                    // What constitutes the replace operation is determined by whether the user chose
                    // to replace all subsequent strings or just the current one.
                    airpad_undo_mark_unit_next();
                    break;
            }
        }
        else if (last_cursor_pos > gtk_text_iter_get_offset(&bounds[0]))
            last_cursor_pos += replace_adjustment;

        // Save the found text's bounds as offsets.
        const int pos_bounds[2] = {gtk_text_iter_get_offset(&bounds[0]), gtk_text_iter_get_offset(&bounds[1])};

        // Replace the found instance with the replacement text.
        gtk_text_buffer_delete(text_buffer, &bounds[0], &bounds[1]);
        gtk_text_buffer_place_cursor(text_buffer, &bounds[0]);
        gtk_text_buffer_insert_at_cursor(text_buffer, data_application->data_search->text_replace, -1);

        // If the user chose to replace just the current string, mark the previous buffer as the end buffer.
        if (!replace_all)
        {
            airpad_undo_flush();
            airpad_undo_mark_unit_prev();
        }

        // Advance the start pointer.
        gtk_text_buffer_get_iter_at_offset(text_buffer, &iter_start, data_application->data_search->backward_search ? pos_bounds[0] : pos_bounds[1] + replace_adjustment);
    }
    
    // First save the modified buffer fully and then restore the cursor
    // position to the last position when the user had interacted with
    // the program.
    if (matched)
    {
        // If the user chose to replace all the subsequent instances, mark the previous buffer as the end buffer.
        if (replace_all)
        {
            airpad_undo_flush();
            airpad_undo_mark_unit_prev();
        }

        // Restore the cursor.
        GtkTextIter iter_last_cursor_pos;
        gtk_text_buffer_get_iter_at_offset(text_buffer, &iter_last_cursor_pos, last_cursor_pos);
        gtk_text_buffer_place_cursor(text_buffer, &iter_last_cursor_pos);
    }

    return matched;
}

// Displays a replace dialog to the user and replaces occurences of the entered text in the text buffer.
void airpad_replace_replace(GtkWidget *widget, const struct AirpadDataApplication *data_application)
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

    // Display the replace dialog to the user.
    if (!airpad_replace_dialog_replace(data_application->data_window->window, data_application->data_search))
    {
        gtk_text_buffer_select_range(text_buffer, &iter_insert, &iter_selection_bound);
        return;
    }

    // Replace the instance strings, optionally presenting a dialog to the user that asks him
    // what to do next.
    if (!airpad_replace_replace_instances(data_application, &iter_insert, &iter_selection_bound))
    {
        airpad_dialog_no_matches_found(data_application->data_window->window);
        gtk_text_buffer_select_range(text_buffer, &iter_insert, &iter_selection_bound);
    }

    // Enable the "Find Next" and "Find Previous" menu items.
    gtk_widget_set_sensitive(data_application->data_window->data_menu_bar->menu_item_search_find_next, TRUE);
    gtk_widget_set_sensitive(data_application->data_window->data_menu_bar->menu_item_search_find_prev, TRUE);
}
