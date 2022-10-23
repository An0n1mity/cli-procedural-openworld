#include "Tilemap.h"

Tilemap_s *CreateTilemap(const int width, const int height)
{
    Tilemap_s *tilemap = (Tilemap_s *)calloc(1, sizeof(Tilemap_s));
    tilemap->m_width = width;
    tilemap->m_height = height;
    tilemap->m_blocks = (Block_s ***)calloc(height * width, sizeof(Block_s **));
    if (!(tilemap->m_blocks))
        exit(1);

    /*for (size_t i = 0; i < height * width; i++)
    {
        tilemap->m_blocks[i] = calloc(2, sizeof(Block_s *));
    }*/

    for (size_t i = 0; i < MAX_CHUNK_DISTANCE; i++)
    {
        for (size_t j = 0; j < MAX_CHUNK_DISTANCE; j++)
        {
            tilemap->m_chunks[i][j] = NULL;
        }
    }

    return tilemap;
}

TilemapBlock_s *createTilemapBlock(Block_s *block)
{
    TilemapBlock_s *tilemapblock = (TilemapBlock_s *)calloc(1, sizeof(TilemapBlock_s));
    tilemapblock->m_block = block;
    return tilemapblock;
}

void addBlockToTilemapBlock(TilemapBlock_s **tilemapblock, Block_s *block)
{
    if (!*tilemapblock)
    {
        *tilemapblock = createTilemapBlock(block);
        return;
    }

    TilemapBlock_s *new = createTilemapBlock(block);
    new->m_next = *tilemapblock;
    *tilemapblock = new;
}

// Tilemap_s* CreateTilemapFromFile(const char* mapfile)
// {
//     FILE* file = fopen(mapfile, "rb");
//     int width, height;
//     fscanf(file, "%d %d", &width, &height);
//     fclose(file);

//     Tilemap_s* tilemap = CreateTilemap(width, height);

//     FillTilemap(tilemap, mapfile);

//     return tilemap;

// }

Tilemap_s *CreateTilemapProcedurally(int width, int height, int seed)
{
    Tilemap_s *tilemap = CreateTilemap(width, height);
    srand(seed);
    return tilemap;
}

Block_s *CharToBlock(char c)
{
    Block_s *block = NULL;
    switch (c)
    {
    case 'W':
        block = CreateBlock(EVERGREEN_TREE, BREAKABLE);
        break;
    case 'w':
        block = CreateBlock(DECIDIOUS_TREE, BREAKABLE);
        break;
    case 'R':
        block = CreateBlock(ROCK, MOVABLE);
        break;
    case 'G':
        block = CreateBlock(GRASS, WALKABLE);
        break;
    case 'D':
        block = CreateBlock(DIRT, WALKABLE);
        break;
    default:
        break;
    }

    return block;
}

void PrintTilemap(Tilemap_s *tilemap)
{
    for (size_t i = 0; i < tilemap->m_height; i++)
    {
        for (size_t j = 0; j < tilemap->m_width; j++)
        {

            if (tilemap->m_blocks[i * tilemap->m_width + j][0])
            {
                switch (tilemap->m_blocks[i * tilemap->m_width + j][0]->m_type)
                {
                case WATER:
                    printf("\e[44m~");

                    break;
                case GRASS:
                    printf("\e[42m");
                    break;
                case SAND:
                    printf("\033[48;2;255;165;0m");
                    break;
                case STONE:
                    printf("\e[0;37m");
                    break;
                }
            }

            if (tilemap->m_blocks[i * tilemap->m_width + j][1])
            {
                switch (tilemap->m_blocks[i * tilemap->m_width + j][1]->m_type)
                {
                case EVERGREEN_TREE:
                    printf("🌲");
                    break;
                case DECIDIOUS_TREE:
                    printf("🌳");
                    break;
                case ROCK:
                    printf("🪨");
                    break;
                case GRASS:
                    printf("🌿");
                    break;
                default:
                    break;
                }
            }

            else if (i == tilemap->m_entities->m_entity->m_position.m_y && j == tilemap->m_entities->m_entity->m_position.m_x)
                printf("👨");
            else
                printf("  ");
        }
        printf("\e[0m");
        printf("\033[0;37m");
        printf("\n\r");
    }
    printf("\e[0m");
    printf("\033[0;37m");
}

inline void addEntityToTilemap(Tilemap_s *tilemap, Entity_s *entity)
{
    entity->m_tilemap = tilemap;
    entity->m_chunk_position = getEntityChunkCoordinate(entity);
    addEntityToList(&tilemap->m_entities, entity);
}

void freeEntitiesList(Entitieslist_s *list)
{
    if (!list)
        return;

    freeEntitiesList(list->m_next);

    free(list);
}

void freeTilemap(Tilemap_s *tilemap)
{
    freeEntitiesList(tilemap->m_entities);
    for (size_t i = 0; i < tilemap->m_width * tilemap->m_height; i++)
    {
        freeBlock(tilemap->m_blocks[i]);
    }

    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            freeChunk(tilemap->m_chunks[i][j]);
        }
    }

    free(tilemap->m_blocks);
    free(tilemap);
}

/************************************ COORDINATE CALCULATION***************************************/

Coordinate_s getEntityTilemapCoordinate(Entity_s *entity)
{
    // Get tilemap top left corner world coordinate
    Coordinate_s tilemap_top_coord = getTopCoordinateFromChunk(entity->m_tilemap, (Coordinate_s){entity->m_chunk_position.m_x - 1, entity->m_chunk_position.m_y - 1});

    return (Coordinate_s){entity->m_position.m_x - tilemap_top_coord.m_x, entity->m_position.m_y - tilemap_top_coord.m_y};
}

Coordinate_s getTopCoordinateFromChunk(Tilemap_s *tilemap, Coordinate_s chunk_coord)
{
    return (Coordinate_s){
        chunk_coord.m_x * CHUNK_SIZE,
        chunk_coord.m_y * CHUNK_SIZE};
}

Coordinate_s getEntityCoordinateInChunk(Entity_s *entity)
{
    Coordinate_s entity_chunk_coord = entity->m_chunk_position;
    Coordinate_s chunk_top_coord = getTopCoordinateFromChunk(entity->m_tilemap, entity_chunk_coord);

    return (Coordinate_s){entity->m_position.m_x - chunk_top_coord.m_x, entity->m_position.m_y - chunk_top_coord.m_y};
}

Coordinate_s TilemapToChunkCoordinates(Coordinate_s tilemap_coord)
{
    return (Coordinate_s){tilemap_coord.m_x / CHUNK_SIZE, tilemap_coord.m_y / CHUNK_SIZE};
}