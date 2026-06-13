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

#ifndef AIRPAD_CONST_H
#define AIRPAD_CONST_H
#include <gtk/gtk.h>

// Program information.
#define AIRPAD_INFO_NAME        "Stilus"
#ifndef AIRPAD_INFO_IDEN
#define AIRPAD_INFO_IDEN        "com.leviagravia.Stilus"
#endif
#define AIRPAD_INFO_VERS        "0.1.0"
#define AIRPAD_INFO_LICE        "GPL3"
#define AIRPAD_INFO_LICE_TYPE   GTK_LICENSE_GPL_3_0
#define AIRPAD_INFO_SITE        "https://github.com/leviagravia/stilus"
#define AIRPAD_INFO_AUTH        "Luciano Squizzato"
#define AIRPAD_INFO_AUTH_EMAIL  "leviagravia@zohomail.eu"
#define AIRPAD_INFO_YEAR        "2026"

// Types of errors that can occur during an execution of the program
// and of which the user will be alarmed about via message dialog.
enum AirpadErrorType
{
    AIRPAD_ERROR_TYPE_FILE_READ_FAILURE,
    AIRPAD_ERROR_TYPE_FILE_WRITE_FAILURE,
    AIRPAD_ERROR_TYPE_TEXT_INVALID_UTF8,
    AIRPAD_ERROR_TYPE_TEXT_CONVERSION_ERROR
};

// Menubar data.
struct AirpadDataMenuBar
{
    GtkWidget *menu_bar;    // Menubar itself.

    // Menus.
    GtkWidget *menu_file;                       // "File" menu.
    GtkWidget *menu_edit;                       // "Edit" menu.
    GtkWidget *menu_research;                   // "Research" menu.
    GtkWidget *menu_navigate;                   // "Navigate" menu.
    GtkWidget *menu_writing;                    // "Writing" menu.
    GtkWidget *menu_revise;                     // "Revise" menu.
    GtkWidget *menu_search;                     // "Search" menu.
    GtkWidget *menu_view;                       // "View" menu.
    GtkWidget *menu_options;                    // "Options" menu.
    GtkWidget *menu_tools;                      // "Tools" menu.
    GtkWidget *menu_options_text_wrap;          // "Options -> Text Wrap" menu.
    GtkWidget *menu_options_scrollbar_policy;   // "Options -> Scrollbar Policy" menu.
    GtkWidget *menu_help;                       // "Help" menu.

    // Menu items.
    GtkWidget *menu_item_file;                                  // "File" menu item.
    GtkWidget *menu_item_edit;                                  // "Edit" menu item.
    GtkWidget *menu_item_research;                              // "Research" menu item.
    GtkWidget *menu_item_navigate;                              // "Navigate" menu item.
    GtkWidget *menu_item_writing;                               // "Writing" menu item.
    GtkWidget *menu_item_writing_statistics;                    // "Writing -> Writing Statistics" menu item.
    GtkWidget *menu_item_writing_word_goal;                     // "Writing -> Word Goal..." menu item.
    GtkWidget *menu_item_writing_clear_word_goal;               // "Writing -> Clear Word Goal" menu item.
    GtkWidget *menu_item_revise;                                // "Revise" menu item.
    GtkWidget *menu_item_search;                                // "Search" menu item.
    GtkWidget *menu_item_view;                                  // "View" menu item.
    GtkWidget *menu_item_options;                               // "Options" menu item.
    GtkWidget *menu_item_tools;                                 // "Tools" menu item.
    GtkWidget *menu_item_help;                                  // "Help" menu item.
    GtkWidget *menu_item_file_new;                              // "File -> New" menu item.
    GtkWidget *menu_item_file_open;                             // "File -> Open..." menu item.
    GtkWidget *menu_item_file_save;                             // "File -> Save" menu item.
    GtkWidget *menu_item_file_save_as;                          // "File -> Save As..." menu item.
    GtkWidget *menu_item_file_close;                            // "File -> Close" menu item.
    GtkWidget *menu_item_navigate_go_to_line;                 // "Navigate -> Go To Line..." menu item.
    GtkWidget *menu_item_revise_uppercase;                    // "Revise -> UPPERCASE" menu item.
    GtkWidget *menu_item_revise_lowercase;                    // "Revise -> Lowercase" menu item.
    GtkWidget *menu_item_revise_remove_trailing_spaces;       // "Revise -> Remove Trailing Spaces" menu item.
    GtkWidget *menu_item_edit_copy;                             // "Edit -> Copy" menu item.
    GtkWidget *menu_item_edit_paste;                            // "Edit -> Paste" menu item.
    GtkWidget *menu_item_edit_paste_as_plain_text;              // "Edit -> Paste as Plain Text" menu item.
    GtkWidget *menu_item_edit_cut;                              // "Edit -> Cut" menu item.
    GtkWidget *menu_item_edit_delete;                           // "Edit -> Delete" menu item.
    GtkWidget *menu_item_edit_select_all;                       // "Edit -> Select All" menu item.
    GtkWidget *menu_item_edit_insert_date_time;                  // "Edit -> Insert Date / Time" menu item.
    GtkWidget *menu_item_edit_undo;                             // "Edit -> Undo" menu item.
    GtkWidget *menu_item_edit_redo;                             // "Edit -> Redo" menu item.
    GtkWidget *menu_item_search_find;                           // "Search -> Find..." menu item.
    GtkWidget *menu_item_search_find_next;                      // "Search -> Find Next" menu item.
    GtkWidget *menu_item_search_find_prev;                      // "Search -> Find Previous" menu item.
    GtkWidget *menu_item_search_replace;                        // "Search -> Replace..." menu item.
    GtkWidget *menu_item_view_fullscreen;                       // "View -> Fullscreen" menu item.
    GtkWidget *menu_item_view_always_on_top;                    // "View -> Always on Top" menu item.
    GtkWidget *menu_item_view_font_bigger;                      // "View -> Font Bigger" menu item.
    GtkWidget *menu_item_view_font_smaller;                     // "View -> Font Smaller" menu item.
    GtkWidget *menu_item_options_font;                          // "Options-> Font..." menu item.
    GtkWidget *menu_item_options_text_wrap;                     // "Options -> Text Wrap" menu item.
    GtkWidget *menu_item_options_text_wrap_none;                // "Options -> Text Wrap -> None" menu item.
    GtkWidget *menu_item_options_text_wrap_on_char;             // "Options -> Text Wrap -> On Character" menu item.
    GtkWidget *menu_item_options_text_wrap_on_word;             // "Options -> Text Wrap -> On Word" menu item.
    GtkWidget *menu_item_options_text_wrap_on_char_and_word;    // "Options -> Text Wrap -> On Character and Word " menu item.
    GtkWidget *menu_item_options_overlay_scrolling;             // "Options -> Overlay Scrolling" menu item.
    GtkWidget *menu_item_options_scrollbar_policy;              // "Options -> Scrollbar Policy" menu item.
    GtkWidget *menu_item_options_scrollbar_policy_auto;         // "Options -> Scrollbar Policy -> Automatic" menu item.
    GtkWidget *menu_item_options_scrollbar_policy_always;       // "Options -> Scrollbar Policy -> Always" menu item.
    GtkWidget *menu_item_options_scrollbar_policy_never;        // "Options -> Scrollbar Policy -> Never" menu item.
    GtkWidget *menu_item_help_about;                            // "Help -> About Stilus" menu item.
    GtkWidget *menu_item_help_writing_principles;               // "Help -> Writing Principles" menu item.
    GtkWidget *menu_item_help_keyboard_shortcuts;                // "Help -> Keyboard Shortcuts" menu item.
    GtkWidget *menu_item_help_writing_workflow;                 // "Help -> Writing Workflow" menu item.
};

// Text view data.
struct AirpadDataTextView
{
    GtkWidget *text_view;       // Text view itself.
    GtkWidget *scrolled_window; // Scrollable area that holds the text view.
};

// Window data.
struct AirpadDataWindow
{
    GtkWidget *window;                          // Window itself.
    GdkPixbuf *icon;                            // Icon of the window.
    GtkWidget *content;                         // Grid that holds the window contents.
    GtkWidget *status_bar;                      // Status bar.
    struct AirpadDataMenuBar *data_menu_bar;    // Menubar of the window.
    struct AirpadDataTextView *data_text_view;  // Text area of the window.
};

// Signal IDs.
struct AirpadDataSignals
{
    gulong window_delete_event;                                     // Top-level window: "delete-event"
    gulong window_focus_in;                                         // Top-level window: "focus-in-event"
    gulong window_state;                                            // Top-level window: "window-state-change"
    gulong text_view_copy_clipboard;                                // Text view: "copy-clipboard"
    gulong text_view_cut_clipboard;                                 // Text view: "cut-clipboard"
    gulong text_buffer_modified;                                    // Text buffer: "modified-changed"
    gulong text_buffer_has_selection;                               // Text buffer: "notify::has-selection"
    gulong text_buffer_insert_text;                                 // Text buffer: "insert-text"
    gulong text_buffer_delete_range;                                // Text buffer: "delete-range"
    gulong text_buffer_paste_done;                                  // Text buffer: "paste-done"
    gulong menu_item_file_new_activate;                             // "File -> New" menu item: "activate"
    gulong menu_item_file_open_activate;                            // "File -> Open" menu item: "activate"
    gulong menu_item_file_save_activate;                            // "File -> Save" menu item: "activate"
    gulong menu_item_file_save_as_activate;                         // "File -> Save As" menu item: "activate"
    gulong menu_item_file_close_activate;                           // "File -> Close" menu item: "activate"
    gulong menu_item_navigate_go_to_line_activate;             // "Navigate -> Go To Line..." menu item: "activate"
    gulong menu_item_writing_statistics_activate;              // "Writing -> Writing Statistics" menu item: "activate"
    gulong menu_item_writing_word_goal_activate;               // "Writing -> Word Goal..." menu item: "activate"
    gulong menu_item_writing_clear_word_goal_activate;         // "Writing -> Clear Word Goal" menu item: "activate"
    gulong menu_item_revise_uppercase_activate;                // "Revise -> UPPERCASE" menu item: "activate"
    gulong menu_item_revise_lowercase_activate;                // "Revise -> Lowercase" menu item: "activate"
    gulong menu_item_revise_remove_trailing_spaces_activate;   // "Revise -> Remove Trailing Spaces" menu item: "activate"
    gulong menu_item_edit_copy_activate;                            // "Edit -> Copy" menu item: "activate"
    gulong menu_item_edit_paste_activate;                           // "Edit -> Paste" menu item: "activate"
    gulong menu_item_edit_paste_as_plain_text_activate;             // "Edit -> Paste as Plain Text" menu item: "activate"
    gulong menu_item_edit_cut_activate;                             // "Edit -> Cut" menu item: "activate"
    gulong menu_item_edit_delete_activate;                          // "Edit -> Delete" menu item: "activate"
    gulong menu_item_edit_select_all_activate;                      // "Edit -> Select All" menu item: "activate"
    gulong menu_item_edit_insert_date_time_activate;                 // "Edit -> Insert Date / Time" menu item: "activate"
    gulong menu_item_edit_undo_activate;                            // "Edit -> Undo" menu item: "activate"
    gulong menu_item_edit_redo_activate;                            // "Edit -> Redo" menu item: "activate"
    gulong menu_item_search_find_activate;                          // "Search -> Find" menu item: "activate"
    gulong menu_item_search_find_next_activate;                     // "Search -> Find Next" menu item: "activate"
    gulong menu_item_search_find_prev_activate;                     // "Search -> Find Previous" menu item: "activate"
    gulong menu_item_search_replace_activate;                       // "Search -> Replace" menu item: "activate"
    gulong menu_item_view_fullscreen_activate;                      // "View -> Fullscreen" menu item: "activate"
    gulong menu_item_view_always_on_top_activate;                 // "View -> Always on Top" menu item: "activate"
    gulong menu_item_view_font_bigger_activate;                   // "View -> Font Bigger" menu item: "activate"
    gulong menu_item_view_font_smaller_activate;                  // "View -> Font Smaller" menu item: "activate"
    gulong menu_item_options_font_activate;                         // "Options -> Font" menu item: "activate"
    gulong menu_item_options_text_wrap_none_toggled;                // "Options -> Text Wrap -> None": "toggled"
    gulong menu_item_options_text_wrap_on_word_toggled;             // "Options -> Text Wrap -> On Word": "toggled"
    gulong menu_item_options_text_wrap_on_char_toggled;             // "Options -> Text Wrap -> On Character": "toggled"
    gulong menu_item_options_text_wrap_on_char_and_word_toggled;    // "Options -> Text Wrap -> On Character and Word": "toggled"
    gulong menu_item_options_overlay_scrolling_toggled;             // "Options -> Overlay Scrollbars": "toggled"
    gulong menu_item_options_scrollbar_policy_auto_toggled;         // "Options -> Scrollbar Policy -> Automatic": "toggled"
    gulong menu_item_options_scrollbar_policy_always_toggled;       // "Options -> Scrollbar Policy -> Always": "toggled"
    gulong menu_item_options_scrollbar_policy_never_toggled;        // "Options -> Scrollbar Policy -> Never": "toggled"
    gulong menu_item_help_about_activate;                           // "Help -> About Stilus": "activate"
    gulong menu_item_help_writing_principles_activate;              // "Help -> Writing Principles": "activate"
    gulong menu_item_help_keyboard_shortcuts_activate;               // "Help -> Keyboard Shortcuts": "activate"
    gulong menu_item_help_writing_workflow_activate;                 // "Help -> Writing Workflow": "activate"
};

// Data used by the searching facility.
struct AirpadDataSearch
{
    // Text.
    char *text_find;    // Text to find.
    char *text_replace; // Text to replace.

    // Switches.
    gboolean case_insensitive;
    gboolean backward_search;
    gboolean from_here;
    gboolean search_wraps;
    gboolean replace_all;
};

// File info.
struct AirpadDataFile
{
    GFile *file;    // File itself.
    char *encoding; // Encoding of the file.
};

// Program options, read from the configuration file.
struct AirpadDataOptions
{
    char *font;
    gboolean overlay_scrolling;
    GtkWrapMode text_wrap;
    GtkPolicyType scrollbar_policy;
    gint word_goal;
};

// Holds command-line option values.
struct AirpadDataArguments
{
    // File options.
    char *file_encoding;
};

// Application data.
struct AirpadDataApplication
{
    GtkApplication *application;                // Application itself.
    struct AirpadDataArguments *data_arguments; // Argument holder.
    struct AirpadDataOptions *data_options;     // Configuration options.
    struct AirpadDataFile *data_file;           // Currently opened file.
    struct AirpadDataSearch *data_search;       // Options used for find/replace.
    struct AirpadDataSignals *data_signals;     // Signal connections.
    struct AirpadDataWindow *data_window;       // Application window.
};

#endif
