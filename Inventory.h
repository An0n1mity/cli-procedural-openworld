#include "Player.h"

typedef enum CraftType_E
{
    NONE_CRAFT,
    PICKAXE_CRAFT,
    SWORD_CRAFT,
    SURF_CRAFT
} CraftType_e;

void addBlockToInventory(struct Player_S *player, struct Block_S *block);
void moveInventoryCursorLeft(struct Player_S *player);
void moveInventoryCursorRight(struct Player_S *player);
struct Object_S *getCurrentInventoryObject(struct Player_S *player);

// List all possible crafts
CraftType_e getPossibleCrafts(struct Player_S *player);