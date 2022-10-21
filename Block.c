#include "Block.h"

static inline void SetBlockHealth(struct Block_s* block, int health);

struct Block_s *CreateBlock(enum BlockType_e type, enum BlockFlag_e flag, struct Coordinate_s coord)
{
    struct Block_s* block = (struct Block_s*)malloc(sizeof(struct Block_s));
    block->m_type = type;
    block->m_flags = flag;
    block->m_position = coord;

    switch (type)
    {
        case ROCK:
            SetBlockHealth(block, 10);
            break;

        case EVERGREEN_TREE:
            SetBlockHealth(block, 10);
            break;

        case DECIDIOUS_TREE:
            SetBlockHealth(block, 10);
            break;

        default:
            break;
    }

    return block;
}

static inline void SetBlockHealth(struct Block_s* block, int health)
{
    block->m_health = health;
}