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

	if(initFreeSpace()){
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