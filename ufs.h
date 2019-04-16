/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 4
|     File Name:	ufs.h
|  Dependencies: 	
|       Authors: 	Drake Wood, James Giegerich
|      Language: 	C++
|      Compiler: 	G++
|         Class: 	C435 - Operating Systems
|    Instructor: 	Dr. Hakimzadeh
|  Date Created: 	4/12/2019
|  Last Updated:	4/08/2019
|      Due Date: 	4/08/2019
|==================================================================================|
|  Description: This is the header file which defines the class ufs.
*==================================================================================*/

#ifndef UFS_H
#define UFS_H

class i_node{
	char filename[8];			//filenames can be up to 8 chars long. (including the null byte)
	int owner_task_id;			//owners task_id (this should be user-id but for now we can use task id)
	int starting_block;			// starting block where the file is written.
	int size;				//file size indiacates the actual file size not the number of clocks used by this file
	char permission[4];			//rwrw(owner, others) feel free to implement this as a bit pattern
	unsigned int blocks[4];		//array of integers (block #) belonging to this file (max file size = 4x128 bytes) 
								//you may also try to use a bitmap to represent the used blocks, for example a 16 bit 
								//representation shoing which blocks are used by  which i node
								// file 1 1111 0000 0000 0000
								// file 3 0000 1110 0000 0000
								// to figure out which blocks are still available or we can OR | the bitmaps, all the zeroes 
								// are open blocks
								// free blocks: 1111 1110 0001 0000
								// alternatively we can use a mechanism similar to our memory management (linked list)
	
	//optional
	// time_t creation_time, last modified_time;
							
	
}

class ufs{
	string fs_name; //root
	int fs_block_size; // 128
	int fs_number_of_blocks; //16
	int next_file_handle; // ufs_create_file gets the next handle
	char initialization_char;
	
	//filesystem operations
	voic ifs(fs_name,noBlocks,blockSize,init_char); // only used by superuser (you) to create the filesystem, 
													// format the virtual disk (initialize with 0x5E or "^"), also wipe
													// the i nodes
	void format();									// format the current file system
	
	//file operations
	int Open(T_id,file_handle, filenm, mode);		/* file handle must be valid and belong to t-id or file name should be
													set for others. mode = R or W returns a unique integer file_id or -1
													for an error. an error could be no such file name is created in teh system
													or such a file is not owned by this task_id or the permission set for this
													file do not allow it to be open. */
	int Close(T_id, file_id);						// close the file, returns a -1 if an error occurs
	int Read_Char(T_id, file_handle, char *ch); 	// read a char from file, return -1 if EOF keep track of the current location
	int Write_Char(T_id, file_handle, char ch); 	// write a  char to the file, return -1 if error. keep track of the current file location. 
	
	// optional:
	//multibyte read and  write
	
	//Directory operations:
	int Create_file(T-id, filenm, file_size, permission);
	int Del_file(Task_id, file_name);		// check the files ownership, delete the content of the file from datablocks
											// by writing $'s in it, then delete the file from the inode table
	int Change_Permission(Task_id, file_name, new_permission);	// rwrw only the files owner can change the permission on the file
	
	void Dir();								// show the directory, everyones file anme and permissions
	void Dir(Task_id);						// overloaded method. only show the file belonging to T-id
	void dump();							// dump the ufs inodes as well as the ufs data blocks
	
}

#endif