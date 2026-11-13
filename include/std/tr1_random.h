#ifndef STD_TR1_RANDOM_H
#define STD_TR1_RANDOM_H

// inferred: MSL TR1 <random> as used by the game (MP/CryptMP and atn/MotionBank_unk).

namespace std {
namespace tr1 {

// inferred: MSL TR1 <random> (no source available). The state is kept twice (2 * n words) and
// refilled one half at a time (0x804AE568).
template <class UIntType, int w, int n, int m, int r, UIntType a, int u, int s, UIntType b, int t,
          UIntType c, int l>
class mersenne_twister {
public:
    explicit mersenne_twister(unsigned long value) { seed(value); }
    mersenne_twister(const mersenne_twister& other) { *this = other; }

    void seed(unsigned long value) {
        x[0] = value;
        for (i = 1; i < n; i++) {
            x[i] = 1812433253 * (x[i - 1] ^ (x[i - 1] >> 30)) + i;
        }
    }

    UIntType min() const { return 0; }
    UIntType max() const {
        UIntType res = 0;
        for (int k = 0; k < w; k++) {
            res |= (UIntType)1 << k;
        }
        return res;
    }

    UIntType operator()() {
        if (i == n) {
            twist(0);
        } else if (i >= 2 * n) {
            twist(1);
        }
        UIntType z = x[i];
        i++;
        z ^= z >> u;
        z ^= (z << s) & b;
        z ^= (z << t) & c;
        z ^= z >> l;
        return z;
    }

    void twist(int phase);

    UIntType x[2 * n];
    int i;
};

template <class UIntType, int w, int n, int m, int r, UIntType a, int u, int s, UIntType b, int t,
          UIntType c, int l>
void mersenne_twister<UIntType, w, n, m, r, a, u, s, b, t, c, l>::twist(int phase) {
    if (phase == 0) {
        for (int j = n; j < 2 * n; j++) {
            UIntType y = (x[j - n] & 0x80000000) | (x[j - n + 1] & 0x7FFFFFFF);
            x[j] = (y >> 1) ^ x[j - n + m] ^ (y & 1 ? a : 0);
        }
    } else if (phase == 1) {
        int j;
        for (j = 0; j < n - m; j++) {
            UIntType y = (x[j + n] & 0x80000000) | (x[j + n + 1] & 0x7FFFFFFF);
            x[j] = (y >> 1) ^ x[j + n + m] ^ (y & 1 ? a : 0);
        }
        for (j = n - m; j < n - 1; j++) {
            UIntType y = (x[j + n] & 0x80000000) | (x[j + n + 1] & 0x7FFFFFFF);
            x[j] = (y >> 1) ^ x[j - n + m] ^ (y & 1 ? a : 0);
        }
        UIntType y = (x[2 * n - 1] & 0x80000000) | (x[0] & 0x7FFFFFFF);
        x[n - 1] = x[m - 1] ^ (y >> 1) ^ (y & 1 ? a : 0);
        i = 0;
    }
}

typedef mersenne_twister<unsigned long, 32, 624, 397, 31, 0x9908B0DF, 11, 7, 0x9D2C5680, 15, 0xEFC60000, 18>
    mt19937;

template <class RealType = double>
class uniform_real {
public:
    explicit uniform_real(RealType min_arg = RealType(0), RealType max_arg = RealType(1))
        : mMin(min_arg), mMax(max_arg) {}
    template <class Gen>
    RealType operator()(Gen& gen) {
        return mMin + (mMax - mMin) * ((gen() - gen.min()) / gen.max());
    }

    RealType mMin;
    RealType mMax;
};

// inferred: engine adapter giving values in [0, 1)
template <class Engine>
class __engine_holder_unk {
public:
    __engine_holder_unk(Engine e) : mEng(e) {}

    Engine mEng;
};

template <class Engine>
class __engine_holder2_unk : public __engine_holder_unk<Engine> {
public:
    __engine_holder2_unk(Engine e) : __engine_holder_unk<Engine>(e) {}
};

template <class Engine>
class __engine_wrapper_unk : public __engine_holder2_unk<Engine> {
public:
    __engine_wrapper_unk(Engine e) : __engine_holder2_unk<Engine>(e) {
        mFactor = 1.0 / (1.0 + (double)(this->mEng.max() - this->mEng.min()));
    }
    double operator()() { return mFactor * (double)this->mEng(); }
    double min() const { return 0.0; }
    double max() const { return 1.0; }

    double mFactor;
};

template <class Engine, class Distribution>
class variate_generator {
public:
    variate_generator(Engine e, Distribution d) : mEng(e), mDist(d) {}
    double operator()() { return mDist(mEng); }

    __engine_wrapper_unk<Engine> mEng;
    Distribution mDist;
};

} // namespace tr1
} // namespace std

#endif
