#include "Chunk.h"

struct Chunk_s *CreateChunkFromTilemap(struct Tilemap_s *tilemap, struct Coordinate_s top_coord)
{
    struct Chunk_s *chunk = (struct Chunk_s *)calloc(1, sizeof(struct Chunk_s));
    chunk->m_blocks = (struct Block_s ***)malloc(sizeof(struct Block_s **) * CHUNK_SIZE * CHUNK_SIZE);
    for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
    {
        chunk->m_blocks[i] = calloc(2, sizeof(struct Block_s *));
    }

    /*for (size_t i = 0; i < CHUNK_SIZE; i++)
    {

        memcpy(chunk->m_blocks + (i * CHUNK_SIZE), tilemap->m_blocks + top_coord.m_y * tilemap->m_width + top_coord.m_x, sizeof(struct Block_s *) * CHUNK_SIZE);
    }*/

    for (size_t i = top_coord.m_y, idx = 0; i < top_coord.m_y + CHUNK_SIZE; i++)
    {
        for (size_t j = top_coord.m_x; j < top_coord.m_x + CHUNK_SIZE; j++, idx++)
        {
            chunk->m_blocks[idx] = tilemap->m_blocks[i * tilemap->m_width + j];
        }
    }

    return chunk;
}

void AddEntityToChunk(struct Chunk_s *chunk, struct Entity_s *entity)
{
    addEntityToList(&(chunk->m_entities_list), entity);
}

inline struct Coordinate_s TilemapToChunkCoordinates(struct Coordinate_s tilemap_coord)
{
    return (struct Coordinate_s){tilemap_coord.m_x / CHUNK_SIZE, tilemap_coord.m_y / CHUNK_SIZE};
}

bool isCoordinateInTilemap(struct Tilemap_s *tilemap, struct Coordinate_s coordinate)
{
    if ((coordinate.m_x >= 0 && coordinate.m_x < tilemap->m_width) && (coordinate.m_y >= 0 && coordinate.m_y < tilemap->m_height))
        return true;
    return false;
}

struct Coordinate_s getTopCoordinateFromChunk(struct Tilemap_s *tilemap, struct Coordinate_s chunk_coord)
{
    return (struct Coordinate_s){
        chunk_coord.m_x * CHUNK_SIZE,
        chunk_coord.m_y * CHUNK_SIZE};
}

/*
void UnloadChunkAroundPlayer(struct Player_s *player)
{
    struct Coordinate_s player_chunk_coord = TilemapToChunkCoordinates(player->m_base->m_position);
    struct Tilemap_s *tilemap = player->m_base->m_tilemap;

    size_t nb_chunk_loadable = 0;

    for (int i = player_chunk_coord.m_y - 1; i <= player_chunk_coord.m_y + 1; i++)
    {
        for (int j = player_chunk_coord.m_x - 1; j <= player_chunk_coord.m_x + 1; j++)
        {
            if ((j >= 0 && j < tilemap->m_height / CHUNK_SIZE) && (i >= 0 && i < tilemap->m_width / CHUNK_SIZE))
            {
                if (i)
                    tilemap->m_chunks[nb_chunks_loadable++] = CreateChunkFromTilemap(tilemap, getTopCoordinateFromChunk(tilemap, (struct Coordinate_s){j, i}));
            }
        }
    }
}
*/

void LoadChunkAroundPlayer(struct Player_s *player)
{
    struct Coordinate_s player_chunk_coord = TilemapToChunkCoordinates(player->m_base->m_position);
    struct Tilemap_s *tilemap = player->m_base->m_tilemap;

    size_t nb_chunk_loadable = 0;
    for (int i = player_chunk_coord.m_y - 1, ci = 0; i <= player_chunk_coord.m_y + 1; i++, ci++)
    {
        for (int j = player_chunk_coord.m_x - 1, cj = 0; j <= player_chunk_coord.m_x + 1; j++, cj++)
        {
            if ((j >= 0 && j < tilemap->m_height / CHUNK_SIZE) && (i >= 0 && i < tilemap->m_width / CHUNK_SIZE))
            {
                tilemap->m_chunks[ci][cj] = CreateChunkFromTilemap(tilemap, getTopCoordinateFromChunk(tilemap, (struct Coordinate_s){j, i}));
                nb_chunk_loadable++;
            }
        }
    }
}
