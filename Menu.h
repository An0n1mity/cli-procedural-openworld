#ifndef MENU_H
#define MENU_H

#include <ncurses.h>
#include <form.h>
#include <menu.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "Rendering.h"

enum MenuChoice_e
{
    NEW_GAME,
    LOAD_GAME,
    PLAYER_CONFIG,
    WORLD_CONFIG,
    STATS_HELP,
    CREDITS,
    QUIT
};

enum MenuChoice_e titleLoop(WINDOW *title_window);
void seedMenu(WINDOW *seed_window, int *seed);

void creditsMenu(WINDOW *p_credits_window);

void centerPrint(WINDOW *win, int startY, int startX, int width, char *string, chtype color);
void mainMenu();

#endif /* !MENU_H */