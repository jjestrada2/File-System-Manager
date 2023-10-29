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
#include <stdio.h>
#include <time.h>

#ifndef FILENAME_MAX_LENGTH
#define FILENAME_MAX_LENGTH 255
#endif

#ifndef MAXENTRIES
#define MAXENTRIES 100
#endif

// For use to manage blocks of of the fsDirectoryEntry
typedef struct fsMgmt
{
	int currentBlock;
	int nextBlock;
}fsMgmt ;

typedef struct DirectoryEntry
{
	// Properties for each directoryEntry to keep track of
	fsMgmt dirBlockInfo;
	char fileName[FILENAME_MAX_LENGTH + 1]; //+1 for null terminator
	int location;							// location of the directory entry within the file system
	time_t creationTime;
	time_t modificationTime;
	time_t lastAccessTime;
	long size;		 // Size of file on disk
	unsigned int isDirectory; // 1 is directory, 0 is NOT directory
	unsigned int inUse; // 1 is in use, 0 is NOT in use

} directoryEntry;

// A directory is an array of directory entries.
typedef struct Directory
{
	directoryEntry Directory[MAXENTRIES];

} Directory;

int initializeRootDirectory();

