/**************************************************************
* Class:  CSC-415-03 Fall 2023
* Names: Edmund Huang, Jimmy Pan, Juan Estrada, Kripa Pokhrel
* Student IDs: 918426293, 920950183, 923058731, 922961998
* GitHub Name: EdmUUUndo
* Group Name: HumanOS
* Project: Basic File System
*
* File: fsVcb.h
*
* Description: VCB structure and functions
*
**************************************************************/
// check if header already defined in scope
#ifndef _VCB_H
#define _VCB_H

#ifndef uint64_t
typedef u_int64_t uint64_t;
#endif
#ifndef uint32_t
typedef u_int32_t uint32_t;
#endif

#ifndef FSMDef
#define FSMDef
typedef struct FreeSpaceManager {
    int currentBlock;
    int nextBlock;
    int size;
} FreeSpaceManager;
#endif

#define VCB_HEADER        "VCB"

// 
#define FREESPACESTARTOFFSET 1
#define FREESPACEUNINTIALIZEDFLAG -1
#define ROOTUNINITIALIZEDFLAG -1

// TODO: Error returns/documentation
// initializes the VCB or reads it from disk. 0 if success
int initVolumeCB(uint64_t numberOfBlocks, uint64_t blockSize);

// rewrites the VCB to disk. Returns block size if successful
int writeToVolumeCB();

// returns the total block count from VCB
int getTotalBlockCount();

// returns the total free block count from the VCB
int getFreeBlockCount();

// sets the total free block count in the VCB memory
void setFreeBlockCount(int newBlockCount);

// returns block size from the VCB
int getBlockSize();

// returns a byte pointer to a buffer of the first free block. NULL if an error occurred.
FreeSpaceManager* getFirstFree();

// sets first free block in VCB memory
int setFirstFree(FreeSpaceManager* blockNumber);

// returns a byte pointer to a buffer of the first free block. NULL if an error occurs
FreeSpaceManager* getLastFree();

// sets last free block in VCB memory
int setLastFree(FreeSpaceManager* blockNumber);

int setRootLocation(FreeSpaceManager* rootDirectory);

FreeSpaceManager* getRootLocation();

// saves the VCB to disk then frees all VCB memory
int freeVCB();

#endif