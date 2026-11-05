#include <boost/scoped_ptr.hpp>
#include "atn/Memory.h"
#include "atn/Curve.h"


namespace atn {

// NONMATCHING: 56.7% - see the note on Curve (inline depth of the scoped_ptr reset)
Curve* SinusCurve::vf08() {
    m00.reset(new ((MemoryTag)1, "", "", 0) CosinusCurve(-m0C / m10, m10, m14));
    return m00.get();
}

// inferred: the original passes the frequency as the amplitude
// NONMATCHING: 55.7% - see the note on Curve (inline depth of the scoped_ptr reset)
Curve* SinusCurve::vf0C() {
    m04.reset(new ((MemoryTag)1, "", "", 0) CosinusCurve(m10, m10, m14));
    return m04.get();
}

// NONMATCHING: 56.4% - see the note on Curve (inline depth of the scoped_ptr reset)
Curve* CosinusCurve::vf08() {
    m00.reset(new ((MemoryTag)1, "", "", 0) SinusCurve(m0C / m10, m10, m14));
    return m00.get();
}

// inferred: the original passes the frequency as the amplitude
// NONMATCHING: 56.1% - see the note on Curve (inline depth of the scoped_ptr reset)
Curve* CosinusCurve::vf0C() {
    m04.reset(new ((MemoryTag)1, "", "", 0) SinusCurve(-m10, m10, m14));
    return m04.get();
}

// NONMATCHING: 55.4% - see the note on Curve (inline depth of the scoped_ptr reset)
Curve* ConstantCurve::vf08() {
    m00.reset(new ((MemoryTag)1, "", "", 0) Polynomial1Curve(m0C, 0.0f));
    return m00.get();
}

// NONMATCHING: 54.7% - see the note on Curve (inline depth of the scoped_ptr reset)
Curve* ConstantCurve::vf0C() {
    m04.reset(new ((MemoryTag)1, "", "", 0) ConstantCurve(0.0f));
    return m04.get();
}

// NONMATCHING: 56.7% - see the note on Curve (inline depth of the scoped_ptr reset)
Curve* Polynomial1Curve::vf08() {
    m00.reset(new ((MemoryTag)1, "", "", 0) Polynomial2Curve(m0C * 0.5f, m10, 0.0f));
    return m00.get();
}

// NONMATCHING: 54.7% - see the note on Curve (inline depth of the scoped_ptr reset)
Curve* Polynomial1Curve::vf0C() {
    m04.reset(new ((MemoryTag)1, "", "", 0) ConstantCurve(m0C));
    return m04.get();
}

// inferred: the original returns a zero constant here
// NONMATCHING: 54.7% - see the note on Curve (inline depth of the scoped_ptr reset)
Curve* Polynomial2Curve::vf08() {
    m00.reset(new ((MemoryTag)1, "", "", 0) ConstantCurve(0.0f));
    return m00.get();
}

// inferred: the original halves the first coefficient instead of doubling it
// NONMATCHING: 56.0% - see the note on Curve (inline depth of the scoped_ptr reset)
Curve* Polynomial2Curve::vf0C() {
    m04.reset(new ((MemoryTag)1, "", "", 0) Polynomial1Curve(m0C * 0.5f, m10));
    return m04.get();
}

} // namespace atn
