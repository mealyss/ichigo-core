#ifndef RN_RAINY
#define RN_RAINY

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct RN_Vector3f 
{
    float x, y, z;
} RN_Vector3f;

typedef void RN_Buffer;
extern RN_Buffer*   rn_buffer_create(uint32_t widht, uint32_t height);
extern void         rn_buffer_destroy(RN_Buffer* buffer);  

typedef void RN_Scene;
extern RN_Scene*    rn_scene_create();
extern void         rn_scene_destroy(RN_Scene* scene);


typedef void RN_Sphere;
extern RN_Sphere*   rn_sphere_create_array(size_t lenght);
extern RN_Sphere*   rn_spere_add(RN_Sphere* array, uint32_t index,
                                 RN_Vector3f position,
                                 float radius);
extern RN_Scene*    rn_scene_add_spheres(RN_Sphere* spheres, uint32_t count);


extern void         rn_render(const RN_Scene* scene, RN_Buffer* render_buffer);

extern bool         rn_save_bmp(const RN_Buffer* buffer, const char* file_name);


#endif
