#include <fstream> //to bea able to use file i/o
#include <iostream> // to be able to use cout
#include <string.h>

using namespace std;

void random_file_write(void);
void random_file_read(void);
void random_file_read_write_1(void);
void random_file_read_write_2(void);
//------------------------------------------------------------
int main(){
	random_file_write();
	random_file_read();
	
	random_file_read_write_1();
	random_file_read_write_2();
	
	cout << endl;
	getchar();
	return 0;
}

//-------------------------------------------------------------
void random_file_write(void)
{
	chat myString[80];
	int strSize;
	cout << "Opening the file for write.." << endl;
	
	strcpy(myString, "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n0123456789\nabcdefghijklmnopqrstuvwxyz\b");
	strSize = strlen(myString);
	
	cout << "String being written to file: \n" << myString << endl;
	cout << "String size : " << strSize << endl;
	
	cout << "Now writing to the file....." << endl;
	myfile.seekp(0);					//place the file pointer at the beginning of the file
	myfile.write(myString,strSize); 			//write the string
	myfile << endl; 		// write using << operator
	myfile << "Another line" << endl;
	
	myfile.seekp(38);			//place the file pointer at the beginning of the file
	
	//overwrite the first 10 lowercase letter with uppercase
	for(int i =65; i < 65 + 10; i++)
		myfile.put(i)
	
	myfileseekp(100);			// place the file pointer at location 70
	
	myfile << "Writing to Location 100.... and beyond";
	
	myfile.close();
}

void random_file_read(void)
{
	cout << "Opening the file for read" << endl;
}