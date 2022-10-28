#include "Entity.h"
#include <assert.h>
static inline void SetEntityHealth(Entity_s* entity, short int health);

Entity_s *CreateEntity(enum EntityType_E type)
{
    Entity_s* entity = (Entity_s*) malloc(sizeof(Entity_s));
    entity->m_type = type;
    entity->m_last_move = 0;
    switch (type)
    {
        case PLAYER:
            SetEntityHealth(entity, 3);
            break;
        case MOB:
            SetEntityHealth(entity, 5);
            break;        
        default:
            break;
    }

    return entity;
}

static inline void SetEntityHealth(Entity_s* entity, short int health)
{
    entity->m_health = health;
}

inline void MoveEntityTo(Entity_s *entity, int x, int y)
{
    entity->m_position.m_x = x;
    entity->m_position.m_y = y;
}

inline void setDirection(Entity_s *entity, Direction_e direction)
{
    entity->m_direction = direction;
}

void moveEntityInDirection(Entity_s *entity)
{

    switch (entity->m_direction)
    {
    case NORTH:

        entity->m_position.m_y--;
        entity->m_chunk_position = getEntityChunkCoordinate(entity);
        break;
    case SOUTH:
        entity->m_position.m_y++;
        entity->m_chunk_position = getEntityChunkCoordinate(entity);
        break;
    case WEST:
        entity->m_position.m_x--;
        entity->m_chunk_position = getEntityChunkCoordinate(entity);
        break;
    case EAST:
        entity->m_position.m_x++;
        entity->m_chunk_position = getEntityChunkCoordinate(entity);
        break;

    default:
        break;
    }
}

// Return pointer to array containing block
Block_s **getFrontBlock(Entity_s *entity, Tilemap_s *tilemap)
{
    assert(entity && tilemap);

    int entity_x = entity->m_position.m_x,
        entity_y = entity->m_position.m_y;

    Block_s **blocks = tilemap->m_blocks[1];

    // Get entity coordinate in tilemap space
    Coordinate_s entity_tilemap_coord = getEntityTilemapCoordinate(entity);

    Block_s **block_in_front;

    switch (entity->m_direction)
    {
    case NORTH:
        block_in_front = tilemap->m_blocks[(entity_tilemap_coord.m_y - 1) * CHUNK_SIZE * MAX_CHUNK_DISTANCE + entity_tilemap_coord.m_x];
        break;
    case SOUTH:
        block_in_front = tilemap->m_blocks[(entity_tilemap_coord.m_y + 1) * CHUNK_SIZE * MAX_CHUNK_DISTANCE + entity_tilemap_coord.m_x];
        break;
    case WEST:
        block_in_front = tilemap->m_blocks[entity_tilemap_coord.m_y * CHUNK_SIZE * MAX_CHUNK_DISTANCE + entity_tilemap_coord.m_x - 1];
        break;
    case EAST:
        block_in_front = tilemap->m_blocks[entity_tilemap_coord.m_y * CHUNK_SIZE * MAX_CHUNK_DISTANCE + entity_tilemap_coord.m_x + 1];
        break;
    default:
        break;
    }

    return block_in_front;
}

Entitieslist_s *createEntitieslist(Entity_s *entity)
{
    Entitieslist_s *entities_list = (Entitieslist_s *)calloc(1, sizeof(Entitieslist_s));
    entities_list->m_entity = entity;

    return entities_list;
}

void addEntityToList(Entitieslist_s **list, Entity_s *entity)
{
    if (!*list)
    {
        *list = createEntitieslist(entity);
        return;
    }

    Entitieslist_s *new = createEntitieslist(entity);
    new->m_next = *list;
    *list = new;
}

Coordinate_s getEntityChunkCoordinate(Entity_s *entity)
{
    Coordinate_s entity_coord = entity->m_position;
    return (Coordinate_s){entity_coord.m_x / CHUNK_SIZE, entity_coord.m_y / CHUNK_SIZE};
}