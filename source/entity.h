#ifndef ENTITY_H
#define ENTITY_H
#include <nds.h>
#include "texture.h"
typedef struct s_entity {
    GLvector position;
    GLvector rotation;
    GLvector renderScale;
    void * modelData;
    int modelDataSize;
    Texture * texture;
} Entity;

#endif