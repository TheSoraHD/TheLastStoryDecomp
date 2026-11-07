// MSL C++ (Metrowerks Standard Library for C++, CodeWarrior for Wii): the classes shared by
// iostream.cpp (streams, console stream buffers, ios_base) and locale.cpp (locale, facets).
// No MSL C++ source is public: written from the assembly; names of types and functions from the RTTI
// strings, the USA LastWorld_tools.rso and the C++ standard, member names invented.
// inferred: private header (name invented); the MSL headers proper are not reconstructed.
#ifndef MSL_CPP_IOSTREAM_LOCALE_UNK_H
#define MSL_CPP_IOSTREAM_LOCALE_UNK_H

#include <exception>

extern "C" {
unsigned long strlen(const char* s);
char* strcpy(char* dst, const char* src);
int strcmp(const char* a, const char* b);
void free(void* p);
void* memmove(void* dst, const void* src, unsigned long n);
void* memcpy(void* dst, const void* src, unsigned long n);
wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);
int atexit(void (*func)(void));
}

namespace std {

// inferred: the MWCC type_info (name first, compared by strcmp)
class type_info {
public:
    bool operator==(const type_info& rhs) const { return strcmp(__type_name, rhs.__type_name) == 0; }

private:
    const char* __type_name;
    const void* __base_types;
};

namespace detail {

template <class T>
struct default_delete {
    void operator()(T* p) const { delete p; }
};

template <class T>
struct default_delete<T[]> {
    void operator()(T* p) const { delete[] p; }
};

} // namespace detail

namespace tr1 {
namespace detail {

class shared_ptr_deleter_common {
public:
    shared_ptr_deleter_common() : use_count_(1), weak_count_(1) {}
    virtual ~shared_ptr_deleter_common() {}
    virtual void* get_deleter(const type_info& ti) const = 0;
    virtual void dispose() = 0;
    void release();

    long use_count_;
    long weak_count_;
};

template <class T, class D>
struct __ptr_deleter_pair : public D {
    __ptr_deleter_pair(T* p, const D& d) : D(d), ptr_(p) {}
    T* ptr_;
};

template <class T, class D>
class shared_ptr_deleter : public shared_ptr_deleter_common {
public:
    shared_ptr_deleter(T* p, const D& d) : pair_(p, d) {}
    virtual void* get_deleter(const type_info& ti) const;
    virtual void dispose() { ((D&)pair_)(pair_.ptr_); }

    __ptr_deleter_pair<T, D> pair_;
};

template <class T, class D>
void* shared_ptr_deleter<T, D>::get_deleter(const type_info& ti) const {
    return ti == typeid(D) ? (void*)&pair_ : 0;
}

} // namespace detail

template <class T, class D>
struct __hold_ptr {
    __hold_ptr(T* p, D& d) : ptr_(p), deleter_(d) {}
    ~__hold_ptr() {
        if (ptr_ != 0) {
            deleter_(ptr_);
        }
    }
    T* release() {
        T* p = ptr_;
        ptr_ = 0;
        return p;
    }
    T* ptr_;
    D& deleter_;
};

// inferred: the pointer and the control block live in a base with the destructor (the destructors
// test the address twice before the release)
template <class T>
class __shared_ptr_base {
public:
    template <class Y, class D>
    __shared_ptr_base(Y* p, D& d) : ptr_(p) {
        __hold_ptr<Y, D> hold(p, d);
        cntrl_ = new detail::shared_ptr_deleter<Y, D>(p, d);
        hold.release();
    }
    __shared_ptr_base(const __shared_ptr_base& r) throw() : ptr_(r.ptr_), cntrl_(r.cntrl_) {
        if (cntrl_ != 0) {
            ++cntrl_->use_count_;
        }
    }
    ~__shared_ptr_base() throw() {
        if (cntrl_ != 0) {
            cntrl_->release();
        }
    }

    T* ptr_;
    detail::shared_ptr_deleter_common* cntrl_;
};

template <class T>
class shared_ptr : public __shared_ptr_base<T> {
public:
    template <class Y, class D>
    shared_ptr(Y* p, D d) : __shared_ptr_base<T>(p, d) {}
    T* get() const { return this->ptr_; }
};

} // namespace tr1

class runtime_error : public exception {
public:
    explicit runtime_error(const char* what_arg)
        : what_(new char[strlen(what_arg) + 1], std::detail::default_delete<char[]>()) {
        strcpy(what_.get(), what_arg);
    }
    virtual ~runtime_error();
    virtual const char* what() const;

private:
    tr1::shared_ptr<char> what_;
};

typedef unsigned long size_t;
typedef long streamsize;
typedef long long streamoff;
typedef int mbstate_t;

} // namespace std

void operator delete(void* p) throw();
inline void* operator new(std::size_t, void* p) throw() { return p; }

namespace std {
template <class T>
inline const T& max(const T& a, const T& b) {
    return a < b ? b : a;
}
template <class T>
inline const T& min(const T& a, const T& b) {
    return b < a ? b : a;
}
} // namespace std
inline void operator delete(void*, void*) throw() {}

// the whence values of the C library (stdio.h)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

struct _FILE {
    unsigned char m00[0x50];
};
extern "C" {
extern _FILE __files[];
int fflush(_FILE* file);
std::size_t fwrite(const void* p, std::size_t size, std::size_t n, _FILE* file);
}

namespace Metrowerks {

// inferred: the MSL mutexes of a single-threaded build (empty)
class mutex {
public:
    mutex() {}
    ~mutex() {}
    void lock() {}
    void unlock() {}
};

// inferred: the locale tables use a mutex type of their own (its lock has its own destructor, 0x80691EE4)
class recursive_mutex {
public:
    recursive_mutex() {}
    ~recursive_mutex() {}
    void lock() {}
    void unlock() {}
};

// inferred: lock guard (0x8068BA70 ctor, 0x8068BA80 dtor)
template <class M>
class scoped_lock {
public:
    explicit scoped_lock(M& m) : m_(m), locked_(true) { m_.lock(); }
    ~scoped_lock() {
        if (locked_) {
            m_.unlock();
        }
    }

private:
    M& m_;
    bool locked_;
};

} // namespace Metrowerks

namespace std {

template <class stateT>
class fpos {
public:
    fpos(streamoff off) : off_(off), st_() {}
    operator streamoff() const { return off_; }

    streamoff off_;
    stateT st_;
};

template <class C>
struct char_traits;

template <>
struct char_traits<char> {
    typedef char char_type;
    typedef int int_type;
    static int_type eof() { return -1; }
    static char_type* copy(char_type* s1, const char_type* s2, size_t n) { return (char_type*)memcpy(s1, s2, n); }
    static bool eq_int_type(int_type a, int_type b) { return a == b; }
    static int_type not_eof(int_type c) { return eq_int_type(c, eof()) ? ~eof() : c; }
    static char_type to_char_type(int_type c) { return (char_type)c; }
    static int_type to_int_type(char_type c) { return (unsigned char)c; }
};

template <>
struct char_traits<wchar_t> {
    typedef wchar_t char_type;
    typedef unsigned short int_type;
    static int_type eof() { return 0xFFFF; }
    static char_type* copy(char_type* s1, const char_type* s2, size_t n) { return wmemcpy(s1, s2, n); }
    static bool eq_int_type(int_type a, int_type b) { return a == b; }
    static int_type not_eof(int_type c) { return eq_int_type(c, eof()) ? ~eof() : c; }
    static char_type to_char_type(int_type c) { return (char_type)c; }
    static int_type to_int_type(char_type c) { return (int_type)c; }
};

struct __locale_imp;

class locale {
public:
    class facet {
    public:
        // inferred: drops one reference, returns the facet when it is no longer used
        facet* __release_unk() {
            if (--refs_ == 0) {
                return this;
            }
            return 0;
        }

    protected:
        friend struct __locale_imp;
        explicit facet(size_t refs = 0) : refs_(refs) {}
        virtual ~facet() {}

        size_t refs_;
    };

    class id {
    public:
        size_t get() {
            if (id_ == 0) {
                id_ = ++__id_count_;
            }
            return id_;
        }

        size_t id_;
        static size_t __id_count_;
    };

    locale() throw() : imp_(__global().imp_) {}
    locale(const locale& loc) throw() : imp_(loc.imp_) {}
    ~locale() {}

    static const locale& classic();
    static locale& __global() throw();

private:
    explicit locale(__locale_imp* imp);

public:

    tr1::__shared_ptr_base<__locale_imp> imp_;
};

class ios_base {
public:
    class failure : public exception {
    public:
        explicit failure(const char* msg)
            : what_(new char[strlen(msg) + 1], std::detail::default_delete<char[]>()) {
            strcpy(what_.get(), msg);
        }
        virtual const char* what() const throw() { return what_.get(); }

    private:
        tr1::shared_ptr<char> what_;
    };

    enum event { erase_event, imbue_event, copyfmt_event };
    typedef void (*event_callback)(event ev, ios_base& ios, int index);
    enum iostate { goodbit = 0, badbit = 1, eofbit = 2, failbit = 4 };
    enum fmtflags { skipws = 0x0002, dec = 0x1000, unitbuf = 0x2000 };
    enum openmode { app = 0x01, ate = 0x02, binary = 0x04, in = 0x08, out = 0x10, trunc = 0x20 };
    enum seekdir { beg = 1, cur = 2, end = 4 };

    class Init {
    public:
        Init();
        ~Init();

    private:
        static int init_cnt_;
    };

    virtual ~ios_base();
    void throw_failure(const char* msg);
    unsigned short setf(unsigned short flags) {
        unsigned short old = flags_;
        flags_ |= flags;
        return old;
    }
    locale getloc() const;

protected:
    ios_base() {}
    void init(void* sb);
    void __call_callbacks_unk(event ev) {
        for (unsigned long i = ncallbacks_; i != 0;) {
            --i;
            callbacks_[i].fn_(ev, *this, callbacks_[i].index_);
        }
    }

    struct __callback {
        event_callback fn_;
        int index_;
    };

    __callback* callbacks_;       // 0x04
    unsigned long ncallbacks_;    // 0x08
    unsigned long callbacks_cap_; // 0x0C
    long* iwords_;                // 0x10
    unsigned long niwords_;       // 0x14
    void** pwords_;               // 0x18
    unsigned long npwords_;       // 0x1C
    locale* loc_;                 // 0x20
    void* rdbuf_;                 // 0x24
    streamsize precision_;        // 0x28
    streamsize width_;            // 0x2C
    unsigned short flags_;        // 0x30
    unsigned char state_;         // 0x32
    unsigned char exceptions_;    // 0x33
};


struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class T>
class allocator {};

// inferred: the MSL basic_string (12 bytes, short-string optimisation; see docs/msl_cpp.md and
// include/std/string.h). reserve and do_replace are out of line (weak copies kept by the game, 0x80013DC4)
template <class T, class Traits = char_traits<T>, class Alloc = allocator<T> >
class basic_string {
public:
    typedef unsigned long size_type;

    basic_string(const T* s) {
        __zero();
        size_type n = strlen(s);
        reserve(n);
        do_replace(0, 0, s, s + n, random_access_iterator_tag());
    }
    ~basic_string() {
        if (is_long()) {
            ::operator delete(mLong.mData);
        }
    }
    void reserve(size_type res_arg);
    basic_string& do_replace(size_type pos, size_type n1, const T* j1, const T* j2, random_access_iterator_tag tag);
    bool is_long() const { return *(const unsigned long*)this >> 31; }
    void __zero() {
        unsigned long (&a)[3] = mRaw.mWords;
        for (unsigned int i = 0; i < 3; i++) {
            a[i] = 0;
        }
    }

    struct Long {
        unsigned long mCap;
        unsigned long mSize;
        T* mData;
    };
    struct Raw {
        unsigned long mWords[3];
    };
    union {
        Raw mRaw;
        Long mLong;
    };
};

typedef basic_string<char> string;

// inferred: the MSL vector (data, size, capacity); its destructor is inlined through four levels
template <class T>
struct __vec_storage_unk {
    __vec_storage_unk() : data_(0), size_(0), cap_(0) {}
    ~__vec_storage_unk() {
        if (data_ != 0) {
            clear();
            ::operator delete(data_);
        }
    }
    void clear() {
        size_t n = size_;
        size_t& size = size_;
        size -= n;
    }

    T* data_;
    size_t size_;
    size_t cap_;
};

template <class T>
struct __vec_base_unk : public __vec_storage_unk<T> {};

template <class T>
struct __vec_imp_unk : public __vec_base_unk<T> {};

template <class T>
class vector : public __vec_imp_unk<T> {
public:
    size_t size() const { return this->size_; }
    T& operator[](size_t i) { return this->data_[i]; }
};

// inferred: the facet table of a locale (vector of facet pointers at 0x00, the name at 0x0C)
struct __locale_imp {
    explicit __locale_imp(const char* name) : name_(name) {}
    ~__locale_imp() throw();
    void replace(locale::facet* f, size_t id);

    vector<locale::facet*> facets_; // 0x00
    string name_;                   // 0x0C
};

inline locale::locale(__locale_imp* imp)
    : imp_(tr1::shared_ptr<__locale_imp>(imp, std::detail::default_delete<__locale_imp>())) {}

template <class Facet>
inline const Facet& use_facet(const locale& loc) {
    // inferred: an empty object is built first (its byte is cleared at the start of every inlined copy)
    detail::default_delete<Facet> d = detail::default_delete<Facet>();
    size_t i = Facet::id.get();
    if (i >= loc.imp_.ptr_->facets_.size() || loc.imp_.ptr_->facets_[i] == 0) {
        Facet* f = new Facet;
        loc.imp_.ptr_->replace(f, Facet::id.get());
    }
    return *(const Facet*)loc.imp_.ptr_->facets_[i];
}

} // namespace std

// inferred: the classic tables of MSL C (classification, tolower and toupper maps)
extern "C" {
extern const unsigned short lbl_80765338[];
extern const char lbl_80765538[];
extern const char lbl_80765638[];
}

namespace std {

class ctype_base {
public:
    typedef unsigned short mask;
};

template <class C>
class ctype;

template <>
class ctype<char> : public locale::facet, public ctype_base {
public:
    explicit ctype(const mask* tab = 0, bool del = false, size_t refs = 0);
    char widen(char c) const { return do_widen(c); }

    static locale::id id;

protected:
    virtual ~ctype();
    virtual char do_toupper(char c) const;
    virtual const char* do_toupper(char* low, const char* high);
    virtual char do_tolower(char c) const;
    virtual const char* do_tolower(char* low, const char* high);
    virtual char do_widen(char c) const { return c; }
    virtual const char* do_widen(const char* low, const char* high, char* to) const;
    virtual char do_narrow(char c, char dfault) const { return c; }
    virtual const char* do_narrow(const char* low, const char* high, char dfault, char* to) const;

    const mask* table_;           // 0x08
    const char* lower_;           // 0x0C
    const char* upper_;           // 0x10
    bool del_;                    // 0x14
};

template <>
class ctype<wchar_t> : public locale::facet, public ctype_base {
public:
    explicit ctype(size_t refs = 0);
    wchar_t widen(char c) const { return do_widen(c); }

    static locale::id id;

protected:
    virtual ~ctype();
    virtual bool do_is(mask m, wchar_t c) const;
    virtual const wchar_t* do_is(const wchar_t* low, const wchar_t* high, mask* vec) const;
    virtual const wchar_t* do_scan_is(mask m, const wchar_t* low, const wchar_t* high) const;
    virtual const wchar_t* do_scan_not(mask m, const wchar_t* low, const wchar_t* high) const;
    virtual wchar_t do_toupper(wchar_t c) const;
    virtual const wchar_t* do_toupper(wchar_t* low, const wchar_t* high) const;
    virtual wchar_t do_tolower(wchar_t c) const;
    virtual const wchar_t* do_tolower(wchar_t* low, const wchar_t* high) const;
    virtual wchar_t do_widen(char c) const { return c; }
    virtual const char* do_widen(const char* low, const char* high, wchar_t* to) const;
    virtual char do_narrow(wchar_t c, char dfault) const;
    virtual const wchar_t* do_narrow(const wchar_t* low, const wchar_t* high, char dfault, char* to) const;

    char tables_unk_[0x2C]; // inferred: 0x08..0x34 (filled by the constructor, 0x80692C3C)
};

template <class I, class E, class S>
class codecvt;

class codecvt_base {
public:
    enum result { ok, partial, error, noconv };
};

template <>
class codecvt<char, char, int> : public locale::facet, public codecvt_base {
public:
    explicit codecvt(size_t refs = 0);
    result out(int& state, const char* from, const char* from_end, const char*& from_next, char* to, char* to_end,
               char*& to_next) const {
        return do_out(state, from, from_end, from_next, to, to_end, to_next);
    }
    result in(int& state, const char* from, const char* from_end, const char*& from_next, char* to, char* to_end,
              char*& to_next) const {
        return do_in(state, from, from_end, from_next, to, to_end, to_next);
    }
    result unshift(int& state, char* to, char* to_end, char*& to_next) const {
        return do_unshift(state, to, to_end, to_next);
    }
    int length(int& state, const char* from, const char* end, size_t max) const {
        return do_length(state, from, end, max);
    }
    bool always_noconv() const throw() { return do_always_noconv(); }
    int encoding() const throw() { return do_encoding(); }
    int max_length() const throw() { return do_max_length(); }

    static locale::id id;

protected:
    virtual ~codecvt() {}
    virtual result do_out(int& state, const char* from, const char* from_end, const char*& from_next, char* to,
                          char* to_end, char*& to_next) const {
        from_next = from;
        to_next = to;
        return noconv;
    }
    virtual result do_in(int& state, const char* from, const char* from_end, const char*& from_next, char* to,
                         char* to_end, char*& to_next) const {
        from_next = from;
        to_next = to;
        return noconv;
    }
    virtual result do_unshift(int& state, char* to, char* to_end, char*& to_next) const {
        to_next = to;
        return noconv;
    }
    virtual int do_encoding() const throw() { return 1; }
    virtual bool do_always_noconv() const throw() { return true; }
    virtual int do_length(int& state, const char* from, const char* end, size_t max) const;
    virtual int do_max_length() const throw() { return 1; }
};

template <>
class codecvt<wchar_t, char, int> : public locale::facet, public codecvt_base {
public:
    explicit codecvt(size_t refs = 0);
    result out(int& state, const wchar_t* from, const wchar_t* from_end, const wchar_t*& from_next, char* to, char* to_end,
               char*& to_next) const {
        return do_out(state, from, from_end, from_next, to, to_end, to_next);
    }
    result in(int& state, const char* from, const char* from_end, const char*& from_next, wchar_t* to, wchar_t* to_end,
              wchar_t*& to_next) const {
        return do_in(state, from, from_end, from_next, to, to_end, to_next);
    }
    result unshift(int& state, char* to, char* to_end, char*& to_next) const {
        return do_unshift(state, to, to_end, to_next);
    }
    int length(int& state, const char* from, const char* end, size_t max) const {
        return do_length(state, from, end, max);
    }
    bool always_noconv() const throw() { return do_always_noconv(); }
    int encoding() const throw() { return do_encoding(); }
    int max_length() const throw() { return do_max_length(); }

    static locale::id id;

protected:
    virtual ~codecvt() {}
    virtual result do_out(int& state, const wchar_t* from, const wchar_t* from_end, const wchar_t*& from_next,
                          char* to, char* to_end, char*& to_next) const;
    virtual result do_in(int& state, const char* from, const char* from_end, const char*& from_next, wchar_t* to,
                         wchar_t* to_end, wchar_t*& to_next) const;
    virtual result do_unshift(int& state, char* to, char* to_end, char*& to_next) const {
        to_next = to;
        return noconv;
    }
    virtual int do_encoding() const throw() { return 2; }
    virtual bool do_always_noconv() const throw() { return true; }
    virtual int do_length(int& state, const char* from, const char* end, size_t max) const;
    virtual int do_max_length() const throw() { return 2; }
};

template <class C, class T = char_traits<C> >
class basic_streambuf {
public:
    typedef C char_type;
    typedef typename T::int_type int_type;

    virtual ~basic_streambuf() {}
    locale getloc() const { return loc_; }
    basic_streambuf* pubsetbuf(char_type* s, streamsize n) { return setbuf(s, n); }
    int_type sbumpc() { return gptr_ < egptr_ ? T::to_int_type(*gptr_++) : uflow(); }
    int_type sputc(char_type c) {
        if (pptr_ < epptr_) {
            char_type* p = pptr_;
            *p = c;
            pptr_ = p + 1;
            return T::to_int_type(*p);
        }
        return overflow(T::to_int_type(c));
    }

protected:
    basic_streambuf() : eback_(0), gptr_(0), egptr_(0), pbase_(0), pptr_(0), epptr_(0) {}

    void setg(char_type* gbeg, char_type* gnext, char_type* gend) {
        eback_ = gbeg;
        gptr_ = gnext;
        egptr_ = gend;
    }
    void setp(char_type* pbeg, char_type* pend) {
        pbase_ = pptr_ = pbeg;
        epptr_ = pend;
    }

    virtual void imbue(const locale& loc);
    virtual basic_streambuf* setbuf(char_type* s, streamsize n);
    virtual fpos<mbstate_t> seekoff(streamoff off, ios_base::seekdir way, ios_base::openmode which);
    virtual fpos<mbstate_t> seekpos(fpos<mbstate_t> sp, ios_base::openmode which);
    virtual int sync();
    virtual streamsize showmanyc();
    virtual streamsize xsgetn(char_type* s, streamsize n);
    virtual int_type underflow();
    virtual int_type uflow();
    virtual int_type pbackfail(int_type c);
    virtual streamsize xsputn(const char_type* s, streamsize n);
    virtual int_type overflow(int_type c);

    char_type* eback_; // 0x04
    char_type* gptr_;  // 0x08
    char_type* egptr_; // 0x0C
    char_type* pbase_; // 0x10
    char_type* pptr_;  // 0x14
    char_type* epptr_; // 0x18
    locale loc_;       // 0x1C
};

template <class C, class T>
void basic_streambuf<C, T>::imbue(const locale& loc) {}

template <class C, class T>
basic_streambuf<C, T>* basic_streambuf<C, T>::setbuf(char_type* s, streamsize n) {
    return this;
}

template <class C, class T>
fpos<mbstate_t> basic_streambuf<C, T>::seekoff(streamoff off, ios_base::seekdir way, ios_base::openmode which) {
    return -1;
}

template <class C, class T>
fpos<mbstate_t> basic_streambuf<C, T>::seekpos(fpos<mbstate_t> sp, ios_base::openmode which) {
    return -1;
}

template <class C, class T>
streamsize basic_streambuf<C, T>::xsgetn(char_type* s, streamsize n) {
    streamsize num = std::min(n, streamsize(egptr_ - gptr_));
    if (num > 0) {
        T::copy(s, gptr_, num);
        s += num;
        gptr_ += num;
        n -= num;
    }
    for (; n > 0; ++num, ++s, --n) {
        int_type c = sbumpc();
        if (T::eq_int_type(c, T::eof())) {
            break;
        }
        *s = T::to_char_type(c);
    }
    return num;
}

template <class C, class T>
typename basic_streambuf<C, T>::int_type basic_streambuf<C, T>::uflow() {
    return T::eq_int_type(underflow(), T::eof()) ? T::eof() : T::to_int_type(*gptr_++);
}

template <class C, class T>
streamsize basic_streambuf<C, T>::xsputn(const char_type* s, streamsize n) {
    streamsize num = std::min(n, streamsize(epptr_ - pptr_));
    if (num > 0) {
        T::copy(pptr_, s, num);
        s += num;
        pptr_ += num;
        n -= num;
    }
    while (n > 0) {
        if (T::eq_int_type(sputc(*s++), T::eof())) {
            break;
        }
        ++num;
        --n;
    }
    return num;
}

template <class C, class T>
int basic_streambuf<C, T>::sync() {
    return 0;
}

template <class C, class T>
streamsize basic_streambuf<C, T>::showmanyc() {
    return 0;
}

template <class C, class T>
typename basic_streambuf<C, T>::int_type basic_streambuf<C, T>::underflow() {
    return T::eof();
}

template <class C, class T>
typename basic_streambuf<C, T>::int_type basic_streambuf<C, T>::pbackfail(int_type c) {
    return T::eof();
}

template <class C, class T>
typename basic_streambuf<C, T>::int_type basic_streambuf<C, T>::overflow(int_type c) {
    return T::eof();
}

template <class C, class T>
class basic_ostream;

template <class C, class T = char_traits<C> >
class basic_ios : public ios_base {
public:
    typedef C char_type;

    virtual ~basic_ios() {}
    basic_ostream<C, T>* tie(basic_ostream<C, T>* tiestr) {
        basic_ostream<C, T>* old = tie_;
        tie_ = tiestr;
        return old;
    }

protected:
    basic_ios() {}
    void init(basic_streambuf<C, T>* sb) {
        ios_base::init(sb);
        tie_ = 0;
        fill_ = use_facet<ctype<C> >(getloc()).widen(' ');
    }

    basic_ostream<C, T>* tie_; // 0x34
    char_type fill_;           // 0x38
};

template <class C, class T = char_traits<C> >
class basic_istream : virtual public basic_ios<C, T> {
public:
    explicit basic_istream(basic_streambuf<C, T>* sb);
    virtual ~basic_istream() {}

protected:
    streamsize gcount_;
};

template <class C, class T = char_traits<C> >
class basic_ostream : virtual public basic_ios<C, T> {
public:
    explicit basic_ostream(basic_streambuf<C, T>* sb);
    virtual ~basic_ostream() {}
    basic_ostream& flush();
};

template <class C, class T>
basic_istream<C, T>::basic_istream(basic_streambuf<C, T>* sb) : gcount_(0) {
    this->init(sb);
}

template <class C, class T>
basic_ostream<C, T>::basic_ostream(basic_streambuf<C, T>* sb) {
    this->init(sb);
}

typedef basic_istream<char> istream;
typedef basic_ostream<char> ostream;
typedef basic_istream<wchar_t> wistream;
typedef basic_ostream<wchar_t> wostream;

} // namespace std

namespace Metrowerks {

template <class C, class T = std::char_traits<C> >
class bufferedbuf : public std::basic_streambuf<C, T> {
public:
    typedef std::basic_streambuf<C, T> base;
    typedef typename T::int_type int_type;
    typedef std::codecvt<C, char, std::mbstate_t> cvt;

    bufferedbuf()
        : buf_size_(0), buffer_(0), state_(0), unshift_(false), buffered_(false), owns_buffer_(false), m4C_(false) {
        this->setg(0, 0, 0);
        this->setp(0, 0);
        ext_beg_ = ext_next_ = ext_end_ = 0;
        a_codecvt_ = &std::use_facet<cvt>(this->getloc());
        always_noconv_ = a_codecvt_->always_noconv();
        encoding_ = a_codecvt_->encoding();
        max_length_ = a_codecvt_->max_length();
        this->setbuf(0, 0);
    }
    virtual ~bufferedbuf() {
        if (owns_buffer_) {
            delete[] buffer_;
        }
    }

protected:
    virtual void imbue(const std::locale& loc);
    virtual base* setbuf(C* s, std::streamsize n);
    virtual std::fpos<std::mbstate_t> seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which);
    virtual std::fpos<std::mbstate_t> seekpos(std::fpos<std::mbstate_t> sp, std::ios_base::openmode which);
    virtual int sync();
    virtual std::streamsize showmanyc();
    virtual int_type underflow();
    virtual int_type pbackfail(int_type c);
    virtual int_type overflow(int_type c);
    virtual std::streamsize write(const char* s, std::streamsize n) = 0;
    virtual std::streamsize read(char* s, std::streamsize n) = 0;
    virtual std::streamoff seek(std::streamoff off, int whence) = 0;

    int flush_put();
    int empty_get();
    std::streamsize fill_get();
    // inferred: sets up the get and put areas over the buffer (inlined in setbuf)
    void __init_areas_unk() {
        C* p = buffer_;
        this->setg(0, 0, 0);
        C* b = p;
        if (!always_noconv_) {
            b = p + 1;
            if (max_length_ > (int)sizeof(C)) {
                b += buf_size_ - buf_size_ * sizeof(C) / max_length_;
            }
        }
        if (buffered_) {
            this->setp(b, p + buf_size_ - 1);
        } else {
            this->setp(b, b);
        }
        ext_beg_ = ext_next_ = (char*)buffer_;
        ext_end_ = (char*)(buffer_ + buf_size_);
    }
    // inferred: sets up an empty get area (inlined in underflow and imbue)
    void __init_get_unk() {
        C* p = buffer_;
        this->setp(0, 0);
        this->setg(p, p, p);
        ext_beg_ = ext_next_ = ext_end_ = (char*)(p + buf_size_);
        if (!always_noconv_) {
            ext_beg_ -= (buf_size_ - 1) * __ext_width_unk();
        }
    }
    // inferred: external bytes per character
    int __ext_width_unk() const { return encoding_ <= 0 ? 1 : (encoding_ > (int)sizeof(C) ? (int)sizeof(C) : encoding_); }
    // inferred: writes [ext_beg_, ext_next_) to the device
    int __write_ext_unk() {
        while (ext_beg_ < ext_next_) {
            std::streamsize n = write(ext_beg_, ext_next_ - ext_beg_);
            if (n <= 0) {
                return -1;
            }
            ext_beg_ += n;
        }
        return 0;
    }

    static std::size_t __min_size_unk;

    std::size_t buf_size_; // 0x24
    C* buffer_;            // 0x28
    char* ext_beg_;        // 0x2C: converted characters not written yet
    char* ext_next_;       // 0x30
    char* ext_end_;        // 0x34
    std::mbstate_t state_; // 0x38
    int max_length_;       // 0x3C
    int encoding_;         // 0x40
    const cvt* a_codecvt_; // 0x44
    bool unshift_;         // 0x48: the encoding has a state (encoding() < 0)
    bool always_noconv_;   // 0x49
    bool buffered_;        // 0x4A
    bool owns_buffer_;     // 0x4B
    bool m4C_;             // 0x4C
    bool text_mode_unk_;   // 0x4D: inferred: counts '\n' as two characters when seeking back
};

template <class C, class T = std::char_traits<C> >
class console_outputbuf : public bufferedbuf<C, T> {
public:
    explicit console_outputbuf(_FILE* file);
    virtual ~console_outputbuf() {}

protected:
    virtual int sync() {
        int r = bufferedbuf<C, T>::sync();
        if (r >= 0) {
            r = fflush(file_);
        }
        return r;
    }
    virtual std::streamsize write(const char* s, std::streamsize n) { return fwrite(s, 1, n, file_); }
    virtual std::streamsize read(char* s, std::streamsize n) { return -1; }
    virtual std::streamoff seek(std::streamoff off, int whence) { return -1; }

    _FILE* file_; // 0x50
};

template <class C, class T = std::char_traits<C> >
class console_inputbuf : public std::basic_streambuf<C, T> {
public:
    typedef typename T::int_type int_type;
    typedef std::codecvt<C, char, std::mbstate_t> cvt;

    explicit console_inputbuf(_FILE* file);
    virtual ~console_inputbuf() {}

protected:
    virtual void imbue(const std::locale& loc);
    virtual std::basic_streambuf<C, T>* setbuf(C* s, std::streamsize n);
    virtual int sync();
    virtual std::streamsize showmanyc();
    virtual int_type underflow();
    virtual int_type uflow();
    virtual int_type pbackfail(int_type c);

    _FILE* file_;          // 0x24
    int m28_;              // 0x28
    const cvt* a_codecvt_; // 0x2C
    int encoding_;         // 0x30
    bool m34_;             // 0x34
    bool always_noconv_;   // 0x35
    bool m36_;             // 0x36
    bool m37_;             // 0x37
    bool m38_;             // 0x38
};

// inferred: 16 (0x8087EC24 char, 0x8087EC20 wchar_t)
template <class C, class T>
std::size_t bufferedbuf<C, T>::__min_size_unk = 16;

template <class C, class T>
typename bufferedbuf<C, T>::base* bufferedbuf<C, T>::setbuf(C* s, std::streamsize n) {
    bool had_put = this->pptr_ != 0;
    if (this->sync() < 0) {
        return 0;
    }
    this->setg(0, 0, 0);
    this->setp(0, 0);
    ext_beg_ = ext_next_ = ext_end_ = 0;
    if (s == 0) {
        if (owns_buffer_) {
            delete[] buffer_;
        }
        buffer_ = 0;
        buf_size_ = std::max(std::max(2 * max_length_ / sizeof(C), (std::size_t)n), __min_size_unk);
        buffer_ = new C[buf_size_];
        owns_buffer_ = true;
        buffered_ = n != 0;
    } else {
        if (n < 2 * max_length_ / (std::streamsize)sizeof(C)) {
            return 0;
        }
        if (owns_buffer_) {
            delete[] buffer_;
        }
        buffer_ = s;
        buf_size_ = n;
        owns_buffer_ = false;
        buffered_ = true;
    }
    if (buffered_ && had_put) {
        __init_areas_unk();
    }
    return this;
}

template <class C, class T>
int bufferedbuf<C, T>::flush_put() {
    if (always_noconv_) {
        ext_next_ = (char*)this->pptr_;
        if (__write_ext_unk() == -1) {
            return -1;
        }
    } else {
        const C* from_next;
        for (const C* from = this->pbase_; from < this->pptr_; from = from_next) {
            switch (a_codecvt_->out(state_, from, this->pptr_, from_next, ext_beg_, (char*)(this->pptr_ - 1), ext_next_)) {
            case std::codecvt_base::error:
                return -1;
            case std::codecvt_base::noconv:
                ext_beg_ = (char*)from;
                ext_next_ = (char*)this->pptr_;
                from_next = this->pptr_;
            case std::codecvt_base::ok:
                if (unshift_ && ext_next_ < ext_end_) {
                    switch (a_codecvt_->unshift(state_, ext_next_, ext_end_, ext_next_)) {
                    case std::codecvt_base::error:
                        return -1;
                    case std::codecvt_base::noconv:
                    case std::codecvt_base::ok:
                        unshift_ = false;
                        break;
                    }
                }
            case std::codecvt_base::partial:
                if (__write_ext_unk() == -1) {
                    return -1;
                }
                ext_beg_ = ext_next_ = (char*)buffer_;
                break;
            }
        }
        if (unshift_) {
            std::codecvt_base::result r;
            do {
                r = a_codecvt_->unshift(state_, ext_beg_, ext_end_, ext_next_);
                switch (r) {
                case std::codecvt_base::error:
                    return -1;
                case std::codecvt_base::ok:
                case std::codecvt_base::partial:
                    if (__write_ext_unk() == -1) {
                        return -1;
                    }
                    break;
                }
            } while (r == std::codecvt_base::partial);
            unshift_ = false;
        }
    }
    if (buffered_) {
        __init_areas_unk();
    } else {
        this->setg(0, 0, 0);
        this->setp(0, 0);
        ext_beg_ = ext_next_ = ext_end_ = 0;
    }
    return 0;
}

// inferred: before the get area is dropped, the unread characters are converted again to know how many
// external characters to seek back
template <class C, class T>
int bufferedbuf<C, T>::empty_get() {
    if (encoding_ < 0 && this->gptr_ < this->egptr_) {
        return -1;
    }
    int off = ext_end_ - ext_next_;
    char* end = (char*)(buffer_ + buf_size_);
    ext_next_ = ext_end_ = end;
    C* e = this->egptr_;
    C* g = this->gptr_;
    if (g < e) {
        if (encoding_ == 0) {
            const C* from_next = (C*)end - (e - g);
            char* to_end = (char*)from_next;
            memmove((void*)from_next, g, (e - g) * sizeof(C));
            char* to_next;
            std::codecvt_base::result r;
            do {
                r = a_codecvt_->out(state_, from_next, (C*)end, from_next, (char*)g, to_end, to_next);
                switch (r) {
                case std::codecvt_base::error:
                    return -1;
                case std::codecvt_base::noconv:
                    off += ((const C*)end - from_next) * sizeof(C);
                    break;
                case std::codecvt_base::partial:
                    if (to_next == (char*)g) {
                        return -1;
                    }
                    to_end = (char*)from_next;
                case std::codecvt_base::ok:
                    off += to_next - (char*)g;
                    break;
                }
            } while (r == std::codecvt_base::partial);
        } else {
            off += encoding_ * (e - g);
        }
    }
    if (off > 0) {
        if (text_mode_unk_) {
            for (C* p = this->gptr_; p < this->egptr_; ++p) {
                if (*p == C('\n')) {
                    ++off;
                }
            }
        }
        off = seek(-off, SEEK_CUR);
    }
    if (off < 0) {
        return -1;
    }
    this->egptr_ = this->gptr_;
    return 0;
}

template <class C, class T>
std::streamsize bufferedbuf<C, T>::fill_get() {
    std::streamsize n = this->gptr_ - this->eback_;
    if (n > 4) {
        n = 4;
        memmove(this->eback_, this->gptr_ - 4, 4 * sizeof(C));
        this->egptr_ = this->gptr_ = this->eback_ + 4;
    }
    if (always_noconv_) {
        std::streamsize r = read((char*)this->gptr_, (buf_size_ - n) * sizeof(C));
        if (r <= 0) {
            return r;
        }
        int odd = r % (int)sizeof(C);
        if (odd != 0) {
            if (seek(-odd, SEEK_CUR) < 0) {
                return -1;
            }
            r -= odd;
        }
        this->egptr_ = this->gptr_ + r / sizeof(C);
    } else {
        ext_beg_ = (char*)(buffer_ + buf_size_) - __ext_width_unk() * (buf_size_ - (this->gptr_ - this->eback_) - 1);
        std::size_t k = 0;
        if (ext_next_ < ext_end_) {
            k = ext_end_ - ext_next_;
            memmove(ext_beg_, ext_next_, k);
        }
        ext_next_ = ext_beg_ + k;
        ext_end_ = (char*)(buffer_ + buf_size_);
        std::streamsize r = read(ext_next_, ext_end_ - ext_next_);
        ext_end_ = ext_next_;
        ext_next_ = ext_beg_;
        if (r <= 0) {
            return r;
        }
        ext_end_ += r;
        C* to_next;
        const char* from_next;
        std::codecvt_base::result res = a_codecvt_->in(state_, ext_beg_, ext_end_, from_next, this->gptr_,
                                                       buffer_ + buf_size_ - 1, to_next);
        ext_next_ = (char*)from_next;
        switch (res) {
        case std::codecvt_base::error:
            return -1;
        case std::codecvt_base::noconv:
            memmove(this->gptr_, ext_beg_, ext_end_ - ext_beg_);
            ext_next_ = ext_end_;
            to_next = this->gptr_ + (ext_end_ - ext_beg_) / sizeof(C);
        case std::codecvt_base::ok:
        case std::codecvt_base::partial:
            this->egptr_ = to_next;
            break;
        }
    }
    return this->egptr_ - this->gptr_;
}

template <class C, class T>
typename bufferedbuf<C, T>::int_type bufferedbuf<C, T>::underflow() {
    if (!m4C_) {
        return T::eof();
    }
    if (this->pptr_ != 0) {
        unshift_ = encoding_ < 0;
        if (flush_put() < 0) {
            return T::eof();
        }
        this->setp(0, 0);
    }
    if (this->gptr_ == 0) {
        __init_get_unk();
    }
    if (this->gptr_ >= this->egptr_ && fill_get() <= 0) {
        return T::eof();
    }
    return T::to_int_type(*this->gptr_);
}

template <class C, class T>
typename bufferedbuf<C, T>::int_type bufferedbuf<C, T>::overflow(int_type c) {
    if (!m4C_) {
        return T::eof();
    }
    if (this->gptr_ != 0) {
        if (empty_get() < 0) {
            return T::eof();
        }
        this->setg(0, 0, 0);
    }
    if (this->pptr_ == 0) {
        __init_areas_unk();
    }
    if (!T::eq_int_type(c, T::eof())) {
        *this->pptr_ = T::to_char_type(c);
        ++this->pptr_;
    }
    if (flush_put() < 0) {
        return T::eof();
    }
    return T::not_eof(c);
}

template <class C, class T>
std::fpos<std::mbstate_t> bufferedbuf<C, T>::seekoff(std::streamoff off, std::ios_base::seekdir way,
                                                     std::ios_base::openmode which) {
    if (!m4C_ || (which & (std::ios_base::in | std::ios_base::out)) == 0 || off != 0 && encoding_ <= 0) {
        return -1;
    }
    if (this->sync() < 0) {
        return -1;
    }
    int whence;
    switch (way) {
    case std::ios_base::beg:
        whence = SEEK_SET;
        break;
    case std::ios_base::cur:
        whence = SEEK_CUR;
        break;
    case std::ios_base::end:
        whence = SEEK_END;
        break;
    default:
        return -1;
    }
    std::streamoff pos = seek(off * (encoding_ > 0 ? encoding_ : 0), whence);
    if (pos < 0) {
        return -1;
    }
    return pos;
}

template <class C, class T>
std::fpos<std::mbstate_t> bufferedbuf<C, T>::seekpos(std::fpos<std::mbstate_t> sp, std::ios_base::openmode which) {
    if (!m4C_ || (which & (std::ios_base::in | std::ios_base::out)) == 0) {
        return -1;
    }
    if (this->sync() < 0) {
        return -1;
    }
    if (seek(sp, SEEK_SET) < 0) {
        return -1;
    }
    return sp;
}

template <class C, class T>
void bufferedbuf<C, T>::imbue(const std::locale& loc) {
    if (this->sync() < 0) {
        throw std::runtime_error("bufferedbuf::imbue failed");
    }
    a_codecvt_ = &std::use_facet<cvt>(loc);
    always_noconv_ = a_codecvt_->always_noconv();
    encoding_ = a_codecvt_->encoding();
    max_length_ = a_codecvt_->max_length();
    if (max_length_ <= 0 || max_length_ > buf_size_ / 2) {
        if (max_length_ > 0 && owns_buffer_) {
            this->setbuf(0, buffered_);
        } else {
            throw std::runtime_error("codecvt facet max_length is out of range");
        }
    }
    if (this->pptr_ != 0) {
        __init_areas_unk();
    } else if (this->gptr_ != 0) {
        __init_get_unk();
    }
}

template <class C, class T>
int bufferedbuf<C, T>::sync() {
    if (this->pptr_ != 0) {
        unshift_ = encoding_ < 0;
        if (flush_put() < 0) {
            return -1;
        }
    } else if (this->gptr_ != 0) {
        if (empty_get() < 0) {
            return -1;
        }
    }
    return 0;
}

template <class C, class T>
typename bufferedbuf<C, T>::int_type bufferedbuf<C, T>::pbackfail(int_type c) {
    if (!m4C_) {
        return T::eof();
    }
    if (this->gptr_ <= this->eback_) {
        return T::eof();
    }
    --this->gptr_;
    if (!T::eq_int_type(c, T::eof())) {
        *this->gptr_ = T::to_char_type(c);
    }
    return T::not_eof(c);
}

template <class C, class T>
std::streamsize bufferedbuf<C, T>::showmanyc() {
    return this->egptr_ - this->gptr_;
}

template <class C, class T>
console_inputbuf<C, T>::console_inputbuf(_FILE* file) : file_(file), m28_(0), m36_(false), m37_(false), m38_(false) {
    if (file_ == 0) {
        throw std::runtime_error("console_inputbuf constructor given null FILE*");
    }
    std::locale loc = this->getloc();
    a_codecvt_ = &std::use_facet<cvt>(loc);
    always_noconv_ = a_codecvt_->always_noconv();
    encoding_ = a_codecvt_->encoding();
    if ((unsigned int)encoding_ > 12 || always_noconv_ && encoding_ != 1) {
        throw std::runtime_error("console_inputbuf: bad encoding");
    }
}

template <class C, class T>
console_outputbuf<C, T>::console_outputbuf(_FILE* file) : file_(file) {
    this->text_mode_unk_ = false;
    this->m4C_ = true;
}

} // namespace Metrowerks

#endif
