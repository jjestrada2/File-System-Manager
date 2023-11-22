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
 * File: b_io.c
 *
 * Description: Basic File System - Key File I/O Operations
 **************************************************************/
#ifndef _DIR_H
#define _DIR_H

#include <time.h>

#ifndef uint64_t
typedef u_int64_t uint64_t;
#endif
#ifndef uint32_t
typedef u_int32_t uint32_t;
#endif

#ifndef FSMDef
#define FSMDef
typedef struct FreeSpaceManager
{
    int currentBlock;
    int nextBlock;
    int size;
} FreeSpaceManager;
#endif

#ifndef DIRMAX_LEN
#define DIRMAX_LEN 4096
#endif

#ifndef DEFAULTFILESIZE
#define DEFAULTFILESIZE 4096
#endif

#define MAXDIRENTRIES 50
#define NAMESIZE 32

typedef struct DirEntry
{
    FreeSpaceManager dirEntBlockInfo; // this struct contains block info.
    char fileName[NAMESIZE];          // directory entry name
    time_t creationTime;
    time_t modificationTime;
    time_t lastAccessTime;
    unsigned char isDirectory; // 1 is directory, 0 is other file;
    unsigned char isUse;       // 1 is free , 0 is taken;
} DirEntry;

typedef struct Directory
{
    DirEntry dirArray[MAXDIRENTRIES];
    int isOpen; // An isOpen flag (0 for closed, 1 for open)
} Directory;

// pointer to get information of the directory
DirEntry *getDEntryInfo(char *dirName);

int unassignDEntry(DirEntry *dirEnt);

int initRootDirectory();

int getDirectoryNum();

DirEntry *createDEntry(char *name, int size, unsigned char isDir);

Directory *createDirectory(DirEntry *dirEnt, Directory *parent);

int assignDEntryToDirectory(DirEntry *dirEnt, Directory *parent);

int deleteDirectory(Directory *directory);

int deleteDEntry(DirEntry *dirEnt);

DirEntry *copyDEntry(DirEntry *dirEnt);

FreeSpaceManager *copyDEntryFsm(DirEntry *entry);

FreeSpaceManager *getFreeSpageManager(Directory *directory);

FreeSpaceManager *getParentFreeSpaceManager(Directory *directory);

uint64_t writeDirectory(Directory *directory);

uint64_t writeDEntry(FreeSpaceManager *dirEntry, void *buffer);


uint64_t readPartialDirEntry(FreeSpaceManager *dirEntry, int blockOffset, int blockCount, void *buffer, int bufferIndex);

void *readDEntry(DirEntry *dirEntry);

DirEntry *seekDirectory(Directory *directory, char *name);

DirEntry *seekDirectoryByBlock(Directory *directory, FreeSpaceManager *blockInfo);

int freeDirectoryPtr(Directory *dir);

int freeDirs();

Directory *getRootDirectory();

Directory *getCWD();

int setCWD(Directory *directory);

char *getCWDPath();

int setCWDPath(char *newpath);

int isDirectoryEmpty(Directory *directory);

int getSizeofBlocks();

FreeSpaceManager *copyFreeSpaceManager(FreeSpaceManager *oldFSM);

#endif