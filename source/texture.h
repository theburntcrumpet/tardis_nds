#ifndef TEXTURE_H
#define TEXTURE_H
#include <nds.h>
typedef struct s_texture {
    GLvector textureSize;
    void * textureData;
    unsigned int textureDataSize;
    int textureId;
} Texture;

#endif