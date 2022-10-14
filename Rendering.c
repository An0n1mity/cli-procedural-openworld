#include "Rendering.h"

Term_s* initDisplaying()
{
    initscr();
    raw();
    noecho();
    curs_set(0);
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();

    keypad(stdscr, TRUE);
    
    Term_s* term = calloc(1, sizeof(Term_s));
    term->height = getmaxy(stdscr);
    term->width = getmaxx(stdscr);

    term->displayMode = WORLD;

    term->world = createWindow(50, 50, 0, 0);

    init_color(COLOR_WATER, 500, 500, 1000);
    init_color(COLOR_GRASS, 10, 700, 450);
    init_color(COLOR_SAND, 1000, 800, 0);
    init_color(COLOR_STONE, 500, 500, 500);

    for(int i = 1; i < 16; i++)
        init_pair(i, COLOR_RED, COLOR_RED);
    init_pair(COLOR_WATER, COLOR_WHITE, COLOR_WATER);
    init_pair(COLOR_GRASS, COLOR_WHITE, COLOR_GRASS);
    init_pair(COLOR_SAND, COLOR_WHITE, COLOR_SAND);
    init_pair(COLOR_STONE, COLOR_WHITE, COLOR_STONE);

    return term;
}

void displayTerm(Term_s *term, View_s *view)
{
    // if(term->displayMode == MAIN_MENU)
    // {
    //     displayWorld(term, view);
    // }

    if(term->displayMode == WORLD)
    {
        displayWorld(term, view);
    }
}

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

void displayWorld(Term_s *term, View_s *view)
{
    //clear();

    if (view->m_coord.m_x >= term->tilemap->m_width - view->m_width)
        view->m_coord.m_x = term->tilemap->m_width - view->m_width - 1;
    if (view->m_coord.m_y >= term->tilemap->m_height - view->m_height)
        view->m_coord.m_y = term->tilemap->m_height - view->m_height - 1;
    if (view->m_coord.m_x < 0)
        view->m_coord.m_x = 0;
    if (view->m_coord.m_y < 0)
        view->m_coord.m_y = 0;
    for (int h = view->m_coord.m_y; h < view->m_coord.m_y + view->m_height; ++h)
    {
        for (int w = view->m_coord.m_x; w < view->m_coord.m_x + view->m_width; ++w)
        {
            struct Block_s **actualBlock = term->tilemap->m_blocks[h * term->tilemap->m_width + w];
            attr_t attr = 1 & A_STANDOUT;
            short color = 0;
            if (actualBlock[0])
            {
                switch (actualBlock[0]->m_type)
                {
                case WATER:
                    color = COLOR_WATER;
                    break;
                case GRASS:
                    color = COLOR_GRASS;
                    break;
                case SAND:
                    color = COLOR_SAND;
                    break;
                case STONE:
                    color = COLOR_STONE;
                    break;
                }
            }

            wattron(term->world, COLOR_PAIR(color));

            if (actualBlock[1])
            {
                switch (actualBlock[1]->m_type)
                {
                case EVERGREEN_TREE:
                    waddwstr(term->world, L"^");
                    break;
                case ROCK:
                    waddwstr(term->world, L"r");
                    break;
                }
            }

            else
                wprintw(term->world, " ");
        }
        wprintw(term->world, "\n");
    }
    wrefresh(term->world);
    wmove(term->world, 0, 0);
    usleep(50000);
}
