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
#include <unistd.h>
#include <stdlib.h>			// for malloc
#include <string.h>			// for memcpy
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "b_io.h"
#include "fsDirectory.h"
#include "mfs.h"
#include "fsLow.h"


#define MAXFCBS 20
#define B_CHUNK_SIZE 512

typedef struct b_fcb
	{
	FreeSpaceManager *fileInfo;/** TODO add al the information you need in the file control block **/
	off_t fileOffset; //cuur buffer position
	char * buf;		//holds the open file buffer
	int index;		//holds the current position in the buffer
	int buflen;		//holds how many valid bytes are in the buffer
	} b_fcb;
	
b_fcb fcbArray[MAXFCBS];

int startup = 0;	//Indicates that this has not been initialized
int getBlckPosition(off_t fileOffset);
int getPositionInsideBlck(off_t fileOffset);

uint64_t wrtPartialDEntry(FreeSpaceManager *dirEntry, void *buffer, int blockOffset, int blockCount)
{
    if (blockOffset + blockCount > dirEntry->size)
    {
        return -1;
    }
    return LBAwrite(buffer, blockCount, dirEntry->currentBlock + blockOffset);
}

//Method to initialize our file system
void b_init ()
	{
	//init fcbArray to all free
	for (int i = 0; i < MAXFCBS; i++)
		{
		fcbArray[i].buf = NULL; //indicates a free fcbArray
		}
		
	startup = 1;
	}

//Method to get a free FCB element
b_io_fd b_getFCB ()
	{
	for (int i = 0; i < MAXFCBS; i++)
		{
		if (fcbArray[i].buf == NULL)
			{
			return i;		//Not thread safe (But do not worry about it for this assignment)
			}
		}
	return (-1);  //all in use
	}
	
// Interface to open a buffered file
// Modification of interface for this assignment, flags match the Linux flags for open
// O_RDONLY, O_WRONLY, or O_RDWR
b_io_fd b_open (char * filename, int flags)
	{
	b_io_fd returnFd;

	//parsepath -> directory
	//

	//*** TODO ***:  Modify to save or set any information needed
	//
	//
		
	if (startup == 0) b_init();  //Initialize our system
	
	returnFd = b_getFCB();				// get our own file descriptor
										// check for error - all used FCB's
	if(returnFd == -1){
		printf("-----FAILED-TO-OPEN-BUFFER-ALL-FILE-DESCRP-IN-USE------\n");
		return -1;
	}

	char* nameBuffer = malloc(sizeof(char)*NAMESIZE);
	Directory* parentDir = parsePath(filename,nameBuffer);
	if(parentDir==NULL){
		printf("path is invalid");
		return -1;
	}
	DirEntry* entry = seekDirectory(parentDir,nameBuffer);
	if (entry == NULL){
		if(flags & O_CREAT){
			//check if file name is already taken
			entry = createDEntry(nameBuffer,DEFAULTFILESIZE,0);
			assignDEntryToDirectory(entry,parentDir);
			char* buffer = malloc(entry->dirEntBlockInfo.size);
			memset(buffer,'\0',entry->dirEntBlockInfo.size);
			writeDEntry(&(entry->dirEntBlockInfo), buffer);
			writeDirectory(parentDir);
			free(buffer);
		}else{
			printf("-----FAILED-TO-OPEN-BUFFER-%s-IS-INVALID-----",filename);
			return -1;
		}
		
	}else{
		entry = copyDEntry(entry);
	}
	fcbArray[returnFd].fileInfo = copyDEntryFsm(entry);
	free(nameBuffer);
	freeDirectoryPtr(parentDir);
	free(entry);

	fcbArray[returnFd].buf = (char *) malloc(sizeof(char)*B_CHUNK_SIZE);
	if(fcbArray[returnFd].buf == NULL){
		printf("-----FAILED-TO-INITIALIZE-BUFFER----\n");
		return -1;
	}
	fcbArray[returnFd].index = 0;
	fcbArray[returnFd].buflen = 0;
	return (returnFd);						// all set
	}


// Interface to seek function	
int b_seek (b_io_fd fd, off_t offset, int whence)
	{
	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
		
		if(getBlckPosition(offset + whence) > fcbArray[fd].fileInfo->size){
			return -1;
		}
		fcbArray[fd].fileOffset = offset + whence;
	return (0); //Change this
	}



// Interface to write function	
int b_write (b_io_fd fd, char * buffer, int count)
	{
	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
	
	if(fcbArray[fd].buf == NULL){
		printf("-----FAILED-TO-WRITE-BUFFER-%i-FILE-DESC-IS-INVALID-----\n",fd);
		return -1;
	}

	int blocksWritten = wrtPartialDEntry(fcbArray[fd].fileInfo, buffer, getBlckPosition(fcbArray[fd].fileOffset),count);

		
	return blocksWritten; 

	}



// Interface to read a buffer

// Filling the callers request is broken into three parts
// Part 1 is what can be filled from the current buffer, which may or may not be enough
// Part 2 is after using what was left in our buffer there is still 1 or more block
//        size chunks needed to fill the callers request.  This represents the number of
//        bytes in multiples of the blocksize.
// Part 3 is a value less than blocksize which is what remains to copy to the callers buffer
//        after fulfilling part 1 and part 2.  This would always be filled from a refill 
//        of our buffer.
//  +-------------+------------------------------------------------+--------+
//  |             |                                                |        |
//  | filled from |  filled direct in multiples of the block size  | filled |
//  | existing    |                                                | from   |
//  | buffer      |                                                |refilled|
//  |             |                                                | buffer |
//  |             |                                                |        |
//  | Part1       |  Part 2                                        | Part3  |
//  +-------------+------------------------------------------------+--------+
int b_read (b_io_fd fd, char * buffer, int count)
	{

	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
		
	if(fcbArray[fd].buf == NULL){
		printf("-----FAILED-TO-READ-BUFFER-%i-FILE-DESC-IS-INVALID-----\n",fd);
		return -1;
	}
	int bytesReturned = 0;
	int fileRemaining = fcbArray[fd].fileInfo->size - fcbArray[fd].fileOffset;
	int initialBytes = (fileRemaining > count) ? count : fileRemaining;
	if(initialBytes >= fcbArray[fd].buflen && fcbArray[fd].buflen!= 0){
		memcpy(buffer, fcbArray[fd].buf + fcbArray[fd].index,fcbArray[fd].buflen);
		bytesReturned = fcbArray[fd].buflen;
		fcbArray[fd].fileOffset += bytesReturned;
		fcbArray[fd].index = B_CHUNK_SIZE;
		fcbArray[fd].buflen = 0;
		fileRemaining = fcbArray[fd].fileInfo->size - fcbArray[fd].fileOffset;
	}
	int blockOffset = getBlckPosition(fcbArray[fd].fileOffset);
	int remainingBytes = count -bytesReturned;
	int remainingFullBlocks = ((fileRemaining > remainingBytes) ? remainingBytes : fileRemaining)/B_CHUNK_SIZE;
	
	//second step
	if(remainingFullBlocks){
		readPartialDirEntry(fcbArray[fd].fileInfo, blockOffset, remainingFullBlocks, buffer, bytesReturned);
		bytesReturned += B_CHUNK_SIZE * remainingFullBlocks;
		fcbArray[fd].fileOffset += B_CHUNK_SIZE * remainingFullBlocks;

		remainingBytes = count - bytesReturned;
		fileRemaining =fcbArray[fd].fileInfo->size - fcbArray[fd].fileOffset;
		blockOffset += remainingFullBlocks;
	}

	//Third Step
	if(remainingBytes && fileRemaining){
		if(fcbArray[fd].index == B_CHUNK_SIZE){
			readPartialDirEntry(fcbArray[fd].fileInfo,blockOffset,1,fcbArray[fd].buf,0);
			fcbArray[fd].index = 0;
			fcbArray[fd].buflen = B_CHUNK_SIZE;
		}

		int lastBytes = (fileRemaining > remainingBytes) ? remainingBytes : fileRemaining;
		memcpy(buffer + bytesReturned, fcbArray[fd].buf + fcbArray[fd].index, lastBytes);

		bytesReturned += lastBytes;
		fcbArray[fd].fileOffset +=lastBytes;
		fcbArray[fd].index += lastBytes;
		fcbArray[fd].buflen = fcbArray[fd].buflen - lastBytes;
	}

	return bytesReturned;	
	}
	
// Interface to Close the file	
int b_close (b_io_fd fd)
	{
		if(fcbArray[fd].fileInfo != NULL){
			free(fcbArray[fd].fileInfo);
			fcbArray[fd].fileInfo = NULL;
		}

		if(fcbArray[fd].buf != NULL){
			free(fcbArray[fd].buf);
			fcbArray[fd].buf = NULL;
		}

	}

int getBlckPosition(off_t fileOffset){
	return fileOffset / getSizeofBlocks();
}

int getPositionInsideBlck(off_t fileOffset){
	return fileOffset % getSizeofBlocks();
}

