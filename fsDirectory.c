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

// This function initializes the blocks and marks them as not in use
struct DirectoryEntry *initDirEntryBlocks(struct DirectoryEntry *directoryArray)
{
    int arraySize = MAXENTRIES; // 100 entries

    // i starts at 2 because 0 and 1 are ".." and "." directories
    for (size_t i = 2; i < arraySize; i++)
    {
        directoryArray[i].inUse = 0;
    }
    return directoryArray;
}

struct DirectoryEntry *createDirEntry(char *name, int size, unsigned int inUse)
{
    struct DirectoryEntry *thisEntry = malloc(sizeof(struct DirectoryEntry));
    fsMgmt *entryBlockManager = getFreeBlockCount(size);
    memcpy(thisEntry, entryBlockManager, sizeof(fsMgmt));
    free(entryBlockManager);
    strncpy(thisEntry->fileName, name, FILENAME_MAX_LENGTH);
    thisEntry->creationTime = time(NULL);
    thisEntry->modificationTime = time(NULL);
    thisEntry->lastAccessTime = time(NULL);
    thisEntry->isDirectory = 0;
    thisEntry->inUse = 0;
    return thisEntry;
}

void initRootDirectoryEntry(struct DirectoryEntry *entry)
{
    strcpy(entry->fileName, "/");
    entry->location = 0;
    entry->creationTime = time(NULL);
    entry->modificationTime = time(NULL);
    entry->lastAccessTime = time(NULL);
    entry->isDirectory = 1; // This is a directory
    entry->inUse = 1;       // It is in use
}

struct Directory *createDirectory(struct DirectoryEntry *entry, struct Directory *parent)
{
    // New directories should always have the size of the parent
    struct Directory *newDir = malloc(sizeof(Directory));

    // Creating the "." and ".." directories
    newDir->Directory[0] = *entry; // Entry for "."
    newDir->Directory[1] = *entry; // Entry for ".."

    strcpy(newDir->Directory[0].fileName, ".");
    strcpy(newDir->Directory[1].fileName, "..");
    // Assuming root directory is at block 0
    newDir->Directory[0].location = 0;
    // Checks if Directory[1] has parent, if yes assigns the location of directory to be same as "parent directory."
    newDir->Directory[1].location = (parent != NULL) ? parent->Directory[0].location : 0;

    return newDir;
}
