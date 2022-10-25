#include "Block.h"

static inline void SetBlockHealth(Block_s *block, int health);

Block_s *CreateBlock(BlockType_e type, BlockFlag_e flag)
{
    Block_s *block = (Block_s *)malloc(sizeof(Block_s));
    block->m_type = type;
    block->m_flags = flag;

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

static inline void SetBlockHealth(Block_s *block, int health)
{
    block->m_health = health;
}

inline void reduceBlockHealth(Block_s *block)
{
    if (block->m_health <= 0)
        return;

    block->m_health--;

    // Update the block type
    if (block->m_health <= 0)
    {
        switch (block->m_type)
        {
        case EVERGREEN_TREE:
            block->m_type = PLANK;
            break;

        default:
            break;
        }
    }
}

void freeBlock(Block_s **block)
{
    if (!block)
        return;
    if (block[0])
        free(block[0]);
    if (block[1])
        free(block[1]);

    free(block);
}