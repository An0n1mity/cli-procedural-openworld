#include <ncurses.h>
#include <form.h>
#include <string.h>

enum MenuChoice_e
{
    NEW_GAME,
    LOAD_GAME
};

enum MenuChoice_e titleLoop(WINDOW *title_window);
void seedMenu(WINDOW *seed_window, int *seed);