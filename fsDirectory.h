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
#include <stdbool.h>

#ifndef FILENAME_MAX_LENGTH
#define FILENAME_MAX_LENGTH 255
#endif

#ifndef MAXENTRIES
#define MAXENTRIES 512 //*TO BE CHANGED* I want to define MAXENTRIES by how much free space there is on the volume.
#endif

typedef struct FSMetaData
{
	// Properties not neccessarily needed for directoryEntry to keep track of.
	char owner[30]; // owner of the directory entry
	int currentBlock;
	int nextBlock;
	

} FSMetaData;

typedef struct directoryEntry
{
	// Properties for each directoryEntry to keep track of
	char filename[FILENAME_MAX_LENGTH + 1]; //+1 for null terminator
	int location;							// location of the directory entry within the file system
	time_t creationTime;
	time_t modificationTime;
	time_t lastAccessTime;
	long size;		  // size of file on disk
	bool isDirectory; // TRUE for YES, FALSE for NO
	

} directoryEntry;

typedef struct directoryArray
{
	directoryEntry directoryArray[MAXENTRIES];

}directoryArray;

directoryEntry createEntry(char * name, int size, bool isDirectory);

int initRootDir();
