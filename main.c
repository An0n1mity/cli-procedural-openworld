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

int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "");
    Term_s* term = initDisplaying();

    int seed = 563;
    int quit = 0;

    // titleLoop(createWindow(20, 40, 0, 0));
    // seedMenu(createWindow(20, 40, 0, 0), &seed);
    
    // endwin();

    Player_s *player = CreatePlayer();
    Action_e player_action = BREAK;
    Tilemap_s *tilemap = CreateTilemapProcedurally(100, 100, seed);
    // CreateTilemapFromFile("../map.txt");
    addPlayerToTilemap(player, tilemap);

    player->m_base->m_direction = SOUTH;

    //PrintTilemap(tilemap);

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

    //PrintTilemap(tilemap);
    term->tilemap = tilemap;


    // Chunk testing
    MovePlayerTo(player, (Coordinate_s){10, 20});
    LoadChunkAroundPlayer(player, seed);

    View_s view = {10, 20, (Coordinate_s){0, 0}};
    nodelay(term->world, TRUE);

    while (!quit)
    {
        int c = wgetch(term->world);
        switch (c)
        {
        case 'd':
            player->m_base->m_position.m_x++;
            LoadChunkAroundPlayer(player, seed);
            view.m_coord.m_x++;
            break;
        case 'q':
            player->m_base->m_position.m_x--;

            LoadChunkAroundPlayer(player, seed);

            view.m_coord.m_x--;
            break;
        case 'z':
            player->m_base->m_position.m_y--;

            LoadChunkAroundPlayer(player, seed);

            view.m_coord.m_y--;
            break;
        case 's':
            player->m_base->m_position.m_y++;

            LoadChunkAroundPlayer(player, seed);

            view.m_coord.m_y++;
            break;
        case KEY_F(1):
            quit = 1;
            break;

        default:
            break;
        }
        displayTerm(term, &view);
    }

    freePlayer(player);
    freeTilemap(tilemap);

    endwin();
    noraw();
    echo();

    return 0;
}
