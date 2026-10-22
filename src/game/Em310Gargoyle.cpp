#include "game/Em310Gargoyle.h"

Em310Gargoyle::~Em310Gargoyle() {}

// inferred: the damage sphere follows the collision sphere (m614/m620)
void Em310Gargoyle::vf54() {
    if (m624.mSize != 0) {
        Item14_unk* item = m624.mData;
        item->m04.mCenter = m614;
        item->m04.mRadius = m620;
    }
}
