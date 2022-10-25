#include "Player.h"

void addBlockToInventory(struct Player_S *player, struct Block_S *block);
void moveInventoryCursorLeft(struct Player_S *player);
void moveInventoryCursorRight(struct Player_S *player);
struct Object_S *getCurrentInventoryObject(struct Player_S *player);
