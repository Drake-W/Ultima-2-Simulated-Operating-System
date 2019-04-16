/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 4
|     File Name:	queue.cpp
|  Dependencies: 	queue.h
|       Authors: 	Drake Wood, James Giegerich
|      Language: 	C++
|      Compiler: 	G++
|         Class: 	C435 - Operating Systems
|    Instructor: 	Dr. Hakimzadeh
|  Date Created: 	2/16/2019
|  Last Updated:	3/18/2019
|      Due Date: 	4/08/2019
|==================================================================================|
|  Description: Contains the definitions for the functions outlined in queue.h                     
*==================================================================================*/

#include "queue.h"

// Constructor
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
} // end Enqueue

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
