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

// Standard definitions (needed by boost::iterator_facade, boost::tokenizer)
template <class Category, class T, class Distance = long, class Pointer = T*, class Reference = T&>
struct iterator {
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
    typedef Category iterator_category;
};

template <class Iterator>
struct iterator_traits {
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::iterator_category iterator_category;
};
template <class T>
struct iterator_traits<T*> {
    typedef long difference_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef random_access_iterator_tag iterator_category;
};
template <class T>
struct iterator_traits<const T*> {
    typedef long difference_type;
    typedef T value_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef random_access_iterator_tag iterator_category;
};

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for (; first != last; ++first) {
        ++n;
    }
    return n;
}
template <class T>
inline long distance(T* first, T* last) {
    return last - first;
}
template <class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n) {
    for (; n > 0; --n) {
        ++i;
    }
}

} // namespace std

#endif
