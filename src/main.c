#include <stdio.h>

#include "rainy.h"

int main(int argc, char** argv)
{
    RN_Buffer* buffer = rn_buffer_create(512, 512);
    
    RN_Scene* scene = rn_scene_create();

    rn_render(scene, buffer);

    rn_save_bmp(buffer, "result.bmp");

    rn_scene_destroy(scene);
    rn_buffer_destroy(buffer);
    return 0;
}
