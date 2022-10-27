#include "Player.h"

Player_s *CreatePlayer()
{
    Player_s* player = (Player_s*)malloc(sizeof(Player_s));
    player->m_base = CreateEntity(PLAYER);

    memset(player->m_inventory.m_objects, 0, sizeof(Object_s) * 9);
    player->m_inventory.m_idx = 0;

    player->m_vitals[FOOD_LVL] = 100;
    player->m_vitals[WATER_LVL] = 100;

    player->update_stats = true;
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

    Block_s **front_block = getFrontBlock(player->m_base, tilemap);
    if (((front_block[1] && front_block[1]->m_flags & WALKABLE) || !front_block[1]) && (front_block[0]->m_flags & WALKABLE))
    {
        // Update current player action
        player->m_action = MOVE;

        moveEntityInDirection(player->m_base);
        reducePlayerFoodLevel(player);
        reducePlayerWaterLevel(player);
    }
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

void printPlayer(Player_s *player)
{
    printf("\033[%d;%dH", player->m_base->m_position.m_y + 1, player->m_base->m_position.m_x + 1);
    printf("👨");
}

inline void addPlayerToTilemap(Player_s *player, Tilemap_s *tilemap)
{
    addEntityToTilemap(tilemap, player->m_base);
    tilemap->m_player = player;
}

inline void reducePlayerFoodLevel(Player_s *player)
{
    if (player->m_vitals[FOOD_LVL] <= 0)
        return;
    player->m_vitals[FOOD_LVL]--;
    player->update_stats = true;
}

inline void reducePlayerWaterLevel(Player_s *player)
{
    if (player->m_vitals[WATER_LVL] <= 0)
        return;
    player->m_vitals[WATER_LVL]--;
    player->update_stats = true;
}

inline void reducePlayerHealth(Player_s *player)
{
    if (player->m_base->m_health <= 0)
        return;
    player->m_base->m_health--;
    player->update_stats = true;
}

void breakBlockInFront(Player_s *player)
{
    Block_s **block = getFrontBlock(player->m_base, player->m_base->m_tilemap);
    // If the block can be breaked
    if (block[1] && block[1]->m_flags & BREAKABLE)
    {
        player->m_action = BREAK;
        // If the player is holding a tool
        if (player->m_inventory.m_objects[player->m_inventory.m_idx].m_type == TOOL)
        {
            Tool_s *current_tool = player->m_inventory.m_objects[player->m_inventory.m_idx].m_data;
            reduceBlockHealth(block[1], current_tool->m_block_damage);
        }
        else
            reduceBlockHealth(block[1], 1.f);
    }

    Tilemap_s *tilemap = player->m_base->m_tilemap;
    tilemap->m_previous_chunk = tilemap->m_chunks[MAX_CHUNK_DISTANCE / 2][MAX_CHUNK_DISTANCE / 2];
    tilemap->m_save_previous_chunk = true;
}

void pickBlockInFront(Player_s *player)
{
    Block_s **block = getFrontBlock(player->m_base, player->m_base->m_tilemap);
    // If the block can be picked
    if (block[1] && block[1]->m_flags & PICKABLE)
    {
        player->m_action = PICK;
        Block_s *picked_block = malloc(sizeof(Block_s));
        memcpy(picked_block, block[1], sizeof(Block_s));
        free(block[1]);
        block[1] = NULL;
        // Add the block to the inventory
        addBlockToInventory(player, picked_block);
    }

    Tilemap_s *tilemap = player->m_base->m_tilemap;
    tilemap->m_previous_chunk = tilemap->m_chunks[MAX_CHUNK_DISTANCE / 2][MAX_CHUNK_DISTANCE / 2];
    tilemap->m_save_previous_chunk = true;
}

void placeBlockInFront(Player_s *player)
{
    Block_s **block = getFrontBlock(player->m_base, player->m_base->m_tilemap);
    Object_s *holded_object = getCurrentInventoryObject(player);
    if (!block[1] && holded_object->m_type == BLOCK)
    {
        block[1] = malloc(sizeof(Block_s));
        memcpy(block[1], holded_object->m_data, sizeof(Block_s));
        free(holded_object->m_data);
        holded_object->m_type = NONE;
    }
    Tilemap_s *tilemap = player->m_base->m_tilemap;
    tilemap->m_previous_chunk = tilemap->m_chunks[MAX_CHUNK_DISTANCE / 2][MAX_CHUNK_DISTANCE / 2];
    tilemap->m_save_previous_chunk = true;
}

inline void freePlayer(Player_s *player)
{
    free(player->m_base);
    free(player);
}
