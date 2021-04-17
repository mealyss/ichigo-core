#include "../rnpch.h"
#include "../core/pixel_buffer.h"
#include "../core/assert.h"

#define PLANES 1
#define COMPRESSION 0
#define BIT_PER_PIXEL 24
#define X_PIXEL_PER_METER 0x130B //2835 , 72 DPI
#define Y_PIXEL_PER_METER 0x130B //2835 , 72 DPI

#pragma pack(push,1)

typedef struct 
{
    uint8_t     signature[2];
    uint32_t    filesize;
    uint32_t    reserved;
    uint32_t    fileoffset_to_pixelarray;
} FileHeader;

typedef struct 
{
    uint32_t    dibheadersize;
    uint32_t    width;
    uint32_t    height;
    uint16_t    planes;
    uint16_t    bitsperpixel;
    uint32_t    compression;
    uint32_t    imagesize;
    uint32_t    ypixelpermeter;
    uint32_t    xpixelpermeter;
    uint32_t    numcolorspallette;
    uint32_t    mostimpcolor;
} BmpInfoHeader;

typedef struct 
{
    FileHeader      file_header;
    BmpInfoHeader   info_header;
} Bitmap;

#pragma pack(pop)

bool bmp_save_buffer(const Buffer* buffer, const char* fname)
{
    int res = 1;
    FILE* fp = fopen(fname, "wb");

    if(fp == NULL)
        return false;

    Bitmap* pbmp =  (Bitmap*) calloc(1, sizeof(Bitmap));

    pbmp->file_header.signature[0] = 'B';
    pbmp->file_header.signature[1] = 'M';

    pbmp->file_header.filesize = buffer->widht * buffer->height * sizeof(Color24) + sizeof(Bitmap);
    pbmp->file_header.fileoffset_to_pixelarray = sizeof(Bitmap);

    pbmp->info_header.dibheadersize = sizeof(BmpInfoHeader);
    pbmp->info_header.width     = buffer->widht;
    pbmp->info_header.height    = buffer->height;
    pbmp->info_header.planes    = PLANES;
    pbmp->info_header.bitsperpixel = BIT_PER_PIXEL;
    pbmp->info_header.compression = COMPRESSION;
    pbmp->info_header.imagesize = buffer->widht * buffer->height * (BIT_PER_PIXEL / 8);
    pbmp->info_header.xpixelpermeter = X_PIXEL_PER_METER;
    pbmp->info_header.ypixelpermeter = Y_PIXEL_PER_METER;
    pbmp->info_header.numcolorspallette = 0;

    fwrite(pbmp, 1, sizeof(Bitmap), fp);
    fwrite(buffer->pixels, 1, buffer->widht * buffer->height * (BIT_PER_PIXEL / 8), fp);
    res = fclose(fp); 
    if(res == EOF)
        return false;

    free(pbmp);

    RN_LOGVVV("Saved as \"%s\" %dx%dpx\n", fname, buffer->widht, buffer->height);
    return res;
}

