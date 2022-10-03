#include <stdio.h>

#include "Player.h"
#include "Block.h"
#include "Tilemap.h"

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

    free(player->m_base);
    free(player);
    for (size_t i = 0; i < tilemap->m_width * tilemap->m_height; i++)
    {
        free(tilemap->m_array[i]);
    }

    free(tilemap->m_array);
    free(tilemap);

    return 0;
}
