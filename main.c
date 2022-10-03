#include <stdio.h>

#include "Player.h"
#include "Block.h"
#include "Tilemap.h"

int main(int argc, char const *argv[])
{
    struct Player_s* player = CreatePlayer();
    struct Block_s* block = CreateBlock(WOOD, BREAKABLE);
    enum Action_e player_action = BREAK;
    struct Tilemap_s* tilemap = CreateTilemapFromFile("../map.txt");
    FillTilemap(tilemap, "../map.txt");

    player->m_base->m_direction = SOUTH;
    struct Block_s *front_block = getFronBlock(player, tilemap);

    PrintTilemap(tilemap);

    if(player_action & block->m_flags)
        printf("Player can break the block\n");
    else
        printf("Player can't break the block\n");


    printf("Block health %d\n", block->m_health);
    MakeAction(player_action, block);
    printf("Block health %d\n", block->m_health);


    free(player->m_base);
    free(player);
    free(block);
    free(tilemap->m_array);
    free(tilemap);

    return 0;
}
