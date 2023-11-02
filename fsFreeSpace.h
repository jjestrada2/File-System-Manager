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
* File: fsFreeSpace.h
*
* Description: 
*
* Description: Basic File System - Key File Free Space Operations
*
**************************************************************/

#include <stdint.h>

typedef struct FreeSpaceManager{
    uint8_t* bitmap;
    size_t count;
} FreeSpaceManager;

int initFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize);
int allocateBlocks(FreeSpaceManager* fsm, uint64_t blockNumber, uint64_t count);