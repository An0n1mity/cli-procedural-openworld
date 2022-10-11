#include <stdio.h>
#include <termios.h>
#include "Player.h"
#include "Block.h"
#include "Tilemap.h"
#include "PerlinNoise.h"
#include "Menu.h"
#include "Rendering.h"

int main(int argc, char const *argv[])
{
    initscr();
    raw();
    noecho();

    int seed = 20;

    // titleLoop(createWindow(20, 40, 0, 0));
    // seedMenu(createWindow(20, 40, 0, 0), &seed);
    // endwin();

    struct Player_s *player = CreatePlayer();
    enum Action_e player_action = BREAK;
    struct Tilemap_s *tilemap = CreateTilemapProcedurally(50, 50, seed);
    // CreateTilemapFromFile("../map.txt");
    addPlayerToTilemap(player, tilemap);

    player->m_base->m_direction = SOUTH;

    PrintTilemap(tilemap);

    printf("Player's position : %d %d\n\r", player->m_base->m_position.m_x, player->m_base->m_position.m_y);
    MakeAction(player, MOVE);
    printf("Player's position : %d %d\n\r", player->m_base->m_position.m_x, player->m_base->m_position.m_y);
    player->m_base->m_direction = EAST;

    struct Block_s *front_block = getFrontBlockP(player, tilemap);
    if (player_action & front_block->m_flags)
        printf("Player can break the block\n\r");
    else
        printf("Player can't break the block\n\r");
    printf("Block health %d\n\r", (front_block)->m_health);
    MakeActionOnBlock(BREAK, front_block);
    printf("Block health %d\n\r", (front_block)->m_health);

    PrintTilemap(tilemap);

    // Chunk testing
    MovePlayerTo(player, (struct Coordinate_s){20, 20});
    LoadChunkAroundPlayer(player);

    freePlayer(player);
    freeTilemap(tilemap);

    return 0;
}
