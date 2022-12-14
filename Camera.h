#ifndef CAMERA_H
#define CAMERA_H

#include "Coordinate.h"
#include <stdbool.h>

struct Camera_s
{
    Coordinate_s m_position;
    int m_width;
    int m_height;
};

struct Camera_s *CreateCamera(Coordinate_s coord, int w, int h);
bool isBlockInCameraField(struct Camera_s *camera, Coordinate_s block_coord);

#endif /* !CAMERA_H */
