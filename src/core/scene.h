#ifndef RN_SCENE_H
#define RN_SCENE_H

#include "../rnpch.h"
#include "sphere.h"

typedef struct Scene
{
    Sphere* spheres;
    uint32_t sphere_count;
} Scene;

#endif
