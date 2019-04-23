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
#include "semaphore.h"

extern semaphore sema_filef;
extern semaphore sema_inodef;

//------------------------------------------------------------------------------
//Filesystem operations
//------------------------------------------------------------------------------

ufs::ufs(char* name, int noBlocks, int blockSize, char init_char) // only used by superuser (you) to create the filesystem, 
{																		// format the virtual disk (initialize with 0x5E or "^"), also wipe
	sema_filef.down();
	fstream filef("filef.txt", ios::out);
	filef.seekp(0);
	int size = blockSize*noBlocks;
	for (int i = 0; i < size; i++)
		filef << init_char;
	filef.close();
	sema_filef.up();
	
	sema_inodef.down();
	fstream inodef("inodef.txt", ios::out);
	inodef.close();
	sema_inodef.up();
	
	fs_name = name;										
	fs_number_of_blocks = noBlocks;
	fs_block_size = blockSize;
	initialization_char = init_char;
}
													
void ufs::format()									// format the current file system
{
	sema_filef.down();
	fstream filef("filef.txt", ios::out);
	filef.seekp(0);
	int size = fs_block_size*fs_number_of_blocks;
	for (int i = 0; i < size; i++)
		filef << initialization_char;
	filef.close();
	sema_filef.up();
	
	sema_inodef.down();
	fstream inodef("inodef.txt", ios::out);
	inodef.close();
	sema_inodef.up();
}

//------------------------------------------------------------------------------
//File operations
//------------------------------------------------------------------------------

int ufs::Open(int T_id, int callingtask, char* filenm, char* mode, WINDOW * Win)		//file handle must be valid and belong to t-id or file name should be
{	
													//set for others. mode = R or W returns a unique integer file_id or -1
													//for an error. an error could be no such file name is created in teh system
													//or such a file is not owned by this task_id or the permission set for this
													//file do not allow it to be open.
	bool owner = FALSE;
	if(T_id == callingtask){
		owner = TRUE;
	}else{
		owner = FALSE;
	}
	i_node temp;								
	for(int i = 0; i < 16; i++){
		temp = Read_inode(i);
		if((temp.owner_task_id == T_id) && (strcmp (temp.filename,filenm) == 0)){
			if(owner){
			if((temp.permission[0]) && (*mode == 'r')){
					return 1;
				}
				if((temp.permission[1]) && (*mode == 'w')){
					return 1;
				}
			}
			if(!owner){
				if((temp.permission[2]) && (*mode == 'r')){
					return 1;
				}
				if((temp.permission[3]) && (*mode == 'w')){
					return 1;
				}
			}
			return -1;
		}
	}
}

int ufs::Close(int T_id, int file_id)						// close the file, returns a -1 if an error occurs
{
	return 1;
}
int ufs::Read_Char(int T_id, char* file_name, char *ch) 	// read a char from file, return -1 if EOF keep track of the current location
{	
	sema_filef.down();
	i_node temp;								
	for(int i = 0; i < 16; i++){
		temp = Read_inode(i);
		if((temp.owner_task_id == T_id) && (strcmp (temp.filename,file_name) == 0)){
			fstream filef("filef.txt", ios::in | ios::out);
			filef.seekp(temp.starting_block*128);
			filef.get(*ch);
			sema_filef.up();
			return 1;
		}
			
	}
	sema_filef.up();
    return -1;
}
int ufs::Write_Char(int T_id, char* file_name, char* ch) 	// write a  char to the file, return -1 if error. keep track of the current file location. 
{
	sema_filef.down();
	i_node temp;								
	for(int i = 0; i < 16; i++){
		temp = Read_inode(i);
		if((temp.owner_task_id == T_id) && (strcmp (temp.filename,file_name) == 0)){
			fstream filef("filef.txt", ios::in | ios::out);
			filef.seekp(temp.starting_block*128);
			filef.put(*ch);
			sema_filef.up();
			return 1;
		}
			
	}
	sema_filef.up();
    return -1;
}

//------------------------------------------------------------------------------
//Directory operations
//------------------------------------------------------------------------------

int ufs::Create_file(int T_id, char* filenm, int file_size, int permission[4], WINDOW * Win)
{	
	//load up a new node
	int count = 0;
	int j=0;
	i_node * temp = new i_node;
	
	sprintf(temp->filename, "%s", filenm);
	temp->owner_task_id = T_id;
	
	temp->size = file_size;
	
	for(int i = 0; i < 4; i++){
		temp->permission[i] = permission[i];
	}
	
	int reqblocks = file_size/ufs::fs_block_size;
	if ( file_size%ufs::fs_block_size > 0){
		reqblocks++;
	}
	
	for(int i = 0; i <16; i++){ // hope this works
		count = 0;
		while(count < reqblocks){
			if(fsystem[i+j] == 0){
				count ++;
				j++;
			}else{
				if(count >= reqblocks){
					temp->starting_block = i;
					i=16;
					break;
				}
				count =20;
				j=0;
			}
		}
	}
	
	temp->blocks[0] = temp->starting_block;
	for (int i = 1; i < reqblocks; i++){
		temp->blocks[i] = temp->starting_block + i;
	}
	
	temp->currentlocation = temp->starting_block * 128;
	Write_Node(T_id, temp);	
	
}
int ufs::Del_file(int Task_id, char* file_name)		// check the files ownership, delete the content of the file from datablocks
{			
									// by writing $'s in it, then delete the file from the inode table
	sema_filef.down();
	i_node temp;	
		char x = '$';
	for(int i = 0; i < 16; i++){
		temp = Read_inode(i);
		if((temp.owner_task_id == Task_id) && (strcmp (temp.filename,file_name) == 0)){
			// delete block file stuff here 	 write $$'s
			fstream filef("filef.txt", ios::in | ios::out);
			filef.seekp(temp.starting_block  * 128);
			for (int q = 0; q < temp.size; q++)
				filef.put(x);
			filef.close();
			
			
			sprintf(temp.filename, "");
			temp.owner_task_id = 0;
			temp.size = 0;
			temp.starting_block = 0;
			for(int j = 0; j < 4; j++){
				temp.permission[j] = 0;
				temp.blocks[j] = 0;
			}
			
			//
			// fsystem delete node [16] blocks
			//
			Write_Node(i, &temp);
			sema_filef.up();
			return 1;
		}
	}
	sema_filef.up();
    return -1;
}
											
int ufs::Change_Permission(int Task_id, char* file_name, int new_permission[4], WINDOW * win)	// rwrw only the files owner can change the permission on the file
{
	i_node temp;								
	for(int i = 0; i < 16; i++){
		temp = Read_inode(i);
		if((temp.owner_task_id == Task_id) && (strcmp (temp.filename,file_name) == 0)){
			// delete block file stuff here 	 write $$'s
		
			for(int j = 0; j < 4; j++){
				temp.permission[j] = new_permission[j];
			}
			Write_Node(i, &temp);
			return 1;
		}
	}
    return -1;
}
	
void ufs::Dir( WINDOW * win)								// show the directory, everyones file anme and permissions
{ 
	char buff[256];
	sprintf(buff, " file name\towner id\tpermissions rwrw\n");
	write_window(win, buff);
	i_node temp;
	for(int i = 0; i < 16; i++){
		temp = Read_inode(i);
		if (temp.owner_task_id > 0){
			sprintf(buff, " %s %d %d %d %d %d %d %d %d %d %d %d\n", temp.filename, temp.owner_task_id, temp.starting_block, temp.size, temp.permission[0], temp.permission[1],temp.permission[2], temp.permission[3], temp.blocks[0], temp.blocks[1],temp.blocks[2],temp.blocks[3]);
			write_window(win, buff);
		}
	}
}

void ufs::Dir(int Task_id, WINDOW * win)						// overloaded method. only show the file belonging to T-id
{
	char buff[256];
	sprintf(buff, " file name\towner id\tpermissions rwrw\n");
	write_window(win, buff);
	i_node temp;
	for(int i = 0; i < 16; i++){
		temp = Read_inode(i);
		if (temp.owner_task_id == Task_id){
			sprintf(buff, " %s %d %d %d %d %d %d %d %d %d %d %d\n", temp.filename, temp.owner_task_id, temp.starting_block, temp.size, temp.permission[0], temp.permission[1],temp.permission[2], temp.permission[3], temp.blocks[0], temp.blocks[1],temp.blocks[2],temp.blocks[3]);
			write_window(win, buff);
		}
	}
}
void ufs::dump( WINDOW * win)							// dump the ufs inodes as well as the ufs data blocks
{
	char ch;
	char buff[9001];
	fstream filef("filef.txt", ios::in);
	filef.seekp(0);
	for (int i = 0; i < 2048; i++){
		filef >> buff[i];
		sprintf(buff, "%c", ch);
	}
	//sprintf(buff, "\n");
	write_window(win, buff);
	//write_window(win, " printed message");
	filef.close();
	
}

ufs::i_node ufs::Write_Node(int index, i_node * node){
	sema_inodef.down();
	fstream inodef("inodef.txt", ios::in | ios::out);
	
	inodef.seekp(index*40);
	inodef.write((char*)&node->filename, 8);
	inodef.write((char*)&node->owner_task_id, 4);
	inodef.write((char*)&node->starting_block, 4);
	inodef.write((char*)&node->size, 4);
	inodef.write((char*)&node->permission, 4);
	inodef.write((char*)&node->blocks, 4);
	
	inodef.close();
	sema_inodef.up();
	//return node; dont need to return a node in the write	
}

ufs::i_node ufs::Read_inode(int index){
	sema_inodef.up();
	fstream inodef("inodef.txt", ios::in | ios::out);
	char buff[256];
	i_node temp;
	inodef.seekp(index*40);
	
	inodef.read((char*)&temp.filename, 8);
	temp.filename[7] = '\0';
	inodef.read((char*)&temp.owner_task_id, 4);
	inodef.read((char*)&temp.starting_block, 4);
	inodef.read((char*)&temp.size, 4);
	inodef.read((char*)&temp.permission, 4);
	inodef.read((char*)&temp.blocks, 4);
	/*
	sprintf(buff, " %s %d %d %d %d %d %d %d %d %d %d %d\n", temp.filename, temp.owner_task_id, temp.starting_block, temp.size, temp.permission[0], temp.permission[1],temp.permission[2], temp.permission[3], temp.blocks[0], temp.blocks[1],temp.blocks[2],temp.blocks[3]);
	write_window(Win, buff);*/

	inodef.close();
	sema_inodef.down();
	return temp;
}