#ifndef TILEMAP_H
#define TILEMAP_H

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "Block.h"

struct Tilemap_s
{
    int m_width;
    int m_height;

    struct Block_s** m_array;
};

struct Tilemap_s* CreateTilemap(const int m_wdith, const int m_height);
void FillTilemap(struct Tilemap_s* tilemap, const char* mapfile);
struct Tilemap_s* CreateTilemapFromFile(const char* mapfile);
void PrintTilemap(struct Tilemap_s* tilemap);

#endif