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