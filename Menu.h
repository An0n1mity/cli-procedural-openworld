#include <ncurses.h>
#include <form.h>
#include <menu.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum MenuChoice_e
{
    NEW_GAME,
    LOAD_GAME
};

enum MenuChoice_e titleLoop(WINDOW *title_window);
void seedMenu(WINDOW *seed_window, int *seed);