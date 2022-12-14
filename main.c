//#define _XOPEN_SOURCE_EXTENDED 1
//#define NCURSES_WIDECHAR 1

#include <stdio.h>
#include <termios.h>
#include <locale.h>
#include <ncurses.h>
#include <time.h>
#include "Player.h"
#include "Block.h"
#include "Tilemap.h"
#include "PerlinNoise.h"
#include "Menu.h"
#include "Rendering.h"
#include "Camera.h"
#include "Chunk.h"

#define ctrl(x) ((x)&0x1f)
int main(int argc, char const *argv[])
{
    int quit = 0;
    if (atexit(cookedOnExit))
        return EXIT_FAILURE;

    setlocale(LC_ALL, "");

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    // Main menu
    int seed = 563;

    // Based on choice new menu
    Player_s *player = CreatePlayer();
    FILE *f;
    FILE *g;
    bool menu;
menu:
    menu = true;
    while (menu)
    {
        enum MenuChoice_e choice = titleLoop(createWindow(10, 30, 0, 10));

        switch (choice)
        {
        case NEW_GAME:
            // Recreate new saved chunks
            if (access("saved_chunks", F_OK) == 0)
                remove("saved_chunks");
            f = fopen("saved_chunks", "wb");
            fclose(f);
            MovePlayerTo(player, (Coordinate_s){40, 17});
            menu = false;
            break;
        case CREDITS:
            creditsMenu(createWindow(11, 45, 0, 0));
            break;

        case LOAD_GAME:
            if ((access("saved_player", F_OK) == 0) && access("saved_chunks", F_OK) == 0)
            {
                readPlayerFromFile(player, "saved_player");

                menu = false;
            }

            break;
        case QUIT:
            exit(0);
            break;
        }
    }
    Term_s *term = initDisplaying();

    Action_e player_action = BREAK;
    Tilemap_s *tilemap = CreateTilemapProcedurally(CHUNK_SIZE * MAX_CHUNK_DISTANCE, CHUNK_SIZE * MAX_CHUNK_DISTANCE, seed);

    addPlayerToTilemap(player, tilemap);
    Coordinate_s previous_chunk_coord = getEntityChunkCoordinate(player->m_base);
    LoadChunkAroundPlayer(player, seed, true, MAX_CHUNK_DISTANCE / 2, MAX_CHUNK_DISTANCE / 2);
    nodelay(stdscr, TRUE);
    int move_x = 0, move_y = 0, c = 0;
    keypad(term->world, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    mouseinterval(0);
    term->tilemap = tilemap;
    size_t try = 0;

    clock_t ticks = clock();
    size_t nb_ticks_sprite = 0;
    size_t nb_ticks_vitals = 0;

    double actualTime_ms = 0;
    double previouTime_ms = 0;

    MEVENT event;
    CraftType_e possible_crafts;

    while (!quit)
    {
        move_x = 0;
        move_y = 0;

        previouTime_ms = actualTime_ms;
        actualTime_ms = (double)(clock() - ticks) * 1000.0 / (double)CLOCKS_PER_SEC;

        c = wgetch(term->world);

        while (c != ERR)
        {
            switch (c)
            {
            // case KEY_RIGHT:
            case 'D':
            case 'd':
                player->m_base->m_direction = EAST;
                MovePlayer(player, actualTime_ms);
                break;
            // case KEY_LEFT:
            case 'Q':
            case 'q':
                player->m_base->m_direction = WEST;
                MovePlayer(player, actualTime_ms);
                break;
            case 'Z':
            case 'z':
                player->m_base->m_direction = NORTH;
                MovePlayer(player, actualTime_ms);
                break;
            case 'S':
            case 's':
                player->m_base->m_direction = SOUTH;
                MovePlayer(player, actualTime_ms);
                break;
            case ctrl('c'):
            case KEY_F(1):
                // Store in a file the player position
                writePlayerToFile(player, "saved_player");
                // Go back to menu
                goto menu;
                break;

            case KEY_MOUSE:
                if (getmouse(&event) == OK)
                {
                    // Get block in front of the player
                    Block_s **block = getFrontBlock(player->m_base, player->m_base->m_tilemap);
                    if (event.bstate & BUTTON1_PRESSED)
                    {
                        // If block in front
                        if (block[1])
                            breakBlockInFront(player);
                    }
                    else if (event.bstate & BUTTON3_PRESSED)
                    {
                        if (block[1])
                            pickBlockInFront(player);
                        else
                            placeBlockInFront(player);
                    }
                    else if (event.bstate & BUTTON4_PRESSED) // scroll up
                    {
                        moveInventoryCursorLeft(player);
                    }
                    else if (event.bstate & BUTTON5_PRESSED) // scroll down
                    {
                        moveInventoryCursorRight(player);
                    }
                }
                break;

            case KEY_DOWN:
                if (player->m_craft_selected && player->m_craft_selected->m_previous)
                    player->m_craft_selected = player->m_craft_selected->m_previous;
                break;

            case KEY_UP:
                if (player->m_craft_selected && player->m_craft_selected->m_next)
                    player->m_craft_selected = player->m_craft_selected->m_next;
                break;

            case KEY_LEFT:
                moveInventoryCursorLeft(player);
                break;

            case KEY_RIGHT:
                moveInventoryCursorRight(player);
                break;

            case 10:
                addSelectedCraftToInventory(player);
                break;

            default:
                break;
            }
            c = wgetch(term->world);
        }

        player->m_base->m_chunk_position = getEntityChunkCoordinate(player->m_base);
        if (memcmp(&player->m_base->m_chunk_position, &previous_chunk_coord, sizeof(Coordinate_s)))
        {
            if (tilemap->m_save_previous_chunk)
            {
                // Check if the changed chunk was already in the save file
                long cursor = whereisChunkInFile(previous_chunk_coord, "saved_chunks");
                if (cursor >= 0)
                {
                    writeChunkToFileAt(tilemap->m_previous_chunk, "saved_chunks", cursor);
                }
                else
                    writeChunkToFile(tilemap->m_previous_chunk, "saved_chunks");

                tilemap->m_save_previous_chunk = false;
            }

            previous_chunk_coord = player->m_base->m_chunk_position;
            LoadChunkAroundPlayer(player, seed, false, MAX_CHUNK_DISTANCE / 2, MAX_CHUNK_DISTANCE / 2);
        }
        displayTerm(term, NULL);
        if (nb_ticks_sprite && !(nb_ticks_sprite % 5000))
        {
            if (player->m_action == MOVE)
                player->m_action = IDLE;
            nb_ticks_sprite = 0;
        }

        if (nb_ticks_sprite && !(nb_ticks_vitals % 10000))
        {
            reducePlayerFoodLevel(player, .1f);
            reducePlayerWaterLevel(player, .2f);
            nb_ticks_vitals = 0;
        }

        calculateFPS(term, actualTime_ms - previouTime_ms);
        nb_ticks_sprite++;
        nb_ticks_vitals++;
    }

    freeEntitiesList(tilemap->m_entities);
    free(tilemap->m_blocks);

    for (size_t i = 0; i < MAX_CHUNK_DISTANCE; i++)
    {
        for (size_t j = 0; j < MAX_CHUNK_DISTANCE; j++)
        {
            for (size_t k = 0; k < CHUNK_SIZE * CHUNK_SIZE; k++)
            {
                free(tilemap->m_chunks[i][j]->m_blocks[k][0]);
                free(tilemap->m_chunks[i][j]->m_blocks[k][1]);
                free(tilemap->m_chunks[i][j]->m_blocks[k]);
            }

            free(tilemap->m_chunks[i][j]->m_blocks);
            free(tilemap->m_chunks[i][j]);
        }
    }
    freePlayer(player);
    free(tilemap);
    noraw();
    echo();
    endwin();

    return 0;
}
