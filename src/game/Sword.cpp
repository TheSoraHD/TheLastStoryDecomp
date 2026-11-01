#include "game/Sword.h"

Sword::Sword(int kind, int id, int arg0C, CharaTask* chara, int arg5) : Arms(kind, id, arg0C, chara, arg5) {}

Sword::~Sword() {}

int Sword::vf0C() {
    int result = 0;
    if (Arms::vf0C()) {
        result = 1;
    }
    return result;
}

void Sword::vf18(const atn::Vector3& a, const atn::Vector3& b, int c, int d) {}
