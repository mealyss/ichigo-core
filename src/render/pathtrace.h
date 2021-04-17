#ifndef RN_RAYCAST_H
#define RN_RAYCAST_H

#include "../core/pixel_buffer.h"
#include "../core/scene.h"

extern void pathtrace_setup(const Scene* scene, Buffer* buffer);
extern void pathtrace_compute(uint32_t x, uint32_t y, Color24* result);

#endif
