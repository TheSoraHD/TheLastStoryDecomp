#include "game/Sword.h"

Sword::Sword(void* arg04, int arg08, int arg0C, int arg270) : Arms(arg04, arg08, arg0C, arg270) {}

Sword::~Sword() {}

int Sword::vf0C() {
    int result = 0;
    if (Arms::vf0C()) {
        result = 1;
    }
    return result;
}

void Sword::vf18(const atn::Vector3& a, const atn::Vector3& b, int c, int d) {}
