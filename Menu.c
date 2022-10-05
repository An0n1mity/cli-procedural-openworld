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
    curs_set(0);

    int seed_window_w, seed_window_h;
    getmaxyx(seed_window, seed_window_h, seed_window_w);

    box(seed_window, 0, 0);
    // mvwprintw(seed_window, seed_window_h / 2, seed_window_w / 2 - 2, "%s", "SEED");

    keypad(stdscr, TRUE);
    FIELD *field[2];
    field[0] = new_field(1, 4, seed_window_h / 2, seed_window_h / 2 + 2, 0, 0);
    field[1] = NULL;
    set_field_opts(field[0], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE | O_STATIC);
    set_field_back(field[0], A_UNDERLINE);
    field_opts_off(field[0], O_AUTOSKIP); /* Don't go to next field when this */

    FORM *my_form = new_form(field);
    // set_form_sub(my_form, seed_window);
    post_form(my_form);

    mvprintw(seed_window_h / 2 - 1, seed_window_h / 2 + 2, "SEED");
    refresh();
    // prefresh(seed_window, 0, 0, 0, 0, LINES, COLS);

    // wrefresh(seed_window);

    while (1)
        ;
}