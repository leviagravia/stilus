#ifndef STILUS_COMMANDS_H
#define STILUS_COMMANDS_H

#include <gtk/gtk.h>

typedef enum
{
    STILUS_CMD_UI_ONLY            = 1 << 0,
    STILUS_CMD_TEXT_MUTATION      = 1 << 1,
    STILUS_CMD_FILE_LIFECYCLE     = 1 << 2,
    STILUS_CMD_REQUIRES_SELECTION = 1 << 3,
    STILUS_CMD_NAVIGATION         = 1 << 4,
    STILUS_CMD_DIALOG             = 1 << 5,
    STILUS_CMD_PERSISTENCE        = 1 << 6,
    STILUS_CMD_READ_ONLY          = 1 << 7
} StilusCommandFlags;

typedef struct
{
    const char *id;
    const char *label;
    const char *menu_path;
    const char *shortcut;
    unsigned int flags;
    void (*callback)(GtkWidget *widget, gpointer data);
    const char *description;
} StilusCommand;

const StilusCommand *stilus_commands_get_all(gsize *count);
const StilusCommand *stilus_commands_find_by_id(const char *id);

void stilus_cmd_navigate_go_to_line(GtkWidget *widget, gpointer data);
void stilus_cmd_writing_statistics(GtkWidget *widget, gpointer data);
void stilus_cmd_writing_word_goal(GtkWidget *widget, gpointer data);
void stilus_cmd_writing_clear_word_goal(GtkWidget *widget, gpointer data);
void stilus_cmd_revise_uppercase(GtkWidget *widget, gpointer data);
void stilus_cmd_revise_lowercase(GtkWidget *widget, gpointer data);
void stilus_cmd_revise_remove_trailing_spaces(GtkWidget *widget, gpointer data);
void stilus_cmd_view_font_bigger(GtkWidget *widget, gpointer data);
void stilus_cmd_view_font_smaller(GtkWidget *widget, gpointer data);

#endif
