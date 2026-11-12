#ifndef GAME_MSLSORT_UNK_H
#define GAME_MSLSORT_UNK_H

// inferred: MSL std::sort of a std::vector with a class iterator, as instantiated by the camp and costume
// menus (game/CosEditItemList_unk.cpp, game/NewCampPartsItem.cpp). The other units of the game that sort
// raw arrays have their own copies (game/CharaTask.cpp, game/GameTask.cpp, atn/Loader.cpp). Names invented.

// inferred: random access iterator of std::vector (a class holding the element pointer: std::sort gets it by
// address and calls its operators out of line when it is over the inlining budget, e.g. the weak copies
// 0x801DBAC0-0x801DC07C of game/CosEditItemList_unk); names invented
template <class T>
struct MslIter_unk {
    MslIter_unk(T* const& p) : mPtr(p) {}
    MslIter_unk& operator=(const MslIter_unk& other) {
        mPtr = other.mPtr;
        return *this;
    }
    T& operator*() const { return *mPtr; }
    MslIter_unk& operator++() {
        mPtr++;
        return *this;
    }
    MslIter_unk& operator--() {
        mPtr--;
        return *this;
    }
    T* operator+(int n) const { return mPtr + n; }
    T* operator-(int n) const { return mPtr + -n; }
    T* mPtr;
};
template <class T>
inline int operator-(const MslIter_unk<T>& a, const MslIter_unk<T>& b) {
    return a.mPtr - b.mPtr;
}
template <class T>
inline bool operator==(const MslIter_unk<T>& a, const MslIter_unk<T>& b) {
    return a.mPtr == b.mPtr;
}
template <class T>
inline bool operator!=(const MslIter_unk<T>& a, const MslIter_unk<T>& b) {
    return a.mPtr != b.mPtr;
}
template <class T>
inline bool operator<(const MslIter_unk<T>& a, const MslIter_unk<T>& b) {
    return a.mPtr < b.mPtr;
}
template <class T>
inline bool operator>=(const MslIter_unk<T>& a, const MslIter_unk<T>& b) {
    return a.mPtr >= b.mPtr;
}

template <class T>
inline void MslSwap_unk(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

// inferred: comparison by reference in the recursive copies of the sort (as game/CharaTask.cpp)
template <class T>
struct MslRef_unk {
    typedef T& type;
};
template <class T>
struct MslRef_unk<T&> {
    typedef T& type;
};

// inferred: median of three of the quicksort below (MSL __sort; see game/CharaTask.cpp)
// NONMATCHING: 86-98% (registers); 32-40% when the original calls the iterator operators out of line
template <class Iter, class Compare>
void MslMedian3_unk(Iter a, Iter b, Iter c, Compare& comp) {
    bool ca = !comp(*c, *a);
    bool bc = !comp(*b, *c);
    if (ca && bc) {
        return;
    }
    if (!ca && !bc) {
        MslSwap_unk(*a, *b);
    } else {
        if (comp(*b, *a)) {
            MslSwap_unk(*a, *b);
        }
        if (ca) {
            MslSwap_unk(*b, *c);
        } else {
            MslSwap_unk(*a, *c);
        }
    }
}

// inferred: selection sort of the short ranges (MSL __selection_sort)
template <class Iter, class Compare>
inline void MslSelectionSort_unk(Iter first, Iter last, Compare& comp) {
    if (first != last) {
        for (Iter end(last - 1); first != end; ++first) {
            Iter min(first);
            if (first != last) {
                Iter it(first + 1);
                for (; it != last; ++it) {
                    if (comp(*it, *min)) {
                        min = it;
                    }
                }
            }
            if (min != first) {
                MslSwap_unk(*min, *first);
            }
        }
    }
}

// inferred: MSL __sort of the lists (quicksort with a selection sort below 21 elements, the pivots move with
// a counter of each instantiation; as game/CharaTask.cpp Sort_unk)
// 100% in ShopBase and NewCampPartsItem (CategoryLess); elsewhere NONMATCHING: 78-96% with inline predicates
// (registers and scheduling); 0% when the original is over the inlining budget and calls the iterator
// operators, the selection sort and the median out of line (here they are inlined); see
// game/CosEditItemList_unk.cpp and game/NewCampPartsItem.cpp
template <class Iter, class Compare>
void MslSort_unk(Iter first, Iter last, Compare comp) {
    typedef typename MslRef_unk<Compare>::type CompareRef;
    static int sOffset = -4; // inferred: one per instantiation (lbl_8087DA70..), -4 to 4
    while (true) {
        int n = last - first;
        if (n <= 1) {
            return;
        }
        if (n <= 20) {
            MslSelectionSort_unk(first, last, comp);
            return;
        }
        Iter i(first + (n / 4 + sOffset % 5));
        if (++sOffset >= 5) {
            sOffset = -4;
        }
        int n3 = n * 3 / 4;
        Iter j(first + (n3 + sOffset % 5));
        if (++sOffset >= 5) {
            sOffset = -4;
        }
        Iter pivot(last - 1);
        MslMedian3_unk(i, j, pivot, comp);
        i = first;
        j = pivot;
        while (comp(*i, *pivot)) {
            ++i;
        }
        while (i != --j && !comp(*j, *pivot)) {
        }
        if (i < j) {
            MslSwap_unk(*i, *j);
            ++i;
            while (true) {
                while (comp(*i, *pivot)) {
                    ++i;
                }
                while (!comp(*--j, *pivot)) {
                }
                if (i >= j) {
                    break;
                }
                MslSwap_unk(*i, *j);
                ++i;
            }
        }
        if (i == first) {
            MslSwap_unk(*i, *pivot);
            ++i;
            j = last;
            if (!comp(*first, *--j)) {
                while (i != last && !comp(*first, *i)) {
                    ++i;
                }
                if (i < j) {
                    MslSwap_unk(*i, *j);
                }
            }
            if (i < j) {
                while (true) {
                    while (!comp(*first, *i)) {
                        ++i;
                    }
                    while (comp(*first, *--j)) {
                    }
                    if (i >= j) {
                        break;
                    }
                    MslSwap_unk(*i, *j);
                    ++i;
                }
            }
            first = i;
        } else if (i - first < last - i) {
            MslSort_unk<Iter, CompareRef>(first, i, comp);
            first = i;
        } else {
            MslSort_unk<Iter, CompareRef>(i, last, comp);
            last = i;
        }
    }
}

#endif
