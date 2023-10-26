/**************************************************************
* Class:  CSC-415-03 Fall 2023
* Names: Edmund Huang, Jimmy Pan, Juan Estrada, Kripa Pokhrel
* Student IDs: 918426293, 920950183, 923058731, 922961998
* 
* GitHub Name: EdmUUUndo
* 
* Group Name: HumanOS
* 
* Project: Basic File System
*
* File: fsFreeSpace.c
*
* Description: Basic File System - Key File Free Space Operations
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
