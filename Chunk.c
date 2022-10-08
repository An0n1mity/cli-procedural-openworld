#include "Chunk.h"

struct Chunk_s *CreateChunkFromTilemap(struct Tilemap_s *tilemap, struct Coordinate_s top_coord)
{
    struct Chunk_s *chunk = (struct Chunk_s *)calloc(1, sizeof(struct Chunk_s));
    chunk->m_blocks = (struct Block_s ***)malloc(sizeof(struct Block_s **) * CHUNK_SIZE * CHUNK_SIZE);
    for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
    {
        chunk->m_blocks[i] = calloc(2, sizeof(struct Block_s *));
    }

    for (size_t i = 0; i < CHUNK_SIZE; i++)
    {
        memcpy(chunk->m_blocks + (i * CHUNK_SIZE), tilemap->m_blocks + top_coord.m_y * tilemap->m_width + top_coord.m_x, sizeof(struct Block_s *) * CHUNK_SIZE);
    }

    return chunk;
}

void AddEntityToChunk(struct Chunk_s *chunk, struct Entity_s *entity)
{
    addEntityToList(&chunk->m_entities_list, entity);
}

inline struct Coordinate_s TilemapToChunkCoordinates(struct Coordinate_s tilemap_coord)
{
    return (struct Coordinate_s){tilemap_coord.m_x / CHUNK_SIZE, tilemap_coord.m_y / CHUNK_SIZE};
}