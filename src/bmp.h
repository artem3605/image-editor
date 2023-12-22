#ifndef BMP_H
#define BMP_H

#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t type;                    // magic number
    uint32_t size;                    // file size
    uint16_t reserved1, reserved2;    // keep it zero
    uint32_t offset;                  // pointer to pixel data (offset)
} HEADER;

typedef struct {
    uint32_t size;                     // size of header
    uint32_t width, height;            // image dimensions
    uint16_t planes;                   // keep 1
    uint16_t bitsPerPixel;             // 1, 4, 8, 16, 24, 32.
    uint32_t compression;              // 0 = no compression
    uint32_t imageSize;                // 0 for no compression
    int32_t  xResolution, yResolution; // pixel per meter
    uint32_t nColors;                  // keep 0
    uint32_t importantColors;          // keep 0
} INFO;

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} PIXEL;
#pragma pack(pop)

#endif