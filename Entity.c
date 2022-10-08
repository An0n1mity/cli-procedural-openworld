#include "Entity.h"
#include <assert.h>
static inline void SetEntityHealth(struct Entity_s* entity, short int health);

struct Entity_s *CreateEntity(enum EntityType_e type)
{
    struct Entity_s* entity = (struct Entity_s*) malloc(sizeof(struct Entity_s));
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

static inline void SetEntityHealth(struct Entity_s* entity, short int health)
{
    entity->m_health = health;
}

inline void MoveEntityTo(struct Entity_s *entity, int x, int y)
{
    entity->m_position.m_x = x;
    entity->m_position.m_y = y;
}

inline void setDirection(struct Entity_s *entity, enum Direction_e direction)
{
    entity->m_direction = direction;
}

struct Block_s *getFrontBlock(struct Entity_s *entity, struct Tilemap_s *tilemap)
{
    assert(entity && tilemap);

    int entity_x = entity->m_position.m_x,
        entity_y = entity->m_position.m_y;

    struct Block_s **blocks = tilemap->m_blocks[1];

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

struct Entitieslist_s *createEntitieslist(struct Entity_s *entity)
{
    struct Entitieslist_s *entities_list = (struct Entitieslist_s *)calloc(1, sizeof(struct Entitieslist_s));
    entities_list->m_entity = entity;

    return entities_list;
}

void addEntityToList(struct Entitieslist_s **list, struct Entity_s *entity)
{
    if (!*list)
    {
        *list = createEntitieslist(entity);
        return;
    }

    struct Entitieslist_s *new = createEntitieslist(entity);
    new->m_next = *list;
    *list = new;
}
