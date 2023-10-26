/**************************************************************
* Class:  CSC-415-0# - Summer 2023
* Names: 
* Student IDs:
* GitHub Name:
* Group Name:
* Project: Basic File System
*
* File: fsFreeSpace.h
*
* Description: 
*
**************************************************************/

#include <stdint.h>

typedef struct {
    uint8_t* bitmap;
    size_t count;
} FreeSpaceManager;

int initFreeSpace();