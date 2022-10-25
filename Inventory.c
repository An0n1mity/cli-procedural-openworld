#include "Player.h"

Object_s createObject(ObjectType_e type, void *data)
{
    return (Object_s){type, data};
}

void addObjectToInventory(Inventory_s *inventory, Object_s object)
{
    inventory->m_objects[inventory->m_idx++] = object;
}

void addBlockToInventory(Player_s *player, Block_s *block)
{
    // Transform the block into an object
    Object_s object = createObject(BLOCK, block);
    // Add it to the enventory
    addObjectToInventory(&player->m_inventory, object);
}