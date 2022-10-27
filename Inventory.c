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

void moveInventoryCursorLeft(struct Player_S *player)
{
    if (player->m_inventory.m_idx <= 0)
        player->m_inventory.m_idx = 8;
    else
        player->m_inventory.m_idx--;
}

void moveInventoryCursorRight(struct Player_S *player)
{
    if (player->m_inventory.m_idx >= 8)
        player->m_inventory.m_idx = 0;
    else
        player->m_inventory.m_idx++;
}

inline Object_s *getCurrentInventoryObject(struct Player_S *player)
{
    return &player->m_inventory.m_objects[player->m_inventory.m_idx];
}

CraftType_e getPossibleCrafts(struct Player_S *player)
{
    CraftType_e possible_crafts = NONE;

    size_t nb_plank = 0, nb_rock = 0;
    Block_s *block;

    for (size_t i = 0; i < 9; i++)
    {
        switch (player->m_inventory.m_objects[i].m_type)
        {
        case BLOCK:
            block = player->m_inventory.m_objects[i].m_data;
            switch (block->m_type)
            {
            case PLANK:
                nb_plank++;
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }
    }

    // Check if surfboard is possible
    if (nb_plank >= 3)
        possible_crafts |= SURF_CRAFT;

    // Check if pickaxe is possible
    if (nb_plank >= 2 && nb_rock >= 3)
        possible_crafts |= PICKAXE_CRAFT;

    // check if sword possible
    if (nb_plank >= 1 && nb_rock >= 2)
        possible_crafts |= SWORD_CRAFT;

    return possible_crafts;
}