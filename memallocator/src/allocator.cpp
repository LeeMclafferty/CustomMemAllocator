#include <iostream>
#include <windows.h>

// LPVOID VirtualAlloc(
// 	[in, optional] LPVOID lpAddress,
// 	[in]           SIZE_T dwSize,
// 	[in]           DWORD  flAllocationType,
// 	[in]           DWORD  flProtect
// );

// Reserved Memory - Memory that has been marked for future use but is not yet usable.
// Committed Memory Memory that is actually usable by your program.

int main() {
	void* reserved_memory = VirtualAlloc(NULL, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	std::cout << "Allocated Address: " << reserved_memory << std::endl;

	VirtualFree(reserved_memory, 0, MEM_RELEASE);
	std::cout << "After Free: " << reserved_memory << std::endl;

	return 0;
}