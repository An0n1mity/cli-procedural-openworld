#define _XOPEN_SOURCE_EXTENDED 1
#define NCURSES_WIDECHAR 1

#include <ncurses.h>
#include "Block.h"
#include "Tilemap.h"

#define COLOR_WATER 16
#define COLOR_GRASS 17
#define COLOR_SAND 18
#define COLOR_STONE 19

typedef enum DISPLAY_MODE_e
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
typedef struct Term_s
{
    int height;
    int width;

    DISPLAY_MODE_e displayMode;

    WINDOW* mainMenu;
    WINDOW* world;
    WINDOW* stats;

    struct Tilemap_s* tilemap;

} Term_s;

Term_s* initDisplaying();
void displayTerm(Term_s* term);
void displayWorld(Term_s* term);
WINDOW *createWindow(int height, int width, int starty, int startx);
