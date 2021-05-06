#ifndef RN_CAMERA_H
#define RN_CAMERA_H

#include "vec4f.h"

typedef struct Camera 
{
    float FOV, far_clip, near_clip;
    vec4f_stored position;
    mat4x4f rotation;

} Camera;

#endif
