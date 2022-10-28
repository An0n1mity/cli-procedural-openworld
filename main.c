//#define _XOPEN_SOURCE_EXTENDED 1
//#define NCURSES_WIDECHAR 1

#include <stdio.h>
#include <termios.h>
#include <locale.h>
#include <ncurses.h>
#include <time.h>

#include "Constant.h"

#include "Player.h"
#include "Block.h"
#include "Tilemap.h"
#include "PerlinNoise.h"
#include "Menu.h"
#include "Rendering.h"
#include "Camera.h"
#include "Chunk.h"
#include "Chicken.h"

#define ctrl(x) ((x)&0x1f)
int main(int argc, char const *argv[])
{
    if (atexit(cookedOnExit))
        return EXIT_FAILURE;

    setlocale(LC_ALL, "");
    Term_s *term = initDisplaying();

    remove("../saved_chunks");
    FILE *f = fopen("../saved_chunks", "wb");
    fclose(f);
    int seed = 563;
    int quit = 0;

    // titleLoop(createWindow(20, 40, 0, 0));
    // seedMenu(createWindow(20, 40, 0, 0), &seed);

    // endwin();

    Player_s *player = CreatePlayer();
    Chicken_s *chicken = CreateChicken();
    Action_e player_action = BREAK;
    Tilemap_s *tilemap = CreateTilemapProcedurally(CHUNK_SIZE * MAX_CHUNK_DISTANCE, CHUNK_SIZE * MAX_CHUNK_DISTANCE, seed);
    // CreateTilemapFromFile("../map.txt");
    player->m_base->m_direction = EAST;

    // Chunk testing
    MovePlayerTo(player, (Coordinate_s){11, 25});
    addPlayerToTilemap(player, tilemap);
    MoveChickenTo(chicken, (Coordinate_s){11, 26});
    addChickenToTilemap(chicken, tilemap);

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
    size_t nb_ticks = 0;

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
            // case 'e':
            //   possible_crafts = getPossibleCrafts(player);
            //   break;
            case ctrl('c'):
            case KEY_F(1):
                quit = 1;
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
                long cursor = whereisChunkInFile(previous_chunk_coord, "../saved_chunks");
                if (cursor >= 0)
                {
                    writeChunkToFileAt(tilemap->m_previous_chunk, "../saved_chunks", cursor);
                }
                else
                    writeChunkToFile(tilemap->m_previous_chunk, "../saved_chunks");

                tilemap->m_save_previous_chunk = false;
            }

            previous_chunk_coord = player->m_base->m_chunk_position;
            LoadChunkAroundPlayer(player, seed, false, MAX_CHUNK_DISTANCE / 2, MAX_CHUNK_DISTANCE / 2);
        }
        displayTerm(term, NULL);
        if (!(nb_ticks % 5000))
        {
            if (player->m_action == MOVE)
                player->m_action = IDLE;
            nb_ticks = 0;
        }
        Entitieslist_s *entity_list = term->tilemap->m_entities;
        while (entity_list != NULL) //&& entity_list->m_entity != NULL)
        {

            switch (entity_list->m_entity->m_type)
            {

            case CHICKEN:
                if(entity_list->m_entity->m_health == 0)
                    
                    
                break;
            case PLAYER:
            default:
                break;
            }

            entity_list = entity_list->m_next;
        }

        calculateFPS(term, actualTime_ms - previouTime_ms);
        nb_ticks++;
        // if ((double)(clock() - ticks) / CLOCKS_PER_SEC >= 1.0)
        // {
        //     ticks = clock();
        //     if (nb_ticks >= 65535)
        //         nb_ticks = 0;
        //     nb_ticks++;

        //     if (nb_ticks > 0 && !(nb_ticks % 120))
        //         reducePlayerFoodLevel(player);

        //     if (nb_ticks > 0 && !(nb_ticks % 60))
        //         reducePlayerFoodLevel(player);

        //     if (nb_ticks > 0 && !(nb_ticks % 60) && player->m_vitals[FOOD_LVL] <= 0 && player->m_vitals[WATER_LVL] <= 0)
        //         reducePlayerHealth(player);
        // }
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
