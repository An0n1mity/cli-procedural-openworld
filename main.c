#define _XOPEN_SOURCE_EXTENDED 1
#define NCURSES_WIDECHAR 1

#include <stdio.h>
#include <termios.h>
#include <locale.h>
#include <ncurses.h>
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
    if (atexit(cookedOnExit))
        return EXIT_FAILURE;

    setlocale(LC_ALL, "");
    Term_s *term = initDisplaying();

    int seed = 69;
    int quit = 0;

    // titleLoop(createWindow(20, 40, 0, 0));
    // seedMenu(createWindow(20, 40, 0, 0), &seed);

    // endwin();

    Player_s *player = CreatePlayer();
    Action_e player_action = BREAK;
    Tilemap_s *tilemap = CreateTilemapProcedurally(CHUNK_SIZE * MAX_CHUNK_DISTANCE, CHUNK_SIZE * MAX_CHUNK_DISTANCE, seed);
    // CreateTilemapFromFile("../map.txt");

    player->m_base->m_direction = SOUTH;

    // PrintTilemap(tilemap);

    // MakeAction(player, MOVE);
    player->m_base->m_direction = EAST;

    // Block_s *front_block = getFrontBlockP(player, tilemap);
    // MakeActionOnBlock(BREAK, front_block);
    // PrintTilemap(tilemap);

    // Chunk testing
    MovePlayerTo(player, (Coordinate_s){10, 20});
    addPlayerToTilemap(player, tilemap);
    Coordinate_s previous_chunk_coord = getEntityChunkCoordinate(player->m_base);
    LoadChunkAroundPlayer(player, seed, true, MAX_CHUNK_DISTANCE, MAX_CHUNK_DISTANCE);

    setlocale(LC_ALL, "");
    // Term_s *term = initDisplaying();
    nodelay(term->world, TRUE);
    int move_x = 0, move_y = 0, c = 0;
    keypad(term->world, TRUE);
    term->tilemap = tilemap;
    size_t try = 0;
    while (!quit && try < 2)
    {
        move_x = 0;
        move_y = 0;
        c = wgetch(term->world);
        while (c != ERR)
        {
            switch (c)
            {
            case KEY_RIGHT:
            case 'd':
                player->m_base->m_direction = EAST;
                move_x++;
                break;
            case KEY_LEFT:
            case 'q':
                player->m_base->m_direction = WEST;
                move_x--;
                break;
            case KEY_UP:
            case 'z':
                player->m_base->m_direction = NORTH;
                move_y--;
                break;
            case KEY_DOWN:
            case 's':
                player->m_base->m_direction = SOUTH;
                move_y++;
                break;
            case ctrl('c'):
            case KEY_F(1):
                quit = 1;
                break;

            default:
                break;
            }
            c = wgetch(term->world);
        }
        player->m_base->m_position.m_x += (move_x % 2);
        player->m_base->m_position.m_y += (move_y % 2);

        player->m_base->m_chunk_position = getEntityChunkCoordinate(player->m_base);
        if (memcmp(&player->m_base->m_chunk_position, &previous_chunk_coord, sizeof(Coordinate_s)))
        {
            previous_chunk_coord = player->m_base->m_chunk_position;
            LoadChunkAroundPlayer(player, seed, false, MAX_CHUNK_DISTANCE, MAX_CHUNK_DISTANCE);
            // try++;
        }
        displayTerm(term, NULL);
        // RenderCameraView(term, NULL);
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
