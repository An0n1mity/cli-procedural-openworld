#include "Menu.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


// Create the main window for game's title
/*
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
*/

/*
enum MenuChoice_e titleLoop(WINDOW *title_window)
{
    // Strings selection
    const char *title_strings[7] = {"--- DWARF FORTRESS ---",
                                    "NEW GAME",
                                    "LOAD GAME",
                                    "PLAYER CONFIGURATION",
                                    "WORLD CONFIGURATION",
                                    "STATS HELP",
                                    "CREDITS"};
    int width, height;
    getmaxyx(title_window, height, width);
    // Print all the title menu
    for (size_t i = 0; i < 7; i++)
    {
        if (i == 1)
            wattron(title_window, A_STANDOUT);
        if (i == 0)
            mvwprintw(title_window, i + 4, width / 2 - strlen(title_strings[i]) / 2, "%s", title_strings[i]);
        else
            mvwprintw(title_window, i + 1 + (height / 2.5) - 2, width / 2 - strlen(title_strings[i]) / 2, "%s", title_strings[i]);
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
                idx = 1;
            break;
        case KEY_DOWN:
            idx++;
            if (idx > 6)
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
        mvwprintw(title_window, idx + (height / 2.5), width / 2 - strlen(title_strings[idx + 1]) / 2, "%s", title_strings[idx + 1]);
        wattroff(title_window, A_STANDOUT);
        wrefresh(title_window);
    }
}
*/

// DEBUT CODE PROPRE

char *gameOptions[] = {
    "LOAD GAME",
    "NEW GAME",
    "PLAYER CONGIURATION",
    "WORD CONFIGURATION",
    "STATS HELP",
    "CREDITS",
    "QUIT GAME",
    (char *)NULL,
};

void centerPrint(WINDOW *win, int startY, int startX, int width, char *string, chtype color)
{
    int length;
    int x;
    int y;

    float temp;

    if (win == NULL)
        win = stdscr;

    getyx(win, y, x);

    if (startX != 0)
        x = startX;
    
    if (startY != 0)
        y = startY;
    
    if (width == 0)
        width = 80;

    length = strlen(string);
    temp = (width - length) / 2;
    x = startX + (int)temp;

    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh();
}

void mainMenu()
{
    ITEM **menu_items;
    MENU *main_menu;
    WINDOW *main_menu_window;

    int n_gameOptions; 
    int i;
    int c;

    initDisplaying();

    // items creation
    n_gameOptions = ARRAY_SIZE(gameOptions);
    menu_items = (ITEM **)calloc(n_gameOptions + 1, sizeof(ITEM *));

    for (i = 0; i < n_gameOptions; ++i)
    {
        menu_items[i] = new_item(gameOptions[i], gameOptions[i]);
    }
        
    // menu creation
    main_menu = new_menu((ITEM **)menu_items);

    // window creation associated to the menu
    main_menu_window = newwin(10, 40, 4, 4);
    keypad(main_menu_window, TRUE);

    // set main window and sub window
    set_menu_win(main_menu, main_menu_window);
    set_menu_sub(main_menu, derwin(main_menu_window, 6, 38, 3, 1));

    // set menu mark to the string " * "
    set_menu_mark(main_menu, " -> ");

    // print a border around the main window and print a title
    box(main_menu_window, 0, 0);
    centerPrint(main_menu_window, 1, 0, 40, "DWARF FORTRESS", COLOR_PAIR(1));
    mvwaddch(main_menu_window, 2, 0, ACS_LTEE);
    mvwhline(main_menu_window, 2, 1, ACS_HLINE, 38);
    mvwaddch(main_menu_window, 2, 39, ACS_RTEE);
    refresh();

    // post the menu
    post_menu(main_menu);
    wrefresh(main_menu_window);

    while ((c = wgetch(main_menu_window)) != KEY_F(1))
    {
        switch (c)
        {
        case KEY_DOWN:
            menu_driver(main_menu, REQ_DOWN_ITEM);
            break;

        case KEY_UP:
            menu_driver(main_menu, REQ_UP_ITEM);
            break;
        }
    }

    // unpost and free all the memory taken up
    unpost_menu(main_menu);
    free_menu(main_menu);
    
    for (i = 0; i < n_gameOptions; ++i)
    {
        free_item(menu_items[i]);
    }
        
    endwin();

}

// FIN 



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