/*==================================================================================|
|    Assignment:	Ultima 2.0 Phase 2
|     File Name:	window.h
|  Dependencies: 	none
|       Authors: 	Drake Wood, James Giegerich
|      Language: 	C++
|      Compiler: 	G++
|         Class: 	C435 - Operating Systems
|    Instructor: 	Dr. Hakimzadeh
|  Date Created: 	2/16/2019
|  Last Updated:	3/18/2019
|      Due Date: 	3/18/2019
|==================================================================================|
|  Description: This is the header file which defines the class window. There are 5 functions.
|				-create_window draws a new window at a specified size and position, it also turns on scrolling. 
|				-write_window sends text to a specific window either with a position or without.
|				-display_help prints a help window with console options for the user. 
|				-perform_simple_output is the function that the thread will run while alive, it prints an incrementing message.
*==================================================================================*/

#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <pthread.h> 	// Needed for using the pthread library
#include <assert.h>
#include <time.h>
#include <unistd.h> 	// Needed for sleep()
#include <ncurses.h> 	// Needed for Curses windowing
#include <stdarg.h> 	// needed for formatted output to window
#include <termios.h>
#include <fcntl.h>
#include <cstdlib>

WINDOW *create_window(int height, int width, int starty, int startx);
void write_window(WINDOW * Win, const char* text);
void write_window(WINDOW * Win, int x, int y, const char* text);
void display_help(WINDOW * Win);
void *perform_simple_output(void *arguments);
void *ui_loop(void *arguments);

#endif