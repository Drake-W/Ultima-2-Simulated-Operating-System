/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 4
|     File Name:	ufs.cpp
|  Dependencies: 	ufs.h 
|       Authors: 	Drake Wood, James Giegerich
|      Language: 	C++
|      Compiler: 	G++
|         Class: 	C435 - Operating Systems
|    Instructor: 	Dr. Hakimzadeh
|  Date Created: 	4/12/2019
|  Last Updated:	4/08/2019
|      Due Date: 	4/08/2019
|==================================================================================|
|  Description: Contains the definitions for the functions outlined in ufs.h                     
*==================================================================================*/

#include "ufs.h"

	
	//filesystem operations
void ufs::ifs(fs_name,noBlocks,blockSize,init_char) // only used by superuser (you) to create the filesystem, 
{
		
}
													// format the virtual disk (initialize with 0x5E or "^"), also wipe
													// the i nodes
void ufs::format()								// format the current file system
{
}
	
	//file operations
int ufs::Open(T_id,file_handle, filenm, mode)		//file handle must be valid and belong to t-id or file name should be
{													//set for others. mode = R or W returns a unique integer file_id or -1
}													//for an error. an error could be no such file name is created in teh system
													//or such a file is not owned by this task_id or the permission set for this
													//file do not allow it to be open.

int ufs::Close(T_id, file_id)						// close the file, returns a -1 if an error occurs
{
}
int ufs::Read_Char(T_id, file_handle, char *ch) 	// read a char from file, return -1 if EOF keep track of the current location
{
}
int ufs::Write_Char(T_id, file_handle, char ch) 	// write a  char to the file, return -1 if error. keep track of the current file location. 
{
}
	
	// optional:
	//multibyte read and  write
	
	//Directory operations:
int ufs::Create_file(T-id, filenm, file_size, permission)
{
}
int ufs::Del_file(Task_id, file_name)		// check the files ownership, delete the content of the file from datablocks
{
}
											// by writing $'s in it, then delete the file from the inode table
int ufs::Change_Permission(Task_id, file_name, new_permission)	// rwrw only the files owner can change the permission on the file
{
}
	
void ufs::Dir()								// show the directory, everyones file anme and permissions
{
}
void ufs::Dir(Task_id)					// overloaded method. only show the file belonging to T-id
{
}
void ufs::dump()							// dump the ufs inodes as well as the ufs data blocks
{
}
	