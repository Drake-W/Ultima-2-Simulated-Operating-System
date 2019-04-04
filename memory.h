/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 3
|     File Name:	memory.h
|  Dependencies: 	none
|       Authors: 	Drake Wood, James Giegerich
|      Language: 	C++
|      Compiler: 	G++
|         Class: 	C435 - Operating Systems
|    Instructor: 	Dr. Hakimzadeh
|  Date Created: 	2/16/2019
|  Last Updated:	3/18/2019
|      Due Date: 	3/18/2019
|==================================================================================|
|  Description: This is the header file which defines the class mem_mgr.
*==================================================================================*/

#ifndef MEMORY_H
#define MEMORY_H
#include <string>
#include "window.h"


class mem_mgr{
	public:
		struct MemNode{
			int base;
			int limit;
			int handle;
			int size;
			int current_location;
			std::string owner;
			
			
			MemNode * next;
			
			
			bool status; // 0 is a hole, 1 is a process
			bool linked; // 0 is unlinked, 1 is linked to the next node to increase size.
		};
	MemNode * head;
	MemNode * tail;
	
	void create_node(int value);  // creates and initializes Memory Node.
	mem_mgr(int size, char default_initial_value); // allocate 1024 unsigned chars and initialize the entire memory with . dots
	int MemAlloc(int size, std::string owner); // returns a unique integer memory_handle or -1 if not enough memory is available. set the current_location for this memory segment (beginning of the allocated area
	int Mem_Free(int memory_handle); // place #'s in the memory freed, return -1 if errors occur
	int Mem_Read(int memory_handle, char *ch); // read a character from current location in memory and bring it back in ch, return a -1 if at end of bounds, keep track of the current location or the location next char to be read. 
	int Mem_Write(int memory_handle, char ch); // write a character to the current location in memory, return a -1 if at end of bounds. 
											   // overloaded multi-byte read and write
	int Mem_Read(int memory_handle, int offset_from_beg, int text_size, char *text); 
	int Mem_Write(int memory_handle, int offset_from_beg, int text_size, char *text);
	
	unsigned char Mem_Core[1024];
	
	//private:
	int Mem_Left(); // return the amount of core memory left in the OS
	int Mem_Largest(); // return the size of the largest available memory segment
	int Mem_Smallest(); // return the size of the smallest available memory segment
	int Mem_Coalesce(); // combine two or more contiguous blocks of free space and place . dots in the coalesced memory. 
	int Mem_Dump(int starting_from, int num_bytes, WINDOW * win); // dump the contents of memory
};

#endif