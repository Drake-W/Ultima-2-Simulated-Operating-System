/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 4
|     File Name:	semaphore.h
|  Dependencies: 	scheduler.h
|       Authors: 	Drake Wood, James Giegerich
|      Language: 	C++
|      Compiler: 	G++
|         Class: 	C435 - Operating Systems
|    Instructor: 	Dr. Hakimzadeh
|  Date Created: 	2/16/2019
|  Last Updated:	3/18/2019
|      Due Date: 	4/08/2019
|==================================================================================|
|  Description: This is the header file which defines the class semaphore. There are 3 functions along with a
|				constructor and deconstructor.
|				-dump displays information related to this class in a window for debugging.
|				-up creates a mutex and queues it, effectively locking a critical section.
|				-down dequeues a mutex, unlocking it. 
*==================================================================================*/

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <iostream>
#include <pthread.h> 	// Needed for using the pthread library
#include <assert.h>
#include <time.h>
#include <unistd.h> 		// Needed for sleep()
#include <ncurses.h> 	// Needed for Curses windowing
#include <stdarg.h> 		// Needed for formatted output to window
#include <termios.h>
#include <fcntl.h>
#include <cstdlib>
#include "queue.h"
#include "scheduler.h"
#include <string>

class semaphore { 
	int sema_value;			// 0 or 1 in the case of a binary semaphore
	
	Queue <scheduler::TCB*>sema_data;
	std::string resource_name; // The name of the resource being managed
	int count;					
	
	public:
	semaphore(int sema_value, char* sema_name);
	~semaphore();
		
	void down();						// Get the resource or get queued!
	void up();							// Release the resource
	void dump(int level, WINDOW * win);	// Include some
	};									// Functions which will allow you to dump the 
#endif 									// Contents of the semaphore in a readable format.  See the expected 
										// Output section (below) for suggestions.  
