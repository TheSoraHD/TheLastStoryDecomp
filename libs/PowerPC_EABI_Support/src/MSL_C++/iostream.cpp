// MSL C++ (Metrowerks Standard Library for C++, CodeWarrior for Wii) iostream support: the standard
// streams, their console stream buffers, ios_base, locale::classic and the char/wchar_t facets.
// inferred: one translation unit (0x8068B39C-0x8069533C; the RTTI names, vtables and strings are one
// pool). No MSL C++ source is public: written from the assembly; names of types and functions from
// the RTTI strings, the USA LastWorld_tools.rso and the C++ standard, member names invented.

#include <exception>

extern "C" {
unsigned long strlen(const char* s);
char* strcpy(char* dst, const char* src);
void free(void* p);
}

namespace std {

class type_info;

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

inline void* operator new(std::size_t, void* p) throw() { return p; }
inline void operator delete(void*, void*) throw() {}

struct _FILE {
    unsigned char m00[0x50];
};
extern "C" {
extern _FILE __files[];
int fflush(_FILE* file);
std::size_t fwrite(const void* p, std::size_t size, std::size_t n, _FILE* file);
}

namespace Metrowerks {

// inferred: the MSL mutex of a single-threaded build (empty)
class mutex {
public:
    mutex() {}
    ~mutex() {}
};

// inferred: lock guard (0x8068BA70 ctor, 0x8068BA80 dtor)
template <class M>
class scoped_lock {
public:
    explicit scoped_lock(M& m) : m_(m), locked_(true) {}
    ~scoped_lock() {}

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
};

template <>
struct char_traits<wchar_t> {
    typedef wchar_t char_type;
    typedef unsigned short int_type;
    static int_type eof() { return 0xFFFF; }
};

struct __locale_imp;

class locale {
public:
    class facet {
    protected:
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

    static locale& __global();

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


// inferred: the facet table of a locale (vector of facet pointers at 0x00/0x04, the name at 0x0C)
struct __locale_imp {
    locale::facet** facets_; // 0x00
    size_t nfacets_;         // 0x04
    size_t capacity_;        // 0x08
    char name_[0xC];         // 0x0C: basic_string<char>

    void replace(locale::facet* f, size_t id);
};

template <class Facet>
inline const Facet& use_facet(const locale& loc) {
    size_t i = Facet::id.get();
    __locale_imp* imp = loc.imp_.ptr_;
    if (i >= imp->nfacets_ || imp->facets_[i] == 0) {
        Facet* f = new Facet;
        imp->replace(f, Facet::id.get());
    }
    return *(const Facet*)imp->facets_[i];
}

} // namespace std

// inferred: the classic tables of MSL C (classification, tolower and toupper maps)
extern "C" {
extern const unsigned short lbl_80765338[];
extern const char lbl_80765538[];
extern const char lbl_80765638[];
void* memcpy(void* dst, const void* src, std::size_t n);
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
    virtual char do_toupper(char c) const { return upper_[(unsigned char)c]; }
    virtual const char* do_toupper(char* low, const char* high) const;
    virtual char do_tolower(char c) const { return lower_[(unsigned char)c]; }
    virtual const char* do_tolower(char* low, const char* high) const;
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
    explicit codecvt(size_t refs = 0) : locale::facet(refs) {}
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
                         char* to_end, char*& to_next) const;
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
    explicit codecvt(size_t refs = 0) : locale::facet(refs) {}
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

protected:
    basic_streambuf() : eback_(0), gptr_(0), egptr_(0), pbase_(0), pptr_(0), epptr_(0) {}

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
        : m24_(0), buffer_(0), m38_(0), m48_(false), m4A_(false), owns_buffer_(false), m4C_(false) {
        this->eback_ = 0;
        this->gptr_ = 0;
        this->egptr_ = 0;
        this->pptr_ = 0;
        this->pbase_ = 0;
        this->epptr_ = 0;
        m34_ = 0;
        m30_ = 0;
        m2C_ = 0;
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
    virtual std::fpos<std::mbstate_t> seek(std::streamoff off, std::ios_base::seekdir way) = 0;

    int flush_put();
    int empty_get();
    int fill_get();

    int m24_;              // 0x24
    C* buffer_;            // 0x28
    int m2C_;              // 0x2C
    int m30_;              // 0x30
    int m34_;              // 0x34
    int m38_;              // 0x38
    int max_length_;       // 0x3C
    int encoding_;         // 0x40
    const cvt* a_codecvt_; // 0x44
    bool m48_;             // 0x48
    bool always_noconv_;   // 0x49
    bool m4A_;             // 0x4A
    bool owns_buffer_;     // 0x4B
    bool m4C_;             // 0x4C
};

template <class C, class T = std::char_traits<C> >
class console_outputbuf : public bufferedbuf<C, T> {
public:
    explicit console_outputbuf(_FILE* file);
    virtual ~console_outputbuf() {}

protected:
    virtual int sync();
    virtual std::streamsize write(const char* s, std::streamsize n) { return fwrite(s, 1, n, file_); }
    virtual std::streamsize read(char* s, std::streamsize n) { return -1; }
    virtual std::fpos<std::mbstate_t> seek(std::streamoff off, std::ios_base::seekdir way) { return -1; }

    bool m4D_;     // 0x4D
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
    this->m4D_ = false;
    this->m4C_ = true;
}

} // namespace Metrowerks

namespace std {

void ios_base::throw_failure(const char* msg) {
    throw failure(msg);
}

ios_base::~ios_base() {
    for (unsigned long i = ncallbacks_; i != 0;) {
        --i;
        callbacks_[i].fn_(erase_event, *this, callbacks_[i].index_);
    }
    delete loc_;
    free(pwords_);
    free(iwords_);
    free(callbacks_);
}

void ios_base::init(void* sb) {
    rdbuf_ = sb;
    exceptions_ = goodbit;
    state_ = (unsigned char)(int)(sb == 0);
    flags_ = skipws | dec;
    width_ = 0;
    precision_ = 6;
    callbacks_ = 0;
    ncallbacks_ = 0;
    callbacks_cap_ = 0;
    iwords_ = 0;
    niwords_ = 0;
    pwords_ = 0;
    npwords_ = 0;
    loc_ = 0;
    try {
        loc_ = new locale;
    } catch (...) {
        state_ = badbit;
        throw;
    }
}

} // namespace std

namespace std {

// inferred: raw storage of the standard streams, built by ios_base::Init (base 0x808327C0)
static double __cin_storage[sizeof(istream) / 8 + 1];
static double __cout_storage[sizeof(ostream) / 8];
static double __clog_storage[sizeof(ostream) / 8];
static double __cerr_storage[sizeof(ostream) / 8];
static double __wcin_storage[sizeof(wistream) / 8 + 1];
static double __wcout_storage[sizeof(wostream) / 8];
static double __wclog_storage[sizeof(wostream) / 8];
static double __wcerr_storage[sizeof(wostream) / 8];

#define cin (*(istream*)__cin_storage)
#define cout (*(ostream*)__cout_storage)
#define clog (*(ostream*)__clog_storage)
#define cerr (*(ostream*)__cerr_storage)
#define wcin (*(wistream*)__wcin_storage)
#define wcout (*(wostream*)__wcout_storage)
#define wclog (*(wostream*)__wclog_storage)
#define wcerr (*(wostream*)__wcerr_storage)

int ios_base::Init::init_cnt_;

// inferred: the wide streams have their own initializer (name invented, see include/std/ios_init.h)
class wios_base_Init_unk {
public:
    wios_base_Init_unk();
    ~wios_base_Init_unk();

private:
    static int init_cnt_;
};

int wios_base_Init_unk::init_cnt_;

// inferred: the mutex of the stream initialization, built once (guard 0x80880380, flag 0x80880385)
static bool __ios_mutex_ready_unk;

inline Metrowerks::mutex& __ios_mutex_unk() {
    static Metrowerks::mutex m;
    return m;
}

static Metrowerks::mutex& __get_ios_mutex_unk() {
    if (!__ios_mutex_ready_unk) {
        __ios_mutex_unk();
    }
    __ios_mutex_ready_unk = true;
    return __ios_mutex_unk();
}

ios_base::Init::Init() {
    Metrowerks::scoped_lock<Metrowerks::mutex> lock(__get_ios_mutex_unk());
    static Metrowerks::console_inputbuf<char> fin(&__files[0]);
    static Metrowerks::console_outputbuf<char> fout(&__files[1]);
    static Metrowerks::console_outputbuf<char> ferr(&__files[2]);
    if (init_cnt_++ == 0) {
        new (&cin) istream(&fin);
        new (&cout) ostream(&fout);
        new (&cerr) ostream(&ferr);
        new (&clog) ostream(&ferr);
        cin.tie(&cout);
        cerr.setf(unitbuf);
        cerr.tie(&cout);
        fin.pubsetbuf(0, 0);
        fout.pubsetbuf(0, 0);
        ferr.pubsetbuf(0, 0);
    }
}

ios_base::Init::~Init() {
    Metrowerks::scoped_lock<Metrowerks::mutex> lock(__get_ios_mutex_unk());
    if (--init_cnt_ == 0) {
        cout.flush();
        cerr.flush();
        clog.flush();
        cin.~istream();
        cout.~ostream();
        cerr.~ostream();
        clog.~ostream();
    }
}

} // namespace std

namespace std {

wios_base_Init_unk::wios_base_Init_unk() {
    Metrowerks::scoped_lock<Metrowerks::mutex> lock(__get_ios_mutex_unk());
    static Metrowerks::console_inputbuf<wchar_t> fin(&__files[0]);
    static Metrowerks::console_outputbuf<wchar_t> fout(&__files[1]);
    static Metrowerks::console_outputbuf<wchar_t> ferr(&__files[2]);
    if (init_cnt_++ == 0) {
        new (&wcin) wistream(&fin);
        new (&wcout) wostream(&fout);
        new (&wcerr) wostream(&ferr);
        new (&wclog) wostream(&ferr);
        wcin.tie(&wcout);
        wcerr.setf(ios_base::unitbuf);
        wcerr.tie(&wcout);
        fin.pubsetbuf(0, 0);
        fout.pubsetbuf(0, 0);
        ferr.pubsetbuf(0, 0);
    }
}

wios_base_Init_unk::~wios_base_Init_unk() {
    Metrowerks::scoped_lock<Metrowerks::mutex> lock(__get_ios_mutex_unk());
    if (--init_cnt_ == 0) {
        wcout.flush();
        wcerr.flush();
        wclog.flush();
        wcin.~wistream();
        wcout.~wostream();
        wcerr.~wostream();
        wclog.~wostream();
    }
}

} // namespace std

namespace std {

ctype<char>::ctype(const mask* tab, bool del, size_t refs)
    : locale::facet(refs), table_(tab), lower_(lbl_80765538), upper_(lbl_80765638), del_(del) {
    if (table_ == 0) {
        table_ = lbl_80765338;
        del_ = false;
    }
}

ctype<char>::~ctype() {
    if (del_) {
        delete[] table_;
    }
}

const char* ctype<char>::do_toupper(char* low, const char* high) const {
    for (; low < high; ++low) {
        *low = ctype<char>::do_toupper(*low);
    }
    return high;
}

const char* ctype<char>::do_tolower(char* low, const char* high) const {
    for (; low < high; ++low) {
        *low = ctype<char>::do_tolower(*low);
    }
    return high;
}

const char* ctype<char>::do_widen(const char* low, const char* high, char* to) const {
    memcpy(to, low, high - low);
    return high;
}

const char* ctype<char>::do_narrow(const char* low, const char* high, char dfault, char* to) const {
    memcpy(to, low, high - low);
    return high;
}

} // namespace std
