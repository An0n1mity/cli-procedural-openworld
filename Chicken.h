#ifndef CHICKEN_H
#define CHICKEN_H

#include "Constant.h"

#include <stdlib.h>
#include "Entity.h"
#include "Block.h"
#include "Tilemap.h"
#include "Player.h"
typedef enum Chicken_Action_E
{
    CHICKEN_IDLE,
    CHICKEN_MOVE = 0b0001,
    CHICKEN_RUN_AWAY = 0b0011

} Chicken_Action_e;
typedef struct Chicken_S
{
    // Based struct
    struct Entity_S *m_base;

    // Chicken current action
    Chicken_Action_e m_action;

    // Step to walk
    int m_step;

    // If the Chicken stats have changed
    bool update_stats;

} Chicken_s;

Chicken_s *CreateChicken();
void MoveChickenTo(Chicken_s *chicken, struct Coordinate_S coordinate);
void addChickenToTilemap(Chicken_s *chicken, struct Tilemap_S *tilemap);
void MoveChicken(Chicken_s *chicken, double actualTime);
void freeChicken(Chicken_s *chicken);
void reduceChickenHealth(Chicken_s *chicken);

#endif // CHICKEN_H
