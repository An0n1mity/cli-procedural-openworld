#ifndef ENTITY_H
#define ENTITY_H

#include <stdlib.h>

struct Coordinate_s
{
    int m_x;
    int m_y;
};

enum EntityType_e {PLAYER, MOB};

struct Entity_s
{

    enum EntityType_e m_type;

    // Entity hp
    short int m_health;

    // Entity coordinates 
    struct Coordinate_s m_position;
};

struct Entity_s* CreateEntity(enum EntityType_e type);

#endif