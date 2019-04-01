/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 3
|     File Name:	scheduler.h
|  Dependencies: 	queue.h
|       Authors: 	Drake Wood, James Giegerich
|      Language: 	C++
|      Compiler: 	G++
|         Class: 	C435 - Operating Systems
|    Instructor: 	Dr. Hakimzadeh
|  Date Created: 	2/16/2019
|  Last Updated:	3/18/2019
|      Due Date: 	3/18/2019
|==================================================================================|
|  Description: This is the header file which defines the class scheduler. There are 4 functions along with a
|				constructor, deconstructor, and data structs. 
|				-dump displays information related to this class in a window for debugging.
|				-garbage_collect finds threads that have been killed adn removes them from the process table.
|				-create_task creates a new thread running simple output, its information is added to the process table.
|				-destroy_task finds the task that the user wishes to kill and changes it status, stopping it. 
|				-TCB is the struct containing the thread_data which is stored in the process queue. 
*==================================================================================*/

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <pthread.h> 	// Needed for using the pthread library
#include <assert.h>
#include <time.h>
#include <unistd.h> 	// Needed for sleep()
#include <ncurses.h> 	// Needed for Curses windowing
#include <stdarg.h> 	// Needed for formatted output to window
#include <termios.h>
#include <fcntl.h>
#include <cstdlib>
#include "queue.h"
#include <string>
#include "ipc.h"

class scheduler {
	public:
	struct TCB {
		pthread_t thread;
		std::string name;
		int thread_no;				// Thread number
		WINDOW *thread_win;			// Thread's window
		bool kill_signal;			// Kill signal flag live/kill
		int sleep_time;				// Sleep time 
		int thread_results;			// Results
		int state;
		int pause_state;
		Queue<ipc::Message*> mailbox;
		WINDOW *pdumpwin; // these are for the main input
		WINDOW *sdumpwin;
		WINDOW *conwin;
		WINDOW *logwin;
		WINDOW *messwin;
		char *input;
	};
	
	void run();
	scheduler();
	int task_counter;
	Queue <TCB*>process_table;
	int create_task(char* name, WINDOW *win, WINDOW *pdumpwin);  	// Create appropriate data structures and calls coroutine()
	int create_ui_task(WINDOW *pdumpwin, WINDOW *sdumpwin, WINDOW *conwin, WINDOW *logwin, WINDOW * messwin);
	bool destroy_task(int tasknumber);  				// to kill a task (Set its status to DEAD)
	void yield();  		  											// Strict round robin process switch. 
	void dump(int level, WINDOW * win); 				// Debugging function with level indicating the verbosity of the dump 
	void garbage_collect();		// Include some functions which will allow you to dump the contents of the 
	void pause();
	void unpause();
};																		// process table in a readable format.  See the expected output section 
#endif																// (below) for suggestions. 
																		// remove dead task, free their resources, etc. 