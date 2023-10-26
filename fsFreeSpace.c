/**************************************************************
* Class:  CSC-415-0# - Summer 2023
* Names: 
* Student IDs:
* GitHub Name:
* Group Name:
* Project: Basic File System
*
* File: fsFreeSpace.c
*
* Description: 
*
**************************************************************/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "fsVcb.h"

#define FREE_BLOCKS_OFFSET 1

int startup = 0;

int initFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize) {
    if (!startup) {
        FreeSpaceManager* fsm = (FreeSpaceManager*) malloc(sizeof(FreeSpaceManager));
        
        size_t bmSize = numberOfBlocks - FREE_BLOCKS_OFFSET;
        fsm->bitmap = (uint8_t*) malloc(bmSize);
        fsm->count = bmSize;

        startup = 1;
    }
    return 1;
}