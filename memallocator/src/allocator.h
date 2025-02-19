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
	MemBlock(size_t s, void* ad, MemBlock* p, MemBlock* n)
		:size(s), address(ad), previous(p), next(n) {}

	void* address;
	MemBlock* previous;
	MemBlock* next;
	size_t size;
};

MemBlock* head = nullptr;

void* request_mem(size_t size) {
	void* address = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	MemBlock* block = new MemBlock(size, address, nullptr, head);
	if (head) {
		head->previous = block;
	}
	head = block;
}

void free_mem(void* ptr) {
	MemBlock* buffer = head;
	while (buffer) {
		if (buffer->address == ptr) {
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
		buffer = buffer->next;
	}
}
