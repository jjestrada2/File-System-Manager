/**************************************************************
* Class:  CSC-415-0# Fall 2021
* Names: 
* Student IDs:
* GitHub Name:
* Group Name:
* Project: Basic File System
*
* File: fsInit.c
*
* Description: Main driver for file system assignment.
*
* This file is where you will start and initialize your system
*
**************************************************************/


#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"

#include "fsVcb.h"
#include "fsFreeSpace.h"
#include "fsDirectory.h"

int initFileSystem (uint64_t numberOfBlocks, uint64_t blockSize)
	{
	printf ("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */

	printf ("\n!-----------!Breakpoint Milestone 1 Starts Here!-----------!\n\n");
	
	
	if(initVcb()){
		printf("Error Writing the Volume Control Block");
		//error handeling
		exitFileSystem();
	}

	if(initFreeSpace(numberOfBlocks, blockSize)){
		printf("Error Writing the Free Space Management");
		//error handeling
		exitFileSystem();
	}

	if(initRootDir()){
		printf("Error Writing the Root Directory");
		//error hendeling
		exitFileSystem();
	}

	printf ("\n!-----------!Breakpoint Milestone 1 Ends Here!-----------!\n\n");
	
	

	
	return 0;
	}
	
	
void exitFileSystem ()
	{
	printf ("System exiting\n");
	}