#ifndef TILEMAP_H
#define TILEMAP_H

#define MAX_CHUNK_DISTANCE 64

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
#include "Chunk.h"
#include "Coordinate.h"

typedef struct TilemapBlock_S
{
    Block_s *m_block;
    struct TilemapBlock_S *m_next;
} TilemapBlock_s;

typedef struct Tilemap_S
{
    int m_width;
    int m_height;

    Block_s ***m_blocks;
    struct Chunk_S *m_chunks[3][3];
    struct Entitieslist_S *m_entities;

    struct Player_S *m_player;

    Coordinate_s m_top_coord;
} Tilemap_s;

Tilemap_s* CreateTilemap(const int m_wdith, const int m_height);
//void FillTilemap(Tilemap_s* tilemap, const char* mapfile);
// Tilemap_s* CreateTilemapFromFile(const char* mapfile);
Tilemap_s *CreateTilemapProcedurally(int width, int height, int seed);
void PrintTilemap(Tilemap_s* tilemap);
void addEntityToTilemap(Tilemap_s *tilemap, struct Entity_S *entity);
void freeTilemap(Tilemap_s *tilemap);
void freeEntitiesList(struct Entitieslist_S *list);

/************************** COORDINATE CALCULATION ***********************************/
Coordinate_s getEntityTilemapCoordinate(struct Entity_S *entity);
Coordinate_s getTopCoordinateFromChunk(Tilemap_s *tilemap, Coordinate_s chunk_coord);
Coordinate_s getEntityCoordinateInChunk(struct Entity_S *entity);
Coordinate_s TilemapToChunkCoordinates(Coordinate_s tilemap_coord);
#endif /* !TILEMAP_H */