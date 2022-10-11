#ifndef CHUNK_H
#define CHUNK_H

#include "Entity.h"
#include "Block.h"
#include "Tilemap.h"
#include "Player.h"
#include <string.h>

// width and height for each chunks
#define CHUNK_SIZE 3

/**
 * @brief Create a Chunk From Tilemap object
 *
 * @param tilemap
 * @param top_coord The (x,y) coordinates of the top left corner of the chunk
 * @return struct Chunk_s* The loaded chunk from the tilemap
 */
struct Chunk_s *CreateChunkFromTilemap(struct Tilemap_s *tilemap, struct Coordinate_s top_coord);
/**
 * @brief Adding an entity to a chunk
 *
 * @param chunk
 * @param entity
 */
void AddEntityToChunk(struct Chunk_s *chunk, struct Entity_s *entity);
/**
 * @brief Return a (x, y) coordinate in world referential to a (x, y) coordinate with chunk referential
 *
 * @param tilemap_coord
 * @return struct Coordinate_s
 */
struct Coordinate_s TilemapToChunkCoordinates(struct Coordinate_s tilemap_coord);
/**
 * @brief Load all the 10 chunks around the player
 *
 * @param player
 */
void LoadChunkAroundPlayer(struct Player_s *player);

#endif