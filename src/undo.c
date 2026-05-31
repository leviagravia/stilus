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
#include <stdlib.h>
#include <sys/param.h>
#include "const.h"

// Initial buffer size.
#define AIRPAD_UNDO_BUFFER_INIT_SIZE    64

// Type of the user action stored in the undo buffer.
enum AirpadUndoBufferItemType
{
    AIRPAD_UNDO_BUFFER_ITEM_TYPE_NONE,      // No action (initial state).
    AIRPAD_UNDO_BUFFER_ITEM_TYPE_INSERT,    // Text was inserted.
    AIRPAD_UNDO_BUFFER_ITEM_TYPE_DELETE,    // Text was deleted.
    AIRPAD_UNDO_BUFFER_ITEM_TYPE_UNDO,      // Undo was performed.
    AIRPAD_UNDO_BUFFER_ITEM_TYPE_REDO,      // Redo was performed.
    AIRPAD_UNDO_BUFFER_ITEM_TYPE_SAVE,      // Buffer contents have been saved.
    AIRPAD_UNDO_BUFFER_ITEM_TYPE_FLUSH      // Current item saved manually.
};

// Action stored in the undo buffer.
struct AirpadUndoBufferItem
{
    enum AirpadUndoBufferItemType type; // Type of the action.
    char *text;                         // Text that was inserted/deleted.
    int position[2];                    // Position of the cursor when the action occured.
    gboolean unit_marker;               // Marks start/end of the unit.
};

// Undo buffer and its state.
struct AirpadUndoBuffer
{
    struct AirpadUndoBufferItem *buffer;    // Buffer itself.
    unsigned int next;                      // Next position in the buffer to be filled.
    unsigned int end;                       // Last+1 position in the buffer.
    unsigned int size;                      // Allocated size of the buffer.
};

// Undo buffer instance and its helpers.
static struct AirpadUndoBuffer undo_buffer;         // Undo buffer.
static struct AirpadUndoBufferItem current_action;  // Ongoing action.
static unsigned int saved_item_no;                  // Number of the buffer section in the undo buffer that is currently saved to the disk.
static gboolean unit_marker_next;                   // If TRUE, the next buffer item will be marked as a unit. Afterwards, `unit_marker_next` will be reset to FALSE.

// Returns whether the given text is made out of space characters.
static inline gboolean airpad_undo_is_space(const char *text)
{
    for (const char *i = text;*i;++i)
        if (!g_ascii_isspace(*i))
            return FALSE;

    return TRUE;
}

// Resets buffer in accord with its type.
// To be used when the user makes a change while in the middle of the undo buffer list.
static void airpad_undo_reset_buffer()
{
    for (unsigned int i = undo_buffer.next;i < undo_buffer.end;++i)
        g_free(undo_buffer.buffer[i].text);

    undo_buffer.end = undo_buffer.next;
}

// Expands the undo buffer if its end has been reached.
// Pointers will be set appropriately.
static void airpad_undo_expand_buffer()
{
    if (undo_buffer.next == undo_buffer.size)
        undo_buffer.buffer = g_realloc(undo_buffer.buffer, sizeof(struct AirpadUndoBufferItem) * (undo_buffer.size = MAX(undo_buffer.size * 2, AIRPAD_UNDO_BUFFER_INIT_SIZE)));

    undo_buffer.next++;
    undo_buffer.end++;
}

// Modifies the contents of the text buffer so that it matches the previous buffer item
// in the undo buffer.
static void airpad_undo_move_backward(GtkTextBuffer *text_buffer)
{
    GtkTextIter start, end;

    undo_buffer.next--;

    gboolean is_unit = undo_buffer.buffer[undo_buffer.next].unit_marker;

undo:
    switch (undo_buffer.buffer[undo_buffer.next].type)
    {
        case AIRPAD_UNDO_BUFFER_ITEM_TYPE_INSERT:
        {
            gtk_text_buffer_get_iter_at_offset(text_buffer, &start, undo_buffer.buffer[undo_buffer.next].position[0]);
            gtk_text_buffer_get_iter_at_offset(text_buffer, &end, undo_buffer.buffer[undo_buffer.next].position[1]);
            gtk_text_buffer_delete(text_buffer, &start, &end);
            gtk_text_buffer_get_iter_at_offset(text_buffer, &start, undo_buffer.buffer[undo_buffer.next].position[0]);
            gtk_text_buffer_place_cursor(text_buffer, &start);
        }
        break;
        case AIRPAD_UNDO_BUFFER_ITEM_TYPE_DELETE:
        {
            gtk_text_buffer_get_iter_at_offset(text_buffer, &start, undo_buffer.buffer[undo_buffer.next].position[0]);
            gtk_text_buffer_insert(text_buffer, &start, undo_buffer.buffer[undo_buffer.next].text, -1);
            gtk_text_buffer_get_iter_at_offset(text_buffer, &start, undo_buffer.buffer[undo_buffer.next].position[0]);
            gtk_text_buffer_get_iter_at_offset(text_buffer, &end, undo_buffer.buffer[undo_buffer.next].position[1]);
            gtk_text_buffer_select_range(text_buffer, &start, &end);
        }
        break;
        default:
            break;
    }

    if (is_unit)
    {
        if (undo_buffer.buffer[--undo_buffer.next].unit_marker)
            is_unit = FALSE;

        goto undo;
    }
}

// Modifies contents of the text buffer so that it matches the next buffer item
// in the undo buffer.
static void airpad_undo_move_forward(GtkTextBuffer *text_buffer)
{
    GtkTextIter start, end;

    gboolean is_unit = undo_buffer.buffer[undo_buffer.next].unit_marker;

redo:
    switch (undo_buffer.buffer[undo_buffer.next].type)
    {
        case AIRPAD_UNDO_BUFFER_ITEM_TYPE_INSERT:
        {
            gtk_text_buffer_get_iter_at_offset(text_buffer, &start, undo_buffer.buffer[undo_buffer.next].position[0]);
            gtk_text_buffer_insert(text_buffer, &start, undo_buffer.buffer[undo_buffer.next].text, -1);
            gtk_text_buffer_get_iter_at_offset(text_buffer, &start, undo_buffer.buffer[undo_buffer.next].position[0]);
            gtk_text_buffer_get_iter_at_offset(text_buffer, &end, undo_buffer.buffer[undo_buffer.next].position[1]);
            gtk_text_buffer_select_range(text_buffer, &start, &end);
        }
        break;
        case AIRPAD_UNDO_BUFFER_ITEM_TYPE_DELETE:
        {
            gtk_text_buffer_get_iter_at_offset(text_buffer, &start, undo_buffer.buffer[undo_buffer.next].position[0]);
            gtk_text_buffer_get_iter_at_offset(text_buffer, &end, undo_buffer.buffer[undo_buffer.next].position[1]);
            gtk_text_buffer_delete(text_buffer, &start, &end);
            gtk_text_buffer_get_iter_at_offset(text_buffer, &start, undo_buffer.buffer[undo_buffer.next].position[0]);
            gtk_text_buffer_place_cursor(text_buffer, &start);
        }
        break;
        default:
            break;
    }

    if (is_unit)
    {
        if (undo_buffer.buffer[++undo_buffer.next].unit_marker)
            is_unit = FALSE;

        goto redo;
    }

    undo_buffer.next++;
}

// Updates the current action item when text is inserted/deleted from the buffer.
static void airpad_undo_update_current_action(enum AirpadUndoBufferItemType type, char *text, int pos_start, int pos_end, gboolean unit_marker)
{
    if (type == current_action.type && !unit_marker)
    {
        switch (type)
        {
            case AIRPAD_UNDO_BUFFER_ITEM_TYPE_INSERT:
            {
                // If text is added to the current action's range, modify the current action's range to include it.
                if (pos_start >= current_action.position[0] && pos_start <= current_action.position[1] && !(airpad_undo_is_space(text) && !airpad_undo_is_space(current_action.text)))
                {
                    char first_part[(pos_start - current_action.position[0]) * 4 + 1];
                    g_utf8_strncpy(first_part, current_action.text, pos_start - current_action.position[0]);
                    *g_utf8_offset_to_pointer(first_part, pos_start - current_action.position[0]) = '\0';

                    char * const current_text_copy = current_action.text;
                    current_action.text = g_strconcat(first_part, text, g_utf8_offset_to_pointer(current_action.text, pos_start - current_action.position[0]), NULL);
                    g_free(current_text_copy);

                    current_action.position[1] += pos_end - pos_start;

                    return;
                }
            }
            break;
            case AIRPAD_UNDO_BUFFER_ITEM_TYPE_DELETE:
            {
                // If text is deleted from the current action's range, modify the current action's range to include it.
                if (pos_start <= current_action.position[0] && pos_end >= current_action.position[0])
                {
                    char first_part[(current_action.position[0] - pos_start) * 4 + 1];
                    g_utf8_strncpy(first_part, text, current_action.position[0] - pos_start);
                    *g_utf8_offset_to_pointer(first_part, current_action.position[0] - pos_start) = '\0';

                    char * const current_text_copy = current_action.text;
                    current_action.text = g_strconcat(first_part, current_action.text, g_utf8_offset_to_pointer(text, current_action.position[0] - pos_start), NULL);
                    g_free(current_text_copy);

                    current_action.position[1] += pos_end - current_action.position[0];
                    current_action.position[0] = pos_start;

                    return;
                }
            }
            break;
            default:
                break;
        }
    }

    // Start a new action.
    current_action.type = type;
    current_action.text = type == AIRPAD_UNDO_BUFFER_ITEM_TYPE_INSERT ? g_strdup(text) : text;
    current_action.position[0] = pos_start;
    current_action.position[1] = pos_end;
    current_action.unit_marker = unit_marker;
}

// Sets up the next buffer item to be marked with the unit marker.
void airpad_undo_mark_unit_next()
{
    unit_marker_next = TRUE;
}

// Marks the previous undo buffer item with the unit marker.
// `unit_marker_next` flag is also reset: since this function is used to set end unit marker,
// we do not want to mark the subsequent buffer item as a unit.
void airpad_undo_mark_unit_prev()
{
    undo_buffer.buffer[undo_buffer.next - 1].unit_marker = TRUE;
    unit_marker_next = FALSE;
}

// Mark the current undo buffer item as saved to the disk.
void airpad_undo_save(GtkTextBuffer *text_buffer)
{
    if (!gtk_text_buffer_get_modified(text_buffer))
        return;

    // If the current action is eligible for storing, push it into the undo buffer.
    if (current_action.type == AIRPAD_UNDO_BUFFER_ITEM_TYPE_INSERT || current_action.type == AIRPAD_UNDO_BUFFER_ITEM_TYPE_DELETE)
    {
        airpad_undo_expand_buffer();
        memcpy(undo_buffer.buffer + undo_buffer.next - 1, &current_action, sizeof(struct AirpadUndoBufferItem));
    }

    current_action.type = AIRPAD_UNDO_BUFFER_ITEM_TYPE_SAVE;
    current_action.text = NULL;

    saved_item_no = undo_buffer.next;
}

// Flushes the current item into the undo buffer.
void airpad_undo_flush()
{
    airpad_undo_expand_buffer();
    memcpy(undo_buffer.buffer + undo_buffer.next - 1, &current_action, sizeof(struct AirpadUndoBufferItem));

    current_action.type = AIRPAD_UNDO_BUFFER_ITEM_TYPE_FLUSH;
    current_action.text = NULL;
}

// Initializes sensitivity of the undo widgets.
void airpad_undo_init(const struct AirpadDataWindow *data_window)
{
    // Undo and redo are always disabled at the start of the program or when a new file is opened.
    gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_edit_undo, FALSE);
    gtk_widget_set_sensitive(data_window->data_menu_bar->menu_item_edit_redo, FALSE);
}

// Frees the undo buffer and sets its fields to the initial values.
void airpad_undo_reset()
{
    for (unsigned int i = 0;i < undo_buffer.end;++i)
        g_free(undo_buffer.buffer[i].text);

    g_free(undo_buffer.buffer);

    g_free(current_action.text);

    memset(&undo_buffer, 0, sizeof(undo_buffer));
    memset(&current_action, 0, sizeof(current_action));

    saved_item_no = 0;
}

// If conditions are met adds the text buffer contents to the undo buffer.
// Also updates the state of the menu bar items "Undo" and "Redo".
void airpad_undo_add_on_insert(GtkTextBuffer *text_buffer, GtkTextIter *position, char *new_text, int new_text_length, const struct AirpadDataMenuBar *data_menu_bar)
{
    const int pos_start = gtk_text_iter_get_offset(position);
    const int pos_end = pos_start + g_utf8_strlen(new_text, new_text_length);

    airpad_undo_reset_buffer();

    // Save the current action to the undo buffer.
    if (current_action.type == AIRPAD_UNDO_BUFFER_ITEM_TYPE_DELETE || (current_action.type == AIRPAD_UNDO_BUFFER_ITEM_TYPE_INSERT && (current_action.position[0] - pos_start > 0 || pos_start - current_action.position[1] > 0 || (airpad_undo_is_space(new_text) && !airpad_undo_is_space(current_action.text)))))
    {
        airpad_undo_expand_buffer();
        memcpy(undo_buffer.buffer + undo_buffer.next - 1, &current_action, sizeof(struct AirpadUndoBufferItem));
    }

    // Update the current action.
    airpad_undo_update_current_action(AIRPAD_UNDO_BUFFER_ITEM_TYPE_INSERT, new_text, pos_start, pos_end, unit_marker_next);
    unit_marker_next = FALSE;

    // Set the sensitivity of the "Undo" and "Redo" menu items.
    gtk_widget_set_sensitive(data_menu_bar->menu_item_edit_undo, TRUE);
    gtk_widget_set_sensitive(data_menu_bar->menu_item_edit_redo, FALSE);
}

// If conditions are met adds the text buffer contents to the undo buffer.
// Also updates the state of the menu bar items "Undo" and "Redo".
void airpad_undo_add_on_delete(GtkTextBuffer *text_buffer, GtkTextIter *start, GtkTextIter *end, const struct AirpadDataMenuBar *data_menu_bar)
{
    const int pos_start = gtk_text_iter_get_offset(start);
    const int pos_end = gtk_text_iter_get_offset(end);

    airpad_undo_reset_buffer();

    // Save the current action to the undo buffer.
    if (current_action.type == AIRPAD_UNDO_BUFFER_ITEM_TYPE_INSERT || (current_action.type == AIRPAD_UNDO_BUFFER_ITEM_TYPE_DELETE && !(pos_start <= current_action.position[0] && pos_end >= current_action.position[0])))
    {
        airpad_undo_expand_buffer();
        memcpy(undo_buffer.buffer + undo_buffer.next - 1, &current_action, sizeof(struct AirpadUndoBufferItem));
    }

    // Update the current action.
    airpad_undo_update_current_action(AIRPAD_UNDO_BUFFER_ITEM_TYPE_DELETE, gtk_text_buffer_get_text(text_buffer, start, end, FALSE), pos_start, pos_end, unit_marker_next);
    unit_marker_next = FALSE;

    // Set the sensitivity of the "Undo" and "Redo" menu items.
    gtk_widget_set_sensitive(data_menu_bar->menu_item_edit_undo, TRUE);
    gtk_widget_set_sensitive(data_menu_bar->menu_item_edit_redo, FALSE);
}

// Updates the text buffer to match the previous undo buffer item.
// The current action is also pushed into the undo buffer, if it is eligible for that.
// Also updates the state of the menu bar items "Undo", "Redo" and "Saved".
void airpad_undo_action_undo(GtkWidget *widget, const struct AirpadDataApplication *data_application)
{
    // Get the text buffer.
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view));

    // Block the handlers that run when the text buffer is modified.
    g_signal_handler_block(text_buffer, data_application->data_signals->text_buffer_modified);
    g_signal_handler_block(text_buffer, data_application->data_signals->text_buffer_insert_text);
    g_signal_handler_block(text_buffer, data_application->data_signals->text_buffer_delete_range);

    // Push the current action into the undo buffer, unless the previous action was undo, redo or save.
    if (current_action.type == AIRPAD_UNDO_BUFFER_ITEM_TYPE_INSERT || current_action.type == AIRPAD_UNDO_BUFFER_ITEM_TYPE_DELETE)
    {
        airpad_undo_expand_buffer();
        memcpy(undo_buffer.buffer + undo_buffer.next - 1, &current_action, sizeof(struct AirpadUndoBufferItem));
    }

    current_action.type = AIRPAD_UNDO_BUFFER_ITEM_TYPE_UNDO;
    current_action.text = NULL;

    // Reload the previous buffer item.
    const gboolean modified = gtk_text_buffer_get_modified(text_buffer);
    airpad_undo_move_backward(text_buffer);
    gtk_text_buffer_set_modified(text_buffer, modified);

    // Unblock the handlers that run when the text buffer is modified.
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_delete_range);
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_insert_text);
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_modified);

    // Scroll to the position where the change occured.
    gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view), gtk_text_buffer_get_selection_bound(text_buffer), 0.1, FALSE, 0.0, 0.0);

    // Set the sensitivity of the "Undo" and "Redo" menu items.
    gtk_widget_set_sensitive(data_application->data_window->data_menu_bar->menu_item_edit_undo, undo_buffer.next);
    gtk_widget_set_sensitive(data_application->data_window->data_menu_bar->menu_item_edit_redo, TRUE);

    // Mark the text buffer as not modified if this buffer is saved to the disk.
    gtk_text_buffer_set_modified(text_buffer, saved_item_no != undo_buffer.next);
}

// Changes the text buffer such that it matches the subsequent buffer item.
// Also updates the state of the menu bar items "Undo", "Redo" and "Saved".
void airpad_undo_action_redo(GtkWidget *widget, const struct AirpadDataApplication *data_application)
{
    // Get the text buffer.
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view));

    // Block the handlers that run when the text buffer is modified.
    g_signal_handler_block(text_buffer, data_application->data_signals->text_buffer_modified);
    g_signal_handler_block(text_buffer, data_application->data_signals->text_buffer_insert_text);
    g_signal_handler_block(text_buffer, data_application->data_signals->text_buffer_delete_range);

    // Save the previous action.
    current_action.type = AIRPAD_UNDO_BUFFER_ITEM_TYPE_REDO;

    // Reload the previous buffer item.
    const gboolean modified = gtk_text_buffer_get_modified(text_buffer);
    airpad_undo_move_forward(text_buffer);
    gtk_text_buffer_set_modified(text_buffer, modified);

    // Unblock the handlers that run when the text buffer is modified.
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_delete_range);
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_insert_text);
    g_signal_handler_unblock(text_buffer, data_application->data_signals->text_buffer_modified);

    // Scroll to the position where the change occured.
    gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(data_application->data_window->data_text_view->text_view), gtk_text_buffer_get_selection_bound(text_buffer), 0.1, FALSE, 0.0, 0.0);

    // Set the sensitivity of the "Undo" and "Redo" menu items.
    gtk_widget_set_sensitive(data_application->data_window->data_menu_bar->menu_item_edit_undo, TRUE);
    gtk_widget_set_sensitive(data_application->data_window->data_menu_bar->menu_item_edit_redo, undo_buffer.next != undo_buffer.end);

    // Mark the text buffer as not modified if this buffer is saved to the disk.
    gtk_text_buffer_set_modified(text_buffer, saved_item_no != undo_buffer.next);
}
