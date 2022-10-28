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
        srand(seed);
        for (int y = top_coord.m_y, i = 0, idx = 0; y < top_coord.m_y + CHUNK_SIZE; y++, i++)
        {
            for (int x = top_coord.m_x, j = 0; x < top_coord.m_x + CHUNK_SIZE; x++, idx++, j++)
            {
                float value = perlin2d(x, y, 0.1, 1, seed);
                Coordinate_s block_coord = {x, y};
                if (value >= 0.55f)
                {
                    chunk->m_blocks[i * CHUNK_SIZE + j][0] = CreateBlock(GRASS, WALKABLE | PLACABLE);
                    if (value >= 0.55 && value <= 0.7 && !(rand() % 2))
                        chunk->m_blocks[i * CHUNK_SIZE + j][1] = CreateBlock(EVERGREEN_TREE, BREAKABLE);
                }
                else if (value <= 0.4)
                    chunk->m_blocks[i * CHUNK_SIZE + j][0] = CreateBlock(WATER, 0);
                else if (value > 0.4 && value <= 0.46)
                    chunk->m_blocks[i * CHUNK_SIZE + j][0] = CreateBlock(SAND, WALKABLE | PLACABLE);
                else if (value > 0.46 && value < 0.55)
                {
                    chunk->m_blocks[i * CHUNK_SIZE + j][0] = CreateBlock(STONE, WALKABLE | PLACABLE);
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
    for (size_t i = 0; i < MAX_CHUNK_DISTANCE * CHUNK_SIZE; i++)
    {
        for (size_t j = 0; j < MAX_CHUNK_DISTANCE * CHUNK_SIZE; j++)
        {
            size_t chunk_i = i / CHUNK_SIZE;
            size_t chunk_j = j / CHUNK_SIZE;

            Chunk_s *chunk = tilemap->m_chunks[chunk_i][chunk_j];
            Block_s **block = chunk->m_blocks[(i - chunk_i * CHUNK_SIZE) * CHUNK_SIZE + (j - chunk_j * CHUNK_SIZE)];

            tilemap->m_blocks[block_idx++] = block;
        }
    }
}

void LoadChunkAroundPlayer(Player_s *player, float seed, bool first, int chunkCount_x, int chunkCount_y)
{
    Coordinate_s player_chunk_coord = TilemapToChunkCoordinates(player->m_base->m_position);
    Tilemap_s *tilemap = player->m_base->m_tilemap;

    // Unload previous chunk from memory
    if (!first)
    {
        switch (player->m_base->m_direction)
        {
        case NORTH:
            for (int i = 0; i < MAX_CHUNK_DISTANCE; ++i)
                freeChunk(tilemap->m_chunks[MAX_CHUNK_DISTANCE - 1][i]);

            for (int x = 0; x < MAX_CHUNK_DISTANCE; ++x)
                for (int y = MAX_CHUNK_DISTANCE - 1; y > 0; --y)
                {
                    tilemap->m_chunks[y][x]->m_blocks = tilemap->m_chunks[y - 1][x]->m_blocks;
                    tilemap->m_chunks[y][x]->world_position = tilemap->m_chunks[y - 1][x]->world_position;
                }
            for (int i = 0; i < MAX_CHUNK_DISTANCE; i++)
                tilemap->m_chunks[0][i] = NULL;

            break;

        case SOUTH:
            for (int i = 0; i < MAX_CHUNK_DISTANCE; ++i)
                freeChunk(tilemap->m_chunks[0][i]);

            for (int x = 0; x < MAX_CHUNK_DISTANCE; ++x)
                for (int y = 0; y < MAX_CHUNK_DISTANCE - 1; ++y)
                {
                    tilemap->m_chunks[y][x]->m_blocks = tilemap->m_chunks[y + 1][x]->m_blocks;
                    tilemap->m_chunks[y][x]->world_position = tilemap->m_chunks[y + 1][x]->world_position;
                }
            for (int i = 0; i < MAX_CHUNK_DISTANCE; i++)
                tilemap->m_chunks[MAX_CHUNK_DISTANCE - 1][i] = NULL;

            break;

        case WEST:
            for (int i = 0; i < MAX_CHUNK_DISTANCE; ++i)
                freeChunk(tilemap->m_chunks[i][MAX_CHUNK_DISTANCE - 1]);

            for (int x = 0; x < MAX_CHUNK_DISTANCE; ++x)
                for (int y = MAX_CHUNK_DISTANCE - 1; y > 0; --y)
                {
                    tilemap->m_chunks[x][y]->m_blocks = tilemap->m_chunks[x][y - 1]->m_blocks;
                    tilemap->m_chunks[x][y]->world_position = tilemap->m_chunks[x][y - 1]->world_position;
                }
            for (int i = 0; i < MAX_CHUNK_DISTANCE; i++)
                tilemap->m_chunks[i][0] = NULL;

            break;

        case EAST:
            for (int i = 0; i < MAX_CHUNK_DISTANCE; ++i)
                freeChunk(tilemap->m_chunks[i][0]);

            for (int x = 0; x < MAX_CHUNK_DISTANCE; ++x)
                for (int y = 0; y < MAX_CHUNK_DISTANCE - 1; ++y)
                {
                    tilemap->m_chunks[x][y]->m_blocks = tilemap->m_chunks[x][y + 1]->m_blocks;
                    tilemap->m_chunks[x][y]->world_position = tilemap->m_chunks[x][y + 1]->world_position;
                }
            for (int i = 0; i < MAX_CHUNK_DISTANCE; i++)
                tilemap->m_chunks[i][MAX_CHUNK_DISTANCE - 1] = NULL;

            break;

        default:
            break;
        }
    }

    Coordinate_s tilemap_top_coord;
    bool first_chunk = true;
    for (int i = player_chunk_coord.m_y - chunkCount_y, ci = 0; i <= player_chunk_coord.m_y + chunkCount_y && ci < MAX_CHUNK_DISTANCE; i++, ci++)
    {
        for (int j = player_chunk_coord.m_x - chunkCount_x, cj = 0; j <= player_chunk_coord.m_x + chunkCount_x && cj < MAX_CHUNK_DISTANCE; j++, cj++)
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
    Block_s block_to_write[2];
    for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
    {
        block_to_write[0] = *(chunk->m_blocks[i][0]);
        if (chunk->m_blocks[i][1])
        {
            block_to_write[1] = *(chunk->m_blocks[i][1]);
        }
        else
            block_to_write[1].m_type = VOID;

        fwrite(block_to_write, sizeof(Block_s), 2, chunks_savefile);
    }

    fclose(chunks_savefile);
}

void writeChunkToFileAt(Chunk_s *chunk, const char *filename, long cursor)
{
    FILE *chunks_savefile = fopen(filename, "ab");

    // Set the cursor
    fseek(chunks_savefile, cursor, SEEK_SET);

    // Write chunk coordinate
    fwrite(&chunk->world_position, sizeof(Coordinate_s), 1, chunks_savefile);

    // Write chunk blocks
    Block_s block_to_write[2];
    for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
    {
        block_to_write[0] = *(chunk->m_blocks[i][0]);
        if (chunk->m_blocks[i][1])
            block_to_write[1] = *(chunk->m_blocks[i][1]);
        else
            block_to_write[1].m_type = VOID;

        fwrite(block_to_write, sizeof(Block_s), 2, chunks_savefile);
    }

    fclose(chunks_savefile);
}

void writeFileToChunk(Chunk_s *chunk, const char *filename, long cursor)
{
    FILE *chunks_savefile = fopen(filename, "rb");
    fseek(chunks_savefile, cursor, SEEK_SET);

    if (!chunk)
    {
        chunk = malloc(sizeof(Chunk_s));
    }

    // Read chunk coordinate
    fread(&chunk->world_position, sizeof(Coordinate_s), 1, chunks_savefile);

    // Read chunk blocks
    Block_s readed_block[2];
    for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
    {
        fread(readed_block, sizeof(Block_s), 2, chunks_savefile);
        chunk->m_blocks[i][0] = CreateBlock(readed_block[0].m_type, readed_block[0].m_flags);

        if (readed_block[1].m_type != VOID)
            chunk->m_blocks[i][1] = CreateBlock(readed_block[1].m_type, readed_block[1].m_flags);

        // if (readed_block[1].m_type == PLANK)
        // asm("nop");
        // exit(1);
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
