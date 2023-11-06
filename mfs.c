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

int fs_mkdir(const char *pathname, mode_t mode){
    //Ensure input parameter are valid, name is not empty and follows conventions


    //Find free blocks getfreeblocks()
    //Creat Directory Entry ()
    //Update parent directory
    //writing to second storage LBAwrite
    


}

int fs_isDir(const char *pathname){
   //Initialize the return value
   int isDirectory = 0;

    //Get directory entry
    DirEntry * entry = get


}

int fs_isFile(const char *pathname){
    //Ensure input is not empty and follows format 
    //traverse path to check components exists
    //Once reach directory check tipe 
    /
}


Directory *parsePath(const *path, char *nameBuffer){
    
}


/**
 * Parses the given path and returns the corresponding Directory.
 * Also populates the provided nameBuffer with the last component of the path.
 */
Directory *parsePath(const char *path, char *nameBuffer) {
    // Reset nameBuffer to NULL in case it's not empty
    memset(nameBuffer, '\0', sizeof(char) * NAMESIZE);

    // Tokenize the path using '/' and ',' as delimiters
    char **arguments = malloc(strlen(path) * sizeof(char *));
    int counter = 0;

    // Allocate memory for a temporary copy of the path
    char *tempPath = malloc((strlen(path) + 1) * sizeof(char));
    strncpy(tempPath, path, strlen(path) + 1);

    // If path[0] is '/', it's an absolute path
    if (strncmp(tempPath, "/", 1) == 0) {
        arguments[counter] = "/";
        counter++;
    }

    // Tokenize the path and store each component in arguments array
    char *token = strtok(tempPath, "/ ,");
    while (token != NULL) {
        arguments[counter] = token;
        counter++;
        token = strtok(NULL, "/ ,");
    }

    counter--;

    // Create Directory pointer
    Directory *tempDir;

    // Check if the first argument is "/"
    if (strncmp(arguments[0], "/", 1) == 0) {
        // If it is, tempDir points to the Root directory
        tempDir = getRootDirectory();
    } else {
        // Otherwise, tempDir points to the current working directory (cwd)
        tempDir = getCWD();
    }

    DirEntry *tempEntry;
    Directory *oldDir;

    // Traverse through the DirEntry array to find the last component of the path
    for (int i = 1; i < counter; i++) {
        tempEntry = searchDirectory(tempDir, arguments[i]);
        if (tempEntry != NULL && tempEntry->isDir == 1) {
            // Update tempDir to the found subdirectory
            oldDir = tempDir;
            tempDir = (Directory *)readDirEntry(tempEntry);
            freeDirectoryPtr(oldDir);
        } else {
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
