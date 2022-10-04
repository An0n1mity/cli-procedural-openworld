#include <stdio.h>
#include <termios.h>
#include "Player.h"
#include "Block.h"
#include "Tilemap.h"
#include "PerlinNoise.h"

int getche(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

int main(int argc, char const *argv[])
{

    struct Player_s *player = CreatePlayer();
    enum Action_e player_action = BREAK;
    struct Tilemap_s *tilemap = CreateTilemapProcedurally(50, 50, 100);
    // CreateTilemapFromFile("../map.txt");

    addPlayerToTilemap(player, tilemap);

    player->m_base->m_direction = SOUTH;

    PrintTilemap(tilemap);

    printf("Player's position : %d %d\n", player->m_base->m_position.m_x, player->m_base->m_position.m_y);
    MakeAction(player, MOVE);
    printf("Player's position : %d %d\n", player->m_base->m_position.m_x, player->m_base->m_position.m_y);
    player->m_base->m_direction = EAST;

    //wolla je veut mourir

    struct Block_s *front_block = getFrontBlockP(player, tilemap);
    if (player_action & front_block->m_flags)
        printf("Player can break the block\n");
    else
        printf("Player can't break the block\n");
    printf("Block health %d\n", (front_block)->m_health);
    MakeActionOnBlock(BREAK, front_block);
    printf("Block health %d\n", (front_block)->m_health);

    PrintTilemap(tilemap);

    freePlayer(player);
    freeTilemap(tilemap);

       return 0;
}
