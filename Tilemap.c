#include "Tilemap.h"

struct Tilemap_s* CreateTilemap(const int width, const int height)
{
    struct Tilemap_s *tilemap = (struct Tilemap_s *)calloc(1, sizeof(struct Tilemap_s));
    tilemap->m_width = width; tilemap->m_height = height;
    tilemap->m_array = (struct Block_s **)malloc(height * width * sizeof(struct Block_s *));

    return tilemap;
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

struct Block_s* CharToBlock(char c)
{
    struct Block_s* block = NULL;
    switch (c)
    {
        case 'W':
            block = CreateBlock(WOOD, BREAKABLE | WALKABLE);
            break;
        case 'R':
            block = CreateBlock(ROCK, MOVABLE);
            break;
        case 'G':
            block = CreateBlock(GRASS, WALKABLE);
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
    for(size_t i = 0; i < statbuf.st_size; i++)
    {
        if(map[i] == '\n' && !start)
            start = 1;
        if(map[i] != '\n' && start && (idx < tilemap->m_width * tilemap->m_height))
        {
            tilemap->m_array[idx++] = CharToBlock(map[i]);
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
           switch (tilemap->m_array[i * tilemap->m_width + j]->m_type)
           {
           case WOOD:
               printf("🪵 ");
               break;
           case ROCK:
               printf("🪨 ");
               break;
           case GRASS:
               printf("🌿");
               break;
           }
       }
       printf("\n");
    }
    printf("\033[0;37m");
}

struct Entitieslist_s *createEntitieslist(struct Entity_s *entity)
{
    struct Entitieslist_s *entities_list = (struct Entitieslist_s *)calloc(1, sizeof(struct Entitieslist_s));
    entities_list->m_entity = entity;

    return entities_list;
}

void addEntityToList(struct EntitiesList_s **list, struct Entity_s *entity)
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
        free(tilemap->m_array[i]);
    }
    free(tilemap->m_array);
    free(tilemap);
}