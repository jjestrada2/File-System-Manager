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
// Ensure the header is not defined multiple times
#ifndef _FREE_H
#define _FREE_H

// Define a constant indicating the last block in the sequence
#define LASTBLOCK -1

// Check if FSMDef is not defined and define it
#ifndef FSMDef
#define FSMDef
// Structure representing the management of free space. Every block in memory or on disk has one
typedef struct FreeSpaceManager {
    // Logical Block Address (LBA) representing the current block
    int currentBlock;
    // LBA representing the next block in the linked list
    int nextBlock;
    // Number of consecutive blocks
    int size;
} FreeSpaceManager;
#endif

// Initialize the free space if it has not been initialized yet
// Return 0 if already initialized or 1 if initialization occurred
int initFreeSpace();

// Retrieve the calculated number of available free bytes
int getBytesCountAvailabe();


// Add the space between two blocks to the free space and update the VCB in memory
// Return 0 if successful
int fsFree(FreeSpaceManager* firstBlock, FreeSpaceManager* lastBlock, int fileSize);

// Obtain a buffer of the specified size rounded up to the nearest block
// Return NULL if the requested bytes are too large
FreeSpaceManager* getAvailableBytes(int bytes);



// Create a copy of the FreeSpaceManager structure
FreeSpaceManager* copyFreeSpaceManager(FreeSpaceManager* oldFSM);

#endif