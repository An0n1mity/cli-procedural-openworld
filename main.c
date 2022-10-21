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
    Term_s *term = initDisplaying();

    int seed = 563;
    int quit = 0;

    // titleLoop(createWindow(20, 40, 0, 0));
    // seedMenu(createWindow(20, 40, 0, 0), &seed);

    // endwin();

    Player_s *player = CreatePlayer();
    Action_e player_action = BREAK;
    struct Tilemap_s *tilemap = CreateTilemapProcedurally(CHUNK_SIZE * 3, CHUNK_SIZE * 3, seed);
    // CreateTilemapFromFile("../map.txt");

    player->m_base->m_direction = SOUTH;

    // PrintTilemap(tilemap);

    // printf("Player's position : %d %d\n\r", player->m_base->m_position.m_x, player->m_base->m_position.m_y);
    // MakeAction(player, MOVE);
    //  printf("Player's position : %d %d\n\r", player->m_base->m_position.m_x, player->m_base->m_position.m_y);
    player->m_base->m_direction = EAST;

    // Block_s *front_block = getFrontBlockP(player, tilemap);
    //  if (player_action & front_block->m_flags)
    //      printf("Player can break the block\n\r");
    //  else
    //      // printf("Player can't break the block\n\r");
    //  printf("Block health %d\n\r", (front_block)->m_health);
    // MakeActionOnBlock(BREAK, front_block);
    // printf("Block health %d\n\r", (front_block)->m_health);

    // PrintTilemap(tilemap);
    term->tilemap = tilemap;

    // Chunk testing
    MovePlayerTo(player, (Coordinate_s){10, 20});
    addPlayerToTilemap(player, tilemap);
    Coordinate_s previous_chunk_coord = {player->m_base->m_position.m_x, player->m_base->m_position.m_y};
    LoadChunkAroundPlayer(player, seed);


    nodelay(term->world, TRUE);
    int move_x = 0, move_y = 0, c = 0;
    keypad(term->world, TRUE);

    while (!quit)
    {
        switch (c)
            move_x = 0;
        move_y = 0;
        c = wgetch(term->world);
        switch (c)
        {
        case 'd':
            player->m_base->m_position.m_x++;
            break;
        case 'q':
            player->m_base->m_position.m_x--;
            break;
        case 'z':
            player->m_base->m_position.m_y--;
            break;
        }

        player->m_base->m_chunk_position = getEntityChunkCoordinate(player->m_base);
        if (memcmp(&player->m_base->m_chunk_position, &previous_chunk_coord, sizeof(Coordinate_s)))
            LoadChunkAroundPlayer(player, seed);
        displayTerm(term, NULL);
    }

    //freePlayer(player);
    //freeTilemap(tilemap);

    endwin();
    noraw();
    echo();

    return 0;
}
