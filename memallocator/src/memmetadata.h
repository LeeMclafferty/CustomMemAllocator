#pragma once

struct MemData{
	MemData* previous;
	MemData* next;
	unsigned int size;
};
