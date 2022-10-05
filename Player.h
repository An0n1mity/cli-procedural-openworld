#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "Entity.h"
#include "Block.h"
#include "Tilemap.h"

struct Attribute_s
{
    enum Type_e {STRENGTH, AGILITY, TOUGHNESS, ENDURANCE} m_type;
    int m_value;
};

enum Action_e
{
    BREAK       = 0b0001,
    DISPLACE    = 0b0010,
    PICK        = 0b0100,
    MOVE        = 0b1000
};

struct Player_s
{
    // Based struct
    struct Entity_s* m_base;

    // Player name
    const char* m_name;

    // Palyer attributes
    struct Attribute_s m_attributes[4];
};

struct Player_s* CreatePlayer();
void MovePlayerTo(struct Player_s* player, struct Coordinate_s coordinate);
void MakeAction(struct Player_s *player, enum Action_e action);
void MakeActionOnBlock(enum Action_e action, struct Block_s *block);
void addPlayerToTilemap(struct Player_s *player, struct Tilemap_s *tilemap);
struct Block_s *getFrontBlockP(struct Player_s *player, struct Tilemap_s *tilemap);
struct TilemapBlock_s *getFrontTilemapBlockP(struct Player_s *player, struct Tilemap_s *tilemap);
void MovePlayer(struct Player_s *player);
void freePlayer(struct Player_s *player);
void printPlayer(struct Player_s *player);
#define getFronBlock(a, b) _Generic(a, struct Player_s *                \
                                    : getFrontBlockP, struct Entity_s * \
                                    : getFrontBlock)(a, b)

#endif 

 
