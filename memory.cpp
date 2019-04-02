/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 3
|     File Name:	memory.cpp
|  Dependencies: 	
|       Authors: 	Drake Wood, James Giegerich
|      Language: 	C++
|      Compiler: 	G++
|         Class: 	C435 - Operating Systems
|    Instructor: 	Dr. Hakimzadeh
|  Date Created: 	2/16/2019
|  Last Updated:	3/18/2019
|        Due Date: 	3/18/2019
|==================================================================================|
|  Description: Contains the definitions for the functions outlined in memory.h                     
*==================================================================================*/

#include "memory.h"
mem_mgr::Mem_mgr(int size, char default_initial_value){// allocate 1024 unsigned chars and initialize the entire memory with . dots
	
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
	if (head = NULL){
		head = temp;
		tail = temp;
		temp = NULL;
	}
	else {
		tail->next = temp;
		tail = temp;
	}
}

int mem_mgr::MemAlloc(int size){// returns a unique integer memory_handle or -1 if not enough memory is available. set the current_location for this memory segment (beginning of the allocated area
	

}

int mem_mgr::Mem_Free(int memory_handle){// place #'s in the memory freed, return -1 if errors occur

	for (int i = 1; i < 9; i++){
		if (i = 
	}

}

int mem_mgr::Mem_Read(int memory_handle, char *ch){// read a character from current location in memory and bring it back in ch, return a -1 if at end of bounds, keep track of the current location or the location next char to be read. 
}

int mem_mgr::Mem_Write(int memory_handle, char ch){// write a character to the current location in memory, return a -1 if at end of bounds. 
} 
	// overloaded multi-byte read and write
int mem_mgr::Mem_Read(int memory_handle, int offset_from_beg, int text_size, chat *text){
}

int mem_mgr::Mem_Write(int memory_handle, int offset_from_beg, int text_size, chat *text){
}
	
int mem_mgr::Mem_Left(){// return the amount of core memory left in the OS
} 

int mem_mgr::Mem_Largest(){// return the size of the largest available memory segment
} 

int mem_mgr::Mem_Smallest(){// return the size of the smallest available memory segment
} 

int mem_mgr::Mem_Coalesce(){ // combine two or more contiguous blocks of free space and place . dots in the coalesced memory.
} 

int mem_mgr::Mem_Dump(int starting_from, int num_bytes){// dump the contents of memory
} 
