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
// Committed Memory - Memory that is actually usable by your program.
namespace allocator {

	struct MemBlock {
		MemBlock() {}
		MemBlock(size_t s, void* ad, MemBlock* p, MemBlock* n, bool f)
			:size(s), address(ad), previous(p), next(n), free(f) {
		}

		void* address;
		MemBlock* previous;
		MemBlock* next;
		size_t size;
		bool free = false;
	};

	MemBlock* head = nullptr;
	static int active_allocations = 0;

	/*
	Parameters
		args - object constructor arguments.
	*/
	template <typename T, typename... Args>
	T* request_mem(Args&&... args) {

		// Check for an already freed block before allocating more memory. 
		MemBlock* buffer = head;
		while (buffer) {
			if (buffer->free) {
				if (buffer->size >= sizeof(T)) {
					T* obj = new(buffer->address) T(std::forward<Args>(args)...);
					buffer->free = false;
					++active_allocations;
					return obj;
				}
			}

			buffer = buffer->next;
		}

		void* address = VirtualAlloc(nullptr, sizeof(T), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		T* obj = new(address) T(std::forward<Args>(args)...);
		MemBlock* block = new MemBlock(sizeof(T), address, nullptr, head, false);
		if (head) {
			head->previous = block;
		}
		head = block;

		++active_allocations;
		return obj;
	}

	/*
	 Parameters
		ptr - reference to the pointer that will be freed.
		free_block - Default value is false, which will delete the block from memory.
		if free_block is set to true, the block will remain in memory as a "free block" for
		reuse.
	*/
	template<typename T>
	void free_mem(T*& ptr, bool free_block = false) {
		MemBlock* buffer = head;
		while (buffer) {
			if (buffer->address == ptr) {
				// Fix the linked list and delete the node
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
					ptr = nullptr;

					delete buffer;
					--active_allocations;
					break;
				}
				// Mark the block as free to reuse.
				else
				{
					static_cast<T*>(buffer->address)->~T();
					ptr = nullptr;
					buffer->free = true;
					--active_allocations;
				}

			}
			buffer = buffer->next;
		}
	}
}