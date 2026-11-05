#ifndef ATN_CURVE_H
#define ATN_CURVE_H

#include <boost/scoped_ptr.hpp>

extern "C" double sin(double x);
extern "C" double cos(double x);

namespace atn {

// inferred: curve y = f(x) that keeps its integral and its derivative; member names invented
// NONMATCHING: the weak copies checked_delete<Curve> (22.5%) and ~Curve (26.8%) and the ten
// vf08/vf0C (about 55%) differ only in how deep the mutually recursive inlines ~scoped_ptr ->
// checked_delete -> ~Curve are expanded: the original expands checked_delete less (it calls ~Curve)
// and ~Curve and the reset in vf08/vf0C more (three levels of ~Curve, then a call to ~Curve).
// ~scoped_ptr<Curve> already matches. Tried: explicit inline / out-of-line / late inline ~Curve,
// virtual ~Curve, own scoped_ptr variants (reset without swap, delete without checked_delete).
class Curve {
public:
    Curve() {}

    boost::scoped_ptr<Curve> m00; // inferred: integral (filled by vf08)
    boost::scoped_ptr<Curve> m04; // inferred: derivative (filled by vf0C)

    virtual Curve* vf08() = 0;           // inferred: integral
    virtual Curve* vf0C() = 0;           // inferred: derivative
    virtual float vf10(float x) const = 0;    // inferred: value at x
    virtual void vf14(float scale) = 0;  // inferred: scales the curve
};

// inferred: a * sin(b * x + c)
class SinusCurve : public Curve {
public:
    SinusCurve(float a, float b, float c) : m0C(a), m10(b), m14(c) {}
    virtual Curve* vf08();
    virtual Curve* vf0C();
    virtual float vf10(float x) const { return m0C * (float)sin(m10 * x + m14); }
    virtual void vf14(float scale) { m0C *= scale; }

    float m0C;
    float m10;
    float m14;
};

// inferred: a * cos(b * x + c)
class CosinusCurve : public Curve {
public:
    CosinusCurve(float a, float b, float c) : m0C(a), m10(b), m14(c) {}
    virtual Curve* vf08();
    virtual Curve* vf0C();
    virtual float vf10(float x) const { return m0C * (float)cos(m10 * x + m14); }
    virtual void vf14(float scale) { m0C *= scale; }

    float m0C;
    float m10;
    float m14;
};

class ConstantCurve : public Curve {
public:
    ConstantCurve(float a) : m0C(a) {}
    virtual Curve* vf08();
    virtual Curve* vf0C();
    virtual float vf10(float x) const { return m0C; }
    virtual void vf14(float scale) { m0C *= scale; }

    float m0C;
};

// inferred: a * x + b
class Polynomial1Curve : public Curve {
public:
    Polynomial1Curve(float a, float b) : m0C(a), m10(b) {}
    virtual Curve* vf08();
    virtual Curve* vf0C();
    virtual float vf10(float x) const { return m0C * x + m10; }
    virtual void vf14(float scale) {
        m0C *= scale;
        m10 *= scale;
    }

    float m0C;
    float m10;
};

// inferred: a * x * x + b * x + c
class Polynomial2Curve : public Curve {
public:
    Polynomial2Curve(float a, float b, float c) : m0C(a), m10(b), m14(c) {}
    virtual Curve* vf08();
    virtual Curve* vf0C();
    virtual float vf10(float x) const { return m0C * (x * x) + m10 * x + m14; }
    virtual void vf14(float scale) {
        m0C *= scale;
        m10 *= scale;
        m14 *= scale;
    }

    float m0C;
    float m10;
    float m14;
};

} // namespace atn

#endif
