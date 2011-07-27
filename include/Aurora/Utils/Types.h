#ifndef Types_H
#define Types_H

class NibbleArray
{
	public:
		char* array;
		int size;
		NibbleArray(char* tbl, int size);
		NibbleArray(int size);
		~NibbleArray();
	
		char operator[](int element);
};


#endif
