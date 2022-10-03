#ifndef TILEMAP_H
#define TILEMAP_H

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "Block.h"
#include "Entity.h"

struct Entitieslist_s
{
    struct Entity_s *m_entity;
    struct Entitieslist_s *m_next;
};

struct TilemapBlock_s
{
    struct Block_s *m_block;
    struct TilemapBlock_s *m_next;
};

struct Tilemap_s
{
    int m_width;
    int m_height;

    struct Block_s ***m_array;
    struct Entitieslist_s *m_entities;
};

struct Tilemap_s* CreateTilemap(const int m_wdith, const int m_height);
void FillTilemap(struct Tilemap_s* tilemap, const char* mapfile);
struct Tilemap_s* CreateTilemapFromFile(const char* mapfile);
void PrintTilemap(struct Tilemap_s* tilemap);
void addEntityToTilemap(struct Tilemap_s *tilemap, struct Entity_s *entity);
void freeTilemap(struct Tilemap_s *tilemap);

#endif