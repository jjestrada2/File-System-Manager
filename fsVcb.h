/**************************************************************
* Class:  CSC-415-03 Fall 2023
* Names: Edmund Huang, Jimmy Pan, Juan Estrada, Kripa Pokhrel
* Student IDs: 918426293, 920950183, 923058731, 922961998
<<<<<<< HEAD
* 
* GitHub Name: EdmUUUndo
* 
* Group Name: HumanOS
* 
* Project: Basic File System
*
* Description: 
*
* 
=======
* GitHub Name: EdmUUUndo
* Group Name: HumanOS
* Project: Basic File System
*
* File: fsVcb.h
*
* Description: VCB structure and functions
>>>>>>> 98eac11 (VCB done)
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

//Structure to keep track of the free space 
typedef struct FSM {
    int currentBlock;
    int nextBlock;
    int size;
} FSM;
#endif

#define VCB_HEADER        "VCB"

// 
#define FREESPACESTARTOFFSET 1
#define FREESPACEUNINTIALIZEDFLAG -1
#define ROOTUNINITIALIZEDFLAG -1

// TODO: Error returns/documentation

// initializes the VCB or reads it from disk. 0 if success
int initVcb(uint64_t numberOfBlocks, uint64_t blockSize);

/*
writeVcb is responsible for ensuring that the changes made to the VCB 
during the file system initialization are persisted to dis
*/
int writeVcb();

/* Returns the total number of blocks in the file system, as stored 
in the Volume Control Block (VCB).*/
int getTotalBlockCount();

/*retrieve the total number of free blocks in the file system,the 
function likely accesses the VCB, which is a data structure containing 
metadata about the file system. In particular, the VCB keeps track of 
the total number of blocks in the file system and the count of free blocks.*/
int getFreeBlockCount();

/*
The purpose of this function is to modify the Volume Control Block (VCB) of 
the file system by updating the count of free blocks. It is typically called by
other functions that allocate or deallocate blocks,ensuring that the VCB reflects 
the correct state of free blocks in the system.
*/
void setFreeBlockCount(int newBlockCount);

/*
The purpose of this function is to provide the caller with information about the 
block size used in the file system. It is used in calculations for various file 
system operations, such as determining the size of file allocation units and managing 
storage efficiently.
*/
int getBlockSize();

/*
The purpose of this function is to provide the caller with information about the first 
free block in the file system.
*/
FSM* getFirstFree();

/*
The purpose of this function is to update the information about the first free block in 
the file system. It allows the caller to set a new value for the first free block in the VBC
Setting the first free block is a write operation that modifies the state of the file system.
*/
int setFirstFree(FSM* blockNumber);

/*The purpose of this function is to provide information about the last free block in the file 
system. It returns a pointer to an FSM structure that likely contains details about the last free block,*/
FSM* getLastFree();

/*The purpose of this function is to update the information about the last free block in the file system.
Setting the last free block is likely part of managing the file system's free space. The function is responsible 
for updating metadata to reflect changes in the state of free blocks.
*/
int setLastFree(FSM* blockNumber);

/*The purpose of this function is to set or update the information about the location of the root directory 
in the file system.Setting the root directory location is a critical step during file system initialization 
or when the root directory is moved or changed.
*/
int setRootLocation(FSM* rootDirectory);

/*The purpose of this function is to obtain information about the location of the root directory in the file system.*/
FSM* getRootLocation();

/*The purpose of the freeVCB function is to release resources associated with the Volume Control Block (VCB) and 
perform any necessary cleanup. This function is typically called when the file system is exiting or being closed.*/
int freeVcb();
#endif