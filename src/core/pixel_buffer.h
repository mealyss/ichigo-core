#ifndef RN_BUFFER_H
#define RN_BUFFER_H

#include "../core/color.h"
#include "../core/scene.h"

typedef struct Buffer 
{
    uint32_t widht, height;
    Color24 pixels[];
} Buffer;

#endif
