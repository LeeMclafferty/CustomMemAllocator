#pragma once

struct MemBlock{
	void* address;
	MemBlock* previous;
	MemBlock* next;
	size_t size;
};
