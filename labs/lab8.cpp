
cout << "====Understanding your Compiler and Datatypes====" << endl;
cout << "Size of char: 					" << sizeof(char) << endl;
cout << "Size of unsigned char:			" << sizeof(unsigned char) << endl;
cout << "Size of unicode widechar:		" << sizeof(wchar_t) << endl;
cout << "Size of int: 					" << sizeof(int) << endl;
cout << "Size of float: 				" << sizeof(float) << endl;
cout << "Size of double: 				" << sizeof(double) << endl;

cout << "================Displaying Information in Diferent Formats======================" << endl;
unsigned in aNum;
aNum = 65;
cout << "------------ aNum (int) ----------------" << endl;
std::cout.flags ( std::ios::showbase);
cout << "Char format(C++)		= " << char(aNum) << endl;
cout << "Dec format (C++)		= " << std::dec << (aNum) << endl;
cout << "Oct format (C++)		= " << std::oct << (aNum) << endl;
cout << "HEX format (C++)		= " << std::hex << (aNum) << endl;
cout << "BIN format (C++)		= " << std::bitset<32>(aNum) << endl; // #include <bitset>

cout << "----- Print Using Printf -------------" << endl;
printf("Char format(C)			= %c\n",aNum); 			//C style printf printing Char number
printf("Dec format(C)			= %d\n",aNum); 			//C style printf printing decimal number
printf("Oct format(C)			= %o\n",aNum); 			//C style printf printing octal number
printf("Hex format(C)			= %x\n",aNum); 			//C style printf printing hex number

//same thing with a char
unsigned char aChar;
aChar = 'A';
cout << " ----------- aChar ------------" << endl;
//std::cout.flags ( std::ios::showbase );
cout << "char format(C++)		= " << char(aNum) << endl;
cout << "dec format(C++)		= " << int(aChar) << endl;
cout << "octal format(C++)		= " << std::oct << int(aChar) << endl;
cout << "hex format(C++)		= " << std::hex << int(aChar) << endl;
cout << "BIN format(C++)		= " << std::bitset<8>(aChar) << endl;
cout << "----------------------------------" << endl;
cout << "binary format(C++)		= " << char_to_binary(aChar) << endl;
cout << "----------------------------------" << endl;

cout << "================= Understanding Bitwise Shift Operations ====================" << endl;
unsigned char clearChar;
charChar = 0x01;

cout << "0x01 in binary format = " << char_to_binary(clearChar) << endl;

clearChar = (clearChar << 2);
cout << "Shift left 2 bits		= " << char_to_binary(clearChar) << endl;

clearChar = (clearChar >> 1);
cout << "Shift right 1 bit		= " << char_to_binary(clearChar) << endl;

clearChar = (clearChar << 6);
cout << "Shift left 6 bits		= " << char_to_binary(clearChar) << endl;

cout "===================== OR OPERATION ============================" << endl;
clearChar = 0x01;

cout << "0000 0001 | 00001100 = " << (clearChar | 0x0C ) << endl;
cout << "Hex:					" << std::hex << (learChar | 0x0C ) << endl;
cout << "Binary Result: 		" << char_to_binary(clearChar | 0x0C) << endl;

cout "===================== AND OPERATION ============================" << endl;
clearChar = 0xff;

cout << "1111 1111 | 00001100 = " << (clearChar | 0x0C ) << endl;
cout << "Hex:					" << std::hex << (learChar | 0x0C ) << endl;
cout << "Binary Result: 		" << char_to_binary(clearChar | 0x0C) << endl;

cout "===================== XOR OPERATION ============================" << endl;
clearChar = 0x09;

cout << "0000 1001 | 0000 1100 = " << (char_to_binary(clearChar | 0x0C) << endl;

cout "===================== Double XOR OPERATION ============================" << endl;

cout << "Back to the original char:" << endl;
cout << chat_to_binary(clearChar) << " ^ 0000 1100 ^ 0000 1100= " << chat_to_binary(clearChar ^ 0x0C ^ 0x0c) << endl;

cout "===================== Complement OPERATION ============================" << endl;

clearChar = 0x09;
cout << "~0000 1001 = " << chat_to_binary(~clearChar) << endl;





string char_to_binary(unsigned char value)
{
	char theResult[128];
	unsigned char mask = 0x80;
	int i;
	
	for (i = 0; i <sizeof(value) * 8; i++){
		if ((vlaue & mask) == 0)
			theResult[i] = '0';
		else
			theResult[i] = '1';
		mask >>= 1;
	}
	theResult[i] = '\0';
	string result = theResult;
	return(result);
}



