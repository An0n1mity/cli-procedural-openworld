#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "Entity.h"
#include "Block.h"
#include "Tilemap.h"
#include "Inventory.h"

typedef struct Attribute_S
{
    enum Type_e {STRENGTH, AGILITY, TOUGHNESS, ENDURANCE} m_type;
    int m_value;
} Attribute_s;

typedef enum Action_E
{
    IDLE,
    BREAK = 0b0001,
    DISPLACE = 0b0010,
    PICK = 0b0100,
    MOVE = 0b1000
} Action_e;

typedef enum VitalType_E
{
    FOOD_LVL,
    WATER_LVL
} VitalType_E;

typedef enum ToolType_E
{
    PICKAXE,
    SWORD,
    SURFBOARD
} ToolType_e;

typedef struct Tool_S
{
    ToolType_e m_type;
    float m_block_damage;
    float m_entity_damage;
} Tool_s;

typedef enum ObjectType_E
{
    NONE,
    BLOCK,
    TOOL
} ObjectType_e;

typedef struct Object_S
{
    ObjectType_e m_type;
    void *m_data;
} Object_s;

typedef struct Inventory_S
{
    Object_s m_objects[9];
    size_t m_idx;
} Inventory_s;

typedef struct Player_S
{
    // Based struct
    struct Entity_S *m_base;

    // Player name
    const char *m_name;

    // Palyer attributes
    Attribute_s m_attributes[4];

    // Player vitals stats
    size_t m_vitals[2];

    // Player current action
    Action_e m_action;

    // Player inventory of objects
    Inventory_s m_inventory;

    // Player selected craft
    size_t m_craft_selected;

    // If the player stats have changed
    bool update_stats;

} Player_s;

Player_s* CreatePlayer();
void MovePlayerTo(Player_s* player, struct Coordinate_S coordinate);
void MakeAction(Player_s *player, Action_e action);
void MakeActionOnBlock(Action_e action, Block_s *block);
void addPlayerToTilemap(Player_s *player, struct Tilemap_S *tilemap);
struct TilemapBlock_S *getFrontTilemapBlockP(Player_s *player, struct Tilemap_S *tilemap);
void MovePlayer(Player_s *player);
void breakBlockInFront(Player_s *player);
void pickBlockInFront(Player_s *player);
void placeBlockInFront(Player_s *player);
void freePlayer(Player_s *player);
void printPlayer(Player_s *player);
void reducePlayerFoodLevel(Player_s *player);
void reducePlayerWaterLevel(Player_s *player);
void reducePlayerHealth(Player_s *player);
#endif 

 
