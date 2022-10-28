#include "Chicken.h"

Chicken_s *CreateChicken()
{
    Chicken_s *chicken = (Chicken_s *)malloc(sizeof(Chicken_s));
    chicken->m_base = CreateEntity(CHICKEN);
    chicken->m_base->m_direction = SOUTH;

    chicken->update_stats = true;
    return chicken;
}
void MoveChickenTo(Chicken_s *chicken, struct Coordinate_S coordinate)
{
    chicken->m_base->m_position = coordinate;
}
void addChickenToTilemap(Chicken_s *chicken, struct Tilemap_S *tilemap)
{
    addEntityToTilemap(tilemap, chicken->m_base, chicken);
}
void MoveChicken(Chicken_s *chicken, double actualTime)
{
    if(chicken->m_base->m_last_move + CHICKEN_MV_SPEED > actualTime)
        return;
    if (chicken->m_step <= 0)
    {
        chicken->m_action = CHICKEN_IDLE;
    }
    if (chicken->m_action == CHICKEN_IDLE)
    {
        return;
    }
    // Coordinate_s *chicken_position = &chicken->m_base->m_position;
    // Direction_e chicken_direction = chicken->m_base->m_direction;
    Tilemap_s *tilemap = chicken->m_base->m_tilemap;

    Block_s **front_block = getFrontBlock(chicken->m_base, tilemap);
    if (((front_block[1] && front_block[1]->m_flags & WALKABLE) || !front_block[1]))
    {
        // Update current chicken action

        moveEntityInDirection(chicken->m_base);
        chicken->m_step--;
        PlayerAttack(tilemap->m_player);
        chicken->m_base->m_last_move = actualTime;
    }
}
void freeChicken(Chicken_s *chicken)
{
    free(chicken->m_base);
    free(chicken);
}
void reduceChickenHealth(Chicken_s *chicken)
{
    if (chicken->m_base->m_health <= 0)
        return;
    chicken->m_base->m_health--;
    chicken->update_stats = true;
}