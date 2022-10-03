#include <stdio.h>
#include <termios.h>
#include "Player.h"
#include "Block.h"
#include "Tilemap.h"

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
    struct Tilemap_s* tilemap = CreateTilemapFromFile("../map.txt");

    addPlayerToTilemap(player, tilemap);

    player->m_base->m_direction = SOUTH;
    struct Block_s *front_block = getFronBlock(player, tilemap);

    PrintTilemap(tilemap);

    if (player_action & front_block->m_flags)
        printf("Player can break the block\n");
    else
        printf("Player can't break the block\n");

    printf("Block health %d\n", (front_block)->m_health);
    MakeAction(player, BREAK);
    printf("Block health %d\n", (front_block)->m_health);

    printf("Player's position : %d %d\n", player->m_base->m_position.m_x, player->m_base->m_position.m_y);
    MakeAction(player, MOVE);
    printf("Player's position : %d %d\n", player->m_base->m_position.m_x, player->m_base->m_position.m_y);

    freePlayer(player);
    freeTilemap(tilemap);

    return 0;
}
