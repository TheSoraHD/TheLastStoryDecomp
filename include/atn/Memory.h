#ifndef ATN_MEMORY_H
#define ATN_MEMORY_H

// inferred: engine allocation operator (0x80084320). Called as
// new (heap, name, file, line) T(...); release builds pass "" for both strings and 0 as line.
void* operator new(unsigned long size, int heap, const char* name, const char* file, int line);

#endif
