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

int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "");
    Term_s* term = initDisplaying();

    int seed = 563;
    int quit = 0;

    // titleLoop(createWindow(20, 40, 0, 0));
    // seedMenu(createWindow(20, 40, 0, 0), &seed);
    
    // endwin();

    struct Player_s *player = CreatePlayer();
    enum Action_e player_action = BREAK;
    struct Tilemap_s *tilemap = CreateTilemapProcedurally(CHUNK_SIZE * 3, CHUNK_SIZE * 3, seed);
    // CreateTilemapFromFile("../map.txt");

    player->m_base->m_direction = SOUTH;

    //PrintTilemap(tilemap);

    // printf("Player's position : %d %d\n\r", player->m_base->m_position.m_x, player->m_base->m_position.m_y);
    // MakeAction(player, MOVE);
    //  printf("Player's position : %d %d\n\r", player->m_base->m_position.m_x, player->m_base->m_position.m_y);
    player->m_base->m_direction = EAST;

    // struct Block_s *front_block = getFrontBlockP(player, tilemap);
    //  if (player_action & front_block->m_flags)
    //      printf("Player can break the block\n\r");
    //  else
    //      // printf("Player can't break the block\n\r");
    //  printf("Block health %d\n\r", (front_block)->m_health);
    // MakeActionOnBlock(BREAK, front_block);
    // printf("Block health %d\n\r", (front_block)->m_health);

    //PrintTilemap(tilemap);
    term->tilemap = tilemap;

    // Chunk testing
    // MovePlayerTo(player, (struct Coordinate_s){10, 20});
    player->m_base->m_position = (struct Coordinate_s){50, 50};
    addPlayerToTilemap(player, tilemap);

    LoadChunkAroundPlayer(player, seed);

    View_s view = {10, 20, (struct Coordinate_s){0, 0}};
    struct Camera_s *camera = CreateCamera((struct Coordinate_s){0, 0}, 10, 20);
    nodelay(term->world, TRUE);

    struct Coordinate_s previous_chunk_coord = player->m_base->m_chunk_position;

    while (!quit)
    {
        int c = wgetch(term->world);
        switch (c)
        {
        case 'd':
            player->m_base->m_position.m_x++;
            camera->m_position.m_x++;
            break;
        case 'q':
            player->m_base->m_position.m_x--;
            camera->m_position.m_x--;
            break;
        case 'z':
            player->m_base->m_position.m_y--;
            camera->m_position.m_y--;
            break;
        case 's':
            player->m_base->m_position.m_y++;
            camera->m_position.m_y++;

            break;
        case KEY_F(1):
            quit = 1;
            break;

        default:
            break;
        }

        player->m_base->m_chunk_position = getEntityChunkCoordinate(player->m_base);
        if (memcmp(&player->m_base->m_chunk_position, &previous_chunk_coord, sizeof(struct Coordinate_s)))
            LoadChunkAroundPlayer(player, seed);
        displayTerm(term, &view);
        // RenderCameraView(term, NULL);
    }

    freePlayer(player);
    freeTilemap(tilemap);

    endwin();
    noraw();
    echo();

    return 0;
}
