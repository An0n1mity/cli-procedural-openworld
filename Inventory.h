#ifndef INVENTORY_H
#define INVENTORY_H

#include "Player.h"

typedef enum CraftType_E
{
    PICKAXE_CRAFT,
    SWORD_CRAFT,
    SURF_CRAFT
} CraftType_e;

typedef struct CraftList_S
{
    CraftType_e m_craft;
    struct CraftList_s *m_next;
    struct CraftList_s *m_previous;
} CraftList_s;

void addBlockToInventory(struct Player_S *player, struct Block_S *block);
void moveInventoryCursorLeft(struct Player_S *player);
void moveInventoryCursorRight(struct Player_S *player);
struct Object_S *getCurrentInventoryObject(struct Player_S *player);

// List all possible crafts
CraftList_s *createCraftList(CraftType_e craft);
void addCraftToList(CraftList_s **craft_list, CraftType_e craft);
CraftList_s *getPossibleCrafts(struct Player_S *player);
void addSelectedCraftToInventory(struct Player_S *player);
void freeCraftList(CraftList_s *craft_list);

#endif