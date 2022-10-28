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

void quitGame()
{
    endwin();
    exit(0);
}

static FORM *form;
static FIELD *fields[5];
static WINDOW *win_body, *win_form;

/*
 * This is useful because ncurses fill fields blanks with spaces.
 */
static char *trim_whitespaces(char *str)
{
    char *end;

    // trim leading space
    while (isspace(*str))
        str++;

    if (*str == 0) // all spaces?
        return str;

    // trim trailing space
    end = str + strnlen(str, 128) - 1;

    while (end > str && isspace(*end))
        end--;

    // write new null terminator
    *(end + 1) = '\0';

    return str;
}

static void driver(int ch)
{
    int i;

    switch (ch)
    {
    case KEY_F(2):
        // Or the current field buffer won't be sync with what is displayed
        form_driver(form, REQ_NEXT_FIELD);
        form_driver(form, REQ_PREV_FIELD);
        move(LINES - 3, 2);

        for (i = 0; fields[i]; i++)
        {
            printw("%s", trim_whitespaces(field_buffer(fields[i], 0)));

            if (field_opts(fields[i]) & O_ACTIVE)
                printw("\"\t");
            else
                printw(": \"");
        }

        refresh();
        pos_form_cursor(form);
        break;

    case KEY_DOWN:
        form_driver(form, REQ_NEXT_FIELD);
        form_driver(form, REQ_END_LINE);
        break;

    case KEY_UP:
        form_driver(form, REQ_PREV_FIELD);
        form_driver(form, REQ_END_LINE);
        break;

    case KEY_LEFT:
        form_driver(form, REQ_PREV_CHAR);
        break;

    case KEY_RIGHT:
        form_driver(form, REQ_NEXT_CHAR);
        break;

    // Delete the char before cursor
    case KEY_BACKSPACE:
    case 127:
        form_driver(form, REQ_DEL_PREV);
        break;

    // Delete the char under the cursor
    case KEY_DC:
        form_driver(form, REQ_DEL_CHAR);
        break;

    default:
        form_driver(form, ch);
        break;
    }

    wrefresh(win_form);
}

void seedMenu(WINDOW *seed_window, int *seed)
{
    /*int seed_window_w, seed_window_h;
    getmaxyx(seed_window, seed_window_h, seed_window_w);

    WINDOW *seed_form_window = derwin(seed_window, seed_window_h - 2, seed_window_w - 2, 1, 1);

    box(seed_window, 0, 0);

    keypad(stdscr, TRUE);
    cbreak();
    FIELD *field[2];
    field[0] = new_field(1, 4, seed_window_h / 2, seed_window_w / 2 - 2, 0, 5);
    field[1] = NULL;
    set_field_back(field[0], A_UNDERLINE);
    set_field_opts(field[0], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE); /* Don't go to next field when this

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
            form_driver(my_form, REQ_NEXT_FIELD);

            form_driver(my_form, REQ_END_LINE);
            break;
        case KEY_UP:
$            form_driver(my_form, REQ_PREV_FIELD);
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
            // free_form(my_form);
            free_field(field[0]);

            refresh();
            return;
        default:
            // if (isdigit(ch))
            form_driver(my_form, ch);
            break;
        }
    }*/

    win_body = newwin(24, 80, 0, 0);
    box(win_body, 0, 0);
    win_form = derwin(win_body, 20, 78, 3, 1);
    box(win_form, 0, 0);
    mvwprintw(win_body, 1, 2, "Press F1 to quit and F2 to print fields content");

    fields[0] = new_field(1, 10, 0, 0, 0, 0);
    fields[1] = new_field(1, 40, 0, 15, 0, 0);
    fields[2] = new_field(1, 10, 2, 0, 0, 0);
    fields[3] = new_field(1, 40, 2, 15, 0, 0);
    fields[4] = NULL;

    set_field_buffer(fields[0], 0, "label1");
    set_field_buffer(fields[1], 0, "val1");
    set_field_buffer(fields[2], 0, "label2");
    set_field_buffer(fields[3], 0, "val2");

    set_field_opts(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(fields[1], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_opts(fields[2], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(fields[3], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

    set_field_back(fields[1], A_UNDERLINE);
    set_field_back(fields[3], A_UNDERLINE);

    form = new_form(fields);
    set_form_win(form, win_form);
    set_form_sub(form, derwin(win_form, 18, 76, 1, 1));
    post_form(form);

    refresh();
    wrefresh(win_body);
    wrefresh(win_form);
    int ch;
    while ((ch = getch()) != KEY_F(1))
        driver(ch);

    unpost_form(form);
    free_form(form);
    free_field(fields[0]);
    free_field(fields[1]);
    free_field(fields[2]);
    free_field(fields[3]);
    delwin(win_form);
    delwin(win_body);
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
    mvwprintw(credits_window, 4, 2, "   - CÃ©dric MARTY");
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
