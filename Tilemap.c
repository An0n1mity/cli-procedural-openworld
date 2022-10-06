#include "Tilemap.h"

struct Tilemap_s* CreateTilemap(const int width, const int height)
{
    struct Tilemap_s *tilemap = (struct Tilemap_s *)calloc(1, sizeof(struct Tilemap_s));
    tilemap->m_width = width; tilemap->m_height = height;
    tilemap->m_array = (struct Block_s ***)calloc(height * width, sizeof(struct Block_s **));
    for (size_t i = 0; i < height * width; i++)
    {
        tilemap->m_array[i] = calloc(2, sizeof(struct Block_s *));
    }

    return tilemap;
}

struct TilemapBlock_s *createTilemapBlock(struct Block_s *block)
{
    struct TilemapBlock_s *tilemapblock = (struct TilemapBlock_s *)calloc(1, sizeof(struct TilemapBlock_s));
    tilemapblock->m_block = block;
    return tilemapblock;
}

void addBlockToTilemapBlock(struct TilemapBlock_s **tilemapblock, struct Block_s *block)
{
    if (!*tilemapblock)
    {
        *tilemapblock = createTilemapBlock(block);
        return;
    }

    struct TilemapBlock_s *new = createTilemapBlock(block);
    new->m_next = *tilemapblock;
    *tilemapblock = new;
}

struct Tilemap_s* CreateTilemapFromFile(const char* mapfile)
{
    FILE* file = fopen(mapfile, "rb");
    int width, height;
    fscanf(file, "%d %d", &width, &height);
    fclose(file);

    struct Tilemap_s* tilemap = CreateTilemap(width, height);

    FillTilemap(tilemap, mapfile);

    return tilemap;

}

struct Tilemap_s *CreateTilemapProcedurally(int width, int height, int seed)
{
    struct Tilemap_s *tilemap = CreateTilemap(width, height);
    srand(seed);
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            float value = perlin2d(x, y, 0.1, 1, seed);
            if (value >= 0.55f)
            {
                tilemap->m_array[y * width + x][0] = CreateBlock(GRASS, WALKABLE);
                if (value >= 0.55 && value <= 0.7 && !(rand() % 2))
                    tilemap->m_array[y * width + x][1] = CreateBlock(EVERGREEN_TREE, WALKABLE);
            }
            else if (value <= 0.4)
                tilemap->m_array[y * width + x][0] = CreateBlock(WATER, WALKABLE);
            else if (value > 0.4 && value <= 0.46)
                tilemap->m_array[y * width + x][0] = CreateBlock(SAND, WALKABLE);
            else if (value > 0.46 && value < 0.55)
            {
                tilemap->m_array[y * width + x][0] = CreateBlock(STONE, WALKABLE);
                if (!(rand() % 5))
                    tilemap->m_array[y * width + x][1] = CreateBlock(ROCK, WALKABLE);
            }
        }
    }

    return tilemap;
}

struct Block_s *CharToBlock(char c)
{
    struct Block_s* block = NULL;
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

void FillTilemap(struct Tilemap_s* tilemap, const char* mapfile)
{
    const int fd = open(mapfile, O_RDONLY);

    struct stat statbuf;
    fstat(fd, &statbuf);

    int start = 0;
    char* map = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);

    size_t idx = 0;
    size_t level = 0;
    for(size_t i = 0; i < statbuf.st_size; i++)
    {
        if(map[i] == '\n' && !start)
        {
            start = 1;
            i++;
        }
        if (map[i] != '\n' && map[i] != ',' && start && (idx < tilemap->m_width * tilemap->m_height))
        {
            (tilemap->m_array[idx])[level++] = CharToBlock(map[i]);
        }
        if ((map[i] == ',' || map[i] == '\n') && start)
        {
            ++idx;
            level = 0;
        }
    }

    munmap(map, statbuf.st_size);

    close(fd);
}

void PrintTilemap(struct Tilemap_s* tilemap)
{
    for (size_t i = 0; i < tilemap->m_height; i++)
    {
        for (size_t j = 0; j < tilemap->m_width; j++)
        {

            if (tilemap->m_array[i * tilemap->m_width + j][0])
            {
                switch (tilemap->m_array[i * tilemap->m_width + j][0]->m_type)
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

            if (tilemap->m_array[i * tilemap->m_width + j][1])
            {
                switch (tilemap->m_array[i * tilemap->m_width + j][1]->m_type)
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

struct Entitieslist_s *createEntitieslist(struct Entity_s *entity)
{
    struct Entitieslist_s *entities_list = (struct Entitieslist_s *)calloc(1, sizeof(struct Entitieslist_s));
    entities_list->m_entity = entity;

    return entities_list;
}

void addEntityToList(struct Entitieslist_s **list, struct Entity_s *entity)
{
    if (!*list)
    {
        *list = createEntitieslist(entity);
        return;
    }

    struct Entitieslist_s *new = createEntitieslist(entity);
    new->m_next = *list;
    *list = new;
}

inline void addEntityToTilemap(struct Tilemap_s *tilemap, struct Entity_s *entity)
{
    entity->m_tilemap = tilemap;
    addEntityToList(&tilemap->m_entities, entity);
}

void freeEntitiesList(struct Entitieslist_s *list)
{
    if (!list)
        return;

    freeEntitiesList(list->m_next);

    free(list);
}

void freeTilemap(struct Tilemap_s *tilemap)
{
    freeEntitiesList(tilemap->m_entities);
    for (size_t i = 0; i < tilemap->m_width * tilemap->m_height; i++)
    {
        for (size_t j = 0; j < 2; j++)
            free(tilemap->m_array[i][j]);
        free(tilemap->m_array[i]);
    }
    free(tilemap->m_array);
    free(tilemap);
}

// TODO Render only tiles near the player
