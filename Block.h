#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <stdbool.h>

typedef enum BlockFlag_E
{
    BREAKABLE = 0b0001,
    MOVABLE   = 0b0010,
    DROPABLE  = 0b0100,
    WALKABLE  = 0b1000
} BlockFlag_e;
typedef enum BlockType_E
{
    EVERGREEN_TREE,
    DECIDIOUS_TREE,
    ROCK,
    GRASS,
    WATER,
    DIRT,
    SAND,
    STONE
} BlockType_e;

typedef struct Block_S
{
    BlockType_e m_type;
    BlockFlag_e m_flags;

    int m_health;
} Block_s;

Block_s* CreateBlock(BlockType_e type, BlockFlag_e flag);

#endif