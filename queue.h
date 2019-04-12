/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 3
|     File Name:	queue.h
|  Dependencies: 	none
|       Authors: 	Drake Wood, James Giegerich
|      Language: 	C++
|      Compiler: 	G++
|         Class: 	C435 - Operating Systems
|    Instructor: 	Dr. Hakimzadeh
|  Date Created: 	2/16/2019
|  Last Updated:	3/18/2019
|      Due Date: 	4/08/2019
|==================================================================================|
|  Description: This is the header file which defines the queue template class used by the semaphore
|				and scheduler classes. 
*==================================================================================*/

#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <pthread.h> // Needed for using the pthread library
#include <assert.h>
#include <time.h>
#include <unistd.h> // Needed for sleep()
#include <ncurses.h> // Needed for Curses windowing
#include <stdarg.h> // needed for formatted output to window
#include <termios.h>
#include <fcntl.h>
#include <cstdlib>

template <class T>
	class Queue{
		private:
			struct node{
				T data;
				node* next;
				};
			node* head;
			node* tail;
			node* temp;
 
		public:
			Queue();
			void Enqueue(const T data);
			T Dequeue();
			bool isEmpty();
			int qSize();
			T qnext();
};


// Constructor.
template <class T>
Queue<T>::Queue(){
	head = NULL;
	tail = NULL;
	temp = NULL;
}

template <class T>
int Queue<T>::qSize(){
	int count = 0;
	temp = head;
	
	if (isEmpty()){
		return count;	
	} // end if
	else if (temp->next == NULL){
		count++;
		return count;
	} // end if
	else{
		while (temp != NULL){
			count++;
			temp = temp->next;
		} // end while
		
		return count;
	} // end else
} // end of qSize

//get data of next item, will probably be able to use this in yield
//so that we can pass the cpu to the next ID in the QUIT
//also in semaphore dump to show the Q there 
template <class T>
T Queue<T>::qnext(){
	T returnData;
	returnData = head->next->data;
	return returnData;
}

// Enqueues new node and populates the data
// with T data as passed in.
template <class T> 
void Queue<T>::Enqueue(const T data){
	if (tail == NULL){
		head = tail = new node;
		tail->next = NULL;
		tail->data = data;
	} // end if
	else {
	tail->next = new node;
	tail->next->data = data;
	tail->next->next = NULL;
	tail = tail->next;
	} // end else
} // end of Enqueue

// Dequeues node and returns data stored
// in dequeued node.
template <class T>
T Queue<T>::Dequeue(){
	T returnData;
	returnData = head->data;
	temp = head->next;
	delete head;
	head = temp;
	tail = (!head? NULL: tail);
	return returnData;
}

// Checks if the queue is empty.
template <class T>
bool Queue<T>::isEmpty(){
  return (!tail);
}

#endif 