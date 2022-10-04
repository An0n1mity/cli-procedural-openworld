#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <stdbool.h>

enum BlockFlag_e
{
    BREAKABLE = 1,
    MOVABLE = 2,
    DROPABLE = 4,
    WALKABLE = 8
};
enum BlockType_e
{
    EVERGREEN_TREE,
    DECIDIOUS_TREE,
    ROCK,
    GRASS,
    WATER,
    DIRT,
    SAND,
    STONE
};

struct Block_s
{
    enum BlockType_e m_type;
    enum BlockFlag_e m_flags;

    int m_health;
};

struct Block_s* CreateBlock(enum BlockType_e type, enum BlockFlag_e flag);

#endif