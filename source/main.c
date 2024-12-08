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

int main(int argc, char **argv)
{
    glInit();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ANTIALIAS);
    consoleDemoInit();
    soundEnable();
    u8 volume = 60;
    soundPlaySample(theme_bin,SoundFormat_16Bit,theme_bin_size,8000,volume,64,true,0);
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
    int textureId;
    // load textures
    glGenTextures(1, &textureId);
    glBindTexture(0, textureId);
    if(glTexImage2D(0,0,GL_RGBA,256,256,0,TEXGEN_TEXCOORD,tardis_texBitmap) == 0){
        printf("tex load failed\n");
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 256.0 / 192.0, 0.1, 20);

    printf("Dr Who 3D Demo\n");
    printf("By crumpy - Just messin' around\n");
    printf("Start: exit\n");


    

    s32 rotateX = 0;
    s32 rotateY = 0;
    u32 tick = 0;
    const s32 radiusX =4000, radiusY = 4000; 

    GLvector position = {0,0,0};
    
    while(1)
    {
        swiWaitForVBlank();
        scanKeys();
        u16 keys = keysHeld();
        rotateY += 3 << 5;
        rotateX = (sin(tick * M_PI / 180.0) * 30) * (100);
        position.x = sin(tick * M_PI * 0.01) * radiusX;
        position.y = cos(tick * M_PI * 0.01) * radiusY;

        glMatrixMode(GL_MODELVIEW);
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 |
                      POLY_ID(0) | POLY_DECAL);

        glPushMatrix();
        {
            glRotateXi(rotateX);
            glRotateYi(rotateY);

            glTranslatev(&position);
            glCallList(tardis_bin);
        }

        glColor3f(1, 1, 1);
        glPopMatrix(1);
        glFlush(0);
        tick++;
        if (keys & KEY_START)
        {
            break;
        }
    }


    return 0;
}
