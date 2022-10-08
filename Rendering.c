#include "Rendering.h"

Term_s* initDisplaying()
{
    initscr();
    raw();
    noecho();
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();
    
    Term_s* term = calloc(1, sizeof(Term_s));
    term->height = getmaxy(stdscr);
    term->width = getmaxx(stdscr);

    term->displayMode = WORLD;

    term->world = createWindow(term->height, term->width, 0, 0);

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

void displayTerm(Term_s* term)
{
    if(term->displayMode == WORLD)
    {
        displayWorld(term);
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

void displayWorld(Term_s* term)
{
    //move(1, 1);
    clear();
    //init_pair(1, COLOR_WHITE, COLOR_RED);
    
    for (int h = 0; h < term->height; ++h)
    {
        for (int w = 0; w < term->width; ++w)
        {
            struct Block_s** actualBlock = term->tilemap->m_array[h * term->tilemap->m_width + w];
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
            // init_color(COLOR_WATER, 500, 500, 1000);
            

        
            //wchgat(term->world, -1, A_BLINK, 1, NULL);
            // attron(COLOR_PAIR(1));
            
            // // attroff(COLOR_PAIR(1));
            wattron(term->world, COLOR_PAIR(color));
            //wprintw(term->world, "🌲");
            
            waddwstr(term->world,  L"🌲");
            wrefresh(term->world);
            refresh();
            // if (actualBlock[1])
            // {
            //     switch (actualBlock[1]->m_type)
            //     {
            //     case EVERGREEN_TREE:
            //         printf("🌲");
            //         break;
            //     case DECIDIOUS_TREE:
            //         printf("🌳");
            //         break;
            //     case ROCK:
            //         printf("🪨");
            //         break;
            //     case GRASS:
            //         printf("🌿");
            //         break;
            //     default:
            //         break;
            //     }
            // }

            // else if (h == term->tilemap->m_entities->m_entity->m_position.m_y && w == term->tilemap->m_entities->m_entity->m_position.m_x)
            //     printf("👨");
            // else
            //     printf("  ");
            
        }
        //move(5,5);
        // printf("👨");
        
        
    }
   // wrefresh(term->world);
    // refresh();

}

