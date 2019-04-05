/*==================================================================================|
|   Assignment:		Ultima 2.0 Phase 3
|    File Name:		scheduler.cpp
| Dependencies: 	scheduler.h semaphore.h window.h
|      Authors: 	Drake Wood, James Giegerich
|     Language: 	C++
|     Compiler: 	G++
|        Class: 	C435 - Operating Systems
|   Instructor: 	Dr. Hakimzadeh
| Date Created: 	2/16/2019
| Last Updated:		3/18/2019
|     Due Date: 	3/18/2019
|==================================================================================|
|  Description: Contains the definitions for the functions outlined in scheduler.h                     
*==================================================================================*/

#include "semaphore.h"
#include "scheduler.h"
#include "window.h"
#include "memory.h"

#define BLOCKED 0
#define READY 1
#define RUNNING 2
#define DEAD 3

extern mem_mgr Mem_Mgr;
extern semaphore sema_screen;
extern semaphore sema_ptable;

void scheduler::dump(int level, WINDOW * win) {
	sema_screen.down();
	wclear(win);
	sema_screen.up();
	write_window(win, 1, 5, "      PROCESS TABLE DUMP \n -------------------------------------\n");
	char buff[256];
	int procnum1;
	int memhandle;
	int size = process_table.qSize();
	
	if (size == 0)					// Check if anything is in the queue
		write_window(win, " No tasks currently running...\n");
	sema_ptable.down();
	for(int i = 0 ; i < size ; i++){		// Search for a dead task
		TCB *tcb = process_table.Dequeue();
		process_table.Enqueue(tcb);
		procnum1 = tcb->thread_no;
		memhandle = tcb->memhandle;
		
	
		if ( tcb->state == READY){
			sprintf(buff, " Task #%d status: Ready\t\tMemory Handle: %d\n", procnum1, memhandle);
		}
		else if (tcb->state == RUNNING){
			sprintf(buff, " Task #%d status: Running\tMemory Handle: %d\n", procnum1, memhandle);
		} 
		else if (tcb->state == BLOCKED){
			sprintf(buff, " Task #%d status: Blocked\t\tMemory Handle: %d\n", procnum1, memhandle);
		}
		else if (tcb->state == DEAD){
			sprintf(buff, " Task #%d status: Dead\t\tMemory Handle: %d\n", procnum1, memhandle);
		}// end else
		write_window(win, buff);
	} // end for
	sema_ptable.up();
	write_window(win, " -------------------------------------\n");	
}// end of dump

void scheduler::garbage_collect() {				// Delete those with  dead status
	sema_ptable.down();
	int size = process_table.qSize();
	for(int i = 0 ; i < size ; i++){
		TCB *tcb = process_table.Dequeue();
		if (tcb->state != DEAD){
			process_table.Enqueue(tcb);
		} // end if
	} // end for
	sema_ptable.up();
} // end of garbage_collect													

int scheduler::create_task(char* name, WINDOW *win, WINDOW *pdumpwin){
	int result_code;
	TCB * tcb = new TCB;
	this->task_counter++;
	tcb->thread_win = win;
	tcb->pdumpwin = pdumpwin;
	write_window(tcb->thread_win, 13, 1, "Starting Thread.....\n");
	tcb->thread_no = this->task_counter;
	tcb->name = name;
	tcb->state = READY;
	
	
	tcb->memhandle = Mem_Mgr.MemAlloc(128, tcb->name); 
	if (tcb->memhandle == -1){
		// error getting memory
		// return -1; 
		// return -1 will not start UI loop but doesnt say anything 
	}
	
	
	// create thread running simple output in its own window
	result_code = pthread_create(&tcb->thread, NULL, perform_simple_output, tcb);
	assert(!result_code); 			// if there is any problems with result code. display it and end program.
	sema_ptable.down();
	process_table.Enqueue(tcb);		// add to process table
	sema_ptable.up();
	return 0;
} // end of create_task

int scheduler::create_ui_task(WINDOW *pdumpwin, WINDOW *sdumpwin, WINDOW *conwin, WINDOW *logwin, WINDOW * messwin, WINDOW * memwin){
	int result_code;
	TCB * tcb = new TCB;
	this->task_counter++;
	tcb->pdumpwin = pdumpwin;
	tcb->sdumpwin = sdumpwin;
	tcb->conwin = conwin;
	tcb->logwin = logwin;
	tcb->messwin = messwin;
	tcb->memwin = memwin;
	
	tcb->thread_no = this->task_counter;
	tcb->name = (char*) "UI Thread";
	tcb->state = READY;
	
	// get memory
	tcb->memhandle = Mem_Mgr.MemAlloc(128, tcb->name); 
	if (tcb->memhandle == -1){
		// error getting memory
		// return -1; 
		// return -1 will not start UI loop but doesnt say anything 
	}
	
	// create thread ui loop
	result_code = pthread_create(&tcb->thread, NULL, ui_loop, tcb);
	assert(!result_code); 			// if there is any problems with result code. display it and end program.
	sema_ptable.down();
	process_table.Enqueue(tcb);		// add to process table
	sema_ptable.up();
	return 0;
}
scheduler::scheduler(){
	task_counter = 0;
}

bool scheduler::destroy_task(int tasknumber) {
	bool flag = false;
	sema_ptable.down();
	for( int i =0; i < process_table.qSize(); i++){
		if (!process_table.isEmpty()){						// this block searches Q for a specific process
			TCB *tcb = process_table.Dequeue();
			process_table.Enqueue(tcb);					// if it is found kill signal is changed
								
			if (tcb->thread_no == tasknumber){
				if (tcb->kill_signal == 0){			// other processes are added back to Q
					tcb->kill_signal = 1;	
					flag = true;							
				}else {		//already killed. 	
					//return 0;
				} // end else						
			} // end if
		} // end if
	} // end for
	sema_ptable.up();
	return flag;
} // end of destroy_task

void scheduler::yield() { // Give scheduler the option to change state to ready or to continue running
	sema_ptable.down();
	// Change current thread state to READY.
	TCB * tcb = process_table.Dequeue();
	if (tcb->state != DEAD){ //dont ready if yielding after death
		tcb->state = READY;
	}
	process_table.Enqueue(tcb);
	
	// Find a thread that is READY.
	tcb = process_table.Dequeue();
	while (tcb->state != READY){ // might get stuck here?
		process_table.Enqueue(tcb);
		tcb = process_table.Dequeue();
	}
	
	tcb->state = RUNNING;
	process_table.Enqueue(tcb);
	
	// Shuffle the queue to put runnning thread back at front of queue.
	for (int i = 0; i < (process_table.qSize() - 1); i++){
		tcb = process_table.Dequeue();
		process_table.Enqueue(tcb);
	}
sema_ptable.up();
}

void scheduler::run(){
	sema_ptable.down();
	TCB *tcb = process_table.Dequeue();
	tcb->state = RUNNING;
	process_table.Enqueue(tcb);
	
	// Shuffle the queue to put runnning thread back at front of queue.
	for (int i = 0; i < (process_table.qSize() - 1); i++){
		tcb = process_table.Dequeue();
		process_table.Enqueue(tcb);
	}
	sema_ptable.up();
}

void scheduler::pause(){
	sema_ptable.down();
	for (int i = 0; i < (process_table.qSize()); i++){
		TCB * tcb = process_table.Dequeue();
		tcb->pause_state = tcb->state;
		tcb->state = BLOCKED;
		process_table.Enqueue(tcb);
	}
	sema_ptable.up();
}

void scheduler::unpause(){
	sema_ptable.down();
	for (int i = 0; i < (process_table.qSize()); i++){
		TCB * tcb = process_table.Dequeue();
		if (tcb->state != DEAD){
		tcb->state = tcb->pause_state;
		}
		
		process_table.Enqueue(tcb);
	}
	sema_ptable.up();
}