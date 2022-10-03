#include "Player.h"

struct Player_s *CreatePlayer()
{
    struct Player_s* player = (struct Player_s*)malloc(sizeof(struct Player_s));
    player->m_base = CreateEntity(PLAYER);
    return player;
}

void MakeAction(struct Player_s *player, enum Action_e action)
{
    struct Block_s *front_block = getFrontBlock(player->m_base, player->m_base->m_tilemap);
    if (!front_block)
        return;

    if ((action == BREAK) && (action & (front_block)->m_flags))
        (front_block)->m_health--;
}

struct Block_s *getFrontBlockP(struct Player_s *player, struct Tilemap_s *tilemap)
{
    struct Block_s *front_block = getFrontBlock(player->m_base, tilemap);
    return front_block;
}

inline void addPlayerToTilemap(struct Player_s *player, struct Tilemap_s *tilemap)
{
    addEntityToTilemap(player->m_base, tilemap);
}
