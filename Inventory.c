#include "Player.h"

Object_s createObject(ObjectType_e type, void *data)
{
    return (Object_s){type, data};
}

void addObjectToInventory(Inventory_s *inventory, Object_s object)
{
    // Check for empty slot in the enventory
    for (size_t i = 0; i < 9; i++)
    {
        if (inventory->m_objects[i].m_type == NONE)
        {
            inventory->m_objects[i] = object;
            return;
        }
    }
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

CraftList_s *getPossibleCrafts(struct Player_S *player)
{
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

    CraftList_s *crafts_list = NULL;

    // Check if surfboard is possible
    if (nb_plank >= 3)
        addCraftToList(&crafts_list, SURF_CRAFT);

    // Check if pickaxe is possible
    if (nb_plank >= 2 && nb_rock >= 3)
        addCraftToList(&crafts_list, PICKAXE_CRAFT);

    // check if sword possible
    if (nb_plank >= 1 && nb_rock >= 2)
        addCraftToList(&crafts_list, SWORD_CRAFT);

    return crafts_list;
}

CraftList_s *createCraftList(CraftType_e craft)
{
    CraftList_s *craft_list = calloc(1, sizeof(CraftList_s));
    craft_list->m_craft = craft;

    return craft_list;
}

void addCraftToList(CraftList_s **craft_list, CraftType_e craft)
{
    if (*craft_list)
    {
        *craft_list = createCraftList(craft);
        return;
    }

    CraftList_s *craft_list_to_add = createCraftList(craft);
    craft_list_to_add->m_next = *craft_list;
    if (*craft_list)
        (*craft_list)->m_previous = craft_list_to_add;
    *craft_list = craft_list_to_add;
}

void freeCraftList(CraftList_s *craft_list)
{

    if (!craft_list->m_next)
    {
        free(craft_list);
        return;
    }

    freeCraftList(craft_list->m_next);
    free(craft_list);
}

void freeInventoryObject(Object_s object)
{
    free(object.m_data);
}

Tool_s *createTool(ToolType_e type)
{
    Tool_s *tool = malloc(sizeof(Tool_s));
    tool->m_type = type;
    switch (type)
    {
    case SURFBOARD:
        tool->m_block_damage = 2.f;
        tool->m_entity_damage = 5.f;
        break;

    default:
        break;
    }

    return tool;
}

void addSelectedCraftToInventory(Player_s *player)
{
    if (player->m_craft_selected)
    {
        // Remove neccessary blocks for craft from the enventory
        size_t nb_planks = 0;
        size_t idx = 0;
        Object_s inventory_object;
        Block_s *inventory_block;
        Object_s object_to_add;
        Tool_s *tool;
        switch (player->m_craft_selected->m_craft)
        {
        case SURFBOARD:
            nb_planks = 3;
            while (nb_planks > 0)
            {
                inventory_object = player->m_inventory.m_objects[idx];
                if (inventory_object.m_type == BLOCK)
                {
                    inventory_block = inventory_object.m_data;
                    if (inventory_block->m_type == PLANK)
                    {
                        player->m_inventory.m_objects[idx].m_type = NONE;
                        freeInventoryObject(inventory_object);
                        nb_planks--;
                    }
                }
                idx++;
            }

            // Add to the enventory
            object_to_add = createObject(TOOL, createTool(SURFBOARD));
            addObjectToInventory(&player->m_inventory, object_to_add);
            break;

        default:
            break;
        }

        // Free and recreate possible crafts
        freeCraftList(player->m_possible_crafts);
        player->m_possible_crafts = NULL;
        player->m_possible_crafts = getPossibleCrafts(player);
        player->m_craft_selected = player->m_possible_crafts;
    }
}