#include "Menu.h"

// Create the main window for game's title
WINDOW *createTitleWindow(int height, int width, int startx, int starty)
{
    WINDOW *title_window;

    title_window = newwin(height, width, starty, startx);
    box(title_window, 0, 0); /* 0, 0 gives default characters
                              * for the vertical and horizontal
                              * lines			*/
    wrefresh(title_window);  /* Show that box 		*/

    return title_window;
}

enum MenuChoice_e titleLoop(WINDOW *title_window)
{
    // Strings selection
    const char *title_strings[3] = {"DWARF CLONE",
                                    "NEW GAME",
                                    "LOAD GAME"};
    int width, height;
    getmaxyx(title_window, height, width);
    // Print all the title menu
    for (size_t i = 0; i < 3; i++)
    {
        if (i == 1)
            wattron(title_window, A_STANDOUT);
        if (i == 0)
            mvwprintw(title_window, i + 1, width / 2 - strlen(title_strings[i]) / 2, "%s", title_strings[i]);
        else
            mvwprintw(title_window, i + 1 + (height / 2) - 2, width / 2 - strlen(title_strings[i]) / 2, "%s", title_strings[i]);
        wattroff(title_window, A_STANDOUT);
    }

    // Update
    wrefresh(title_window);

    // Selection loop
    int input;
    int idx = 0;
    noecho();                   // disable echoing of characters on the screen
    keypad(title_window, TRUE); // enable keyboard input for the window.
    curs_set(0);
    while ((input = wgetch(title_window)) != 'q')
    {
        mvwprintw(title_window, idx + (height / 2), width / 2 - strlen(title_strings[idx + 1]) / 2, "%s", title_strings[idx + 1]);

        switch (input)
        {
        case KEY_UP:
            idx--;
            if (idx < 0)
                idx = 1;
            break;
        case KEY_DOWN:
            idx++;
            if (idx > 1)
                idx = 0;
            break;
        case '\n':
            wclear(title_window); // Refresh it (to leave it blank)
            delwin(title_window); // and delete
            if (idx == 0)
                return NEW_GAME;
            else
                return LOAD_GAME;
            break;
        }

        wattron(title_window, A_STANDOUT);
        mvwprintw(title_window, idx + (height / 2), width / 2 - strlen(title_strings[idx + 1]) / 2, "%s", title_strings[idx + 1]);
        wattroff(title_window, A_STANDOUT);
        wrefresh(title_window);
    }
}

void seedMenu(WINDOW *seed_window, int *seed)
{

    int seed_window_w, seed_window_h;
    getmaxyx(seed_window, seed_window_h, seed_window_w);

    WINDOW *seed_form_window = derwin(seed_window, seed_window_h - 2, seed_window_w - 2, 1, 1);

    box(seed_window, 0, 0);

    keypad(stdscr, TRUE);
    FIELD *field[2];
    field[0] = new_field(1, 4, seed_window_h / 2, seed_window_w / 2 - 2, 0, 0);
    field[1] = NULL;
    set_field_back(field[0], A_UNDERLINE);
    field_opts_off(field[0], O_AUTOSKIP); /* Don't go to next field when this */

    FORM *my_form = new_form(field);
    set_form_win(my_form, seed_window);
    set_form_sub(my_form, seed_form_window);
    post_form(my_form);

    mvwprintw(seed_window, seed_window_h / 2 - 1, seed_window_w / 2 - 1, "SEED");

    wrefresh(seed_window);
    wrefresh(seed_form_window);

    int ch;
    while ((ch = wgetch(seed_form_window)) != KEY_F(1))
    {
        switch (ch)
        {
        case KEY_DOWN:
            /* Go to next field */
            form_driver(my_form, REQ_NEXT_FIELD);
            /* Go to the end of the present buffer */
            /* Leaves nicely at the last character */
            form_driver(my_form, REQ_END_LINE);
            break;
        case KEY_UP:
            /* Go to previous field */
            form_driver(my_form, REQ_PREV_FIELD);
            form_driver(my_form, REQ_END_LINE);
            break;
        case KEY_LEFT:
            form_driver(my_form, REQ_PREV_CHAR);
            break;

        case KEY_RIGHT:
            form_driver(my_form, REQ_NEXT_CHAR);
            break;

        // Delete the char before cursor
        case KEY_BACKSPACE:
        case 127:
            form_driver(my_form, REQ_DEL_PREV);
            break;

        // Delete the char under the cursor
        case KEY_DC:
            form_driver(my_form, REQ_DEL_CHAR);
            break;

        case '\n':
            form_driver(my_form, REQ_VALIDATION);
            *seed = atoi(field_buffer(field[0], 0));

            wclear(seed_window); // Refresh it (to leave it blank)
            wclear(seed_form_window);

            delwin(seed_window);
            delwin(seed_form_window);

            unpost_form(my_form);
            free_form(my_form);
            free_field(field[0]);

            refresh();
            return;
        default:
            /* If this is a normal character, it gets */
            /* Printed	*/
            if (isdigit(ch))
                form_driver(my_form, ch);
            break;
        }
    }
}