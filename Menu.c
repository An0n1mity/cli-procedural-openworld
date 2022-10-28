#include "Menu.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

// Create the main window for game's title
WINDOW *createTitleWindow(int height, int width, int startx, int starty)
{
    WINDOW *title_window;

    title_window = newwin(height, width, starty, startx);
    box(title_window, 0, 0); // 0, 0 gives default characters
                             // for the vertical and horizontal
                             // lines
    wrefresh(title_window);  // Show that box

    return title_window;
}

enum MenuChoice_e titleLoop(WINDOW *title_window)
{
    // Strings selection
    const char *title_strings[5] = {"--- DWARF FORTRESS ---",
                                    "START",
                                    "LOAD GAME",
                                    "CREDITS",
                                    "QUIT GAME"};
    int width, height;
    getmaxyx(title_window, height, width);

    // Print all the title menu
    for (size_t i = 0; i < 5; i++)
    {
        if (i == 1)
            wattron(title_window, A_STANDOUT);

        if (i == 0)
            mvwprintw(title_window, i + 1, width / 2 - strlen(title_strings[i]) / 2, "%s", title_strings[i]);

        else
            mvwprintw(title_window, i + (height / 2) - 2, width / 2 - strlen(title_strings[i]) / 2, "%s", title_strings[i]);

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
        mvwprintw(title_window, idx + (height / 2.5), width / 2 - strlen(title_strings[idx + 1]) / 2, "%s", title_strings[idx + 1]);

        switch (input)
        {
        case KEY_UP:
            idx--;
            if (idx < 0)
                idx = 3;
            break;
        case KEY_DOWN:
            idx++;
            if (idx > 3)
                idx = 0;
            break;
        case '\n':
            wclear(title_window); // Refresh it (to leave it blank)
            delwin(title_window); // and delete
            if (idx == 0)
                return NEW_GAME;

            if (idx == 1)
                return LOAD_GAME;

            if (idx == 2)
                return CREDITS;

            else
                return QUIT;

            break;
        }

        wattron(title_window, A_STANDOUT);
        mvwprintw(title_window, idx + (height / 2.5), width / 2 - strlen(title_strings[idx + 1]) / 2, "%s", title_strings[idx + 1]);
        wattroff(title_window, A_STANDOUT);
        wrefresh(title_window);
    }
}

void creditsMenu(WINDOW *p_credits_window)
{

    // clear and remove all draw from main window
    wclear(stdscr);
    // Refresh main screen
    refresh();
    // Create a window
    WINDOW *credits_window = newwin(p_credits_window->_maxy, p_credits_window->_maxx, 0, 0);
    // Draw a border around the window
    box(credits_window, 0, 0);
    // Print to the created window the credits
    mvwprintw(credits_window, 1, 2, "CREDITS");
    mvwprintw(credits_window, 2, 2, "----------------");
    mvwprintw(credits_window, 3, 2, "Game made by :");
    mvwprintw(credits_window, 4, 2, "   - Cédric MARTY");
    mvwprintw(credits_window, 5, 2, "   - Alexandre MONTHOUEL");
    mvwprintw(credits_window, 6, 2, "   - Dimitri RUSSO");
    mvwprintw(credits_window, 7, 2, "----------------");
    mvwprintw(credits_window, 8, 2, "Press any key to return to the main menu");

    // Refresh the window
    wrefresh(credits_window);

    // Wait for a key to be pressed
    getch();

    // Clear the window
    wclear(stdscr);
    wrefresh(stdscr);
    delwin(credits_window);
}

void quitGame()
{
    endwin();
    exit(0);
}

void seedMenu(WINDOW *seed_window, int *seed)
{
    // Clear and remove all draw from main window
    wclear(stdscr);

    // Refresh main screen
    refresh();

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

    mvwprintw(seed_window, seed_window_h / 2 - 1, seed_window_w / 2 - 1, "MAP SEED");

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