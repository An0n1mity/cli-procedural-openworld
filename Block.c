#include "Block.h"

static inline void SetBlockHealth(Block_s* block, int health);

Block_s* CreateBlock(BlockType_e type, BlockFlag_e flag)
{
    Block_s* block = (Block_s*)malloc(sizeof(Block_s));
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

static inline void SetBlockHealth(Block_s* block, int health)
{
    block->m_health = health;
}