#include "stilus_commands.h"
#include "const.h"
#include <glib/gi18n.h>

static const StilusCommand stilus_commands[] =
{
    {
        "navigate.go-to-line",
        "Go To Line...",
        "Navigate/Go To Line...",
        "<Control>L",
        STILUS_CMD_NAVIGATION | STILUS_CMD_DIALOG | STILUS_CMD_READ_ONLY,
        stilus_cmd_navigate_go_to_line,
        "Move the cursor to a specific line number."
    },
    {
        "revise.uppercase",
        "UPPERCASE",
        "Revise/Case/UPPERCASE",
        NULL,
        STILUS_CMD_TEXT_MUTATION | STILUS_CMD_REQUIRES_SELECTION,
        stilus_cmd_revise_uppercase,
        "Convert the selected text to uppercase."
    },
    {
        "revise.lowercase",
        "Lowercase",
        "Revise/Case/Lowercase",
        NULL,
        STILUS_CMD_TEXT_MUTATION | STILUS_CMD_REQUIRES_SELECTION,
        stilus_cmd_revise_lowercase,
        "Convert the selected text to lowercase."
    },
    {
        "revise.remove-trailing-spaces",
        "Remove Trailing Spaces",
        "Revise/Cleanup/Remove Trailing Spaces",
        NULL,
        STILUS_CMD_TEXT_MUTATION,
        stilus_cmd_revise_remove_trailing_spaces,
        "Remove trailing spaces and tabs from document lines."
    },
    {
        "view.font-bigger",
        "Font Bigger",
        "View/Font Bigger",
        "<Control>plus",
        STILUS_CMD_UI_ONLY | STILUS_CMD_PERSISTENCE,
        stilus_cmd_view_font_bigger,
        "Increase the editor font size."
    },
    {
        "view.font-smaller",
        "Font Smaller",
        "View/Font Smaller",
        "<Control>minus",
        STILUS_CMD_UI_ONLY | STILUS_CMD_PERSISTENCE,
        stilus_cmd_view_font_smaller,
        "Decrease the editor font size."
    }
};

const StilusCommand *
stilus_commands_get_all(gsize *count)
{
    if (count != NULL)
        *count = G_N_ELEMENTS(stilus_commands);

    return stilus_commands;
}

const StilusCommand *
stilus_commands_find_by_id(const char *id)
{
    gsize i;

    if (id == NULL)
        return NULL;

    for (i = 0; i < G_N_ELEMENTS(stilus_commands); i++)
    {
        if (g_strcmp0(stilus_commands[i].id, id) == 0)
            return &stilus_commands[i];
    }

    return NULL;
}

void
stilus_cmd_navigate_go_to_line(GtkWidget *widget, gpointer data)
{
    struct AirpadDataApplication *data_application = data;

    (void)widget;

    if (data_application == NULL ||
        data_application->data_window == NULL ||
        data_application->data_window->data_text_view == NULL ||
        data_application->data_window->data_text_view->text_view == NULL)
        return;

    GtkWidget *text_view = data_application->data_window->data_text_view->text_view;
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    const gint line_count = gtk_text_buffer_get_line_count(text_buffer);

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        _("Go To Line"),
        GTK_WINDOW(data_application->data_window->window),
        GTK_DIALOG_MODAL,
        _("Cancel"),
        GTK_RESPONSE_CANCEL,
        _("Go"),
        GTK_RESPONSE_ACCEPT,
        NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    GtkWidget *label = gtk_label_new(_("Line number:"));
    GtkAdjustment *adjustment = gtk_adjustment_new(1, 1, line_count, 1, 10, 0);
    GtkWidget *spin_button = gtk_spin_button_new(adjustment, 1, 0);

    gtk_container_set_border_width(GTK_CONTAINER(box), 8);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), spin_button, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(content_area), box);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        const gint line_number = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_button));
        GtkTextIter iter;

        gtk_text_buffer_get_iter_at_line(text_buffer, &iter, line_number - 1);
        gtk_text_buffer_place_cursor(text_buffer, &iter);
        gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(text_view), &iter, 0.1, TRUE, 0.5, 0.5);
        gtk_widget_grab_focus(text_view);
    }

    gtk_widget_destroy(dialog);
}

void
stilus_cmd_revise_uppercase(GtkWidget *widget, gpointer data)
{
    struct AirpadDataApplication *data_application = data;

    (void)widget;

    if (data_application == NULL ||
        data_application->data_window == NULL ||
        data_application->data_window->data_text_view == NULL ||
        data_application->data_window->data_text_view->text_view == NULL)
        return;

    GtkWidget *text_view = data_application->data_window->data_text_view->text_view;
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter start;
    GtkTextIter end;

    if (!gtk_text_buffer_get_selection_bounds(text_buffer, &start, &end))
        return;

    char *selected_text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);
    char *upper_text = g_utf8_strup(selected_text, -1);

    gtk_text_buffer_delete(text_buffer, &start, &end);
    gtk_text_buffer_insert(text_buffer, &start, upper_text, -1);

    g_free(upper_text);
    g_free(selected_text);
}

void
stilus_cmd_revise_lowercase(GtkWidget *widget, gpointer data)
{
    struct AirpadDataApplication *data_application = data;

    (void)widget;

    if (data_application == NULL ||
        data_application->data_window == NULL ||
        data_application->data_window->data_text_view == NULL ||
        data_application->data_window->data_text_view->text_view == NULL)
        return;

    GtkWidget *text_view = data_application->data_window->data_text_view->text_view;
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter start;
    GtkTextIter end;

    if (!gtk_text_buffer_get_selection_bounds(text_buffer, &start, &end))
        return;

    char *selected_text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);
    char *lower_text = g_utf8_strdown(selected_text, -1);

    gtk_text_buffer_delete(text_buffer, &start, &end);
    gtk_text_buffer_insert(text_buffer, &start, lower_text, -1);

    g_free(lower_text);
    g_free(selected_text);
}

void
stilus_cmd_revise_remove_trailing_spaces(GtkWidget *widget, gpointer data)
{
    (void)widget;
    (void)data;
}

void
stilus_cmd_view_font_bigger(GtkWidget *widget, gpointer data)
{
    (void)widget;
    (void)data;
}

void
stilus_cmd_view_font_smaller(GtkWidget *widget, gpointer data)
{
    (void)widget;
    (void)data;
}
