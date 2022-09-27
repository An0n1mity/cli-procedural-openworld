#include "Tilemap.h"

struct Tilemap_s* CreateTilemap(const int width, const int height)
{
    struct Tilemap_s* tilemap = (struct Tilemap_s*)malloc(sizeof(struct Tilemap_s));
    tilemap->m_width = width; tilemap->m_height = height;
    tilemap->m_array = (struct Block_s**)malloc(sizeof(struct Block_s*) * height * width);

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
            block = CreateBlock(WOOD, BREAKABLE);
            break;
        case 'R':
            block = CreateBlock(ROCK, MOVABLE);
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
   

    close(fd);
}

void PrintTilemap(struct Tilemap_s* tilemap)
{
    for (size_t i = 0; i < tilemap->m_height; i++)
    {
       for (size_t j = 0; j < tilemap->m_width; j++)
       {
            printf("%d ", tilemap->m_array[i * tilemap->m_width + j]->m_type);
       }
       printf("\n");
    }
    
}

