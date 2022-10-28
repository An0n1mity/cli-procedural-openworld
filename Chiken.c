#include "Chicken.h"

Chicken_s* CreateChicken()
{
    Chicken_s* chicken = (Chicken_s*)malloc(sizeof(Chicken_s));
    chicken->m_base = CreateEntity(PLAYER);

    int life = CHICKEN_LIFE;

    chicken->update_stats = true;
    return chicken;
}
void MoveChickenTo(Chicken_s* chicken, struct Coordinate_S coordinate)
{

}
void addChickenToTilemap(Chicken_s *chicken, struct Tilemap_S *tilemap)
{

}
struct TilemapBlock_S *getFrontTilemapBlockP(Chicken_s *chicken, struct Tilemap_S *tilemap)
{

}
void MoveChicken(Chicken_s *chicken)
{

}
void freeChicken(Chicken_s *chicken)
{

}
void reduceChickenHealth(Chicken_s *chicken)
{
    
}