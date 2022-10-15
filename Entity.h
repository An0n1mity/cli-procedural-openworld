#ifndef ENTITY_H
#define ENTITY_H

#include <stdlib.h>
#include "Tilemap.h"
#include "Coordinate.h"

enum Direction_e
{
    NORTH,
    SOUTH,
    WEST,
    EAST
};

enum EntityType_e {PLAYER, MOB};

struct Entity_s
{

    enum EntityType_e m_type;

    // Entity hp
    short int m_health;

    // Entity coordinates 
    struct Coordinate_s m_position;

    // Entity direction
    enum Direction_e m_direction;

    struct Tilemap_s *m_tilemap;
};

struct Entitieslist_s
{
    struct Entity_s *m_entity;
    struct Entitieslist_s *m_next;
};

struct Entity_s *CreateEntity(enum EntityType_e type);
struct Block_s *getFrontBlock(struct Entity_s *entity, struct Tilemap_s *tilemap);
struct TilemapBlock_s *getFrontTilemapBlock(struct Entity_s *entity, struct Tilemap_s *tilemap);
void addEntityToList(struct Entitieslist_s **list, struct Entity_s *entity);

#endif