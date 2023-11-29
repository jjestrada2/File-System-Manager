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
* File: fsVcb.c
*
* Description: his fsVcb.c file serves as an implementation of
* various operations related to the Volume Control Block (VCB) 
* and Free Space Management (FSM) in a basic file system. It manages
* the initialization, reading/writing to disk, and manipulation 
* of the VCB and free space information. The file promotes 
* modularity by encapsulating related functionalities in 
* functions and structures.
*
**************************************************************/
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "fsVcb.h"

typedef struct volumeCB
{
    char header[16];
    u_int32_t totalBlockCount;   // total # of blocks
    u_int32_t numFreeBlockCount; // # of free blocks
    u_int32_t blockSize;         // block size
    u_int32_t freeFCBCount;      // free FCB count
    FreeSpaceManager root;              // pointer to root directory (0 index)
    FreeSpaceManager firstFreeSpace;    // free block pointer
    FreeSpaceManager lastFreeSpace;     // last free block pointer
    // bFCB nextFCB;        // free FCB pointer
    // type of volume (file system)  // need more research to understand implementation
} volumeCB;

// reads VCB from disk to memory
// WARNING: WILL OVERWRITE MEMORY VCB
int loadVolumeCB();

int loadFirstFree();
int loadLastFree();

volumeCB* vcb;
FreeSpaceManager* firstFree;
FreeSpaceManager* lastFree;

int initVolumeCB(uint64_t numberOfBlocks, uint64_t blockSize)
{
    
    vcb = (volumeCB *)malloc(blockSize); 
    LBAread(vcb, 1, 0);

    if (strcmp(vcb->header, "HUMAN--VolumeCB") != 0)
    {
        printf("Creating VCB in Block 1\n");
        stpcpy(vcb->header, "HUMANVolumeCB");
        vcb->totalBlockCount = numberOfBlocks;
        vcb->numFreeBlockCount = FREESPACEUNINTIALIZEDFLAG;
        vcb->blockSize = blockSize;
        vcb->root.currentBlock = ROOTUNINITIALIZEDFLAG;
    }
    firstFree = NULL;
    lastFree = NULL;
    return 0;
}

int writeToVolumeCB()
{
    return LBAwrite(vcb, 1, 0);
}

int getTotalBlockCount()
{
    return vcb->totalBlockCount;
}

int getFreeBlockCount()
{
    return vcb->numFreeBlockCount;
}

void setFreeBlockCount(int newBlockCount)
{
    vcb->numFreeBlockCount = newBlockCount;
}

int getBlockSize()
{
    return vcb->blockSize;
}

FreeSpaceManager* getFirstFree()
{
    if (firstFree == NULL || (firstFree->currentBlock != (vcb->firstFreeSpace).currentBlock))
    {
        if (loadFirstFree() == 0)
        {
            return firstFree;
        }
        else
        {
            return NULL;
        }
    }
    return firstFree;
}

int setFirstFree(FreeSpaceManager *blockNumber)
{
    memcpy(&(vcb->firstFreeSpace), blockNumber, sizeof(FreeSpaceManager));
    if(firstFree == NULL) {
        loadFirstFree();
    }
    memcpy(firstFree, blockNumber, sizeof(FreeSpaceManager));
    return 0;
}

FreeSpaceManager* getLastFree()
{
    if (lastFree == NULL)
    {
        if (loadLastFree() != 0)
        {
            return NULL;
        }
    }
    if((lastFree->currentBlock != (vcb->lastFreeSpace).currentBlock)) {
        printf("ERROR: FREE SPACE MISMATCH DETECTED! FILES OR DIRECTORIES MAY BE LOST!");
        return NULL;
    }
    return lastFree;
}

int setLastFree(FreeSpaceManager *blockNumber)
{
    memcpy(&(vcb->lastFreeSpace), blockNumber, sizeof(FreeSpaceManager));
    if(lastFree == NULL) {
        loadLastFree();
    }
    memcpy(lastFree, blockNumber, sizeof(FreeSpaceManager));
    return 0;
}

int setRootLocation(FreeSpaceManager* rootDirectory) {
    memcpy(&(vcb->root), rootDirectory, sizeof(FreeSpaceManager));
    return 0;
}

FreeSpaceManager* getRootLocation() {
    return &(vcb->root);
}

int freeVCB()
{
    //rewriteVCB();
    if(vcb != NULL) {
        free(vcb);
        vcb = NULL;
    }
    if (firstFree != NULL)
    {
        free(firstFree);
        firstFree = NULL;
    }
    if (lastFree != NULL)
    {
        free(lastFree);
        lastFree = NULL;
    }
    return 0;
}

int loadVolumeCB()
{
    return LBAread(vcb, 1, 0);
}

int loadFirstFree()
{
    char *firstFreeBuffer = malloc(vcb->blockSize);
    LBAread(firstFreeBuffer, 1, (vcb->firstFreeSpace).currentBlock);
    if(firstFree == NULL) {
        firstFree = malloc(sizeof(FreeSpaceManager));
    }
    memcpy(firstFree, firstFreeBuffer, sizeof(FreeSpaceManager));
    free(firstFreeBuffer);
    return 0;
}

int loadLastFree()
{
    char *lastFreeBuffer = malloc(vcb->blockSize);
    LBAread(lastFreeBuffer, 1, (vcb->lastFreeSpace).currentBlock);
    if(lastFree == NULL) {
        lastFree = malloc(sizeof(FreeSpaceManager));
    }
    memcpy(lastFree, lastFreeBuffer, sizeof(FreeSpaceManager));
    free(lastFreeBuffer);
    return 0;
}