#include <Aurora/Utils/Types.h>

#include <pspkernel.h>
#include <string.h>

#define HINIBBLE(b) (((b) >> 4) & 0x0F)
#define LONIBBLE(b) ((b) & 0x0F)

NibbleArray::NibbleArray(int size)
{
	int s = size/2;
	s += size%2;
	NibbleArray::array = new char[s];
	NibbleArray::size = s;
}

NibbleArray::NibbleArray(char* tbl, int size)
{
	NibbleArray::array = new char[size];
	memcpy(NibbleArray::array, tbl, size);
	NibbleArray::size = size;
}

NibbleArray::~NibbleArray()
{
	delete [] NibbleArray::array;
}

char NibbleArray::operator[](int el)
{
	int elm = el/2;
	if(el%2)
	{
		return HINIBBLE(NibbleArray::array[elm]);
	
	}
	else
	{
		return LONIBBLE(NibbleArray::array[elm]);
	}

}
