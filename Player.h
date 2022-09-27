#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "Entity.h"
#include "Block.h"

struct Attribute_s
{
    enum Type_e {STRENGTH, AGILITY, TOUGHNESS, ENDURANCE} m_type;
    int m_value;
};

enum Action_e {BREAK = 1, MOVE = 2, PICK = 4};

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
void MakeAction(enum Action_e action, struct Block_s* block);

#endif 

 
