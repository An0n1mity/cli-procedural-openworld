#ifndef RENDERING_H
#define RENDERING_H

#define _XOPEN_SOURCE_EXTENDED 1
#define NCURSES_WIDECHAR 1

#include <ncurses.h>
#include "Block.h"
#include "Tilemap.h"

#define COLOR_WATER 16
#define COLOR_GRASS 17
#define COLOR_SAND 18
#define COLOR_STONE 19

typedef enum DISPLAY_MODE_E
{
    MAIN_MENU,
    WORLD
} DISPLAY_MODE_e;

// typedef struct Panel_s
// {
//     WINDOW* win;
    
//     int height;
//     int width;

// } Panel_s;

typedef struct View_S
{
    int m_height;
    int m_width;

    Coordinate_s m_coord;
} View_s;

typedef struct Term_S
{
    int height;
    int width;

    DISPLAY_MODE_e displayMode;

    WINDOW* mainMenu;
    WINDOW* world;
    WINDOW* stats;

    Tilemap_s* tilemap;
    View_s *view;

} Term_s;

Term_s * initDisplaying();
void displayTerm(Term_s *term, View_s *view);
void displayWorld(Term_s *term, View_s *view);
void displayChunks(Term_s *term, View_s *view);
WINDOW *createWindow(int height, int width, int starty, int startx);


#endif /* !RENDERING_H */