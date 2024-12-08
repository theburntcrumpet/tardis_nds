# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2024

BLOCKSDS	?= /opt/blocksds/core

# User config

NAME		:= tardis
GAME_TITLE	:= TARDIS Demo
GAME_SUBTITLE	:= Crumpy's TARDIS demo

# Source code paths

GFXDIRS		= graphics

BINDIRS		:= data
AUDIODIRS := audio

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
