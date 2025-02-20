#include <iostream>
#include <windows.h>
#include <string>
#include "allocator.h"

// LPVOID VirtualAlloc(
// 	[in, optional] LPVOID lpAddress,
// 	[in]           SIZE_T dwSize,
// 	[in]           DWORD  flAllocationType,
// 	[in]           DWORD  flProtect
// );

// Reserved Memory - Memory that has been marked for future use but is not yet usable.
// Committed Memory Memory that is actually usable by your program.

class entity{
public:
	entity() 
	{};

	std::string name;
	int number;
	float other_number;

};

int main() {

	entity* e = request_mem<entity>();
	std::cout << "e address after request " << e << std::endl;
	free_mem<entity>(e, true);
	std::cout << "e address after free " << e << std::endl;

	return 0;
}