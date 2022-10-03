#include "Entity.h"

static inline void SetEntityHealth(struct Entity_s* entity, short int health);

struct Entity_s* CreateEntity(enum EntityType_e type)
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

void MoveEntityTo(struct Entity_s* entity, int x, int y)
{
    entity->m_position.m_x = x;
    entity->m_position.m_y = y;
}

inline void setPosition(struct Entity_s *entity, enum Direction_e direction)
{
    entity->m_direction = direction;
}

struct Block_s *getFrontBlock(struct Entity_s *entity, struct Tilemap_s *tilemap)
{
    int player_x = entity->m_position.m_x, player_y = entity->m_position.m_y;

    if (entity->m_direction == NORTH && (entity->m_position.m_y - 1 >= 0))
        return tilemap->m_array[player_y * tilemap->m_width + player_x];
    else if (entity->m_direction == SOUTH && (entity->m_position.m_y + 1 < tilemap->m_height))
        return tilemap->m_array[player_y + tilemap->m_width + player_x];
    else if (entity->m_direction == WEST && (entity->m_position.m_x - 1 >= 0))
        return tilemap->m_array[player_y + tilemap->m_width + player_x];
    else if (entity->m_direction == EAST && (entity->m_position.m_y + 1 < tilemap->m_width))
        return tilemap->m_array[player_y + tilemap->m_width + player_x];
    else
        return NULL;
}