// inferred: animation curve evaluation (Hermite and monotonic Bezier segments) following the public
// "animEngine.c" sample of the Maya devkit (engineBezierCreate/Evaluate, engineHermiteCreate/
// Evaluate, checkMonotonic, constrainInsideBounds, zeroin, polyZeroes), plus the game segment
// built by 0x80507790 and evaluated by 0x805077F0. Names invented.

extern "C" double sqrt(double x);
// inferred: fabs of a float through the double intrinsic (fabs + frsp in the original)
static inline float ffabs_unk(float x) {
    return __fabs(x);
}
#define fabs(x) ffabs_unk(x)

#define kOneThird (1.0f / 3.0f)
#define kTwoThirds (2.0f / 3.0f)
#define kFourThirds (4.0f / 3.0f)
#define kMaxTan 5729577.9485111479f

// inferred: Bezier segment of the engine (power basis of x and y)
struct EngineBezier_unk {
    int mIsLinear;     // 0x00
    float mX1;         // 0x04
    float mX4;         // 0x08
    float mCoeff[4];   // 0x0C
    float mPolyY[4];   // 0x1C
};

// inferred: Hermite segment of the engine
struct EngineHermite_unk {
    int m00;
    float mX1;       // 0x04
    float m08;
    float mCoeff[4]; // 0x0C
};

union EngineSegment_unk {
    EngineBezier_unk mBezier;
    EngineHermite_unk mHermite;
};

// inferred: segment of a game curve: control points x[4] (0x00) and y[4] (0x10), the time to
// evaluate (0x20) and whether the tangents are weighted (Bezier) or not (Hermite)
struct CurveSegment_unk {
    float mX[4];
    float mY[4];
    float mTime;          // 0x20
    unsigned char mBezier; // 0x24
};

static float sMachineTolerance;
static int sToleranceInit;

// inferred: polyZeroes (real roots of a polynomial in [a, b]); defined after zeroin
extern "C" int fn_80507BBC(float* poly, int deg, float a, int aClosed, float b, int bClosed, float* roots);
extern "C" int fn_80508458(EngineBezier_unk* bezier, float* x, float* y);
extern "C" float fn_80508818(EngineBezier_unk* bezier, float time);
extern "C" void fn_80508908(EngineHermite_unk* hermite, float* x, float* y);

// inferred: builds a segment from two keys {x, y, tangent x, tangent y}
extern "C" void fn_80507790(CurveSegment_unk* seg, float time, const float* a, const float* b, unsigned char bezier) {
    seg->mTime = time;
    seg->mBezier = bezier;
    seg->mX[0] = a[0];
    seg->mY[0] = a[1];
    seg->mX[1] = a[0] + kOneThird * a[2];
    seg->mY[1] = a[1] + kOneThird * a[3];
    seg->mX[3] = b[0];
    seg->mY[3] = b[1];
    seg->mX[2] = b[0] - kOneThird * b[2];
    seg->mY[2] = b[1] - kOneThird * b[3];
}

static inline float engineHermiteEvaluate(EngineHermite_unk* hermite, float time) {
    if (hermite == 0) {
        return 0.0f;
    }
    float t = time - hermite->mX1;
    return t * (t * (t * hermite->mCoeff[0] + hermite->mCoeff[1]) + hermite->mCoeff[2]) + hermite->mCoeff[3];
}

// inferred: value of the segment at mTime
extern "C" float fn_805077F0(CurveSegment_unk* seg) {
    EngineSegment_unk curve;
    curve.mBezier.mIsLinear = 0;
    if (seg->mBezier) {
        if (fn_80508458(&curve.mBezier, seg->mX, seg->mY)) {
            return fn_80508818(&curve.mBezier, seg->mTime);
        }
        return 0.0f;
    }
    fn_80508908(&curve.mHermite, seg->mX, seg->mY);
    return engineHermiteEvaluate(&curve.mHermite, seg->mTime);
}

// inferred: polynomial of the root finder (coefficients from degree 0, degree)
struct Polynomial_unk {
    float* mCoeff;
    int mDeg;
};

static inline float evalPoly(Polynomial_unk* poly, float x) {
    float* c = poly->mCoeff;
    int i = poly->mDeg;
    float r = c[i];
    while (--i >= 0) {
        r = x * r + c[i];
    }
    return r;
}

static inline float myabs(float x) {
    return x < 0.0f ? -x : x;
}

static inline bool equivalent(float a, float b) {
    return myabs(a - b) <= 0.0001f;
}

// NONMATCHING: 99.0% - register allocation of the interpolation temporaries (q in f1 here, f21 in
// the original); the double constants are separate objects here (one 0x20 table in the original).
// inferred: zeroin (Brent's method) on [a, b] with f(a) = fa, f(b) = fb of opposite signs
extern "C" float fn_805078A4(Polynomial_unk* poly, float a, float b, float fa, float fb, float tol) {
    float c, d, e, fc, tol1, xm, p, q, r, s;
    float eps = sMachineTolerance;
    for (;;) {
        c = a;
        fc = fa;
        d = b - a;
        e = d;
        do {
            if (fabs(fc) < fabs(fb)) {
                a = b;
                b = c;
                c = a;
                fa = fb;
                fb = fc;
                fc = fa;
            }
            tol1 = 2.0f * eps * fabs(b) + 0.5f * tol;
            xm = 0.5f * (c - b);
            bool more = fabs(xm) > tol1 && !equivalent(fb, 0.0f);
            if (!more) {
                goto done;
            }
            if (fabs(e) < tol1 || fabs(fa) <= fabs(fb)) {
                d = xm;
                e = d;
            } else {
                s = fb / fa;
                if (a == c) {
                    p = 2.0f * xm * s;
                    q = 1.0f - s;
                } else {
                    q = fa / fc;
                    r = fb / fc;
                    p = s * (2.0f * xm * q * (q - r) - (b - a) * (r - 1.0f));
                    q = (q - 1.0f) * (r - 1.0f) * (s - 1.0f);
                }
                if (p > 0.0) {
                    q = -q;
                } else {
                    p = -p;
                }
                float olde = e;
                e = d;
                if (2.0 * p < 3.0 * xm * q - fabs(tol1 * q) && p < __fabs(0.5 * olde * q)) {
                    d = p / q;
                } else {
                    d = xm;
                    e = d;
                }
            }
            a = b;
            fa = fb;
            if (fabs(d) > tol1) {
                b += d;
            } else if (xm > 0.0) {
                b += tol1;
            } else {
                b -= tol1;
            }
            fb = evalPoly(poly, b);
        } while (!(fb * (fc / fabs(fc)) > 0.0));
    }
done:
    return b;
}

// NONMATCHING: 84.2% - same structure as the original (devkit polyZeroes); register allocation, the
// stack slots of the int->float temporaries and the operand order of `tol` differ
// inferred: polyZeroes of the Maya devkit sample (animEngine.c): real roots of the polynomial
// `Poly` of degree `deg` in [a, b] (ends included when a_closed/b_closed), sorted; -1 when the
// polynomial is zero
extern "C" int fn_80507BBC(float* Poly, int deg, float a, int a_closed, float b, int b_closed, float* roots) {
    int i, left_ok, right_ok, nr, ndr, skip;
    float e, f, s, pe, ps, tol, p[22], d[22], dr[22];
    Polynomial_unk ply;

    e = pe = 0.0f;
    f = 0.0f;
    for (i = 0; i < deg + 1; ++i) {
        f += fabs(Poly[i]);
    }
    tol = (fabs(a) + fabs(b)) * (deg + 1) * sMachineTolerance;

    if (f <= tol) {
        return -1;
    }

    // normalise the polynomial
    for (i = 0; i < deg + 1; ++i) {
        p[i] = (1.0f / f) * Poly[i];
    }

    // find the real degree
    while (deg > 0 && fabs(p[deg]) < tol) {
        deg--;
    }

    nr = 0;
    if (deg == 0) {
        return 0;
    }
    if (deg == 1) {
        roots[0] = -p[0] / p[1];
        left_ok = a_closed ? (a < roots[0] + tol) : (a < roots[0] - tol);
        right_ok = b_closed ? (b > roots[0] - tol) : (b > roots[0] + tol);
        nr = (left_ok && right_ok) ? 1 : 0;
        if (nr) {
            if (a_closed && roots[0] < a) {
                roots[0] = a;
            } else if (b_closed && roots[0] > b) {
                roots[0] = b;
            }
        }
        return nr;
    }

    // derivative
    ply.mCoeff = p;
    ply.mDeg = deg;
    for (i = 1; i <= deg; ++i) {
        d[i - 1] = (float)i * p[i];
    }
    ndr = fn_80507BBC(d, deg - 1, a, 0, b, 0, dr);
    if (ndr == -1) {
        return 0;
    }

    skip = 0;
    for (i = 0; i <= ndr; ++i) {
        if (nr > deg) {
            return nr;
        }
        if (i == 0) {
            s = a;
            ps = evalPoly(&ply, s);
            if (fabs(ps) <= tol && a_closed) {
                roots[nr++] = a;
            }
        } else {
            s = e;
            ps = pe;
        }
        e = (i == ndr) ? b : dr[i];
        pe = evalPoly(&ply, e);
        if (skip) {
            skip = 0;
        } else {
            if (fabs(pe) < tol) {
                if (i != ndr || b_closed) {
                    roots[nr++] = e;
                    skip = 1;
                }
            } else if ((ps < 0.0f && pe > 0.0f) || (ps > 0.0f && pe < 0.0f)) {
                roots[nr++] = fn_805078A4(&ply, s, e, ps, pe, 0.0f);
                if (nr > 1 && roots[nr - 2] >= roots[nr - 1] - tol) {
                    roots[nr - 2] = (roots[nr - 2] + roots[nr - 1]) * 0.5f;
                    nr--;
                }
            }
        }
    }
    return nr;
}

// inferred: comparison through an inline returning bool (mfcr in the original)
static inline bool __IsGreater_unk(float a, float b) {
    return a > b;
}

static inline void init_tolerance() {
    float one, half, sum;
    one = 1.0f;
    half = 0.5f;
    sMachineTolerance = 1.0f;
    do {
        sMachineTolerance *= half;
        sum = sMachineTolerance + one;
    } while (__IsGreater_unk(sum, one));
    sMachineTolerance = 2.0f * sMachineTolerance;
}

static inline void constrainInsideBounds(float* x1, float* x2) {
    float b, c, discr, root;
    if ((*x1 + sMachineTolerance) < kFourThirds) {
        b = *x1 - 2.0f;
        c = *x1 - 1.0f;
        discr = sqrt(b * b - 4.0f * c * c);
        root = (-b + discr) * 0.5f;
        if ((*x2 + sMachineTolerance) > root) {
            *x2 = root - sMachineTolerance;
        } else {
            root = (-b - discr) * 0.5f;
            if (*x2 < (root + sMachineTolerance)) {
                *x2 = root + sMachineTolerance;
            }
        }
    } else {
        *x1 = kFourThirds - sMachineTolerance;
        *x2 = kOneThird - sMachineTolerance;
    }
}

static inline void checkMonotonic(float* x1, float* x2) {
    *x2 = 1.0f - *x2;
    if (*x1 < 0.0) {
        *x1 = 0.0f;
    }
    if (*x2 < 0.0) {
        *x2 = 0.0f;
    }
    if ((*x1 > 1.0f) || (*x2 > 1.0f)) {
        float d = *x1 * (*x1 - 2.0f + *x2) + *x2 * (*x2 - 2.0f) + 1.0f;
        if (d + sMachineTolerance > 0.0f) {
            constrainInsideBounds(x1, x2);
        }
    }
    *x2 = 1.0f - *x2;
}

static inline void bezierToPower(float a1, float b1, float c1, float d1, float* a2, float* b2, float* c2,
                                 float* d2) {
    float a = b1 - a1;
    float b = c1 - b1;
    float c = d1 - c1;
    float d = b - a;
    *a2 = c - b - d;
    *b2 = d + d + d;
    *c2 = a + a + a;
    *d2 = a1;
}

// NONMATCHING: 98.2% - the flag test is scheduled before the FPR saves in the original, and the
// .rodata offsets of 1.0/-1.0 move because polyZeroes (0x80507BBC, not reconstructed) adds the
// int-to-double constant before them.
// inferred: engineBezierCreate
extern "C" int fn_80508458(EngineBezier_unk* bezier, float* x, float* y) {
    if (!sToleranceInit) {
        init_tolerance();
        sToleranceInit = 1;
    }
    if (bezier == 0) {
        return 0;
    }
    float rangeX = x[3] - x[0];
    if (rangeX == 0.0) {
        return 0;
    }
    float dx1 = x[1] - x[0];
    float dx2 = x[2] - x[0];
    float nX1 = dx1 / rangeX;
    float nX2 = dx2 / rangeX;
    if (nX1 == kOneThird && nX2 == kTwoThirds) {
        bezier->mIsLinear = 1;
    } else {
        bezier->mIsLinear = 0;
    }
    float oldX1 = nX1;
    float oldX2 = nX2;
    if (nX1 < 0.0) {
        nX1 = 0.0f;
    }
    if (nX2 > 1.0) {
        nX2 = 1.0f;
    }
    if (nX1 > 1.0 || nX2 < -1.0) {
        checkMonotonic(&nX1, &nX2);
    }
    if (nX1 != oldX1) {
        x[1] = x[0] + nX1 * rangeX;
        if (oldX1 != 0.0) {
            y[1] = y[0] + (y[1] - y[0]) * nX1 / oldX1;
        }
    }
    if (nX2 != oldX2) {
        x[2] = x[0] + nX2 * rangeX;
        if (oldX2 != 1.0) {
            y[2] = y[3] - (y[3] - y[2]) * (1.0f - nX2) / (1.0f - oldX2);
        }
    }
    bezier->mX1 = x[0];
    bezier->mX4 = x[3];
    bezierToPower(0.0f, nX1, nX2, 1.0f, &bezier->mCoeff[3], &bezier->mCoeff[2], &bezier->mCoeff[1],
                  &bezier->mCoeff[0]);
    bezierToPower(y[0], y[1], y[2], y[3], &bezier->mPolyY[3], &bezier->mPolyY[2], &bezier->mPolyY[1],
                  &bezier->mPolyY[0]);
    return 1;
}

// inferred: engineBezierEvaluate
extern "C" float fn_80508818(EngineBezier_unk* bezier, float time) {
    float t, s, poly[4], roots[5];
    int numRoots;
    if (bezier == 0) {
        return 0.0f;
    }
    if (bezier->mX1 == time) {
        s = 0.0f;
    } else if (bezier->mX4 == time) {
        s = 1.0f;
    } else {
        s = (time - bezier->mX1) / (bezier->mX4 - bezier->mX1);
    }
    if (bezier->mIsLinear) {
        t = s;
    } else {
        poly[3] = bezier->mCoeff[3];
        poly[2] = bezier->mCoeff[2];
        poly[1] = bezier->mCoeff[1];
        poly[0] = bezier->mCoeff[0] - s;
        numRoots = fn_80507BBC(poly, 3, 0.0f, 1, 1.0f, 1, roots);
        if (numRoots == 1) {
            t = roots[0];
        } else {
            t = 0.0f;
        }
    }
    return t * (t * (t * bezier->mPolyY[3] + bezier->mPolyY[2]) + bezier->mPolyY[1]) + bezier->mPolyY[0];
}

// NONMATCHING: 99.4% - x[0] and x[1] are loaded in the other order
// inferred: engineHermiteCreate
extern "C" void fn_80508908(EngineHermite_unk* hermite, float* x, float* y) {
    float dx, dy, tan_x, m1, m2, length, d1, d2;
    if (hermite == 0) {
        return;
    }
    hermite->mX1 = x[0];
    m1 = m2 = kMaxTan;
    tan_x = x[1] - x[0];
    dx = x[3] - x[0];
    dy = y[3] - y[0];
    if (tan_x != 0.0) {
        m1 = (y[1] - y[0]) / tan_x;
    }
    tan_x = x[3] - x[2];
    if (tan_x != 0.0) {
        m2 = (y[3] - y[2]) / tan_x;
    }
    length = 1.0f / (dx * dx);
    d1 = dx * m1;
    d2 = dx * m2;
    hermite->mCoeff[0] = (d1 + d2 - dy - dy) * length / dx;
    hermite->mCoeff[1] = (dy + dy + dy - d1 - d1 - d2) * length;
    hermite->mCoeff[2] = m1;
    hermite->mCoeff[3] = y[0];
}
