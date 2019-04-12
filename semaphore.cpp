/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 3
|     File Name:	semaphore.cpp
|  Dependencies: 	semaphore.h window.h
|       Authors: 	Drake Wood, James Giegerich
|      Language: 	C++
|      Compiler: 	G++
|         Class: 	C435 - Operating Systems
|    Instructor: 	Dr. Hakimzadeh
|  Date Created: 	2/16/2019
|  Last Updated:	4/08/2019
|      Due Date: 	4/08/2019
|==================================================================================|
|  Description: Contains the definitions for the functions outlined in semaphore.h                     
*==================================================================================*/

#include "window.h"
#include "semaphore.h"
#include "scheduler.h"

extern semaphore sema_screen;
extern scheduler sched;
extern scheduler TCB;

void semaphore::down(){ 						// Lock resource
		
		scheduler::TCB * tcb; // new tcb object
		
		if (sema_value > 0){ // if no 1 else has access
			sema_value--;	// get access here
		} else{ // if someone else has access then we need to queue and block ourselves
			tcb = sched.process_table.Dequeue(); 
			tcb->state = 0; // blocked
			sched.process_table.Enqueue(tcb);  //add back to process table
			sema_data.Enqueue(tcb); // stick in sema queue waitlist
			
			for (int i = 0; i < sched.process_table.qSize() -1 ; i++)
			{
				tcb = sched.process_table.Dequeue();
				sched.process_table.Enqueue(tcb); //shuffle queue
			}
		}
			
} // end of down		

void semaphore::up(){		 										// Release the resource

		if (sema_data.isEmpty()){ // if no one is in line then set value back to 1
			sema_value++;
		} else {
			scheduler::TCB * tcb = sema_data.Dequeue(); // get first in semaphore queue
			scheduler::TCB * tcb2; // new tcb for comparison
			for (int i = 0; i < sched.process_table.qSize() ; i++)
			{
				tcb2 = sched.process_table.Dequeue(); //get scheduler task
				if (tcb->thread_no == tcb2->thread_no){ // if task id in scheduler matches the one in semaphore
					tcb2->state = 1;  //ready // change it to ready from blocked
				}
				sched.process_table.Enqueue(tcb2); //shuffle queue
			}
		}
} // end of up				

void semaphore::dump(int level, WINDOW * win){
								// giving semaphore a resource name

	char buff[256];


	sprintf(buff, " Resource: %s Sema Value: \t%d \n", resource_name.c_str(), sema_value); // Print the resource name of the current semaphore
	write_window(win, buff);
	
	std::string name;
	if (sema_data.qSize() > 0){
		scheduler::TCB * tcb;
		
		for (int i = 0 ; sema_data.qSize(); i++){
			tcb = sema_data.Dequeue();
			name = tcb->name;
			sema_data.Enqueue(tcb);
			sprintf(buff, "%s -> ", name.c_str());
			write_window(win, buff);
		}
	} else{
			write_window(win, " \tSema Queue empty\n");
	}
} // end of dump

semaphore::semaphore(int sema_value, char* sema_name){
	count = 0;
	this->sema_value = sema_value;	
	this->resource_name = sema_name;
}

semaphore::~semaphore(){
}
