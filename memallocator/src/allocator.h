#pragma once
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

struct MemBlock {
	MemBlock() {}
	MemBlock(size_t s, void* ad, MemBlock* p, MemBlock* n, bool f)
		:size(s), address(ad), previous(p), next(n), free() {}

	void* address;
	MemBlock* previous;
	MemBlock* next;
	size_t size;
	bool free = false;
};

MemBlock* head = nullptr;

void* request_mem(size_t size) {
	MemBlock* buffer = head;
	while (buffer) {
		if (!buffer->free) {
			continue;
		}

		if (buffer->size == size) {
			return buffer->address;
		}
	} 

	void* address = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	MemBlock* block = new MemBlock(size, address, nullptr, head, false);
	if (head) {
		head->previous = block;
	}
	head = block;

	return block->address;
}

/*
 Parameter
	ptr - memory address that you want to free
	free_block - Default value is false, which will delete the block from memory.
	if free_block is set to true, the block will remain in memory as a "free block" for 
	reuse. 
*/
void free_mem(void* ptr, bool free_block = false) {
	MemBlock* buffer = head;
	while (buffer) {
		if (buffer->address == ptr) {
			if (!free_block) {
				if (buffer->previous) {
					buffer->previous->next = buffer->next;
				}
				if (buffer->next && buffer->next->previous) {
					buffer->next->previous = buffer->previous;
				}

				if (buffer == head) {
					head = buffer->next;
				}
	
				VirtualFree(ptr, 0, MEM_RELEASE);
				delete buffer;
				break;
			}
			else
			{
				buffer->free = true;
			}
	
		}
		buffer = buffer->next;
	}
}
