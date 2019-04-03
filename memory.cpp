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
	temp->linked = 1;
	temp->current_location = temp->base;
	
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
	//this->status = 1;
	int count 128;
	if (size > mem_largest())
	{
		return -1;
	}
	MemNode * temp = this->head;
	MemNode * temp2 = this->head;
	
	while (temp){
		if (temp->status == 0){
			while ( size > count){
				if (temp->linked){
					count = count + 128;
					temp2 = temp2->next;
				}else{
					temp = temp->next;
				}
					
			}// we have one big enough
		temp->status = 1;
		while( temp != temp2)
		{
			temp->linked =1;
			temp = temp->next;
			temp->status = 1;
		}
	}
}

int mem_mgr::Mem_Free(int memory_handle){// place #'s in the memory freed, return -1 if errors occur

	MemNode* temp = this->head;
	
	while(temp){
		if (temp->handle = memory_handle){
			for(int i = temp->base; i < temp->limit + 1; i++){
				Mem_Core[i] = '#';
			}
			this->status = 0;
			this->owner = "none";
			temp->current_location = temp->base;
			break;
		}
		else{
			temp = temp->next;
		}
	}

}

int mem_mgr::Mem_Read(int memory_handle, char *ch){// read a character from current location in memory and bring it back in ch, return a -1 if at end of bounds, keep track of the current location or the location next char to be read. 
	
	MemNode* temp = this->head;
	while(temp){
		if (temp->handle == memory_handle){
			
			ch = Mem_Core[temp->current_location];
			
			temp->current_location = temp->current_location - 1;
			break;
		}
		else{
			temp = temp->next;
		}
	}
	return -1;
}

int mem_mgr::Mem_Write(int memory_handle, char ch){	// write a character to the current location in memory, return a -1 if at end of bounds. 

	MemNode* temp = this->head;
	while(temp){
		if (temp->handle == memory_handle){
			
			Mem_Core[temp->current_location] = ch;
			
			temp->current_location = temp->current_location + 1;
			break;
		}
		else{
			temp = temp->next;
		}
	}
	return -1;
} 
	// overloaded multi-byte read and write
int mem_mgr::Mem_Read(int memory_handle, int offset_from_beg, int text_size, char *text){
}

int mem_mgr::Mem_Write(int memory_handle, int offset_from_beg, int text_size, char *text){
}
	
int mem_mgr::Mem_Left(){// return the amount of core memory left in the OS
int counter = 0;
	MemNode* temp = this->head;
	while(temp){
		if (temp->status == 0){
			counter = counter + 128;
		}
		
		temp = temp->next;
		
	}
	return counter;
} 

int mem_mgr::Mem_Largest(){// return the size of the largest available memory segment
int counter = 0;
int tempcount = 0;
	MemNode* temp = this->head;
	while(temp){
		tempcount = 0;
		if (temp->status == 0){
			tempcount = tempcount + 128;
			while (temp->linked){
				temp = temp->next;
				tempcount = tempcount + 128;
			}
		}
		if (tempcount > counter){
			counter = tempcount;
		}
		temp = temp->next;
		
	}
	return counter;
} 

int mem_mgr::Mem_Smallest(){// return the size of the smallest available memory segment
int counter = 1024;
int tempcount = 0;
	MemNode* temp = this->head;
	while(temp){
		tempcount = 0;
		if (temp->status == 0){
			tempcount = tempcount + 128;
			while (temp->linked){
				temp = temp->next;
				tempcount = tempcount + 128;
			}
		}
		if (tempcount < counter){
			counter = tempcount;
		}
		temp = temp->next;
		
	}
	return counter;
} 

int mem_mgr::Mem_Coalesce(){ // combine two or more contiguous blocks of free space and place . dots in the coalesced memory.
	MemNode* temp = this->head;
	while(temp){
		if (temp->status == 0){
			if (temp->next->status == 0){
				temp->linked = 1;
			}
		}
		temp = temp->next;
	}
} 

int mem_mgr::Mem_Dump(int starting_from, int num_bytes){// dump the contents of memory  add window parameter
	int end = starting_from + num_bytes);
	for (int i = starting_from; i < end + 1; i++)
	{
		// make a dump window for this 
		// Mem_Core[i];
	}

} 
