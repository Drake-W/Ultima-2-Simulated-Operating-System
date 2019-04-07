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
	char myString[80];
	int strSize;
	cout << "Opening the file for write.." << endl;
	
	fstream myfile("random_file_1.txt", ios::out);

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
		myfile.put(i);
	
	myfile.seekp(100);			// place the file pointer at location 70
	
	myfile << "Writing to Location 100.... and beyond";
	
	myfile.close();
}

void random_file_read(void)
{
	cout << "Opening the file for read" << endl;
	fstream myfile("random_file_1.txt", ios::in);

	// Go to the end of file
	myfile.seekg(0, std::ios_base::end); // Position eithin the stream + offset (if provided).

	// Get the location of the file pointer
	int fileSize = myfile.tellg();

	cout << "fileSize = " << fileSize << endl;

	char c;
	cout << "Go to the beginning and reaed all the chars in the file" << endl;
	myfile.seekg(0);
	for (int i= 0; i < fileSize; i++){
		myfile.get(c);
		cout << c;
	}
	cout << endl;

	cout << "Read the last chat from the file" << endl;
	myfile.seekg(fileSize-1);	// Read the last char in the file
	myfile.get(c);
	cout << c << endl;
	cout << "Read the first char from the file" << endl;
	myfile.seekg(0);
	myfile.get(c);
	cout << c << endl;

	cout << "Read the file Backward!" << endl;
	for (int fp=fileSize-1; fp >= 0; fp--){
		myfile.seekg(fp, std::ios_base::beg);	// move the file pointer back
		myfile.get(c);

		cout << "fp=" << fp << " "<< c << endl;
	}

	myfile.close();
}

//--------------------------------------------------------------------------------
void random_file_read_write_1(void)
{
	char myString[80];
	int strSize;

	//==========================================================================
	//  check to see if the file exists
	fstream myfile("random_file_2.txt", ios::in);
	if (myfile.fail()){
		// if the file does not exist first then create it
		fstream myfile("my_random_file_2.txt", ios::out);
	}
	myfile.close();
	//==========================================================================

	cout << "Opening the file for both read and write.." << endl;
	myfile.open("random_file_2.txt", ios::in | ios::out );

	if (!myfile.is_open()){
		cout << "File is not open!!!" << endl;
	}
	else {
		//Go to beginning of file
		myfile.seekp(0);
		myfile << "Record 01.";
		myfile.seekp(20);
		myfile << "Record 03.";
		myfile.seekp(40);
		myfile << "Record 05.";

		// Now read the file from the beginning...
		myfile.seekg(0); // Position within the stream + offset (if provided)
		char c;

		myfile.get(c);
		while (!myfile.eof()){
			cout << c;
			myfile.get(c);
		}
	}

	myfile.close();
}

//-----------------------------------------------------------------------
void random_file_read_write_2(void)
{
	char myString[80];
	int strSize;

	//=====================================================
	// check to see if the file exists
	fstream myfile("my_random_file_2.txt", ios::in);
	if (myfile.fail()){
		// if the file does not exist first create it.
		fstream myfile("my_random_file_2.txt", ios::out);
	}
	myfile.close();
	//=====================================================
	cout << "Opening the file for both read and write.." << endl;
	myfile.open("my_random_file_2.txt", ios::in | ios::out);

	if (!myfile.is_open()){
		cout << "File is not open!!" << endl;
	}
	else {
		myfile.seekp(10); 
		myfile << "Record 02.";
		myfile.seekp(30);
		myfile << "Record 04.";
		myfile.seekp(60);
		myfile << "Record 07.";
		// Now read the file from the beginning...
		myfile.seekg(0);
		char c;

		myfile.get(c);
		while (!myfile.eof()){
			cout << c;
			myfile.get(c);
		}
	}
	myfile.close();
}
