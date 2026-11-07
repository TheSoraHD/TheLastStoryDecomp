// MSL C++ tr1::shared_ptr support: the release of a control block (name of the file invented).
// inferred: own translation unit 0x806952C4-0x8069533C, after the __sinit of locale.cpp.
#include "msl_iostream_unk.h"

namespace std {
namespace tr1 {
namespace detail {

void shared_ptr_deleter_common::release() {
    if (--use_count_ == 0) {
        dispose();
        if (--weak_count_ == 0) {
            delete this;
        }
    }
}

} // namespace detail
} // namespace tr1
} // namespace std
