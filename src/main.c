#include <stdio.h>

#include "rainy.h"
#include "core/assert.h"

#include "platform/thread.h"

#include <unistd.h>

void* worker(void* arg)
{
    printf("Entered\n");
    thread_pause((Thread*)arg);
    printf("Paused\n");
    thread_sleep_untill_resumed((Thread*)arg);
    printf("Resumed\n");

    for(int i = 0; i < 10; ++i)
        printf("Hello from Worker\n");

    return NULL;
}

int main(int argc, char** argv)
{
/*
    RN_Buffer* buffer = rn_buffer_create(512, 512);
    
    RN_Scene* scene = rn_scene_create();

    rn_render(scene, buffer);

    rn_save_bmp(buffer, "result.bmp");

    rn_scene_destroy(scene);
    rn_buffer_destroy(buffer);
    return 0;
*/
    Thread* thread = thread_create(NULL, NULL);
    thread_init(thread, worker, thread);

    thread_launch(thread);
    usleep(3 * 1000 * 600);
    printf("Sleeped\n");
    thread_resume(thread);

    thread_wait(thread);
    return 0;
}
