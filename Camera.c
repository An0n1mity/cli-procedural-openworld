#include "Camera.h"

struct Camera_s *CreateCamera(struct Coordinate_s coord, int w, int h)
{
    struct Camera_s *camera = (struct Camera_s *)malloc(sizeof(struct Camera_s));
    *camera = (struct Camera_s){coord, w, h};
    return camera;
}

bool isBlockInCameraField(struct Camera_s *camera, struct Coordinate_s block_coord)
{
    if (block_coord.m_x < camera->m_position.m_x ||
        block_coord.m_y < camera->m_position.m_y ||
        block_coord.m_x > camera->m_position.m_x + camera->m_width ||
        block_coord.m_y > camera->m_position.m_y + camera->m_width)
        return false;

    return true;
}
