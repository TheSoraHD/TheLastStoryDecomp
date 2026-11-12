#ifndef STD_MSL_SIZE_H
#define STD_MSL_SIZE_H

// size_t of MSL: unsigned long. Used where the type has to be exactly size_t (the size argument of
// operator new) or as wide as a pointer (the words of std::basic_string). A typedef mangles as its
// type (Ul), so the output does not change; a port to an LP64/LLP64 target only has to change this
// typedef (size_t is unsigned long long on 64-bit Windows).
typedef unsigned long __msl_size_t;

#endif
