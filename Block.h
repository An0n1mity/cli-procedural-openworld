#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <stdbool.h>

enum BlockFlag_e
{
    BREAKABLE = 0b0001,
    MOVABLE = 0b0010,
    DROPABLE = 0b0100,
    WALKABLE = 0b1000
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