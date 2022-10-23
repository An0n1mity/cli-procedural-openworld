#include "Chunk.h"

void writeChunkToFile(Chunk_s *chunk, const char *filename);
void writeFileToChunk(Chunk_s *chunk, const char *filename, long cursor);
long whereisChunkInFile(Coordinate_s chunk_coord, const char *filename);

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
    chunk->world_position = top_coord;

    // Check if the chunk was already stored on save file
    long cursor = whereisChunkInFile(top_coord, "../saved_chunks");

    // if it's the case load it
    if (cursor >= 0)
        writeFileToChunk(chunk, "../saved_chunks", cursor);
    else
    {
        for (int y = top_coord.m_y, i = 0, idx = 0; y < top_coord.m_y + CHUNK_SIZE; y++, i++)
        {
            for (int x = top_coord.m_x, j = 0; x < top_coord.m_x + CHUNK_SIZE; x++, idx++, j++)
            {
                float value = perlin2d(x, y, 0.1, 1, seed);
                Coordinate_s block_coord = {x, y};
                if (value >= 0.55f)
                {
                    chunk->m_blocks[i * CHUNK_SIZE + j][0] = CreateBlock(GRASS, WALKABLE);
                    if (value >= 0.55 && value <= 0.7 && !(rand() % 2))
                        chunk->m_blocks[i * CHUNK_SIZE + j][1] = CreateBlock(EVERGREEN_TREE, BREAKABLE);
                }
                else if (value <= 0.4)
                    chunk->m_blocks[i * CHUNK_SIZE + j][0] = CreateBlock(WATER, 0);
                else if (value > 0.4 && value <= 0.46)
                    chunk->m_blocks[i * CHUNK_SIZE + j][0] = CreateBlock(SAND, WALKABLE);
                else if (value > 0.46 && value < 0.55)
                {
                    chunk->m_blocks[i * CHUNK_SIZE + j][0] = CreateBlock(STONE, WALKABLE);
                    if (!(rand() % 5))
                        chunk->m_blocks[i * CHUNK_SIZE + j][1] = CreateBlock(ROCK, BREAKABLE);
                }
            }
        }
    }

    return chunk;
}

void AddEntityToChunk(Chunk_s *chunk, Entity_s *entity)
{
    addEntityToList(&(chunk->m_entities_list), entity);
}

bool isCoordinateInTilemap(Tilemap_s *tilemap, Coordinate_s coordinate)
{
    if ((coordinate.m_x >= 0 && coordinate.m_x < tilemap->m_width) && (coordinate.m_y >= 0 && coordinate.m_y < tilemap->m_height))
        return true;
    return false;
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

void LoadChunkAroundPlayer(Player_s *player, float seed, bool first)
{
    Coordinate_s player_chunk_coord = TilemapToChunkCoordinates(player->m_base->m_position);
    Tilemap_s *tilemap = player->m_base->m_tilemap;

    // Unload previous chunk from memory
    if (!first)
    {
        switch (player->m_base->m_direction)
        {
        case NORTH:

            for (int i = 0; i < 3; ++i)
            {
                freeChunk(tilemap->m_chunks[2][i]);
            }

            for (int x = 0; x < 3; ++x)
                for (int y = 3 - 1; y > 0; --y)
                {
                    tilemap->m_chunks[y][x]->m_blocks = tilemap->m_chunks[y - 1][x]->m_blocks;
                    tilemap->m_chunks[y][x]->world_position = tilemap->m_chunks[y - 1][x]->world_position;
                }

            for (int i = 0; i < 3; i++)
            {
                tilemap->m_chunks[0][i] = NULL;
            }
            break;

        case SOUTH:
            freeChunk(tilemap->m_chunks[0][0]);
            freeChunk(tilemap->m_chunks[0][1]);
            freeChunk(tilemap->m_chunks[0][2]);

            tilemap->m_chunks[0][0]->m_blocks = tilemap->m_chunks[1][0]->m_blocks;
            tilemap->m_chunks[0][0]->world_position = tilemap->m_chunks[1][0]->world_position;
            tilemap->m_chunks[1][0]->m_blocks = tilemap->m_chunks[2][0]->m_blocks;
            tilemap->m_chunks[1][0]->world_position = tilemap->m_chunks[2][0]->world_position;
            tilemap->m_chunks[2][0] = NULL;

            tilemap->m_chunks[0][1]->m_blocks = tilemap->m_chunks[1][1]->m_blocks;
            tilemap->m_chunks[0][1]->world_position = tilemap->m_chunks[1][1]->world_position;
            tilemap->m_chunks[1][1]->m_blocks = tilemap->m_chunks[2][1]->m_blocks;
            tilemap->m_chunks[1][1]->world_position = tilemap->m_chunks[2][1]->world_position;
            tilemap->m_chunks[2][1] = NULL;

            tilemap->m_chunks[0][2]->m_blocks = tilemap->m_chunks[1][2]->m_blocks;
            tilemap->m_chunks[0][2]->world_position = tilemap->m_chunks[1][2]->world_position;
            tilemap->m_chunks[1][2]->m_blocks = tilemap->m_chunks[2][2]->m_blocks;
            tilemap->m_chunks[1][2]->world_position = tilemap->m_chunks[2][2]->world_position;
            tilemap->m_chunks[2][2] = NULL;

            break;

        case WEST:
            freeChunk(tilemap->m_chunks[0][2]);
            freeChunk(tilemap->m_chunks[1][2]);
            freeChunk(tilemap->m_chunks[2][2]);

            tilemap->m_chunks[0][2]->m_blocks = tilemap->m_chunks[0][1]->m_blocks;
            tilemap->m_chunks[0][2]->world_position = tilemap->m_chunks[0][1]->world_position;
            tilemap->m_chunks[0][1]->m_blocks = tilemap->m_chunks[0][0]->m_blocks;
            tilemap->m_chunks[0][1]->world_position = tilemap->m_chunks[0][0]->world_position;
            tilemap->m_chunks[0][0] = NULL;

            tilemap->m_chunks[1][2]->m_blocks = tilemap->m_chunks[1][1]->m_blocks;
            tilemap->m_chunks[1][2]->world_position = tilemap->m_chunks[1][1]->world_position;
            tilemap->m_chunks[1][1]->m_blocks = tilemap->m_chunks[1][0]->m_blocks;
            tilemap->m_chunks[1][1]->world_position = tilemap->m_chunks[1][0]->world_position;
            tilemap->m_chunks[1][0] = NULL;

            tilemap->m_chunks[2][2]->m_blocks = tilemap->m_chunks[2][1]->m_blocks;
            tilemap->m_chunks[2][2]->world_position = tilemap->m_chunks[2][1]->world_position;
            tilemap->m_chunks[2][1]->m_blocks = tilemap->m_chunks[2][0]->m_blocks;
            tilemap->m_chunks[2][1]->world_position = tilemap->m_chunks[2][0]->world_position;
            tilemap->m_chunks[2][0] = NULL;

            break;

        case EAST:
            freeChunk(tilemap->m_chunks[0][0]);
            freeChunk(tilemap->m_chunks[1][0]);
            freeChunk(tilemap->m_chunks[2][0]);

            tilemap->m_chunks[0][0]->m_blocks = tilemap->m_chunks[0][1]->m_blocks;
            tilemap->m_chunks[0][0]->world_position = tilemap->m_chunks[0][1]->world_position;
            tilemap->m_chunks[0][1]->m_blocks = tilemap->m_chunks[0][2]->m_blocks;
            tilemap->m_chunks[0][1]->world_position = tilemap->m_chunks[0][2]->world_position;
            tilemap->m_chunks[0][2]->m_blocks = NULL;

            tilemap->m_chunks[1][0]->m_blocks = tilemap->m_chunks[1][1]->m_blocks;
            tilemap->m_chunks[1][0]->world_position = tilemap->m_chunks[1][1]->world_position;
            tilemap->m_chunks[1][1]->m_blocks = tilemap->m_chunks[1][2]->m_blocks;
            tilemap->m_chunks[1][1]->world_position = tilemap->m_chunks[1][2]->world_position;
            tilemap->m_chunks[1][2]->m_blocks = NULL;

            tilemap->m_chunks[2][0]->m_blocks = tilemap->m_chunks[2][1]->m_blocks;
            tilemap->m_chunks[2][0]->world_position = tilemap->m_chunks[2][1]->world_position;
            tilemap->m_chunks[2][1]->m_blocks = tilemap->m_chunks[2][2]->m_blocks;
            tilemap->m_chunks[2][1]->world_position = tilemap->m_chunks[2][2]->world_position;
            tilemap->m_chunks[2][2]->m_blocks = NULL;

            break;

        default:
            break;
        }
    }

    Coordinate_s tilemap_top_coord;
    bool first_chunk = true;
    for (int i = player_chunk_coord.m_y - 1, ci = 0; i <= player_chunk_coord.m_y + 1; i++, ci++)
    {
        for (int j = player_chunk_coord.m_x - 1, cj = 0; j <= player_chunk_coord.m_x + 1; j++, cj++)
        {
            if (!tilemap->m_chunks[ci][cj] || !tilemap->m_chunks[ci][cj]->m_blocks)
            {
                if (tilemap->m_chunks[ci][cj])
                    free(tilemap->m_chunks[ci][cj]);
                tilemap->m_chunks[ci][cj] = CreateChunkProcedurally(getTopCoordinateFromChunk(tilemap, (Coordinate_s){j, i}), seed);
            }
        }
    }

    // Map the chunk's blocks to the tilemap easier to access
    // long cursor_chunk = whereisChunkInFile(tilemap->m_chunks[0][0]->world_position, "../saved_chunks");
    // writeFileToChunk(tilemap->m_chunks[0][0], "../saved_chunks", cursor_chunk);
    LoadChunksToTilemap(tilemap, tilemap_top_coord);
}

long whereisChunkInFile(Coordinate_s chunk_coord, const char *filename)
{
    FILE *chunks_savefile = fopen(filename, "rb");

    Coordinate_s read_coord;
    size_t n = 1;
    while (n)
    {
        n = fread(&read_coord, sizeof(Coordinate_s), 1, chunks_savefile);
        if (!memcmp(&read_coord, &chunk_coord, sizeof(Coordinate_s)))
        {
            long cur = ftell(chunks_savefile);
            fclose(chunks_savefile);
            return cur - sizeof(Coordinate_s);
        }

        fseek(chunks_savefile, CHUNK_SIZE * CHUNK_SIZE * sizeof(Block_s) * 2, SEEK_CUR);
    }

    fclose(chunks_savefile);
    return -1;
}

void writeChunkToFile(Chunk_s *chunk, const char *filename)
{
    FILE *chunks_savefile = fopen(filename, "ab");
    // Write chunk coordinate
    fwrite(&chunk->world_position, sizeof(Coordinate_s), 1, chunks_savefile);

    // Write chunk blocks
    for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
    {
        fwrite(chunk->m_blocks[i][0], sizeof(Block_s), 2, chunks_savefile);
    }

    fclose(chunks_savefile);
}

void writeFileToChunk(Chunk_s *chunk, const char *filename, long cursor)
{
    FILE *chunks_savefile = fopen(filename, "rb");
    fseek(chunks_savefile, cursor, SEEK_SET);
    // Read chunk coordinate
    fread(&chunk->world_position, sizeof(Coordinate_s), 1, chunks_savefile);

    // Read chunk blocks
    for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
    {
        fread(chunk->m_blocks[i][0], sizeof(Block_s), 2, chunks_savefile);
        // fread(chunk->m_blocks[i][1], sizeof(Block_s), 1, chunks_savefile);
    }

    fclose(chunks_savefile);
}

void freeChunk(Chunk_s *chunk)
{
    if (!chunk)
        return;

    // Free blocks
    for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
    {
        free(chunk->m_blocks[i][0]);
        free(chunk->m_blocks[i][1]);
        free(chunk->m_blocks[i]);
    }

    free(chunk->m_blocks);
    chunk->m_blocks = NULL;
}
