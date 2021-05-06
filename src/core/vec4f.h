#include <smmintrin.h>
#include <stdbool.h>
#include <stdlib.h>

#include <malloc.h>

typedef __m128 vec4f;

typedef
#ifdef __GNUC__
struct __attribute__((aligned(16))) vec4f_stored 
#else
_MM_ALIGN16 struct vec4f_stored
#endif
{
    float z, y, x, w;
} vec4f_stored;

typedef struct mat4x4f
{
    vec4f_stored m[4];
} mat4x4f;

static vec4f_stored SIGNMASK; 

#define vec4f_stored(x, y, z)\
    {x, y, z, 0}

#define vec4f_load(stored)\
    _mm_load_ps((const float*)(const vec4f_stored*)(stored))

#define vec4f_store(source, dist)\
    _mm_store_ps((float*)(vec4f_stored*)(dist), source)

#define vec4f_set(x, y, z)\
    _mm_setr_ps(z, y, x, 0)

#define vec4f_set1(f)\
    _mm_set1_ps((float)f)

#define vec4f_zero()\
    _mm_setzero_ps()


#if defined WIN32
    #define vec4f_new(c)\
        (vec4f_stored*)_aligned_malloc(sizeof(vec4f_stored) * (c), 16)
#elif defined __linux__
    #define vec4f_new(c)\
        (vec4f_stored*)memalign(16, sizeof(vec4f_stored) * (c))
#elif defined __MACH__
    #define vec4f_new(c)\
        (vec4f_stored*)_malloc(sizeof(vec4f_stored) * (c))
#else 
    #define vec4f_new(c)\
        (vec4f_stored*)_valloc(sizeof(vec4f_stored) * (c))
#endif

#if defined WIN32
    #define vec4f_free(ptr)\
        _aligned_free(ptr);
#else
    #define vec4f_free(ptr)\
        free(ptr);
#endif

static inline vec4f vadd(vec4f v1, vec4f v2)
{
    return _mm_add_ps(v1, v2);
}

static inline vec4f vsub(vec4f v1, vec4f v2)
{
    return _mm_sub_ps(v1, v2);
}

static inline vec4f vmul(vec4f v1, vec4f v2)
{
    return _mm_mul_ps(v1, v2);
}

static inline vec4f vdiv(vec4f v1, vec4f v2)
{
    return _mm_div_ps(v1, v2);
}

static inline vec4f fadd(vec4f v, float f)
{
    return _mm_add_ps(v, _mm_set1_ps(f));
}

static inline vec4f fsub(vec4f v, float f)
{
    return _mm_sub_ps(v, _mm_set1_ps(f));
}

static inline vec4f fmul(vec4f v, float f)
{
    return _mm_mul_ps(v, _mm_set1_ps(f));
}

static inline vec4f fdiv(vec4f v, float f)
{
    return _mm_div_ps(v, _mm_set1_ps(f));
}


static inline void vec4f_init_lib()
{
    _mm_store_ps((float*)&SIGNMASK, _mm_castsi128_ps(_mm_set1_epi32(0x80000000)));
}

static inline bool vequals(vec4f v1, vec4f v2)
{
    return ((_mm_movemask_ps(_mm_cmpeq_ps(v1, v2)) & 0x7) == 0x7);
}

static inline vec4f vsig(vec4f v)
{
    return _mm_xor_ps(v, vec4f_load(&SIGNMASK));
}

static inline bool any(vec4f v)
{
    return !vequals(v, vec4f_zero());
}

static inline vec4f cross(vec4f v1, vec4f v2)
{
    return _mm_sub_ps(
    _mm_mul_ps(_mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 1, 0, 2))),
    _mm_mul_ps(_mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 0, 2, 1)))
    );
}

static inline float dot(vec4f v1, vec4f v2)
{
    return _mm_cvtss_f32(_mm_dp_ps(v1, v2, 0x71));
}

static inline float length(vec4f v)
{
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(v, v, 0x71)));
}

static inline vec4f normalize(vec4f v)
{
    return _mm_div_ps(v, _mm_sqrt_ps(_mm_dp_ps(v, v, 0xFF)));
}

static inline vec4f reflect(vec4f normal, vec4f incident)
{
    return vsub(incident, vmul(normal, vec4f_set1(2 * dot(normal, incident))));
}

static inline float fsqrt(float f)
{
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set1_ps(f)));
}

static inline vec4f refract(vec4f normal, vec4f incident, float eta)
{
    float dotNI = dot(normal, incident);
    float k = 1.0 - eta * eta * (1.0 - dotNI * dotNI);
    if(k < 0.0)
        return vec4f_zero();
    else
        return vsub(fmul(incident, eta), fmul(normal, eta * dotNI + fsqrt(k)));
}

static inline void mmmul(const mat4x4f* Am, const mat4x4f* Bm, mat4x4f* Cm)
{
    const float* A = (float*)&(Am->m);
    const float* B = (float*)&(Bm->m);
    float* C = (float*)&(Cm->m);

    vec4f row1 = _mm_load_ps(&B[0]);
    vec4f row2 = _mm_load_ps(&B[4]);
    vec4f row3 = _mm_load_ps(&B[8]);
    vec4f row4 = _mm_load_ps(&B[12]);

    for(size_t i = 0; i < 4; ++i)
    {
        vec4f brod1 = _mm_set1_ps(A[4*i + 0]);
        vec4f brod2 = _mm_set1_ps(A[4*i + 1]);
        vec4f brod3 = _mm_set1_ps(A[4*i + 2]);
        vec4f brod4 = _mm_set1_ps(A[4*i + 3]);

        vec4f row = _mm_add_ps(
                    _mm_add_ps(
                        _mm_mul_ps(brod1, row1),
                        _mm_mul_ps(brod2, row2)),
                    _mm_add_ps(
                        _mm_mul_ps(brod3, row3),
                        _mm_mul_ps(brod4, row4)));

        _mm_store_ps(&C[4*i], row);
    }

}

static inline vec4f mmul(const mat4x4f* m, const vec4f v)
{
    vec4f_stored A;
    vec4f_store(v, &A);
    const float* B = (float*)&(m->m);

    vec4f row1 = _mm_load_ps(&B[0]);
    vec4f row2 = _mm_load_ps(&B[4]);
    vec4f row3 = _mm_load_ps(&B[8]);
    vec4f row4 = _mm_load_ps(&B[12]);

    vec4f brod1 = _mm_set1_ps(A.x);
    vec4f brod2 = _mm_set1_ps(A.y);
    vec4f brod3 = _mm_set1_ps(A.z);
    vec4f brod4 = _mm_set1_ps(A.w);

    return _mm_add_ps(
                    _mm_add_ps(
                        _mm_mul_ps(brod1, row1),
                        _mm_mul_ps(brod2, row2)),
                    _mm_add_ps(
                        _mm_mul_ps(brod3, row3),
                        _mm_mul_ps(brod4, row4)));

}
