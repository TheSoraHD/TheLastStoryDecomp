// MSL C++ iostream.cpp: ios_base, the standard streams and their initialisation (ios_base::Init).
// inferred: own translation unit 0x8068B39C-0x80691E50 (its string pool @stringBase0 0x8076645C ends the
// .rodata of the unit; locale.cpp starts at 0x80766538).
#include "msl_iostream_unk.h"

namespace std {

// inferred: called with bl by the sentry and flush (0x8069194C, 0x8068E3FC); MWCC would inline it there
__declspec(noinline) void ios_base::throw_failure(const char* msg) {
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
