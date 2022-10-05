#include "Rendering.h"

WINDOW *createWindow(int height, int width, int starty, int startx)
{
    WINDOW *window;

    window = newwin(height, width, starty, startx);
    box(window, 0, 0); /* 0, 0 gives default characters
                        * for the vertical and horizontal
                        * lines			*/
    wrefresh(window);  /* Show that box 		*/

    return window;
}
