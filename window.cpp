/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 4
|     File Name:	window.cpp
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
|  Description: Contains the definitions for the functions outlined in window.h                     
*==================================================================================*/

#include "window.h"
#include "semaphore.h"
#include "ipc.h"
#include "time.h"
#include "memory.h"
#include <cstring>

extern semaphore sema_screen;
extern semaphore sema_t1mail;	
extern semaphore sema_t2mail;	
extern semaphore sema_t3mail;	
extern semaphore sema_t4mail;	
extern semaphore sema_ptable;	
extern scheduler sched;
extern ipc IPC;
extern mem_mgr Mem_Mgr;

WINDOW *create_window(int height, int width, int starty, int startx)
{
	sema_screen.down();
	WINDOW *Win;
	Win = newwin(height, width, starty, startx);
	scrollok(Win, TRUE); 		// Allow scrolling of the window
	scroll(Win); 						// scroll the window
	box(Win, 0 , 0); 				// 0, 0 gives default characters for the vertical and horizontal lines
	wrefresh(Win); 					// draw the window
	sema_screen.up();
	
	return Win;
} // end of create_window

void write_window(WINDOW * Win, const char* text)
{
	sema_screen.down();
	wprintw(Win, text);
	box(Win, 0 , 0);
	wrefresh(Win); 				// Draw the window
	sema_screen.up();
 } // end of write_window

void write_window(WINDOW * Win, int y, int x, const char* text)
{
	sema_screen.down();
	mvwprintw(Win, y, x, text);
	box(Win, 0 , 0);
	wrefresh(Win); 				// Draw the window
	sema_screen.up();
} // end of write_window

void display_help(WINDOW * Win)
{
	sema_screen.down();
	wclear(Win);
	sema_screen.up(); 					// Write window already has its own lock
	write_window(Win, 1, 1, "1: Kill Task 1");
	write_window(Win, 2, 1, "2: Kill Task 2");
	write_window(Win, 3, 1, "3: Kill Task 3");
	write_window(Win, 4, 1, "c: Clear Screen");
	write_window(Win, 5, 1, "d: Pause + Dump");
	write_window(Win, 6, 1, "h: Help Screen");
	write_window(Win, 7, 1, "q: Quit");
	write_window(Win, 8, 1, "g: Garbage Collect");
	write_window(Win, 9, 1, "z: Message testing");
	
 } // end of display_help
 
void *perform_simple_output(void *arguments)
{
	scheduler::TCB * tcb = (scheduler::TCB *) arguments;	// Extract the thread arguments: (method 1)
	int thread_no = tcb->thread_no;						// Cast arguments in to thread_data
	WINDOW * Win = tcb->thread_win;
	WINDOW * pdumpwin = tcb->pdumpwin;
	int CPU_Quantum =0;
	int yield_quantum = 0;
	char buff[256];
	time_t messagetime;
	char write;
	char read = '=';
	unsigned seed= time(0);
    srand(seed);
	
	

		
	while (tcb->state != 3){ // not dead
		while(tcb->state == 2) { // running
			if(yield_quantum == 0){ // updates process table when thread gets cpu time
				//sched.dump(1, pdumpwin);
			}
			
			if (CPU_Quantum == 0) { // First thing a task does is send messages
				for (int i = 1 ; i < 4; i++){
					ipc::Message * message = new ipc::Message; // create message 
					message->Source_Task_Id = thread_no; // source = this task
					message->Destination_Task_Id = i; // destination is every task
					message->Message_Text = "message text"; // placeholder text 
					time(&messagetime); // get time 
					message->Message_Arrival_Time = messagetime; // store time
					message->Message_Size = sizeof(message); // get and store size of message
					if (IPC.Message_Send(message) == 1){ // send the message
						sprintf(buff, " Message sent\n");
						write_window(Win, buff);
					}
				}
			}

		
			if(tcb->kill_signal !=1){ //for some reason we cant die before printing or get corruption for now
				sprintf(buff, " Task-%d running #%d\n", thread_no, CPU_Quantum++);
				write_window(Win, buff);
				
				//writes a random piece of data to memory at the beginning of yield cycle
				if(yield_quantum == 1){
					write = '0' + rand()%77;
					Mem_Mgr.Mem_Write(tcb->memhandle,write); // write to memory once per yield cycle
				}

				
			}
			yield_quantum++;
			if (tcb->kill_signal == 1){ // set to be killed
					write_window(Win, " I'm dying...\n"); 
					tcb->state = 3;
					sched.yield();
				} 
			if (yield_quantum == 1001){// if quantum is up 
					yield_quantum = 0; //reset
					// reads 1 ch of memory and prints at the end of yield cycle
					if(Mem_Mgr.Mem_Read(tcb->memhandle,&read)){
						sprintf(buff," Reading from memory... \n    %c\n", read);
						write_window(Win, buff); 
					}
					write_window(Win, " I'm yielding...\n"); 
					sched.yield();
					//sleep(1);
				}
			}
			
		} // end while
		Mem_Mgr.Mem_Free(tcb->memhandle);
	return 0;
}

void *ui_loop(void *arguments)
{
	scheduler::TCB * tcb = (scheduler::TCB *) arguments;	// Extract the thread arguments: (method 1)
						// Cast arguments in to tcb
	WINDOW * pdumpwin = tcb->pdumpwin;
	WINDOW * sdumpwin = tcb->sdumpwin;
	WINDOW * conwin = tcb->conwin;
	WINDOW * logwin = tcb->logwin;
	WINDOW * messwin = tcb->messwin;
	WINDOW * memwin = tcb->memwin;
	char* text= "This example shows the overloaded write/read function";
	int textsize = strlen(text);
	char* read = "reading didnt work";
	char buff[256];
	
		while (tcb->state != 3){ // not dead
		while(tcb->state == 2) { // running
			 // updates process table when thread gets cpu time, removed for now
			 //sched.dump(1, pdumpwin);
			
			switch(wgetch(conwin))
		{
			case '1':
					write_window(conwin, "1 \n Ultima # ");
					if (sched.destroy_task(1)){
						write_window(logwin, " Task 1 killed. \n");
					}else{
						write_window(logwin, " Task 1 was already dead... \n ");
					}
					sched.yield();
					break;
			case '2':
					write_window(conwin, "2 \n Ultima # ");
					if (sched.destroy_task(2)){
						write_window(logwin, " Task 2 killed. \n");
					}else{
						write_window(logwin, " Task 2 was already dead... \n ");
					}
					break;
			case '3':
					write_window(conwin, "3 \n Ultima # ");
					if (sched.destroy_task(3)){
						write_window(logwin, " Task 3 killed. \n");
					}else{
						write_window(logwin, " Task 3 was already dead... \n ");
					}
					break;
			case 'c':				// CLEAR and Coalesce
				
				sema_screen.down();
				refresh(); 			// Clear the entire screen (in case it is corrupted)
				wclear(conwin); 	// Clear the Console window
				sema_screen.up();
					
				Mem_Mgr.Mem_Coalesce(); 
				
				write_window(conwin, 1, 1, "Ultima # ");
				break;
			case 'd':
				write_window(conwin, "d \n Ultima # ");
				write_window(logwin, " Paused, press any key to continue... \n");
				//sched.pause();
				sched.dump(1, pdumpwin); 	// updates process dump windows
				
				sema_screen.down();
				refresh(); 			// Clear the entire screen (in case it is corrupted)
				wclear(sdumpwin); 	// Clear the sema dump window
				wclear(messwin); 	// Clear the message dump window
				sema_screen.up();
				
				write_window(sdumpwin, 1, 5, "        SEMAPHORE DUMP \n -------------------------------------\n");
				sema_screen.dump(1, sdumpwin);
				sema_t1mail.dump(1, sdumpwin);
				sema_t2mail.dump(1, sdumpwin);
				sema_t3mail.dump(1, sdumpwin);
				sema_t4mail.dump(1, sdumpwin);
				sema_ptable.dump(1, sdumpwin);
					
				//core dump	
				Mem_Mgr.Core_Dump(memwin);	
				sprintf(buff, " memory largest: %d smallest: %d left: %d \n", Mem_Mgr.Mem_Largest(), Mem_Mgr.Mem_Smallest(), Mem_Mgr.Mem_Left());
				write_window(logwin, buff);
					
				// overloaded read does not work
				/*	
				if(Mem_Mgr.Mem_Read(tcb->memhandle,9, 20, &read)){
						sprintf(buff," Reading from memory... \n    %s\n", read);
						write_window(logwin, buff); 
					}
				*/
					
				write_window(messwin, 1, 5, "	     MESSAGING DUMP \n -------------------------------------\n");	
				IPC.ipc_Message_Dump(messwin);
				std::cin.get();
				write_window(logwin, " Unpaused... \n");
				break;
			case 'h':					// HELP and mem usage
				
				display_help(conwin);
				
				Mem_Mgr.Mem_Usage(memwin);
				
				break;
			case 'g':
				sched.garbage_collect();
				write_window(conwin, "g \n Ultima # ");
				write_window(logwin, " Garbage collect\n"); 
				//sched.yield();
				break;
			case 'q':					// QUIT
				write_window(logwin," Quiting the main program....\n" );
				for (int i = 1; i < 4; i++){
					if (sched.destroy_task(i)){		// Killed
						sprintf(buff, " Task %d to be killed\n", i);
						write_window(logwin, buff);
					}else{									// Already dead
						sprintf(buff, " Task %d already dead\n", i);
						write_window(logwin, buff);
						} // end else
					} // end if
					tcb->kill_signal = 1;
					sched.yield();
				break;
			case 'z': // message test case
				IPC.Message_Print(1, messwin); // print messages
				sprintf(buff, " # of messages in task1 box = %d \n" , IPC.Message_Count(1));
				write_window(messwin, buff);
				sprintf(buff, " # of messages in all boxes = %d \n" , IPC.Message_Count());
				write_window(messwin, buff);
				sprintf(buff, " Deleting messages in task 1 box \n");
				write_window(messwin, buff);
				IPC.Message_DeleteAll(1);
				sprintf(buff, " # of messages in task1 box = %d \n" , IPC.Message_Count(1));
				write_window(messwin, buff);
				sprintf(buff, " # of messages in all boxes = %d \n" , IPC.Message_Count());
				write_window(messwin, buff);
				
				
				Mem_Mgr.Mem_Write(tcb->memhandle, 9, textsize, text);
					 
				break;
			case ERR:	// If wgetch() return ERR, that means no keys were pressed
				if (tcb->kill_signal == 1){ // set to be killed
					write_window(logwin, " UI window dying...\n");
					Mem_Mgr.Mem_Free(tcb->memhandle);
					// free this memory if thread is dying
					tcb->state = 3;
				} else {
			//write_window(logwin, " NO INPUT, UI yielding...\n");
					sched.yield();
				}
				
				break;
			default:
				write_window(conwin, "\n -Invalid Command\n");
				write_window(logwin, " -Invalid Command\n");
				write_window(conwin, " Ultima # \n");
				
			break;
		} // end switch
			
		if (tcb->state == 3){	
		write_window(logwin," Ultima 2.0 shutting down...\n" );
		sched.garbage_collect();
		}
	} // while running

} // while not dead
	
	return 0;

}

