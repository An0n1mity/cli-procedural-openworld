#ifndef CHUNK_H
#define CHUNK_H

#include "Entity.h"
#include "Block.h"
#include "Tilemap.h"
#include "Player.h"
#include <string.h>
#include "Coordinate.h"

typedef struct Chunk_S
{
    struct Entitieslist_S *m_entities_list;
    Block_s ***m_blocks;

    Coordinate_s world_position;
} Chunk_s;

// width and height for each chunks
#define CHUNK_SIZE 4

/**
 * @brief Create a Chunk From Tilemap object
 *
 * @param tilemap
 * @param top_coord The (x,y) coordinates of the top left corner of the chunk
 * @return struct Chunk_s* The loaded chunk from the tilemap
 */
Chunk_s *CreateChunkFromTilemap(struct Tilemap_S *tilemap, struct Coordinate_S top_coord);
/**
 * @brief Adding an entity to a chunk
 *
 * @param chunk
 * @param entity
 */
void AddEntityToChunk(Chunk_s *chunk, struct Entity_S *entity);
/**
 * @brief Return a (x, y) coordinate in world referential to a (x, y) coordinate with chunk referential
 *
 * @param tilemap_coord
 * @return struct Coordinate_s
 */
Coordinate_s TilemapToChunkCoordinates(Coordinate_s tilemap_coord);
/**
 * @brief Load all the 10 chunks around the player
 *
 * @param player
 */
void LoadChunkAroundPlayer(struct Player_S *player, float seed, bool first, int chunkCount_x, int chunkCount_y);

void freeChunk(Chunk_s *chunk);

#endif