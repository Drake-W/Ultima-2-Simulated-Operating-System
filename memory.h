/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 4
|     File Name:	memory.h
|  Dependencies: 	string window.h
|       Authors: 	Drake Wood, James Giegerich
|      Language: 	C++
|      Compiler: 	G++
|         Class: 	C435 - Operating Systems
|    Instructor: 	Dr. Hakimzadeh
|  Date Created: 	2/16/2019
|  Last Updated:	4/08/2019
|      Due Date: 	4/08/2019
|==================================================================================|
|  Description: This is the header file which defines the class mem_mgr. 
				-MemNode is the structure that contains information for each block of  memory.
				-create_node creates a node with given size, must be called multiple times.
				-mem_mgr constructor for the core memory.
				-MemAlloc Called by a task to ask for memory to be assigned.
				-Mem_Free Clears memory when task is dead.
				-Mem_Read Returns memory and can be overloaded.
				-Mem_Write Writes to memory and can be overloaded.
				-First_Fit Returns the mem handle for the first fit for requested size.
				-Mem_Left Returns how much memory is left.
				-Mem_Largest Returns the largest piece of free memory left.
				-Mem_Smallest Returns the smalles piece of free memory left. 
				-Mem_Coalesce Combines free memory nodes into a larger block. 
				-Mem_Dump Dumps a specific memory segment.
				-Core_Dump Dumps entire core memory.
				-Mem_Usage Reports memory usage.
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
			int linked; // 0 is unlinked/end of link, 1+ is linked to the node with that handle going forward
			bool start; // 0 is not start of a link, 1 is start of a link
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
	int Mem_Read(int memory_handle, int offset_from_beg, int text_size, char *text, WINDOW * win); 
	int Mem_Write(int memory_handle, int offset_from_beg, int text_size, char *text);
	int First_Fit(int size);// given a desired size will return the first node/link handle that can provide enough space. 
	
	unsigned char Mem_Core[1024];
	
	//private:
	int Mem_Left(); // return the amount of core memory left in the OS
	int Mem_Largest(); // return the size of the largest available memory segment
	int Mem_Smallest(); // return the size of the smallest available memory segment
	int Mem_Coalesce(); // combine two or more contiguous blocks of free space and place . dots in the coalesced memory. 
	int Mem_Dump(int starting_from, int num_bytes, WINDOW * win); // dump the contents of memory
	int Core_Dump(WINDOW * win);
	int Mem_Usage(WINDOW * win);
};

#endif