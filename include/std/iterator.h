#ifndef STD_ITERATOR_H
#define STD_ITERATOR_H

namespace std {

// inferred: MSL iterator tags; they derive from each other, so a temporary tag is not
// zero-initialised (0x80014168 copies an uninitialised byte)
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

} // namespace std

#endif
