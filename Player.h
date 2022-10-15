#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "Entity.h"
#include "Block.h"
#include "Tilemap.h"

typedef struct Attribute_S
{
    enum Type_e {STRENGTH, AGILITY, TOUGHNESS, ENDURANCE} m_type;
    int m_value;
} Attribute_s;

typedef enum Action_E
{
    BREAK       = 0b0001,
    DISPLACE    = 0b0010,
    PICK        = 0b0100,
    MOVE        = 0b1000
} Action_e;

typedef struct Player_S
{
    // Based struct
    struct Entity_S* m_base;

    // Player name
    const char* m_name;

    // Palyer attributes
    Attribute_s m_attributes[4];
} Player_s;

Player_s* CreatePlayer();
void MovePlayerTo(Player_s* player, struct Coordinate_S coordinate);
void MakeAction(Player_s *player, Action_e action);
void MakeActionOnBlock(Action_e action, Block_s *block);
void addPlayerToTilemap(Player_s *player, struct Tilemap_S *tilemap);
Block_s *getFrontBlockP(Player_s *player, struct Tilemap_S *tilemap);
struct TilemapBlock_S *getFrontTilemapBlockP(Player_s *player, struct Tilemap_S *tilemap);
void MovePlayer(Player_s *player);
void freePlayer(Player_s *player);
void printPlayer(Player_s *player);
#define getFronBlock(a, b) _Generic(a, Player_s *                \
                                    : getFrontBlockP, Entity_s * \
                                    : getFrontBlock)(a, b)

#endif 

 
