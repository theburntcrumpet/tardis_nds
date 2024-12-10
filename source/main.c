// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2024

// Billboards are 2D objects drawn in a 3D world that always face the camera.
// For example, if you're drawing many spheres in a 3D environment, using 2D
// circles as billboards to represent the spheres can save you a lot of polygons
// and, depending on your situation, it won't look worse than drawing sphere
// models all over the place.

#include <nds.h>
#include <math.h>

#include "tardis_bin.h"
#include "tardis_tex.h"

#include "theme_bin.h"

#include "test_cube_bin.h"
#include "test_texture.h"

#include "entity.h"

Texture *CreateTexture(GLvector textureSize, void *textureData, unsigned int textureDataSize)
{
    Texture *texture = malloc(sizeof(Texture));
    texture->textureSize = textureSize;
    texture->textureData = textureData;
    texture->textureDataSize = textureDataSize;
    glGenTextures(1, &(texture->textureId));
    glBindTexture(0, texture->textureId);
    if (glTexImage2D(0, 0, GL_RGBA, texture->textureSize.x, texture->textureSize.y, 0, TEXGEN_TEXCOORD, texture->textureData) == 0)
    {
        printf("tex load failed - %i\n", texture->textureId);
        free(texture);
        return NULL;
    }
    glBindTexture(0, 0);
    return texture;
}

void DrawEntity(Entity *entity)
{

    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK |
              POLY_ID(0) | POLY_DECAL);

    glMatrixMode(GL_MODELVIEW);
    if (entity->texture == NULL)
    {
        printf("no texture was bound because texture was null.\n");
        glBindTexture(0, 0);
    }
    else
    {
        glBindTexture(0, entity->texture->textureId);
    }

    glPushMatrix();
    {

        glRotateXi(entity->rotation.x);
        glRotateYi(entity->rotation.y);
        glRotateZi(entity->rotation.z);
        glTranslatev(&(entity->position));
        glCallList(entity->modelData);
    }

    glPopMatrix(1);
    glBindTexture(0, 0);
}

Entity *CreateEntity(GLvector position, GLvector rotation, void *modelData, int modelDataSize, Texture *texture)
{
    Entity *entity = malloc(sizeof(Entity));
    entity->position = position;
    entity->rotation = rotation;
    entity->modelData = modelData;
    entity->modelDataSize = modelDataSize;
    entity->texture = texture;
    return entity;
}

int main(int argc, char **argv)
{
    glInit();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ANTIALIAS);
    consoleDemoInit();
    soundEnable();
    u8 volume = 0;
    soundPlaySample(theme_bin, SoundFormat_16Bit, theme_bin_size, 8000, volume, 64, true, 0);
    videoSetMode(MODE_0_3D);

    glClearColor(0, 0, 6, 31);

    glClearPolyID(63);
    glClearDepth(0x7FFF);

    gluLookAt(0.0, 0.0, 8.0,  // Camera position
              0.0, 0.0, 0.0,  // Look at
              0.0, 1.0, 0.0); // Up

    // glLight(0, RGB15(31, 31, 31),
    //         floattov10(0.58), floattov10(-0.58), floattov10(-0.58));

    glViewport(0, 0, 255, 191);

    vramSetBankA(VRAM_A_TEXTURE);
    vramSetBankB(VRAM_B_TEXTURE);

    Texture *tardisTexture = CreateTexture(
        (GLvector){256, 256, 0},
        (void *)tardis_texBitmap,
        tardis_texBitmapLen);

    Texture *cubeTexture = CreateTexture(
        (GLvector){128, 128, 0},
        (void *)test_textureBitmap,
        test_textureBitmapLen);

    Entity *tardis = CreateEntity(
        (GLvector){0, 0, 0},
        (GLvector){0, 0, 0},
        (void *)tardis_bin,
        tardis_bin_size,
        tardisTexture);

    Entity *cube = CreateEntity(
        (GLvector){0, 0, 0},
        (GLvector){0, 0, 0},
        (void *)test_cube_bin,
        test_cube_bin_size,
        cubeTexture);

    Entity *tardis2 = CreateEntity(
        (GLvector){0, 0, -16000},
        (GLvector){0, 0, 9000},
        (void *)tardis_bin,
        tardis_bin_size,
        cubeTexture);

    Entity *entities[] = {tardis, cube, tardis2};
    int entitiesSize = sizeof(entities) / sizeof(Entity *);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 256.0 / 192.0, 0.1, 20);

    printf("Dr Who 3D Demo\n");
    printf("By crumpy - Just messin' around\n");
    printf("Start: exit\n");

    u32 tick = 0;
    const s32 radiusX = 4000, radiusY = 4000;

    while (1)
    {
        swiWaitForVBlank();
        scanKeys();
        u16 keys = keysHeld();
        tardis->rotation.y += 3 << 5;
        tardis->rotation.x = (sin(tick * M_PI / 180.0) * 30) * (100);
        tardis->position.x = sin(tick * M_PI * 0.01) * radiusX;
        tardis->position.y = cos(tick * M_PI * 0.01) * radiusY;
        cube->rotation.y -= 3 << 5;
        cube->rotation.x = -(sin(tick * M_PI / 180.0) * 30) * (100);
        cube->position.x = -(sin(tick * M_PI * 0.01) * (4 * radiusX));
        cube->position.y = -(cos(tick * M_PI * 0.01) * (4 * radiusY));

        // draw entities
        for (int i = 0; i < entitiesSize; i++)
        {
            DrawEntity(entities[i]);
        }

        glFlush(0);
        tick++;
        if (keys & KEY_START)
        {
            break;
        }
    }

    return 0;
}
