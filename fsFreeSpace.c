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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "fsLow.h"
#include "fsVcb.h"
#include "fsFreeSpace.h"

// converts size in bytes to size in blocks using int math
int getBlockSizeFromBytes(int bytes);
// creates a byte buffer of blocks size and updates VCB memory
FreeSpaceManager *getFreeBlocks(int blocks);

// TODO: Make safer for small block sizes
int initFreeSpace()
{
    int blockCount = getFreeBlockCount();
    if (blockCount == FREESPACEUNINTIALIZEDFLAG)
    {
        blockCount = getTotalBlockCount() - FREESPACESTARTOFFSET;
        printf("-------------------------Free Space Initializing HUMAN OS-------------------------\n");

        char *fsm = malloc(getBlockSize());
        ((FreeSpaceManager *)fsm)->currentBlock = FREESPACESTARTOFFSET;
        ((FreeSpaceManager *)fsm)->nextBlock = LASTBLOCK;
        ((FreeSpaceManager *)fsm)->size = blockCount;

        LBAwrite(fsm, 1, FREESPACESTARTOFFSET);
        setFirstFree((FreeSpaceManager *)fsm);
        setLastFree((FreeSpaceManager *)fsm);
        free(fsm);
        setFreeBlockCount(blockCount);
        printf("-------------------------Free Space Initializing Finished HUMAN OS-------------------------\n");
    }
    return 0;
}

int getBytesCountAvailabe()
{
    return getFreeBlockCount() * getBlockSize();
}

int fsFree(FreeSpaceManager *firstBlock, FreeSpaceManager *lastBlock, int fileSize)
{
    FreeSpaceManager *lastFree = getLastFree();
    /*********************************/
    if (lastFree->currentBlock != LASTBLOCK)
    {
        char *writeBlock = malloc(getBlockSize());
        memcpy(writeBlock, lastFree, sizeof(FreeSpaceManager));
        ((FreeSpaceManager *)writeBlock)->nextBlock = firstBlock->currentBlock;
        LBAwrite(writeBlock, 1, ((FreeSpaceManager *)writeBlock)->currentBlock);
    }
    /********************************/
    setLastFree(lastBlock);
    setFreeBlockCount(getFreeBlockCount() + fileSize);
    return 0;
}

FreeSpaceManager *getAvailableBytes(int bytes)
{
    int requestedBlocks = getBlockSizeFromBytes(bytes);
    if (getFreeBlockCount() < requestedBlocks)
    {
        return NULL;
    }
    return getFreeBlocks(requestedBlocks);
}

int defragmentFreeSpace()
{
    printf("Reached unimplemented function defragmentFreeSpace. Infinite loop imminent. Exiting before crash.");
    exit(-1);
    // return 0;
}

FreeSpaceManager *copyFreeSpaceManager(FreeSpaceManager *oldFSM)
{
    FreeSpaceManager *retObject = NULL;
    if (oldFSM != NULL)
    {
        retObject = malloc(sizeof(FreeSpaceManager));
        memcpy(retObject, oldFSM, sizeof(FreeSpaceManager));
    }
    return retObject;
}

int getBlockSizeFromBytes(int bytes)
{
    return (bytes + getBlockSize() - 1) / getBlockSize();
}

FreeSpaceManager *getFreeBlocks(int blocks)
{
    // pointer to first free space info from vcb
    FreeSpaceManager *head = getFirstFree();

    // pointer to return new allocated block(s) info
    FreeSpaceManager *ret = malloc(sizeof(FreeSpaceManager));
    // ret->currentBlock;
    ret->nextBlock = LASTBLOCK;
    ret->size = blocks;

    int changeLastBlockFlag = 0;
    if (getFirstFree() == getLastFree())
    {
        changeLastBlockFlag = 1;
    }

    // size < blocks
    if (head->size < blocks)
    {
        // find new block that works
        char *blockIterator = malloc(getBlockSize());
        memcpy(blockIterator, head, sizeof(FreeSpaceManager));

        char *blockBefore = malloc(getBlockSize());

        while (((FreeSpaceManager *)blockIterator)->size < blocks && ((FreeSpaceManager *)blockIterator)->nextBlock != LASTBLOCK)
        {
            memcpy(blockBefore, blockIterator, sizeof(FreeSpaceManager));
            LBAread(blockIterator, 1, ((FreeSpaceManager *)blockIterator)->nextBlock);
        }

       
        if (((FreeSpaceManager *)blockIterator)->size < blocks)
        {
            defragmentFreeSpace();
            return getFreeBlocks(blocks);
        }

        if (((FreeSpaceManager *)blockIterator)->size == blocks)
        {
            ((FreeSpaceManager *)blockBefore)->nextBlock = ((FreeSpaceManager *)blockIterator)->nextBlock;
        }
        else
        {
            ((FreeSpaceManager *)blockBefore)->nextBlock = ((FreeSpaceManager *)blockIterator)->currentBlock + ((FreeSpaceManager *)blockIterator)->size;
        }
        LBAwrite(blockBefore, 1, ((FreeSpaceManager *)blockBefore)->currentBlock);
        free(blockBefore);
        blockBefore = NULL;

       
        if (((FreeSpaceManager *)blockIterator)->size > blocks)
        {
            char *blockAfter = malloc(getBlockSize());
            ((FreeSpaceManager *)blockAfter)->currentBlock = ((FreeSpaceManager *)blockIterator)->currentBlock + ((FreeSpaceManager *)blockIterator)->size;
            ((FreeSpaceManager *)blockAfter)->nextBlock = ((FreeSpaceManager *)blockIterator)->nextBlock;
            ((FreeSpaceManager *)blockAfter)->size = ((FreeSpaceManager *)blockIterator)->size - blocks;
            LBAwrite(blockAfter, 1, ((FreeSpaceManager *)blockAfter)->currentBlock);
            free(blockAfter);
            blockAfter = NULL;
        }

      
        ret->currentBlock = ((FreeSpaceManager *)blockIterator)->currentBlock;
        LBAwrite(blockIterator, 1, ((FreeSpaceManager *)blockIterator)->currentBlock);
        free(blockIterator);
        blockIterator = NULL;

        
    }
    else if (head->size > blocks)
    {
        ret->currentBlock = head->currentBlock;

       
        char *blockAfter = malloc(getBlockSize());
        ((FreeSpaceManager *)blockAfter)->currentBlock = head->currentBlock + blocks;
        ((FreeSpaceManager *)blockAfter)->nextBlock = head->nextBlock;
        ((FreeSpaceManager *)blockAfter)->size = head->size - blocks;
        LBAwrite(blockAfter, 1, ((FreeSpaceManager *)blockAfter)->currentBlock);

        
        setFirstFree((FreeSpaceManager *)blockAfter);
        if (changeLastBlockFlag)
        {
            setLastFree((FreeSpaceManager *)blockAfter);
        }
        free(blockAfter);
        blockAfter = NULL;

        
        char *currentBlock = malloc(getBlockSize());
        memcpy(currentBlock, ret, sizeof(FreeSpaceManager));
        LBAwrite(currentBlock, 1, ((FreeSpaceManager *)currentBlock)->currentBlock);
        free(currentBlock);
        currentBlock = NULL;

        
    }
    else
    {
        ret->currentBlock = head->currentBlock;

     
        if (head->nextBlock != LASTBLOCK)
        {
            char *currentBlock = malloc(getBlockSize());
            memcpy(currentBlock, ret, sizeof(FreeSpaceManager));
            LBAwrite(currentBlock, 1, ((FreeSpaceManager *)currentBlock)->currentBlock);
            free(currentBlock);
            currentBlock = NULL;

            
            char *nextBlock = malloc(getBlockSize());
            LBAread(nextBlock, 1, head->nextBlock);
            setFirstFree((FreeSpaceManager *)nextBlock);
            if (changeLastBlockFlag)
            {
                setLastFree((FreeSpaceManager *)nextBlock);
            }
            free(nextBlock);
            nextBlock = NULL;
        }
        else
        {
            FreeSpaceManager *noBlock = malloc(sizeof(FreeSpaceManager));
            noBlock->currentBlock = LASTBLOCK;
            noBlock->nextBlock = LASTBLOCK;
            noBlock->size = 0;

            setFirstFree(noBlock);
            if (changeLastBlockFlag)
            {
                setLastFree(noBlock);
            }
            free(noBlock);
            noBlock = NULL;
        }
    }

    
    setFreeBlockCount(getFreeBlockCount() - blocks);

    return ret;
}