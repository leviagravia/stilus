#include "stilus_commands.h"

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
    (void)widget;
    (void)data;
}

void
stilus_cmd_revise_uppercase(GtkWidget *widget, gpointer data)
{
    (void)widget;
    (void)data;
}

void
stilus_cmd_revise_lowercase(GtkWidget *widget, gpointer data)
{
    (void)widget;
    (void)data;
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
