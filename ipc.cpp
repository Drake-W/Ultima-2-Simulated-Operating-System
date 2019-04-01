/*==================================================================================|
|   Assignment:		Ultima 2.0 Phase 3
|    File Name:		ipc.cpp
| Dependencies: 	ipc.h window.h queue.h scheduler.h semaphore.h
|      Authors: 	Drake Wood, James Giegerich
|     Language: 	C++
|     Compiler: 	G++
|        Class: 	C435 - Operating Systems
|   Instructor: 	Dr. Hakimzadeh
| Date Created: 	3/3/2019
| Last Updated:		3/18/2019
|     Due Date: 	3/18/2019
|==================================================================================|
|  Description: Contains the definitions for the functions outlined in ipc.h                     
*==================================================================================*/

#include "window.h"
#include "queue.h"
#include "scheduler.h"
#include "ipc.h"
#include "semaphore.h"

extern semaphore sema_screen;
extern semaphore sema_t1mail;	
extern semaphore sema_t2mail;	
extern semaphore sema_t3mail;	
extern semaphore sema_t4mail;	
extern semaphore sema_ptable;	
extern scheduler sched;
extern scheduler TCB;

// Sends message using destination in the message which is passed
int ipc::Message_Send( Message *message){				// returns -1 if error occurred. Return 1 if successful. 
	sema_ptable.down();
	int flag = -1;
	int dtID = message->Destination_Task_Id; // gets destination task
	scheduler::TCB * tcb;
	
	switch(dtID) // switch for using correct semaphore
	{
		case '1': 
			sema_t1mail.down();
			break;
		case '2':
			sema_t2mail.down();
			break;
		case '3':
			sema_t3mail.down();
			break;
		case '4':
			sema_t4mail.down();
		break;
	}
	
	
	for (int i = 0; i < sched.process_table.qSize(); i++){ // searches process table for the destination task
		tcb = sched.process_table.Dequeue(); 
		if (dtID == tcb->thread_no){
			
			tcb->mailbox.Enqueue(message); // enqueues the message in the tasks mailbox
			flag = 1;
		}
		sched.process_table.Enqueue(tcb);  //add back to process table.
	}
	switch(dtID) // switch for using correct semaphore
	{
		case '1': 
			sema_t1mail.up();
			break;
		case '2':
			sema_t2mail.up();
			break;
		case '3':
			sema_t3mail.up();
			break;
		case '4':
			sema_t4mail.up();
		break;
	}
sema_ptable.up();
	return flag;
}

int ipc::Message_Receive(int Task_id, Message *message){	// returns 0 if no more messages are available, loads the Message structure with 
	sema_ptable.down();										// the first message from the mailbox and remove the message from the mailbox. 
																// Return -1 if an error occurs. 
	int flag = -1;
	int dtID = message->Destination_Task_Id;
	scheduler::TCB * tcb;
	
	switch(dtID)// switch for using correct semaphore
	{
		case '1': 
			sema_t1mail.down();
			break;
		case '2':
			sema_t2mail.down();
			break;
		case '3':
			sema_t3mail.down();
			break;
		case '4':
			sema_t4mail.down();
		break;
	}
	
	for (int i = 0; i < sched.process_table.qSize(); i++){
		tcb = sched.process_table.Dequeue(); 
		if (dtID == tcb->thread_no){ // finds task
			if (!tcb->mailbox.isEmpty()){ // gets first message from its mailbox
			message = tcb->mailbox.Dequeue();
			flag = 1; // message found
			}
			else{
				flag = 0; // message not found
			}
		}
		sched.process_table.Enqueue(tcb);  //add back to process table.
	}
	
		switch(dtID)// switch for using correct semaphore
	{
		case '1': 
			sema_t1mail.up();
			break;
		case '2':
			sema_t2mail.up();
			break;
		case '3':
			sema_t3mail.up();
			break;
		case '4':
			sema_t4mail.up();
		break;
	}
	sema_ptable.up();
	return flag;
}																

int ipc::Message_Count(int Task_id){ 							// return the number of messages in Task-id’s  message queue. 
	sema_ptable.down();
	int count = 0;
	scheduler::TCB * tcb;
		
	for (int i = 0; i < sched.process_table.qSize(); i++){
		tcb = sched.process_table.Dequeue(); // find task 
		if (Task_id == tcb->thread_no){
			count = tcb->mailbox.qSize(); // gets number of messages
		}
		sched.process_table.Enqueue(tcb);  //add back to process table.
	}

	sema_ptable.up();
	return count;
}

int ipc::Message_Count(){ 										// return the total number of messages in all the message queues. 
	sema_ptable.down();
	int count = 0;
	scheduler::TCB * tcb;
	
	for (int i = 0; i < sched.process_table.qSize(); i++){ // cycle all tasks
		tcb = sched.process_table.Dequeue(); 
		count += tcb->mailbox.qSize(); // add up number of messages
		sched.process_table.Enqueue(tcb);  						//add back to process table.
	}
	sema_ptable.up();
	return count;
}

void ipc::Message_Print(int Task_id, WINDOW * Win){ 							// print the all messages for a given Task-id. 
	sema_ptable.down();
	scheduler::TCB * tcb;
	char buff[256];
	Message * mess;
	struct tm * timeinfo; // create the time data struct for extracting the format from the Message_Arrival_Time
	
	for (int i = 0; i < sched.process_table.qSize(); i++){
		tcb = sched.process_table.Dequeue(); 
		if (Task_id == tcb->thread_no){
			
		switch(Task_id)// switch for using correct semaphore
	{
		case '1': 
			sema_t1mail.down();
			break;
		case '2':
			sema_t2mail.down();
			break;
		case '3':
			sema_t3mail.down();
			break;
		case '4':
			sema_t4mail.down();
		break;
	}
			
			sprintf(buff, " Time\t\tSize\tContent\t\tDestination\tSource\n"); // collumn names for message printing
			write_window(Win, buff);
			for (int j = 0; j < tcb->mailbox.qSize(); j++){
				mess = tcb->mailbox.Dequeue();
				timeinfo = localtime (&mess->Message_Arrival_Time); // change time to local time and store in the new struct
																	// formatting can be found in time.h for day month year ect. 
				sprintf(buff, " %d:%d:%d\t%d\t%s\t%d\t\t%d \n", timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec, 
						mess->Message_Size, mess->Message_Text, mess->Destination_Task_Id, mess->Source_Task_Id);
				write_window(Win, buff);							// single line output of all message data 
				tcb->mailbox.Enqueue(mess);
			}
		}
		sched.process_table.Enqueue(tcb);  //add back to process table.
		
		switch(Task_id)// switch for using correct semaphore
	{
		case '1': 
			sema_t1mail.up();
			break;
		case '2':
			sema_t2mail.up();
			break;
		case '3':
			sema_t3mail.up();
			break;
		case '4':
			sema_t4mail.up();
		break;
	}
	}
	sema_ptable.up();
}

int ipc::Message_DeleteAll(int Task_id){ 						// delete all the messages for Task_id 
	sema_ptable.down();
	scheduler::TCB * tcb;
	
	for (int i = 0; i < sched.process_table.qSize(); i++){
		tcb = sched.process_table.Dequeue(); 
		if (Task_id == tcb->thread_no){
			
		switch(Task_id)// switch for using correct semaphore
	{
		case '1': 
			sema_t1mail.down();
			break;
		case '2':
			sema_t2mail.down();
			break;
		case '3':
			sema_t3mail.down();
			break;
		case '4':
			sema_t4mail.down();
		break;
	}
			
			while (!tcb->mailbox.isEmpty()){
				tcb->mailbox.Dequeue();
			}
			
	switch(Task_id)// switch for using correct semaphore
	{
		case '1': 
			sema_t1mail.up();
			break;
		case '2':
			sema_t2mail.up();
			break;
		case '3':
			sema_t3mail.up();
			break;
		case '4':
			sema_t4mail.up();
		break;
	}
			
		}
		sched.process_table.Enqueue(tcb);  //add back to process table.
	}
sema_ptable.up();
	return 1;
}

void ipc::ipc_Message_Dump(WINDOW * Win){ 						// print all the messages in the message queue, but do not delete them from the queue. 
	
	for (int i = 1; i < 5; i++){
			Message_Print(i, Win);
	}
}

