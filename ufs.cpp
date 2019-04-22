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

extern semaphore sema_files;

//------------------------------------------------------------------------------
//Filesystem operations
//------------------------------------------------------------------------------

ufs::ufs(char* name, int noBlocks, int blockSize, char init_char) // only used by superuser (you) to create the filesystem, 
{																		// format the virtual disk (initialize with 0x5E or "^"), also wipe
	
	fstream filef("filef.txt", ios::out);
	filef.seekp(0);
	int size = blockSize*noBlocks;
	for (int i = 0; i < size; i++)
		filef << init_char;
	filef.close();
	
	fs_name = name;										
	fs_number_of_blocks = noBlocks;
	fs_block_size = blockSize;
	initialization_char = init_char;
}
													
void ufs::format()									// format the current file system
{
	fstream filef("filef.txt", ios::out);
	filef.seekp(0);
	int size = fs_block_size*fs_number_of_blocks;
	for (int i = 0; i < size; i++)
		filef << initialization_char;
	filef.close();
}

//------------------------------------------------------------------------------
//File operations
//------------------------------------------------------------------------------

int ufs::Open(int T_id, int file_handle, char* filenm, char* mode)		//file handle must be valid and belong to t-id or file name should be
{	
													//set for others. mode = R or W returns a unique integer file_id or -1
													//for an error. an error could be no such file name is created in teh system
													//or such a file is not owned by this task_id or the permission set for this
													//file do not allow it to be open.
	i_node* temp = this->head;
	
	while ((temp->owner_task_id != T_id) || (temp->filename != filenm)){ // get us to the right node
		temp = temp->next;
	}												
	// check permissions and it it exists
	
	return 1;
}

int ufs::Close(int T_id, int file_id)						// close the file, returns a -1 if an error occurs
{
	//sema.up()?
}
int ufs::Read_Char(int T_id, char* file_name, char *ch) 	// read a char from file, return -1 if EOF keep track of the current location
{
	i_node* temp = this->head;
	
	while ((temp->owner_task_id != T_id) || (temp->filename != file_name)){ // get us to the right node
		temp = temp->next;
	}
	fstream filef("filef.txt", ios::out);
	filef.seekp(temp->currentlocation);
	filef >> ch;
	temp->currentlocation--;
	filef.close();
	
	return 1;
}
int ufs::Write_Char(int T_id, char* file_name, char* ch) 	// write a  char to the file, return -1 if error. keep track of the current file location. 
{
	i_node* temp = this->head;
	
	while ((temp->owner_task_id != T_id) || (temp->filename != file_name)){ // get us to the right node
		temp = temp->next;
	}
	fstream filef("filef.txt", ios::out);
	filef.seekp(temp->currentlocation);
	filef << ch;
	temp->currentlocation++;
	filef.close();
	return 1;
}

//------------------------------------------------------------------------------
//Directory operations
//------------------------------------------------------------------------------

int ufs::Create_file(int T_id, char* filenm, int file_size, int permission[4])
{
	
	i_node * temp = new i_node;
	//temp->filename = filenm;
	//wrong type 
	temp->owner_task_id = T_id;
	temp->starting_block = T_id;
	temp->size = file_size;
	for(int i = 0; i < 4; i++){
		temp->permission[i] = permission[i];
	}
	temp->blocks[0] =  T_id;
	for (int i = 0; i < 4; i++){
		temp->blocks[i] = 0;
		i++;
	}
	temp->currentlocation =  (T_id * 128) - 128;
	
	
	
	if (head == NULL){
		head = temp;
		tail = temp;
		temp = NULL;
	}
	else {
		tail->next = temp;
		tail = temp;
	}
}
int ufs::Del_file(int Task_id, char* file_name)		// check the files ownership, delete the content of the file from datablocks
{			
									// by writing $'s in it, then delete the file from the inode table
    i_node *current=new i_node;
    i_node *previous=new i_node;
    current=head; // this->head????
   
    if ( (current->owner_task_id != Task_id) || (current->filename != file_name)){
		previous=current;
		current=current->next;
    }
    previous->next=current->next;  

// delete block file stuff here 	 write $$'s
    return 1;
}
											
int ufs::Change_Permission(int Task_id, char* file_name, int new_permission[4])	// rwrw only the files owner can change the permission on the file
{
	i_node* temp = this->head;
	
	while ((temp->owner_task_id != Task_id) || (temp->filename != file_name)){ // get us to the right node
		temp = temp->next;
	}
	for (int i =0; i < 4; i++){
		temp->permission[i] = new_permission[i];
	}
}
	
void ufs::Dir( WINDOW * win)								// show the directory, everyones file anme and permissions
{ 
	char buff[256];
	sprintf(buff, "file name\owner id\tpermissions rwrw");
	write_window(win, buff);
	i_node* temp = this->head;
	while (temp != NULL){
		sprintf(buff, "%c\t%d\t%d%d%d%d", temp->filename, temp->owner_task_id, temp->permission[0],temp->permission[1],temp->permission[2],temp->permission[3]);
		write_window(win, buff);
		temp = temp->next;
	}
}
void ufs::Dir(int Task_id, WINDOW * win)						// overloaded method. only show the file belonging to T-id
{
	char buff[256];
	sprintf(buff, "file name\owner id\tpermissions rwrw");
	write_window(win, buff);
	i_node* temp = this->head;
	while (temp->owner_task_id != Task_id){ // get us to the right node
		temp = temp->next;
	}
		sprintf(buff, "%c\t%d\t%d%d%d%d", temp->filename, temp->owner_task_id, temp->permission[0],temp->permission[1],temp->permission[2],temp->permission[3]);
		write_window(win, buff);
		temp = temp->next;
	
}
void ufs::dump( WINDOW * win)							// dump the ufs inodes as well as the ufs data blocks
{
	
}
	