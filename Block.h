#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <stdbool.h>

typedef enum BlockFlag_e
{
    BREAKABLE = 0b00000001,
    MOVABLE = 0b00000010,
    DROPABLE = 0b00000100,
    WALKABLE = 0b00001000,
    PICKABLE = 0b00010000,
    PLACABLE = 0b00100000
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
    STONE,
    PLANK,
    APPLE,
    SURFBOARD_B,
    VOID
} BlockType_e;

typedef struct Block_S
{
    BlockType_e m_type;
    BlockFlag_e m_flags;

    int m_health;
} Block_s;

Block_s *CreateBlock(BlockType_e type, BlockFlag_e flag);
void reduceBlockHealth(Block_s *block, float damage);
void freeBlock(Block_s **block);

#endif