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
 * File: mfs.c
 *
 * Description: functions to handle shell commands
 **************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mfs.h"
#include "fsDirectory.h"
/*
struct fs_diriteminfo *fs_readdir(fdDir *dir)
{
    if (dir == NULL) {
        return NULL;
    }
    DirEntry *entry = getEntryFromPath(dir->di->d_name);
    if (getEntryFromPath(dir->di->d_name) == NULL) {
        printf("Directory does not exist: %s\n", dir->di->d_name);
        return NULL;
    }
    free(entry);
    if (dir->dirEntryPosition < dir->d_reclen)
    {
        return dir->dirItemArray[dir->dirEntryPosition++];
    }
    return NULL;
}
*/

/*
int fs_stat(const char *pathname, struct fs_stat *stat)
{
    DirEntry *entry = getEntryFromPath(pathname);

    if (entry == NULL)
    {
        printf("Directory does not exist: %s\n", pathname);
        return 1;
    }

    stat->st_size = entry->dirEntBlockInfo.size * getSizeofBlocks();
    stat->st_blksize = entry->dirEntBlockInfo.size;
    stat->st_blocks = (stat->st_size + 511) / 512;
    stat->st_accesstime = entry->dateLastAccessed;
    stat->st_modtime = entry->datelastModified;
    stat->st_createtime = entry->dateCreated;

    free(entry);
    return 0;
}
*/

/*
int fs_rmdir(const char *pathname)
{
    char *nameBuffer = malloc(sizeof(char) * NAMESIZE);
    Directory *folder = parsePath(pathname, nameBuffer);
    if (folder == NULL)
    {
        printf("Path is invalid\n");
        free(nameBuffer);
        return -1;
    }
    DirEntry *entries = searchDirectory(folder, nameBuffer);
    free(nameBuffer);

    if (entries == NULL || entries->isDir == 0)
    {
        printf("A File with that name does not exist\n");
        freeDirectoryPtr(folder);
        return -1;
    }
    Directory *oldDirectory = readDirEntry(entries);
    if (isDirectoryEmpty(oldDirectory) == 0)
    {
        printf("Directory is not empty. Cannot delete\n");
        return -1;
    }

    deleteDirectory(oldDirectory);
    unassignDEntry(entries);
    writeDirectory(folder);

    free(oldDirectory);
    freeDirectoryPtr(folder);
    return 0;
}
*/
/**
 * Parses the given path and returns the corresponding Directory.
 * Also populates the provided nameBuffer with the last component of the path.
 */
Directory *parsePath(const char *path, char *nameBuffer)
{
    // Reset nameBuffer to NULL in case it's not empty
    memset(nameBuffer, '\0', sizeof(char) * NAMESIZE);

    // Tokenize the path using '/' and ',' as delimiters
    char **arguments = malloc(strlen(path) * sizeof(char *));
    int counter = 0;

    // Allocate memory for a temporary copy of the path
    char *tempPath = malloc((strlen(path) + 1) * sizeof(char));
    strncpy(tempPath, path, strlen(path) + 1);

    // If path[0] is '/', it's an absolute path
    if (strncmp(tempPath, "/", 1) == 0)
    {
        arguments[counter] = "/";
        counter++;
    }

    // Tokenize the path and store each component in arguments array
    char *token = strtok(tempPath, "/ ,");
    while (token != NULL)
    {
        arguments[counter] = token;
        counter++;
        token = strtok(NULL, "/ ,");
    }

    counter--;

    // Create Directory pointer
    Directory *tempDir;

    // Check if the first argument is "/"
    if (strncmp(arguments[0], "/", 1) == 0)
    {
        // If it is, tempDir points to the Root directory
        tempDir = getRootDirectory();
    }
    else
    {
        // Otherwise, tempDir points to the current working directory (cwd)
        tempDir = getCWD();
    }

    DirEntry *tempEntry;
    Directory *oldDir;

    // Traverse through the DirEntry array to find the last component of the path
    for (int i = 1; i < counter; i++)
    {
        tempEntry = searchDirectory(tempDir, arguments[i]);
        if (tempEntry != NULL && tempEntry->isDirectory == 1)
        {
            // Update tempDir to the found subdirectory
            oldDir = tempDir;
            tempDir = (Directory *)readDEntry(tempEntry);
            freeDirectoryPtr(oldDir);
        }
        else
        {
            // Clean up and return NULL if a component is not found or is not a directory
            freeDirectoryPtr(tempDir);
            free(arguments);
            return NULL;
        }
    }

    // Copy the last component to nameBuffer
    strncpy(nameBuffer, arguments[counter], strlen(arguments[counter]));

    // Clean up and return the final Directory pointer
    free(arguments);
    return tempDir;
}

/**
 * Retrieves the DirEntry associated with the given path.
 */
DirEntry *getEntryFromPath(const char *path)
{
    // Buffer to store the last component of the path
    char *nameBuffer = malloc(sizeof(char) * NAMESIZE);

    // Get the parent Directory using the parsePath function
    Directory *parent = parsePath(path, nameBuffer);

    // If parsePath fails, free resources and return NULL
    if (parent == NULL)
    {
        free(nameBuffer);
        return NULL;
    }

    // Search for the DirEntry in the parent Directory
    DirEntry *retObject = searchDirectory(parent, nameBuffer);

    // Free the nameBuffer as it's no longer needed
    free(nameBuffer);

    // If the DirEntry is not found, free resources and return NULL
    if (retObject == NULL)
    {
        freeDirectoryPtr(parent);
        return NULL;
    }

    // Copy the DirEntry to a new object
    retObject = copyDEntry(retObject);

    // Free the parent Directory as it's no longer needed
    freeDirectoryPtr(parent);

    // Return the DirEntry pointer
    return retObject;
}
/*
// For: fs_delete()
int removeEntryFromDirectory(Directory *dir, DirEntry *entryToRemove)
{
    if (dir == NULL || entryToRemove == NULL)
    {
        return -1; // Invalid arguments.
    }
    // Traverse the directory entries to find and remove the entry
    for (int i = 0; i < dir->dirArray; i++)
    {
        if (dir->entries[i] == entryToRemove)
        {
            // Shift the remaining entries to fill the gap
            for (int j = i; j < dir->numEntries - 1; j++)
            {
                dir->entries[j] = dir->entries[j + 1];
            }
            dir->numEntries--;
            // Optionally, update the directory's metadata such as size.
            return 0; // Entry removed successfully.
        }
    }
    return -1; // Entry not found in the directory.
} */

int fs_mkdir(const char *pathname, mode_t mode)
{
    int retValue =0; 
    char *nameBuffer = malloc(sizeof(char)*NAMESIZE);
    Directory *parentDir = parsePath(pathname,nameBuffer);
    if(parentDir == NULL){
        printf("Path is not valid\n try again!");
        retValue =-1;
    }else if(searchDirectory(parentDir,nameBuffer)!= NULL){
        printf("That name is in use, change it please!");
        retValue =-1;
    }
    else{
        DirEntry *newEntry = createDEntry(nameBuffer,sizeof(Directory), 1);
        retValue = assignDEntryToDirectory(newEntry,parentDir);
        Directory *newDir = createDirectory(newEntry,parentDir);
        writeDirectory(newDir);
        writeDirectory(parentDir);
        free(newEntry);
    }
    free(nameBuffer);
    freeDirectoryPtr(parentDir);
    return retValue;
}

int fs_isDir(char *pathname)
{
    // Initialize the return value to indicate it's not a directory
    int isDirectory = 0;

    // Get the directory entry from the given path
    DirEntry *entry = getEntryFromPath(pathname);

    // Check if the entry exists and is a directory
    if (entry != NULL && entry->isDirectory == 1)
    {
        // Set the return value to indicate it's a directory
        isDirectory = 1;
    }

    // Free the allocated memory for the directory entry
    free(entry);

    // Return the result
    return isDirectory;
}

int fs_isFile(char *path)
{
    // Initialize the return value to 0 (not a file)
    int retValue = 0;

    // Get the DirEntry associated with the path
    DirEntry *entry = getEntryFromPath(path);

    // Check if the DirEntry is not NULL and represents a file (not a directory)
    if (entry != NULL && entry->isDirectory == 0)
    {
        retValue = 1; // Set return value to 1 (file found)
    }

    // Free the DirEntry as it's no longer needed
    free(entry);

    // Return the determined value (1 if file, 0 otherwise)
    return retValue;
}
/*
Directory *fs_opendir(const char *pathname)
{
    // Check if the given pathname is a directory
    if (!fs_isDir(pathname))
    {
        return NULL; // The specified path is not a directory.
    }

    // Buffer to store the last component of the path
    char nameBuffer[NAMESIZE];

    // Get the parent Directory using the parsePath function
    Directory *parent = parsePath(pathname, nameBuffer);

    // If parsePath fails or the parent is not a directory, return NULL
    if (parent == NULL)
    {
        return NULL;
    }
}
*/

int fs_closedir(fdDir *dirp)
{
    // Check if dirp is NULL
    if (dirp == NULL)
    {
        return -1; // Invalid argument.
    }
    // Free the dirp structure itself.
    free(dirp);
    return 0; // Success.
}
/*
int fs_delete(char *filename)
{
    // Check if the file exists
    if (!fs_isFile(filename))
    {
        return -1; // The specified file does not exist.
    }
    // Buffer to store the last component of the path
    char nameBuffer[NAMESIZE];
    // Get the parent Directory using the parsePath function
    Directory *parent = parsePath(filename, nameBuffer);
    // If parsePath fails or the parent is not a directory, return an error
    if (parent == NULL)
    {
        return -1;
    }
    // Search for the DirEntry in the parent Directory
    DirEntry *fileEntry = searchDirectory(parent, nameBuffer);
    // Check if the DirEntry is not found or is a directory, return an error
    if (fileEntry == NULL || fileEntry->isDirectory == 1)
    {
        freeDirectoryPtr(parent);
        return -1; // Cannot delete a directory using fs_delete.
    }
    // Remove the file entry from the parent directory
    if (removeEntryFromDirectory(parent, fileEntry) != 0)
    {
        freeDirectoryPtr(parent);
        return -1; // Failed to remove the entry.
    }
    // Free the file entry and any associated resources
    freeDEntry(fileEntry);
    // Free the parent directory as it's no longer needed
    freeDirectoryPtr(parent);
    return 0; // Success.

    // Search for the DirEntry in the parent Directory
    DirEntry *dirEntry = searchDirectory(parent, nameBuffer);

    // Free the parent Directory as it's no longer needed
    freeDirectoryPtr(parent);

    // Check if the DirEntry is not found or is not a directory
    if (dirEntry == NULL || dirEntry->isDirectory != 1)
    {
        return NULL; // The specified directory does not exist.
    }

    // Open the specified directory and return it
    Directory *openedDir = (Directory *)readDirEntry(dirEntry);

    // Return the opened directory
    return openedDir;
}*/

char *fs_getcwd(char *pathname, size_t size)
{
    char *cwd = getcwd(pathname, size);
    return cwd;
}



int fs_setcwd(char *pathname)
{
    // Parse the path and get the parent directory
    char nameBuffer[NAMESIZE];
    Directory *parent = parsePath(pathname, nameBuffer);

    // Check if parsing the path failed
    if (parent == NULL)
    {
        printf("Invalid path\n");
        return -1;
    }

    // Find the directory entry in the parent directory
    DirEntry *dirEntry = searchDirectory(parent, nameBuffer);

    // Check if the directory entry doesn't exist
    if (dirEntry == NULL)
    {
        // Generate an absolute path
        char absolutePath[32];
        snprintf(absolutePath, sizeof(absolutePath), "%s/%s", getCWDPath(), pathname);

        // Try to parse the absolute path
        Directory *absPathDir = parsePath(absolutePath, nameBuffer);

        // Check if parsing the absolute path failed
        if (absPathDir == NULL)
        {
            printf("Directory does not exist: %s\n", pathname);
            freeDirectoryPtr(parent);
            return -1;
        }

        // Set the new current working directory
        int result = setCWD(absPathDir);

        // Free allocated resources
        freeDirectoryPtr(parent);
        freeDirectoryPtr(absPathDir);

        return result;
    }

    // Set the new current working directory
    int result = setCWD(readDEntry(dirEntry));

    // Free allocated resources
    freeDirectoryPtr(parent);

    return result;
}

