#ifndef ENTITY_H
#define ENTITY_H

#include <stdlib.h>
#include "Tilemap.h"
#include "Coordinate.h"

typedef enum Direction_E
{
    NORTH,
    SOUTH,
    WEST,
    EAST
} Direction_e;

typedef enum EntityType_E{PLAYER, MOB} EntityType_e;

typedef struct Entity_S
{

    EntityType_e m_type;

    double m_last_move;
    // Entity hp
    short int m_health;

    // Entity coordinates 
    Coordinate_s m_position;

    // Entity direction
    Direction_e m_direction;

    struct Tilemap_s *m_tilemap;

    Coordinate_s m_chunk_position;
} Entity_s;

typedef struct Entitieslist_S
{
    Entity_s *m_entity;
    struct Entitieslist_S *m_next;
} Entitieslist_s;

Entity_s *CreateEntity(EntityType_e type);
Block_s **getFrontBlock(Entity_s *entity, struct Tilemap_S *tilemap);
struct TilemapBlock_S *getFrontTilemapBlock(Entity_s *entity, struct Tilemap_S *tilemap);
void addEntityToList(Entitieslist_s **list, Entity_s *entity);

// Return the entity chunk position
Coordinate_s getEntityChunkCoordinate(Entity_s *entity);
void moveEntityInDirection(Entity_s *entity);
#endif /* !ENTITY_H */