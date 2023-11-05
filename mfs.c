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

int fs_isDir(const char pathname){
    //Ensure input is not empty
    //Traverse the path and check if each of the components exists in the directory
    //Once reach the specified path  check type of the directory 
}

