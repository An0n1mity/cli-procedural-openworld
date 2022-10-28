#ifndef CHICKEN_H
#define CHICKEN_H

#include <stdlib.h>
#include "Entity.h"
#include "Block.h"
#include "Tilemap.h"
#include "Player.h"

typedef struct Chicken_S
{
    // Based struct
    struct Entity_S *m_base;

    // Chicken vitals stats
    int life;

    // Chicken current action
    Action_e m_action;

    // If the Chicken stats have changed
    bool update_stats;

} Chicken_s;

Chicken_s* CreateChicken_s();
void MoveChicken_sTo(Chicken_s* chicken, struct Coordinate_S coordinate);
void addChickenToTilemap(Chicken_s *chicken, struct Tilemap_S *tilemap);
struct TilemapBlock_S *getFrontTilemapBlockP(Chicken_s *chicken, struct Tilemap_S *tilemap);
void MoveChicken(Chicken_s *chicken);
void freeChicken(Chicken_s *chicken);
void reduceChickenHealth(Chicken_s *chicken);

#endif //CHICKEN_H

 
