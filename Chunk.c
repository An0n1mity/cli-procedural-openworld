#include "Chunk.h"

Chunk_s *CreateChunkFromTilemap(Tilemap_s *tilemap, Coordinate_s top_coord)
{
    Chunk_s *chunk = (Chunk_s *)calloc(1, sizeof(Chunk_s));
    chunk->m_blocks = (Block_s ***)malloc(sizeof(Block_s **) * CHUNK_SIZE * CHUNK_SIZE);
    for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
    {
        chunk->m_blocks[i] = calloc(2, sizeof(Block_s *));
    }

    /*for (size_t i = 0; i < CHUNK_SIZE; i++)
    {

        memcpy(chunk->m_blocks + (i * CHUNK_SIZE), tilemap->m_blocks + top_coord.m_y * tilemap->m_width + top_coord.m_x, sizeof(Block_s *) * CHUNK_SIZE);
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

Chunk_s *CreateChunkProcedurally(Coordinate_s top_coord, float seed)
{
    Chunk_s *chunk = (Chunk_s *)calloc(1, sizeof(Chunk_s));
    chunk->m_blocks = (Block_s ***)malloc(sizeof(Block_s **) * CHUNK_SIZE * CHUNK_SIZE);
    for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
    {
        chunk->m_blocks[i] = calloc(2, sizeof(Block_s *));
    }

    size_t nb = 0;
    for (int y = top_coord.m_y, i = 0, idx = 0; y < top_coord.m_y + CHUNK_SIZE; y++, i++)
    {
        for (int x = top_coord.m_x, j = 0; x < top_coord.m_x + CHUNK_SIZE; x++, idx++, j++)
        {
            float value = perlin2d(x, y, 0.1, 1, seed);
            Coordinate_s block_coord = {x, y};
            if (value >= 0.55f)
            {
                chunk->m_blocks[i * CHUNK_SIZE + j][0] = CreateBlock(GRASS, WALKABLE);
                // if (value >= 0.55 && value <= 0.7 && !(rand() % 2))
                //  chunk->m_blocks[i * CHUNK_SIZE + j][1] = CreateBlock(EVERGREEN_TREE, WALKABLE);
            }
            else if (value <= 0.4)
                chunk->m_blocks[i * CHUNK_SIZE + j][0] = CreateBlock(WATER, WALKABLE);
            else if (value > 0.4 && value <= 0.46)
                chunk->m_blocks[i * CHUNK_SIZE + j][0] = CreateBlock(SAND, WALKABLE);
            else if (value > 0.46 && value < 0.55)
            {
                chunk->m_blocks[i * CHUNK_SIZE + j][0] = CreateBlock(STONE, WALKABLE);
                // if (!(rand() % 5))
                // chunk->m_blocks[i * CHUNK_SIZE + j][1] = CreateBlock(ROCK, WALKABLE);
            }
            nb++;
        }
    }

    if (nb < 9)
        return;

    return chunk;
}

void AddEntityToChunk(Chunk_s *chunk, Entity_s *entity)
{
    addEntityToList(&(chunk->m_entities_list), entity);
}

inline Coordinate_s TilemapToChunkCoordinates(Coordinate_s tilemap_coord)
{
    return (Coordinate_s){tilemap_coord.m_x / CHUNK_SIZE, tilemap_coord.m_y / CHUNK_SIZE};
}

bool isCoordinateInTilemap(Tilemap_s *tilemap, Coordinate_s coordinate)
{
    if ((coordinate.m_x >= 0 && coordinate.m_x < tilemap->m_width) && (coordinate.m_y >= 0 && coordinate.m_y < tilemap->m_height))
        return true;
    return false;
}

Coordinate_s getTopCoordinateFromChunk(Tilemap_s *tilemap, Coordinate_s chunk_coord)
{
    return (Coordinate_s){
        chunk_coord.m_x * CHUNK_SIZE,
        chunk_coord.m_y * CHUNK_SIZE};
}

void LoadChunksToTilemap(Tilemap_s *tilemap, Coordinate_s top_coord)
{
    tilemap->m_top_coord = top_coord;

    size_t block_idx = 0;
    for (size_t i = 0; i < 3 * CHUNK_SIZE; i++)
    {
        for (size_t j = 0; j < 3 * CHUNK_SIZE; j++)
        {
            size_t chunk_i = i / CHUNK_SIZE;
            size_t chunk_j = j / CHUNK_SIZE;

            Chunk_s *chunk = tilemap->m_chunks[chunk_i][chunk_j];
            Block_s **block = chunk->m_blocks[(i - chunk_i * CHUNK_SIZE) * CHUNK_SIZE + (j - chunk_j * CHUNK_SIZE)];

            tilemap->m_blocks[block_idx++] = block;
        }
    }
}

void LoadChunkAroundPlayer(Player_s *player, float seed)
{
    Coordinate_s player_chunk_coord = TilemapToChunkCoordinates(player->m_base->m_position);
    Tilemap_s *tilemap = player->m_base->m_tilemap;

    Coordinate_s tilemap_top_coord;
    bool first_chunk = true;
    for (int i = player_chunk_coord.m_y - 1, ci = 0; i <= player_chunk_coord.m_y + 1; i++, ci++)
    {
        for (int j = player_chunk_coord.m_x - 1, cj = 0; j <= player_chunk_coord.m_x + 1; j++, cj++)
        {
            if (first_chunk)
            {
                tilemap_top_coord = getTopCoordinateFromChunk(tilemap, (Coordinate_s){j, i});
                first_chunk = false;
            }
            tilemap->m_chunks[ci][cj] = CreateChunkProcedurally(getTopCoordinateFromChunk(tilemap, (Coordinate_s){j, i}), seed);
        }
    }

    // Map the chunk's blocks to the tilemap easier to access
    LoadChunksToTilemap(tilemap, tilemap_top_coord);
}

