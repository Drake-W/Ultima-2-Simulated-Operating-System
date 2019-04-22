/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 4
|     File Name:	main.cpp
|  Dependencies: 	scheduler.h semaphore.h window.h queue.h
|       Authors: 	Drake Wood, James Giegerich
|      Language: 	C++
|      Compiler: 	G++
|         Class: 	C435 - Operating Systems
|    Instructor: 	Dr. Hakimzadeh
|  Date Created: 	2/16/2019
|  Last Updated:	4/08/2019
|      Due Date: 	4/08/2019
|==================================================================================|
|  Description: Contains the main function that interacts with the scheduler, semaphore, queue, and window classes.
|               Multiple curses windows and 3 pthreads are created which are then passed to other functions to handle. 
|				The rest of main contains an input loop for the user to control what happens while the program is running.
|               Details of the way functions are used and their parameters as well as 
|               computation details are given in the class descriptions.                         
*==================================================================================*/

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
#include "scheduler.h"
#include "semaphore.h"
#include "window.h"
#include "memory.h"
#include "ufs.h"

using namespace std;
semaphore sema_screen(1, (char *)"Screen Print");	// creates semaphores
semaphore sema_t1mail(1, (char *)"t1mail");	
semaphore sema_t2mail(1, (char *)"t2mail");	
semaphore sema_t3mail(1, (char *)"t3mail");	
semaphore sema_t4mail(1, (char *)"t4mail");	
semaphore sema_ptable(1, (char *)"ptable");	
semaphore sema_memory(1, (char *)"memory");
scheduler sched; //creates scheduler
ipc IPC; // creates ipc
mem_mgr Mem_Mgr(1,1); // creates mem manager - size, initial value - dont do anything right now
ufs superuser("superuser", 16, 128, '^');

//------------------------------------------------------------------------------
//------------------------------MAIN--------------------------------------------
//------------------------------------------------------------------------------

int main() {
	initscr(); 					// Start nCurses
	refresh();					// Refresh screen
	
//------------------------------------------------------------------------------
//Creating heading window, printing content
//------------------------------------------------------------------------------
 
	WINDOW * Heading_Win = newwin(5, 80, 0, 2);
	box(Heading_Win, 0,0);
	mvwprintw(Heading_Win, 1, 26, "ULTIMA 2.0 Phase 3 (Spring 2019)");
	mvwprintw(Heading_Win, 3, 28, "Drake Wood 	James Giegerich");
	wrefresh(Heading_Win);
	
//------------------------------------------------------------------------------
//Creating log window
//------------------------------------------------------------------------------

	WINDOW * Log_Win = create_window(15, 60, 20, 2);
	write_window(Log_Win, 1, 18, "...........Log...........\n");
 
//------------------------------------------------------------------------------
//Creating console window
//------------------------------------------------------------------------------

	WINDOW * Console_Win = create_window(15, 20, 20, 62);
	write_window(Console_Win, 1, 1, " ....Console....\n");
	write_window(Console_Win, 2, 1, "Ultima # ");
	write_window(Log_Win, " Main program started\n" );

//------------------------------------------------------------------------------
//Creating 3 windows for tasks
//------------------------------------------------------------------------------

	WINDOW * W1 = create_window(15, 25, 5, 2);
	WINDOW * W2 = create_window(15, 25, 5, 30);
	WINDOW * W3 = create_window(15, 25, 5, 57);
	
//------------------------------------------------------------------------------
//Creating a process table dump window
//------------------------------------------------------------------------------

	WINDOW * Process_Table = create_window(9, 80, 0, 83);
	write_window(Process_Table, 1, 5, "      PROCESS TABLE DUMP \n -------------------------------------\n");
	
//------------------------------------------------------------------------------
//Creating tasks with create_task passing each a name and task window 
//------------------------------------------------------------------------------
	
	sched.create_task((char*)" Task 1", W1,Process_Table);
	sched.create_task((char*)" Task 2", W2,Process_Table);
	sched.create_task((char*)" Task 3", W3,Process_Table);
	
//-------------------------------------------------------------------------------
//Creating a semaphore dump window
//-------------------------------------------------------------------------------

	WINDOW * Sema_Dump = create_window(16, 80, 9, 83);
	write_window(Sema_Dump, 1, 5, "        SEMAPHORE DUMP \n -------------------------------------\n");
	
//-------------------------------------------------------------------------------
//Creating a memory dump window
//-------------------------------------------------------------------------------

	WINDOW * Mem_Dump = create_window(22, 80, 35, 2);
	write_window(Mem_Dump, 1, 5, "                   MEMORY DUMP \n --------------------------------------------------------\n");

//------------------------------------------------------------------------------
//Create a Messaging dump window
//------------------------------------------------------------------------------

	WINDOW * Message_Dump = create_window(17, 80, 25, 83);
	write_window(Message_Dump, 1, 5, "	     MESSAGING DUMP \n -------------------------------------\n");
	
//------------------------------------------------------------------------------
//Setup for user console input
//------------------------------------------------------------------------------
						
	cbreak(); 							// Set up keyboard I/O processing
	noecho(); 							// disable line buffering
	nodelay(Console_Win, true);			// disable automatic echo of characters read by getch(), wgetch()
	  									// nodelay causes getch to be a non-blocking call.
										// If no input is ready, getch returns ERR
	
//------------------------------------------------------------------------------
//Start threads and run till end of program 
//------------------------------------------------------------------------------
	
 	sched.run(); //start running all the tasks
	
	sched.create_ui_task(Process_Table, Sema_Dump, Console_Win, Log_Win, Message_Dump, Mem_Dump);
	
	while(sched.process_table.qSize() != 0){ // loop while threads run
		sleep(1);
	}

endwin();
return 0;
} // end of main


