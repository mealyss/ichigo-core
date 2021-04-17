#ifndef RN_ICHIGO
#define RN_ICHIGO

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct ICH_Vector3f 
{
    float x, y, z;
} ICH_Vector3f;

typedef enum ICH_TilingMode
{
    ICH_NONE,
    ICH_LINEAR
} ICH_TilingMode;


typedef void ICH_Buffer;
extern ICH_Buffer*   ichigo_buffer_create(uint32_t widht, uint32_t height);
extern void          ichigo_buffer_destroy(ICH_Buffer* buffer);  

typedef void ICH_Scene;
extern ICH_Scene*    ichigo_scene_create();
extern void          ichigo_scene_destroy(ICH_Scene* scene);


typedef void ICH_Sphere;
extern ICH_Sphere*   ichigo_sphere_create_array(size_t lenght);
extern ICH_Sphere*   ichigo_spere_add(ICH_Sphere* array, uint32_t index,
                                 ICH_Vector3f position,
                                 float radius);
extern ICH_Scene*    ichigo_scene_add_spheres(ICH_Sphere* spheres, uint32_t count);

extern void         ichigo_render(const ICH_Scene* scene, ICH_Buffer* render_buffer);


typedef void ICH_RenderWorkers;
extern ICH_RenderWorkers* ichigo_workers_create(unsigned short thread_count);
extern void         ichigo_workers_terminate(ICH_RenderWorkers* workers);
extern void         ichigo_parallel_render_tiled_async(const ICH_Scene* scene, ICH_Buffer* render_buffer,
                                             ICH_RenderWorkers* workers, 
                                             ICH_TilingMode mode,
                                             unsigned tile_size,
                                             void (*callback)(void));
extern void         ichigo_parallel_render_tiled(const ICH_Scene* scene, ICH_Buffer* render_buffer,
                                             ICH_RenderWorkers* workers, 
                                             ICH_TilingMode mode,
                                             unsigned tile_size);


extern bool         ichigo_save_bmp(const ICH_Buffer* buffer, const char* file_name);


#ifdef __cplusplus
} //extern "C"
#endif

#endif // RN_ICHIGO
