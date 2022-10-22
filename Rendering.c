#include "Rendering.h"

Term_s *initDisplaying()
{
    initscr();
    raw();
    noecho();
    curs_set(0);
    if (has_colors() == FALSE)
    {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();

    Term_s *term = calloc(1, sizeof(Term_s));
    term->height = getmaxy(stdscr);
    term->width = getmaxx(stdscr);

    term->displayMode = WORLD;

    term->world = createWindow(term->height, term->width, 0, 0);

    init_color(COLOR_WATER, 500, 500, 1000);
    init_color(COLOR_GRASS, 10, 700, 450);
    init_color(COLOR_SAND, 1000, 800, 0);
    init_color(COLOR_STONE, 500, 500, 500);

    for (int i = 1; i < 16; i++)
        init_pair(i, COLOR_RED, COLOR_RED);
    init_pair(COLOR_WATER, COLOR_WHITE, COLOR_WATER);
    init_pair(COLOR_GRASS, COLOR_WHITE, COLOR_GRASS);
    init_pair(COLOR_SAND, COLOR_WHITE, COLOR_SAND);
    init_pair(COLOR_STONE, COLOR_WHITE, COLOR_STONE);

    return term;
}

void displayTerm(Term_s *term, View_s *view)
{

    if (term->displayMode == WORLD)
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

// TODO Do not render all generated chunks only blocks visibles in viewport

void displayChunks(Term_s *term, View_s *view)
{

    Tilemap_s *tilemap = term->tilemap;
    for (size_t i = 0; i < 3; i++)
    {
        Chunk_s *f = tilemap->m_chunks[i][0];
        Chunk_s *s = tilemap->m_chunks[i][1];
        Chunk_s *t = tilemap->m_chunks[i][2];

        for (size_t j = 0; j < CHUNK_SIZE; j++)
        {
            Chunk_s *actual_chunk = f;
            for (size_t k = 0; k < CHUNK_SIZE; k++)
            {

                Block_s **actual_block = actual_chunk->m_blocks[j * CHUNK_SIZE + k];
                attr_t attr = 1 & A_STANDOUT;
                short color = 0;
                if (actual_block[0])
                {
                    switch (actual_block[0]->m_type)
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

                if (actual_block[1])
                {
                    switch (actual_block[1]->m_type)
                    {
                    case EVERGREEN_TREE:
                        waddwstr(term->world, L"🌲");
                        break;
                    case ROCK:
                        waddwstr(term->world, L"🪨");
                        break;
                        // prout
                    }
                }

                else
                    wprintw(term->world, "  ");
            }

            actual_chunk = s;
            for (size_t k = 0; k < CHUNK_SIZE; k++)
            {

                Block_s **actual_block = actual_chunk->m_blocks[j * CHUNK_SIZE + k];
                attr_t attr = 1 & A_STANDOUT;
                short color = 0;
                if (actual_block[0])
                {
                    switch (actual_block[0]->m_type)
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

                if (actual_block[1])
                {
                    switch (actual_block[1]->m_type)
                    {
                    case EVERGREEN_TREE:
                        waddwstr(term->world, L"🌲");
                        break;
                    case ROCK:
                        waddwstr(term->world, L"🪨");
                        break;
                        // prout
                    }
                }

                else
                    wprintw(term->world, "  ");
                wrefresh(term->world);
            }

            actual_chunk = t;
            for (size_t k = 0; k < CHUNK_SIZE; k++)
            {

                Block_s **actual_block = actual_chunk->m_blocks[j * CHUNK_SIZE + k];
                attr_t attr = 1 & A_STANDOUT;
                short color = 0;
                if (actual_block[0])
                {
                    switch (actual_block[0]->m_type)
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

                if (actual_block[1])
                {
                    switch (actual_block[1]->m_type)
                    {
                    case EVERGREEN_TREE:
                        waddwstr(term->world, L"🌲");
                        break;
                    case ROCK:
                        waddwstr(term->world, L"🪨");
                        break;
                        // prout
                    }
                }

                else
                    wprintw(term->world, "  ");
            }
            wprintw(term->world, "\n");
        }
    }
    wrefresh(term->world);
    wmove(term->world, 0, 0);
}

void displayWorld(Term_s *term, View_s *view)
{
    Coordinate_s screen_world_coord = term->tilemap->m_chunks[0][0]->world_position;
    int initial_x = screen_world_coord.m_x;

    for (int h = 0; h < term->tilemap->m_height && h < term->height; ++h, screen_world_coord.m_y++, screen_world_coord.m_x = initial_x)
    {
        for (int w = 0; w < term->tilemap->m_width && w < term->width/2; ++w, screen_world_coord.m_x++)
        {

            Block_s **actualBlock = term->tilemap->m_blocks[h * CHUNK_SIZE * MAX_CHUNK_DISTANCE + w];
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
                    waddwstr(term->world, L"🪨");
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
            //wrefresh(term->world);
        }
        wprintw(term->world, "\n");
    }
    wrefresh(term->world);
    wmove(term->world, 0, 0);
   // usleep(50000);
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