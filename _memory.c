#include "_memory.h"
#include <stdlib.h>

char* _alloc(int size)
{
	return malloc(size);
}

void _dealloc(void* ptr)
{
	free(ptr);
}
