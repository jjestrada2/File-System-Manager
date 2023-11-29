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
 * Description: Directory initializer.
 **************************************************************/
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "fsVcb.h"
#include "fsFreeSpace.h"
#include "fsDirectory.h"




Directory *rootDirectory;
Directory *cwd;
char cwdPath[DIRMAX_LEN];



DirEntry *initDirEntryArrayBlock(DirEntry *dirEntryArray)
{
    
    int arraySize = MAXDIRENTRIES;
    for (size_t i = 2; i < arraySize; i++)
    {
        dirEntryArray[i].isUse = 1;
    }
    return dirEntryArray;
}

DirEntry *createDEntry(char *name, int size, unsigned char isDir)
{
    
    DirEntry *retDirEnt = malloc(sizeof(DirEntry));
    FreeSpaceManager *dirEntFSM = getAvailableBytes(size);
    memcpy(retDirEnt, dirEntFSM, sizeof(FreeSpaceManager));
    free(dirEntFSM);
    strncpy(retDirEnt->fileName, name, NAMESIZE);
    retDirEnt->creationTime = time(NULL);
    retDirEnt->lastAccessTime = time(NULL);
    retDirEnt->modificationTime = time(NULL);
    retDirEnt->isDirectory = isDir;
    retDirEnt->isUse = 0;
   
    return retDirEnt;
}

Directory *createDirectory(DirEntry *dirEnt, Directory *parent)
{
    
    Directory *newDir = malloc(sizeof(Directory));
    memcpy(&(newDir->dirArray[0]), dirEnt, sizeof(DirEntry));
    memcpy(&(newDir->dirArray[1]), parent, sizeof(DirEntry));
    strcpy(newDir->dirArray[0].fileName, ".");
    strcpy(newDir->dirArray[1].fileName, "..");
    initDirEntryArrayBlock(newDir->dirArray);
   

    return newDir;
}

int assignDEntryToDirectory(DirEntry *dirEnt, Directory *parent)
{
    for (int i = 2; i < MAXDIRENTRIES; i++)
    {
        if (parent->dirArray[i].isUse == 1)
        {
            memcpy(&(parent->dirArray[i]), dirEnt, sizeof(DirEntry));
            return 0;
        }
    }
    printf("Failed to add Directory Entry\n");
    return 1;
}

int unassignDEntry(DirEntry *dirEnt)
{
    dirEnt->isUse = 1;
    return 0;
}

int deleteDirectory(Directory *directory)
{
    if (directory == getCWD())
    {
        cwd = rootDirectory;
        strncpy(cwdPath, "/", 2);
    }
    fsFree((FreeSpaceManager *)directory, (FreeSpaceManager *)directory, directory->dirArray[0].dirEntBlockInfo.size);////////////////////**********
    return 0;
}

int deleteDEntry(DirEntry *dirEnt)
{
    fsFree((FreeSpaceManager *)dirEnt, (FreeSpaceManager *)dirEnt, dirEnt->dirEntBlockInfo.size); /*////////////////////*/
    return 0;
}

DirEntry *copyDEntry(DirEntry *dirEnt)
{
    DirEntry *returnDirEnt = malloc(sizeof(DirEntry));
    memcpy(returnDirEnt, dirEnt, sizeof(DirEntry));
    return returnDirEnt;
}

FreeSpaceManager *copyDEntryFsm(DirEntry *entry)
{
    return copyFreeSpaceManager(&(entry->dirEntBlockInfo)); ////////////////
}

FreeSpaceManager *getFreeSpageManager(Directory *directory)
{
    return &(directory->dirArray[0].dirEntBlockInfo);
}

FreeSpaceManager *getParentFreeSpaceManager(Directory *directory)
{
    return &(directory->dirArray[1].dirEntBlockInfo);
}

DirEntry *getParent(Directory *directory)
{
    return &(directory->dirArray[1]);
}

DirEntry *getSelf(Directory *directory)
{
    return &(directory->dirArray[0]);
}

int initRootDirectory()
{
    FreeSpaceManager *possibleRoot = getRootLocation();
    if (possibleRoot->currentBlock == ROOTUNINITIALIZEDFLAG)
    {

        DirEntry *rootDirEnt = createDEntry("root", sizeof(Directory), 1);
        Directory *rootDirect = createDirectory(rootDirEnt, (Directory *)rootDirEnt);
        free(rootDirEnt);
        setRootLocation((FreeSpaceManager *)rootDirect);
        writeDirectory(rootDirect);
        rootDirectory = rootDirect;
        setCWD(rootDirect);

       
    }
    else
    {
        Directory *rootDirect = malloc(getBlockSize() * possibleRoot->size);
        LBAread(rootDirect, possibleRoot->size, possibleRoot->currentBlock);
        rootDirectory = rootDirect;
        setCWD(rootDirect);
    }
    setCWDPath("/");

    return 0;
}

uint64_t writeDirectory(Directory *directory)
{
    return LBAwrite(directory, directory->dirArray[0].dirEntBlockInfo.size, directory->dirArray[0].dirEntBlockInfo.currentBlock);
}

uint64_t writeDEntry(FreeSpaceManager *dirEntry, void *buffer)
{
    return LBAwrite(buffer, dirEntry->size, dirEntry->currentBlock);
}



uint64_t readPartialDirEntry(FreeSpaceManager *dirEntry, int blockOffset, int blockCount, void *buffer, int bufferIndex)
{
    if (blockOffset + blockCount > dirEntry->size)
    {
        return -1;
    }
    return LBAread(buffer + bufferIndex, blockCount, dirEntry->currentBlock + blockOffset);
}

void *readDEntry(DirEntry *dirEntry)
{

    void *buffer = malloc(dirEntry->dirEntBlockInfo.size * getBlockSize());
    LBAread(buffer, dirEntry->dirEntBlockInfo.size, dirEntry->dirEntBlockInfo.currentBlock);

    return buffer;
}

DirEntry *seekDirectory(Directory *directory, char *name)
{

    int arraySize = MAXDIRENTRIES;
    for (int i = 0; i < MAXDIRENTRIES; i++)
    {
        if (directory->dirArray[i].isUse == 0 && strncmp(directory->dirArray[i].fileName, name, NAMESIZE) == 0)
        {

            return &(directory->dirArray[i]);
        }
    }

    return NULL;
}

DirEntry *seekDirectoryByBlock(Directory *directory, FreeSpaceManager *blockInfo)
{

    int arraySize = MAXDIRENTRIES;
    for (int i = 0; i < MAXDIRENTRIES; i++)
    {
        if (directory->dirArray[i].isUse == 0 && directory->dirArray[i].dirEntBlockInfo.currentBlock == blockInfo->currentBlock)
        {

            return &(directory->dirArray[i]);
        }
    }

    return NULL;
}

int freeDirectoryPtr(Directory *dir)
{
    if (dir != NULL && dir != getRootDirectory() && dir != getCWD())
    {
        free(dir);
    }
    return 0;
}

int freeDirs()
{
    if (rootDirectory != NULL)
    {
        if (cwd == rootDirectory)
        {
            cwd = NULL;
        }
        free(rootDirectory);
        rootDirectory = NULL;
    }
    if (cwd != NULL)
    {
        free(cwd);
        cwd = NULL;
    }
    return 0;
}

Directory *getRootDirectory()
{
    return rootDirectory;
}

Directory *getCWD()
{
    return cwd;
}

int setCWD(Directory *directory)
{
    Directory *temp = cwd;
    cwd = directory;
    freeDirectoryPtr(temp);
    return 0;
}

char *getCWDPath()
{
    return cwdPath;
}

int setCWDPath(char *newpath)
{
    strncpy(cwdPath, newpath, DIRMAX_LEN);
    return 0;
}

int isDirectoryEmpty(Directory *directory)
{
    for (int i = 2; i < MAXDIRENTRIES; i++)
    {
        if (directory->dirArray[i].isUse == 0)
        {
            return 0;
        }
    }
    return 1;
}

int getDirectoryNum()
{
    return getBlockSize() / sizeof(DirEntry); //returns the number of directory entries per directory per block.
}

Directory *lbaToDirectory(int blockNumber, int size)
{
    Directory *directory = malloc(size * getBlockSize());
    LBAread(directory, size, blockNumber);
    return directory;
}

int getSizeofBlocks()
{
    return getBlockSize();
}