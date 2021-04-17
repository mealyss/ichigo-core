// SandBox
#include <stdio.h>

#include "ichigo.h"
#include "core/assert.h"

#include "platform/thread.h"
#include "parallel/render_pool.h"

#define WIDHT   1600 
#define HEIGHT  900 
#define TILE_SIZE 32
#define THREADS 4


int main(int argc, char** argv)
{ 
    ICH_Buffer* buffer = ichigo_buffer_create(WIDHT, HEIGHT);

    ICH_Scene* scene = ichigo_scene_create();

    ICH_RenderWorkers* workers = ichigo_workers_create(THREADS);

    ichigo_parallel_render_tiled(scene, buffer, workers, ICH_LINEAR, TILE_SIZE);
    //ichigo_render(scene, buffer);

    ichigo_save_bmp(buffer, "result.bmp");


    return 0;
}
