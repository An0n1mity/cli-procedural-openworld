#include "Player.h"

Player_s *CreatePlayer()
{
    Player_s* player = (Player_s*)malloc(sizeof(Player_s));
    player->m_base = CreateEntity(PLAYER);
    return player;
}

void MovePlayerTo(Player_s *player, Coordinate_s coordinate)
{
    player->m_base->m_position = coordinate;
}

void MovePlayer(Player_s *player)
{
    Coordinate_s *player_position = &player->m_base->m_position;
    Direction_e player_direction = player->m_base->m_direction;
    Tilemap_s *tilemap = player->m_base->m_tilemap;

    if (player_direction == NORTH && (player_position->m_y >= 0))
        --player_position->m_y;
    else if (player_direction == SOUTH && (player_position->m_y < tilemap->m_height))
        ++player_position->m_y;
    else if (player_direction == WEST && (player_position->m_x >= 0))
        --player_position->m_x;
    else if (player_direction == EAST && (player_position->m_x < tilemap->m_width))
        ++player_position->m_x;
}

void MakeActionOnBlock(Action_e action, Block_s *block)
{
    if ((action == BREAK) && (action & block->m_flags))
        block->m_health--;
}

void MakeAction(Player_s *player, Action_e action)
{
    Block_s *front_block = getFrontBlock(player->m_base, player->m_base->m_tilemap);
    if (!front_block)
        return;

    if ((action == BREAK) && (action & front_block->m_flags))
        front_block->m_health--;
    if ((action == MOVE) && (action & front_block->m_flags))
        MovePlayer(player);
}

Block_s *getFrontBlockP(Player_s *player, Tilemap_s *tilemap)
{
    Block_s *front_block = getFrontBlock(player->m_base, tilemap);
    return front_block;
}

void printPlayer(Player_s *player)
{
    printf("\033[%d;%dH", player->m_base->m_position.m_y + 1, player->m_base->m_position.m_x + 1);
    printf("👨");
}

inline void addPlayerToTilemap(Player_s *player, Tilemap_s *tilemap)
{
    addEntityToTilemap(tilemap, player->m_base);
}

inline void freePlayer(Player_s *player)
{
    free(player->m_base);
    free(player);
}
