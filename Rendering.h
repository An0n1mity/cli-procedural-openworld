#ifndef RENDERING_H
#define RENDERING_H

#include "Constant.h"

#include <ncurses.h>
#include <menu.h>
#include "Block.h"
#include "Tilemap.h"
#include "Camera.h"
#include "Player.h"

typedef enum DISPLAY_MODE_E
{
    MAIN_MENU,
    WORLD
} DISPLAY_MODE_e;

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
    WINDOW *crafts;

    double framerate;

    Tilemap_s *tilemap;

} Term_s;

Term_s * initDisplaying();
void calculateFPS(Term_s *term, double timeDif);
void cookedOnExit();
void displayTerm(Term_s *term, View_s *view);
void displayWorld(Term_s *term, View_s *view);
void RenderCameraView(Term_s *term, struct Camera_s *camera);
void displayChunks(Term_s *term, View_s *view);
WINDOW *createWindow(int height, int width, int starty, int startx);


#endif /* !RENDERING_H */