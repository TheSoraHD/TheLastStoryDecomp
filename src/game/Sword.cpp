#include "game/Sword.h"

void Sword::vf18() {}

int Sword::vf0C() {
    int result = 0;
    if (Arms::vf0C()) {
        result = 1;
    }
    return result;
}

Sword::~Sword() {}

Sword::Sword(void* arg04, int arg08, int arg0C, int arg270) : Arms(arg04, arg08, arg0C, arg270) {}
