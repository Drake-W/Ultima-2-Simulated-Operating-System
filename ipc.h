/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 4
|     File Name:	ipc.h
|  Dependencies: 	none
|       Authors: 	Drake Wood, James Giegerich
|      Language: 	C++
|      Compiler: 	G++
|         Class: 	C435 - Operating Systems
|    Instructor: 	Dr. Hakimzadeh
|  Date Created: 	3/3/2019
|  Last Updated:	3/18/2019
|      Due Date: 	4/08/2019
|==================================================================================|
|  Description: This is the header file which defines the class ipc.
|				This class contains 7  funtions and a message structure.
|				-Message_Send takes a message as parameter and delivers it to destination.
|				-Message_Receive reads first message in mailbox for given task.
|				-Message_Count (single) returns the number of messages for given task.
|				-Message_Count (all) returns total number of messages in all mailboxes.
|				-Message_Print prints all messages for given task.
|				-Message_DeleteAll delets all messages for given task. 
|				-ipc_Message_Dump prints all messages for all mailboxes.
*==================================================================================*/

#ifndef IPC_H
#define IPC_H

 class ipc { 
 public:
	struct Message { 
		int Source_Task_Id; 
		int Destination_Task_Id; 
		time_t Message_Arrival_Time; // research time.h, time_t, and tm  
		int   Message_Size;
		char *Message_Text;
		//etc. 
	} ;
	int Message_Send(Message *message);					// returns -1 if error occurred. Return 1 if successful. 
	int Message_Receive(int Task_id, Message *message);	// returns 0 if no more messages are available, loads the Message structure with 
																// the first message from the mailbox and remove the message from the mailbox. 
																// Return -1 if an error occurs. 
	int Message_Count(int Task_id); 							// return the number of messages in Task-id’s  message queue. 
	int Message_Count(); 										// return the total number of messages in all the message queues. 
	void Message_Print(int Task_id, WINDOW * Win); 							// print the all messages for a given Task-id. 
	int Message_DeleteAll(int Task_id); 						// delete all the messages for Task_id 
	void ipc_Message_Dump(WINDOW * Win); 									// print all the messages in the message queue, but do not delete them from the queue. 
																// (note that this function may be best placed in the scheduler!) 
};
#endif	