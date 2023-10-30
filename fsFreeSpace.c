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
#include <stdlib.h>

#include "fsVcb.h"
#include "fsFreeSpace.h"

#define FREE_BLOCKS_OFFSET 1

int startup = 0;

int initFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize) {
    if (!startup) {
        FreeSpaceManager* fsm = (FreeSpaceManager*) malloc(sizeof(FreeSpaceManager));        
        if (fsm == NULL) {
            return 1; // allocation failed
        }

        size_t bmSize = numberOfBlocks - FREE_BLOCKS_OFFSET;
        
        fsm->bitmap = (uint8_t*) malloc(bmSize);
        if (fsm->bitmap == NULL) {
            return 1; //allocation failed
        }

        fsm->count = bmSize;

        setFreeSpaceManager(fsm);
        startup = 1;
    }
    return 0;
}

int allocateBlocks(FreeSpaceManager* fsm, uint64_t blockNumber, uint64_t count) {
    for (uint64_t i = 0; i < count; i++) {
        uint64_t position = blockNumber - FREE_BLOCKS_OFFSET + i;
        uint64_t byteIndex = position / 8;
        uint8_t bitMask = 1 << (position % 8);

        fsm->bitmap[byteIndex] |= bitMask;
    }

    return 0;
}


