#include "Player.h"

struct Player_s* CreatePlayer()
{
    struct Player_s* player = (struct Player_s*)malloc(sizeof(struct Player_s));
    player->m_base = CreateEntity(PLAYER);
    return player;
}

void MakeAction(enum Action_e action, struct Block_s* block)
{
    if((action == BREAK) && (action & block->m_flags))
        block->m_health--;
}
