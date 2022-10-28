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

    term->height = getmaxy(stdscr) - 1;
    term->width = getmaxx(stdscr) - 1;

    term->displayMode = WORLD;

    term->world = createWindow(term->height - 5, term->width, 0, 0);
    term->stats = createWindow(5, term->width - 30, term->height - 5, 0);
    term->crafts = createWindow(5, 20, term->height - 5, 0);

    init_color(COLOR_WATER, 500, 500, 1000);
    init_color(COLOR_GRASS, 10, 700, 450);
    init_color(COLOR_SAND, 1000, 800, 0);
    init_color(COLOR_STONE, 500, 500, 500);

    for (int i = 1; i < 16; i++)
        init_pair(i, COLOR_WHITE, COLOR_RED);
    init_pair(COLOR_WATER, COLOR_WHITE, COLOR_WATER);
    init_pair(COLOR_GRASS, COLOR_WHITE, COLOR_GRASS);
    init_pair(COLOR_SAND, COLOR_WHITE, COLOR_SAND);
    init_pair(COLOR_STONE, COLOR_WHITE, COLOR_STONE);
    init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);
    return term;
}

void displayPlayerStats(Term_s *term);
void displayCrafts(Term_s *term);

void displayTerm(Term_s *term, View_s *view)
{
    int h = getmaxy(stdscr);
    int w = getmaxx(stdscr) - ((getmaxx(stdscr) + 1) % 2);

    if (term->displayMode == WORLD)
    {
        if (term->height != h || term->width != w)
        {
            term->height = h;
            term->width = w;
            wclear(term->world); // clear all screen for deleting remanent caracters on resizing
            wclear(term->stats); // clear all screen for deleting remanent caracters on resizing
            wrefresh(stdscr);
        }
        wresize(term->world, term->height - 5, term->width);
        wresize(term->stats, 5, term->width);

        mvwin(term->stats, term->height - 5, 0);

        displayPlayerStats(term);
        displayWorld(term, view);
        displayCrafts(term);
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
    int minDisplay_x = MIN(term->world->_maxx / 2, (CHUNK_SIZE * MAX_CHUNK_DISTANCE));
    int maxDisplay_x = minDisplay_x;
    minDisplay_x = (CHUNK_SIZE * MAX_CHUNK_DISTANCE) / 2 - (minDisplay_x / 2);
    maxDisplay_x = maxDisplay_x + minDisplay_x;

    int minDisplay_y = MIN(term->world->_maxy, (CHUNK_SIZE * MAX_CHUNK_DISTANCE));
    int maxDisplay_y = minDisplay_y;
    minDisplay_y = (CHUNK_SIZE * MAX_CHUNK_DISTANCE) / 2 - (minDisplay_y / 2);
    maxDisplay_y = maxDisplay_y + minDisplay_y;
    Coordinate_s entity_tilemap_coord = getEntityTilemapCoordinate(term->tilemap->m_entities->m_entity);

    Coordinate_s screen_world_coord = term->tilemap->m_chunks[0][0]->world_position;
    screen_world_coord.m_x += minDisplay_x;
    screen_world_coord.m_y += minDisplay_y;
    int initial_x = screen_world_coord.m_x;
    for (int h = minDisplay_y; h < maxDisplay_y; ++h, screen_world_coord.m_y++, screen_world_coord.m_x = initial_x)
    {
        for (int w = minDisplay_x; w < maxDisplay_x; ++w, screen_world_coord.m_x++)
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
                case PLANK:
                    waddwstr(term->world, L"🟫"); //🟫
                    break;
                case ROCK:
                    waddwstr(term->world, L"🗿");
                    break;
                }
            }

            else if (term->tilemap->m_entities->m_entity->m_position.m_x == screen_world_coord.m_x &&
                     term->tilemap->m_entities->m_entity->m_position.m_y == screen_world_coord.m_y)
            {
                waddwstr(term->world, L"🧍"); //🧍 🏊 🏄🪵
            }

            else
                wprintw(term->world, "  ");
        }
        wprintw(term->world, "\n");
    }

    wattron(term->world, COLOR_PAIR(1));
    mvwprintw(term->world, 0, 0, "%.2lf", term->framerate);
    wrefresh(term->world);
    wmove(term->world, 0, 0);
}

void displayPlayerStats(Term_s *term)
{
    // wborder(term->stats, '|', '|', '-', '-', '+', '+', '+', '+');
    box(term->stats, 0, 0);
    Player_s *player = term->tilemap->m_player;
    // if (player->update_stats)
    // {
    static size_t previous_heart_lvl = 0;
    static size_t previous_food_lvl = 0;
    static size_t previous_water_lvl = 0;

    size_t hp_lvl = player->m_base->m_health;
    size_t food_lvl = player->m_vitals[FOOD_LVL] / 10;
    size_t water_lvl = player->m_vitals[WATER_LVL] / 10;

    // if ((previous_food_lvl == food_lvl) && (previous_water_lvl == water_lvl) && (hp_lvl == previous_heart_lvl))
    //     return;

    previous_heart_lvl = hp_lvl;
    previous_food_lvl = food_lvl;
    previous_water_lvl = water_lvl;

    mvwprintw(term->stats, 1, 1, "HEART : ");

    // Print player HP
    for (size_t i = 0; i < player->m_base->m_health; i++)
    {
        wprintw(term->stats, "♥");
    }

    // Print player food level
    mvwprintw(term->stats, 2, 1, "FOOD   : ");

    for (size_t i = 0; i < food_lvl; i++)
    {
        waddwstr(term->stats, L"🍗");
    }

    for (size_t i = 0; i < 10 - food_lvl; i++)
    {
        wprintw(term->stats, " ");
    }

    // Print player water level
    mvwprintw(term->stats, 3, 1, "WATER  : ");

    for (size_t i = 0; i < water_lvl; i++)
    {
        waddwstr(term->stats, L"💧");
    }

    for (size_t i = 0; i < 10 - water_lvl; i++)
    {
        wprintw(term->stats, " ");
    }

    // Print player current action
    Block_s **block_in_front;
    mvwprintw(term->stats, 1, 35, "ACTION : ");
    switch (term->tilemap->m_player->m_action)
    {
    case MOVE:
        wprintw(term->stats, "MOVING               ");
        break;
    case BREAK:
        block_in_front = getFrontBlock(player->m_base, term->tilemap);
        if (block_in_front[1]->m_health <= 0)
        {
            player->m_action = IDLE;
            break;
        }
        wprintw(term->stats, "BREAKING");
        switch (block_in_front[1]->m_type)
        {
        case EVERGREEN_TREE:
            wprintw(term->stats, "(TREE, %d♥)", block_in_front[1]->m_health);
            break;

        default:
            break;
        }
        break;

    default:
        wprintw(term->stats, "IDLE                   ");
        break;
    }

    // Print player current orientation
    mvwprintw(term->stats, 2, 35, "🧭 : ");
    switch (player->m_base->m_direction)
    {
    case NORTH:
        wprintw(term->stats, "↑");
        break;
    case SOUTH:
        wprintw(term->stats, "↓");
        break;
    case WEST:
        wprintw(term->stats, "←");
        break;
    case EAST:
        wprintw(term->stats, "→");
        break;

    default:
        break;
    }

    // Print player holding tool
    mvwprintw(term->stats, 3, 35, "TOOL : ");
    Block_s *block;
    Tool_s *tool;

    // Print all inventory
    for (size_t i = 0; i < 9; i++)
    {
        if (i == player->m_inventory.m_idx)
            wattron(term->stats, COLOR_PAIR(COLOR_WHITE));

        else
            wattroff(term->stats, COLOR_PAIR(COLOR_WHITE));

        switch (player->m_inventory.m_objects[i].m_type)
        {
        case BLOCK:
            block = player->m_inventory.m_objects[i].m_data;
            switch (block->m_type)
            {
            case PLANK:
                waddwstr(term->stats, L"🟫");
                break;

            default:
                break;
            }
            break;

        case TOOL:
            tool = player->m_inventory.m_objects[i].m_data;
            switch (tool->m_type)
            {
            case PICKAXE:
                break;

            case SWORD:
                break;

            default:
                break;
            }
            break;
        default:
            wprintw(term->stats, "▫️ "); //🔲▱▢▯▭□⨆⌊⌋[]🔳◽
            break;
        }
    }

    wmove(term->stats, 0, 0);
    wattroff(term->stats, COLOR_PAIR(COLOR_WHITE));

    player->update_stats = false;
    // }
    wrefresh(term->stats);
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

void displayCrafts(Term_s *term)
{
    Player_s *player = term->tilemap->m_player;
    // Get list of possible crafts
    CraftType_e possible_crafts = getPossibleCrafts(player);

    wrefresh(term->crafts);
}

void cookedOnExit()
{

    noraw();
    echo();
    endwin();

    return;
}

void calculateFPS(Term_s *term, double timeDif)
{
    term->framerate = 1 / (timeDif / 1000.0);
}