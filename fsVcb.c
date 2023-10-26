/*
* Class:  CSC-415-03 Fall 2023
* Names: Edmund Huang, Jimmy Pan, Juan Estrada, Kripa Pokhrel
* Student IDs: 918426293, 920950183, 923058731, 922961998
* 
* GitHub Name: EdmUUUndo
* 
* Group Name: HumanOS
* 
* Project: Basic File System
*/
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "fsVcb.h"

typedef struct VCB
{
    char header[16];
    u_int32_t totalBlockCount;   // total # of blocks
    u_int32_t numFreeBlockCount; // # of free blocks
    u_int32_t blockSize;         // block size
    u_int32_t freeFCBCount;      // free FCB count
    FSM root;              // pointer to root directory (0 index)
    FSM firstFreeSpace;    // free block pointer
    FSM lastFreeSpace;     // last free block pointer
    // bFCB nextFCB;        // free FCB pointer
    // type of volume (file system)  // need more research to understand implementation
} VCB;

// reads VCB from disk to memory
// WARNING: WILL OVERWRITE MEMORY VCB
int reloadVCB();

int loadFirstFree();
int loadLastFree();

VCB* vcb;
FSM* firstFree;
FSM* lastFree;

int initVcb(uint64_t numberOfBlocks, uint64_t blockSize)
{
    //3 error cases we need to implement:
    //if LbaRead,Lbawrite,malloc
    vcb = (VCB *)malloc(blockSize); // Block 0 buffer
    LBAread(vcb, 1, 0);
    printf("\nHere is vcb first read: %s\n",vcb->header);
    if (strcmp(vcb->header, "HUMANOSVCB") != 0)
    {
        printf("Creating VCB in Block 1\n");
        stpcpy(vcb->header, "HUMANOSVCB");
        vcb->totalBlockCount = numberOfBlocks;
        vcb->numFreeBlockCount = FREESPACEUNINTIALIZEDFLAG;
        vcb->blockSize = blockSize;
        vcb->root.currentBlock = ROOTUNINITIALIZEDFLAG;
    }
    printf("\nHere is vcb first read: %s\n",vcb->header);
    firstFree = NULL;
    lastFree = NULL;
    LBAwrite(vcb, 1, 0);
    return 0;
}

int writeVcb(){
    LBAwrite(vcb,1,0);
}

int getTotalBlockCount(){
    return vcb->totalBlockCount;
}

int getFreeBlockCount(){
    return vcb->numFreeBlockCount;
}

void setFreeBlockCount(int newBlockCount){
    vcb->numFreeBlockCount = newBlockCount;
}

int getBlockSize(){
    return vcb->blockSize;
}

FSM* getFirstFree(){
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


int setFirstFree(FSM* blockNumber){
    memcpy(&(vcb->firstFreeSpace),blockNumber,sizeof(FSM));
    if(firstFree == NULL){
        loadFirstFree();
    }
    memcpy(firstFree,blockNumber,sizeof(FSM));
    return 0;
}

FSM* getLastFree(){
    if(lastFree == NULL){
        if(loadFirstFree() != 0){
            return NULL;
        }
    }
    if(lastFree->currentBlock != vcb->lastFreeSpace.currentBlock){
        printf("ERROR IN FREE SAPCE");
        return NULL;
    }
    return lastFree;
}

int setLastFree(FSM* blockNumber){
    memcpy(&(vcb->lastFreeSpace),blockNumber,sizeof(FSM));
    if(lastFree = NULL){
        loadLastFree();
    }
    memcpy(lastFree,blockNumber,sizeof(FSM));
    return 0;
}

int setRootLocation(FSM* rootDirectory){
    memcpy(&(vcb->root),rootDirectory,sizeof(FSM));
    return 0;
} 

FSM* getRootLocation(){
    return &(vcb->root);
}

int freeVcb(){
    if(vcb != NULL){
        free(vcb);
        vcb = NULL;
    }
    if(firstFree != NULL){
        free(firstFree);
        firstFree = NULL;
    }
    if(lastFree != NULL){
        free(lastFree);
        lastFree =NULL;
    }
    return 0;
}

/*This function is designed to reload the content 
of the Volume Control Block (vcb) from the disk. */
int readVcb(){
    return LBAread(vcb,1,0);
}

/*This function is responsible for loading the content of 
the block pointed to by firstFreeSpace from disk into the 
firstFree structure in memory. It ensures that the necessary
memory is allocated and freed appropriately.
*/
int loadFirstFree(){
    char *firstFreeBuffer = malloc(vcb->blockSize);
    //Read from disk and store in firstFreeBuffer
    LBAread(firstFreeBuffer,1,vcb->firstFreeSpace.currentBlock);
    //Allocate memory for firstFree if it si currently NULL
    if(firstFree == NULL){
        firstFree = malloc(sizeof(FSM));
    }
    //Copy the content of firstFreeBuffer to firstFree
    memcpy(firstFree,firstFreeBuffer,sizeof(FSM));

    free(firstFreeBuffer);
    firstFreeBuffer=NULL;
    return 0;
}

/*This function is responsible for loading the content of 
the block pointed to by lastFreeSpace from disk into the 
lastFree structure in memory. It ensures that the necessary 
memory is allocated and freed appropriately.*/
int loadLastFree(){
    char *lastFreeBuffer = malloc(vcb->blockSize);
    LBAread(lastFreeBuffer,1,vcb->lastFreeSpace.currentBlock);
    if(lastFree == NULL){
        lastFree = malloc(sizeof(FSM));
    }
    memcpy(lastFree,lastFreeBuffer,sizeof(FSM));
    free(lastFreeBuffer);
    lastFreeBuffer = NULL;
}
