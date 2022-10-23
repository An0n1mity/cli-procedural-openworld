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

    
    
    Term_s* term = calloc(1, sizeof(Term_s));

    term->height = getmaxy(stdscr);
    term->width = getmaxx(stdscr);

    term->displayMode = WORLD;

    term->world = createWindow(CHUNK_SIZE * 3, CHUNK_SIZE * 3 * 2 + 1, 0, 0);
    term->stats = createWindow(CHUNK_SIZE * 3, CHUNK_SIZE * 3 * 2 + 1, 0, CHUNK_SIZE * 3 * 2 + 1);

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

void displayPlayerStats(Term_s *term);

void displayTerm(Term_s *term, View_s *view)
{

    if(term->displayMode == WORLD)
    {
        displayWorld(term, view);
        displayPlayerStats(term);
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
    nodelay(window, TRUE);
    return window;
}

void displayWorld(Term_s *term, View_s *view)
{
    Coordinate_s screen_world_coord = term->tilemap->m_chunks[0][0]->world_position;
    int initial_x = screen_world_coord.m_x;
    for (int h = 0; h < term->tilemap->m_height; ++h, screen_world_coord.m_y++, screen_world_coord.m_x = initial_x)
    {
        for (int w = 0; w < term->tilemap->m_width; ++w, screen_world_coord.m_x++)
        {

            Block_s **actualBlock = term->tilemap->m_blocks[h * CHUNK_SIZE * 3 + w];
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
                    waddwstr(term->world, L"🌲");
                    break;
                case ROCK:
                    waddwstr(term->world, L"🗿");
                    break;
                }
            }

            else if (term->tilemap->m_entities->m_entity->m_position.m_x == screen_world_coord.m_x &&
                     term->tilemap->m_entities->m_entity->m_position.m_y == screen_world_coord.m_y)
            {
                waddwstr(term->world, L"🧍");
            }

            else
                wprintw(term->world, "  ");
            wrefresh(term->world);
        }
        wprintw(term->world, "\n");
    }
    wrefresh(term->world);
    wmove(term->world, 0, 0);
}

void displayPlayerStats(Term_s *term)
{
    Player_s *player = term->tilemap->m_player;
    if (player->update_stats)
    {
        static size_t previous_heart_lvl = 0;
        static size_t previous_food_lvl = 0;
        static size_t previous_water_lvl = 0;

        size_t hp_lvl = player->m_base->m_health;
        size_t food_lvl = player->m_vitals[FOOD_LVL] / 10;
        size_t water_lvl = player->m_vitals[WATER_LVL] / 10;

        if ((previous_food_lvl == food_lvl) && (previous_water_lvl == water_lvl) && (hp_lvl == previous_heart_lvl))
            return;

        previous_heart_lvl = hp_lvl;
        previous_food_lvl = food_lvl;
        previous_water_lvl = water_lvl;

        mvwprintw(term->stats, 1, 1, "HEALTH : ");

        // Print player HP
        for (size_t i = 0; i < player->m_base->m_health; i++)
        {
            wprintw(term->stats, "♥");
        }

        // Print player food level
        mvwprintw(term->stats, 2, 1, "FOOD   : ");

        for (size_t i = 0; i < food_lvl; i++)
        {
            wprintw(term->stats, "🍗");
        }

        for (size_t i = 0; i < 10 - food_lvl; i++)
        {
            wprintw(term->stats, " ");
        }

        // Print player water level
        mvwprintw(term->stats, 3, 1, "WATER  : ");

        for (size_t i = 0; i < water_lvl; i++)
        {
            wprintw(term->stats, "💧");
                }

        for (size_t i = 0; i < 10 - water_lvl; i++)
        {
            wprintw(term->stats, " ");
        }

        wrefresh(term->stats);
        wmove(term->stats, 0, 0);

        player->update_stats = false;
    }
}

void RenderCameraView(Term_s *term, struct Camera_s *camera)
{
    for (int h = 0; h < term->tilemap->m_height; ++h)
    {
        for (int w = 0; w < term->tilemap->m_width; ++w)
        {
            Block_s **actualBlock = term->tilemap->m_blocks[h * CHUNK_SIZE + w];
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

}
void cookedOnExit()
{
    
    noraw();
    echo();
    endwin();

    return;
}