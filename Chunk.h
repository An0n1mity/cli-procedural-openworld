#include "Entity.h"
#include "Block.h"
#include "Tilemap.h"
#include <string.h>

// width and height for each chunks
#define CHUNK_SIZE 3

struct Chunk_s
{
    struct EntityList_s *m_entities_list;
    struct Block_s ***m_blocks;
};

// Load top block to chunk size square from the tilemap
struct Chunk_s *CreateChunkFromTilemap(struct Tilemap_s *tilemap, struct Coordinate_s top_coord);
// Add entity to chunk
void AddEntityToChunk(struct Chunk_s *chunk, struct Entity_s *entity);
// Convert tilemap coordinates to chunk coordinates
struct Coordinate_s TilemapToChunkCoordinates(struct Coordinate_s tilemap_coord);