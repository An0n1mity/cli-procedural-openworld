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

void titleLoop(WINDOW *title_window)
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
    char input;
    size_t idx = 0;
    noecho();                   // disable echoing of characters on the screen
    keypad(title_window, TRUE); // enable keyboard input for the window.
    curs_set(0);
    while ((input = wgetch(title_window)) != "q")
    {
        mvwprintw(title_window, idx + 2 + (height / 2) - 2, width / 2 - strlen(title_strings[idx + 1]) / 2, "%s", title_strings[idx + 1]);

        switch (input)
        {
        case KEY_UP:
            idx--;
            idx = (idx < 0) ? 1 : idx;
            break;
        case KEY_DOWN:
            idx++;
            idx = (idx > 1) ? 0 : idx;
            break;
        }

        wattron(title_window, A_STANDOUT);
        mvwprintw(title_window, idx + 1, width / 2 - strlen(title_strings[idx + 1]) / 2, "%s", title_strings[idx + 1]);
        wattroff(title_window, A_STANDOUT);
        wrefresh(title_window);
    }
}