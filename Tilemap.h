#ifndef TILEMAP_H
#define TILEMAP_H

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Block.h"
#include "Entity.h"
#include "PerlinNoise.h"

struct TilemapBlock_s
{
    struct Block_s *m_block;
    struct TilemapBlock_s *m_next;
};

struct Tilemap_s
{
    int m_width;
    int m_height;

    struct Block_s ***m_blocks;
    struct Entitieslist_s *m_entities;
};

struct Tilemap_s* CreateTilemap(const int m_wdith, const int m_height);
void FillTilemap(struct Tilemap_s* tilemap, const char* mapfile);
struct Tilemap_s* CreateTilemapFromFile(const char* mapfile);
struct Tilemap_s *CreateTilemapProcedurally(int width, int height, int seed);
void PrintTilemap(struct Tilemap_s* tilemap);
void addEntityToTilemap(struct Tilemap_s *tilemap, struct Entity_s *entity);
void freeTilemap(struct Tilemap_s *tilemap);

#endif