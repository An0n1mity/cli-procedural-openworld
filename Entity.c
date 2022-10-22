#include "Entity.h"
#include <assert.h>
static inline void SetEntityHealth(Entity_s* entity, short int health);

Entity_s *CreateEntity(enum EntityType_E type)
{
    Entity_s* entity = (Entity_s*) malloc(sizeof(Entity_s));
    entity->m_type = type;
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

Block_s *getFrontBlock(Entity_s *entity, Tilemap_s *tilemap)
{
    assert(entity && tilemap);

    int entity_x = entity->m_position.m_x,
        entity_y = entity->m_position.m_y;

    Block_s **blocks = tilemap->m_blocks[1];

    if (entity->m_direction == NORTH && (entity_y - 1 >= 0))
    {
        if (tilemap->m_blocks[(entity_y - 1) * tilemap->m_width + entity_x][1])
            return tilemap->m_blocks[(entity_y - 1) * tilemap->m_width + entity_x][1];
        else
            return tilemap->m_blocks[(entity_y - 1) * tilemap->m_width + entity_x][0];
    }
    else if (entity->m_direction == SOUTH && (entity_y + 1 < tilemap->m_height))
    {
        if (tilemap->m_blocks[(entity_y + 1) * tilemap->m_width + entity_x][1])
            return tilemap->m_blocks[(entity_y + 1) * tilemap->m_width + entity_x][1];
        else
            return tilemap->m_blocks[(entity_y + 1) * tilemap->m_width + entity_x][0];
    }
    else if (entity->m_direction == WEST && (entity_x - 1 >= 0))
    {
        if (tilemap->m_blocks[entity_y * tilemap->m_width + (entity_x - 1)][1])
            return tilemap->m_blocks[entity_y * tilemap->m_width + (entity_x - 1)][1];
        else
            return tilemap->m_blocks[entity_y * tilemap->m_width + (entity_x - 1)][0];
    }
    else if (entity->m_direction == EAST && (entity_x + 1 < tilemap->m_width))
    {
        if (tilemap->m_blocks[entity_y * tilemap->m_width + (entity_x + 1)][1])
            return tilemap->m_blocks[entity_y * tilemap->m_width + (entity_x + 1)][1];
        else
            return tilemap->m_blocks[entity_y * tilemap->m_width + (entity_x + 1)][0];
    }
    else
        return NULL;
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