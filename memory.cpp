/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 3
|     File Name:	memory.cpp
|  Dependencies: 	memory.h semaphore.h cstring
|       Authors: 	Drake Wood, James Giegerich
|      Language: 	C++
|      Compiler: 	G++
|         Class: 	C435 - Operating Systems
|    Instructor: 	Dr. Hakimzadeh
|  Date Created: 	2/16/2019
|  Last Updated:	4/08/2019
|      Due Date: 	4/08/2019
|==================================================================================|
|  Description: Contains the definitions for the functions outlined in memory.h                     
*==================================================================================*/

#include "memory.h"
#include "semaphore.h"
#include <cstring>

extern semaphore sema_memory;
extern semaphore sema_screen;

mem_mgr::mem_mgr(int size, char default_initial_value){// allocate 1024 unsigned chars and initialize the entire memory with . dots
	
	for (int i = 1; i < 9; i++){
		create_node(i);
	}
		
	for (int i = 0; i < 1024; i++){
		Mem_Core[i] = '.';
	}
} 

void mem_mgr::create_node(int value){
	
	MemNode * temp = new MemNode;
	temp->handle = value;
	temp->limit = temp->handle * 128 - 1;
	temp->base = temp->limit - 127;
	temp->owner = "none";
	temp->status = 0;
	temp->size = 128;
	if (value != 8){
		temp->linked = value + 1; // linked to the next node
	}else{
		temp->linked = 0; // last node wont have anything linked to it
	}
	if (value = 1){
		temp->start = 1; // first node will start the linked nodes for the unused space
	}else{
		temp->start = 0; // // other nodes will not be a starting point
	}
	
	temp->current_location = temp->base; // set the CL to the first point of the node
	
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

int mem_mgr::MemAlloc(int size, std::string owner){// returns a unique integer memory_handle or -1 if not enough memory is available. set the current_location for this memory segment (beginning of the allocated area
	int handle = First_Fit(size);
	sema_memory.down(); // calling this after first fit because it uses sema_memory also
	int count = 128; // temp->size should go here
	
	MemNode * temp = this->head;
	while (temp->handle != handle){ // get us to the right node
		temp = temp->next;
	}
	
	temp->status = 1; //set status to allocated
	temp->owner = owner; // assign the owner
	temp->current_location = temp->base; // se the CL to the first point of the node.
	temp->start = 1;
	
	while (size > count){ // get and link additional nodes if we need more space, we know these are available from first_fit()
		temp = temp->next;
		temp->status = 1; //set status to allocated
		temp->owner = owner; // assign the owner
		temp->current_location = temp->base; // se the CL to the first point of the node.
		count += 128; // temp->size should go here
	}
	
	if (temp->linked){ // if this last node is linked then we need to unlink and set the next node to the start of a link
				temp->linked = 0;
				temp = temp->next;
				temp->start = 1;
			}
	sema_memory.up();
	return handle;
}


int mem_mgr::First_Fit(int size){ // given a desired size will return the first node/link handle that can provide enough space. 
	sema_memory.down();
	int handle;	
	int tempcount = 0;
	MemNode* temp = this->head;
	while(temp){
		tempcount = 0;
		if (temp->status == 0){ // find a hole while is also the first node
			handle = temp->handle; // record handle in case this one ends up being big enough
			tempcount = tempcount + 128; // keep track of size // temp->size should go here

			while (temp->linked){
				temp = temp->next;
				tempcount = tempcount + 128; // find the full size of the hole // temp->size should go here
			}
		}
		if (tempcount >= size){ // if the hole is large enough return the handle
			sema_memory.up();
			return handle;
		}
		temp = temp->next; // check next node if needed. 
	}
	sema_memory.up();
	return -1;
}

int mem_mgr::Mem_Free(int memory_handle){// place #'s in the memory freed, return -1 if errors occur
										// only use this on the first node/handle in a link
sema_memory.down();
	int runagain = 0;
	MemNode* temp = this->head;
	
	while (temp->handle != memory_handle){ // get us to the right node
		temp = temp->next;
	}
	
	
	if (temp->start == 1){ // must be used on start of memory
		do{
			for(int i = temp->base; i < temp->limit + 1; i++){
				Mem_Core[i] = '#'; // clear out all left over data
			}
			// reset attributes
			temp->status = 0; 
			temp->owner = "none";
			temp->current_location = temp->base;
		
			if(temp->linked){
				temp = temp->next; // move to next node and run again 
				runagain = 1;
			}else{
				runagain = 0; // end
			}
		
		}while(runagain); // make sure  to clear out all linked nodes as well
	
		sema_memory.up();
		return 1;
	}
	sema_memory.up();
	return -1;
	
}

// does not move back to previous linked node
int mem_mgr::Mem_Read(int memory_handle, char *ch){// read a character from current location in memory and bring it back in ch, return a -1 if at end of bounds, keep track of the current location or the location next char to be read. 
	sema_memory.down();
	MemNode* temp = this->head;
	while (temp->handle != memory_handle){ // get us to the right node
		temp = temp->next;
	}
	
	while((temp->current_location > temp->limit) && (temp->linked > 0)){
				temp = temp->next; // if the head is full but has a linked node then move to that
			}
	
	if (temp->current_location > temp->base){
		*ch = Mem_Core[temp->current_location -1]; // finds the most recent ch written 
		temp->current_location = temp->current_location - 1;	// move current location
	
	}else{
		sema_memory.up();
		return -1;
	}

	sema_memory.up();
	return 1;
}

			
//does not check if the node is full and if it is linked to another so it can write more.
int mem_mgr::Mem_Write(int memory_handle, char ch){	// write a character to the current location in memory, return a -1 if at end of bounds. 
	sema_memory.down();
	MemNode* temp = this->head;
	while(temp){
		if (temp->handle == memory_handle){
			
			while((temp->current_location > temp->limit) && (temp->linked > 0)){
				temp = temp->next; // if the head is full but has a linked node then move to that
			}
			
			if (temp->current_location <= temp->limit){ // if == to limit current location will end up 1 past limit
				Mem_Core[temp->current_location] = ch;
			
				temp->current_location = temp->current_location + 1;
				sema_memory.up();
				return 1;
			}else{
				sema_memory.up();
				return -1;
			}
		}
		else{
			temp = temp->next;
		}
	}
	sema_memory.up();
	return -1;
} 
			
// overloaded multi-byte read and write
//****************************not currently functioning-causes seg fault******************************
int mem_mgr::Mem_Read(int memory_handle, int offset_from_beg, int text_size, char *text){
	sema_memory.down();
	int cl = 0; //current location for offset
	MemNode* temp = this->head;
	
	while(temp){
		if (temp->handle == memory_handle){
			cl = temp->base + offset_from_beg;
			
			for (int i = 0; i < text_size; i++){
				text[i] = Mem_Core[cl+i];
			}
			sema_memory.up();
		return 1;

		}
		else{
			temp = temp->next;
		}
	}
	sema_memory.up();
	return -1;
}

int mem_mgr::Mem_Write(int memory_handle, int offset_from_beg, int text_size, char *text){
	sema_memory.down();
	int cl = 0; //current location for offset
	MemNode* temp = this->head;
	
	while(temp){
		if (temp->handle == memory_handle){ // find the right handle
			cl = temp->base + offset_from_beg;
			
			for (int i = 0; i < text_size; i++){
				Mem_Core[cl+i] = text[i];
			}
			sema_memory.up();
		return 1;

		}
		else{
			temp = temp->next;
		}
	}
	sema_memory.up();
	return -1;
}

int mem_mgr::Mem_Left(){// return the amount of core memory left in the OS
	sema_memory.down();
	int counter = 0;
	MemNode* temp = this->head;
	while(temp){
		if (temp->status == 0){
			counter = counter + 128; // temp->size should go here
		}
		temp = temp->next;
	}
	sema_memory.up();
	return counter;
} 

int mem_mgr::Mem_Largest(){// return the size of the largest available memory segment
	sema_memory.down();
	int counter = 0;
	int tempcount = 0;
	MemNode* temp = this->head;
	while(temp){
		tempcount = 0;
		if (temp->status == 0){
			tempcount = tempcount + 128; // temp->size should go here
			while (temp->linked){
				temp = temp->next;
				tempcount = tempcount + 128; // temp->size should go here
			}
		
			if (tempcount > counter){
				counter = tempcount;
			}
		}
		temp = temp->next;
	}
	sema_memory.up();
	return counter;
} 

int mem_mgr::Mem_Smallest(){// return the size of the smallest available memory segment
	sema_memory.down();
	int counter = 1024;
	int tempcount = 0;
	MemNode* temp = this->head;
	while(temp){
		tempcount = 0;
		if (temp->status == 0){
			tempcount = tempcount + 128; // temp->size should go here
			while (temp->linked){
				temp = temp->next;
				tempcount = tempcount + 128; // temp->size should go here
			}
			if (tempcount < counter){
			counter = tempcount;
			}
		}
		
		temp = temp->next;
	}
	sema_memory.up();
	return counter;
} 


int mem_mgr::Mem_Coalesce(){ // combine two or more contiguous blocks of free space and place . dots in the coalesced memory.
	sema_memory.down();
	int array[9];
	
	MemNode* temp = this->head;
	
	for(int i = 1; i < 9; i++){
		if (temp->status == 0){
			array[i]=1;
			for(int j = temp->base; j < temp->limit + 1; j++){
				Mem_Core[j] = '.'; // clear out all left over data
			}
		}
		temp = temp->next;
	}
	
	temp = this->head;
	
	for(int i = 1; i < 9; i++){
		if (array[i] == 1){
			temp->start = 1;
			while(array[i+1] == 1){
				temp->linked = temp->handle+1;
				temp = temp->next;
				i++;
			}
		}
		temp = temp->next;
	}
	
	sema_memory.up();
	return 1;
} 

int mem_mgr::Mem_Dump(int starting_from, int num_bytes, WINDOW * win){// dump the contents of memory for specific location
	sema_memory.down();
	char buff[256];
	int end = starting_from + num_bytes;
	
	sprintf(buff, " Memory dump of %d bytes starting at address:%d \n", num_bytes, starting_from);
	write_window(win, buff);
	
	for (int i = starting_from; i < end + 1; i++)
		{
			sprintf(buff, "%c", Mem_Core[i]);
			write_window(win, buff);
		}
		sema_memory.up();
		return 1;
} 
int mem_mgr::Mem_Usage(WINDOW * win){
	sema_memory.down();
	
	MemNode* temp = this->head;
	char buff[256];
	sprintf(buff, "  Status  | Mem Handle | Start Loc | End Loc | Size |  Cur Loc  |  TID  \n");
	write_window(win,buff);
	char * status;
	const char * name;
	int handle, base, limit, size,  current_location;
	int start;
	int status1;
	while(temp){
		start = temp->start;
		if(start == 1){
			status1 = temp->status;
			if (status1){
				status = "Used";
			}else{
				status = "Free";
			}
			name = temp->owner.c_str();
			handle = temp->handle;
			base = temp->base;
			limit = temp->limit;
			size = temp->size;
			current_location = temp->current_location;
			sprintf(buff, "  %s\t\t%d\t\t%d\t%d\t%d\t%d\t%s \n",status, handle, base, limit, size, current_location, name);
			write_window(win,buff);
		}
		temp = temp->next;
	}
	
	sema_memory.up();
}
int mem_mgr::Core_Dump( WINDOW * win){  // dumps entire contents of memory to the screen
	sema_screen.down();
	wclear(win);
	sema_screen.up();
	
	sema_memory.down();
	char buff[256];
	int count = 0;
	write_window(win, "\n -----------Memory core dump---------- \n");
	for (int i = 0; i < 16; i++){
		for (int j = 1; j < 65; j++){
			buff[j] = Mem_Core[count];
			count++;
		}
		buff[0] = ' ';
		buff[65] = '\n';
		write_window(win, buff);
	}
	sema_memory.up();
}
