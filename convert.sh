#!/bin/bash

# This script requires obj2dl from Nitro Engine:
#
# https://github.com/AntonioND/nitro-engine
#
# The model has normals, texture UV information, and no material information.

export OBJ2DL="python3 /opt/blocksds/external/nitro-engine/tools/obj2dl/obj2dl.py"

# rm -rf data
# mkdir data

$OBJ2DL \
    --input assets/tardis.obj \
    --output data/tardis.bin \
    --texture 256 256 \
    --scale 1

$OBJ2DL \
    --input assets/test_cube.obj \
    --output data/test_cube.bin \
    --texture 128 128 \
    --scale 1

$OBJ2DL \
    --input assets/skybox.obj \
    --output data/skybox.bin \
    --texture 128 128 \
    --scale 0.15
