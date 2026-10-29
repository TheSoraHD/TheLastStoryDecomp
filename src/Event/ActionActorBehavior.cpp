#include "game/SetConfig.h"
#include "Event/Cut.h"

extern "C" int fn_805510F8();
extern "C" int fn_80551100();

extern "C" void fn_8053EF70(void* self);
extern "C" void fn_8053F6EC(void* self);
extern "C" void fn_8054FBC8(void* self);

extern "C" void* lbl_8087F540;
extern "C" volatile float lbl_80887E20;
extern "C" volatile float lbl_80887E24;
extern "C" void fn_8047F8C0(void* object, int enabled);
extern "C" void fn_8047F6E4(void* object);
extern "C" void fn_8047F8B0(void* object);
extern "C" void fn_8047F850(void* object, int start, int colorStart, int colorEnd);
extern "C" void fn_8047F88C(void* object, int frame);
extern "C" void fn_8047F994(void* object, int arg1, int arg2);
extern "C" void fn_8047F984(void* object, float value);
extern "C" void fn_8053E8C8(void* self, int state);
extern "C" int fn_8054011C(void* self);
extern "C" void* fn_80541210(void* self);
extern "C" int fn_8010DB54(int eventId);
extern "C" void* lbl_807C9140[];
extern "C" void* lbl_8087F8B0;
void operator delete(void* ptr);
extern "C" void fn_80550A78(void* self);
extern "C" int fn_80541198(void* self);
extern "C" void fn_80541520(void* self, int target, int flags);
extern "C" void fn_8054FAD4(void* self);
extern "C" void fn_8054FB10(void* self, int mark);
extern "C" void fn_8054F9DC(void* self);
extern "C" void fn_8054FBE0(void* self, int mark);
extern "C" void fn_8054FFE0(void* self);
extern "C" void fn_80481A20(void* manager);
extern "C" void fn_80489A50(void* manager, int arg1, int arg2);
extern "C" void fn_80488DB8(void* manager);
extern "C" void fn_8048F9B0(void* manager, void* event);
extern "C" int fn_80541198(void* self);
extern "C" void fn_80541520(void* self, int target, int flags);
extern "C" void fn_80484FBC(void* manager);
extern "C" void fn_804870F4(void* manager);
extern "C" void fn_80486F50(void* manager);
extern "C" void fn_8048F184(void* manager, int arg);
extern "C" void fn_8048FC7C(void* manager);
extern "C" void fn_80370B78(void* object, int arg1, float arg2, int arg3, int arg4);
extern "C" void* lbl_8087F0A8;
extern "C" void* lbl_8087F430;

// NONMATCHING: 17.26% - releases 49 cached action instances and clears the 55-entry action dispatch table.
extern "C" void fn_8054F380() {
    void** cachedActions = reinterpret_cast<void**>(&lbl_8087F8B0);
    for (int i = 48; i >= 0; --i) {
        if (cachedActions[i] != 0) {
            operator delete(cachedActions[i]);
            cachedActions[i] = 0;
        }
    }
    for (int i = 0; i < 55; ++i) {
        lbl_807C9140[i] = 0;
    }
}

// NONMATCHING: 89.19% - state transitions match, but MWCC folds flag and helper paths differently.
extern "C" void fn_8054F9DC(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned int state = *reinterpret_cast<unsigned int*>(bytes + 0x194);
    unsigned int& flags = *reinterpret_cast<unsigned int*>(bytes + 0x98);
    if (state == 4) {
        flags |= 0x00300000;
    } else if (state == 5) {
        fn_8053F6EC(self);
        flags |= 0x00100000;
    } else if (state == 6) {
        flags |= 0x00100000;
    } else if (state == 8 || state == 14) {
        if (fn_8054011C(self)) {
            *reinterpret_cast<float*>(bytes + 0x198) = lbl_80887E20;
            *reinterpret_cast<float*>(bytes + 0x19C) = lbl_80887E20;
            fn_8053E8C8(self, 9);
        } else {
            fn_8053E8C8(self, 6);
            flags |= 0x00100000;
        }
    } else if (state == 12) {
        fn_8053E8C8(self, 10);
    }
    *reinterpret_cast<unsigned int*>(bytes + 0x38) &= ~2u;
    fn_8047F984(lbl_8087F540, lbl_80887E24);
}
extern "C" void fn_8054FBE0(void* self, int dispatch);
extern "C" void fn_8054FFE0(void* self);

extern "C" void fn_8054FAD4(void* self) {
    if (*reinterpret_cast<int*>(static_cast<unsigned char*>(self) + 0x194) == 10) {
        fn_8053E8C8(self, 11);
    }
    fn_8047F984(lbl_8087F540, lbl_80887E20);
}

// NONMATCHING: 95.65% - MWCC reuses each zero-float load for the pair of field stores.
extern "C" void fn_8054FB10(void* self, int mark) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int state = *reinterpret_cast<int*>(bytes + 0x194);
    if (state == 10 || state == 12) {
        fn_80550A78(self);
        *reinterpret_cast<float*>(bytes + 0x198) = lbl_80887E20;
        *reinterpret_cast<float*>(bytes + 0x19C) = lbl_80887E20;
        *reinterpret_cast<int*>(bytes + 0x1A4) = -1;
        fn_8053E8C8(self, 7);
    } else if (state == 14) {
        *reinterpret_cast<float*>(bytes + 0x198) = lbl_80887E20;
        *reinterpret_cast<float*>(bytes + 0x19C) = lbl_80887E20;
        *reinterpret_cast<int*>(bytes + 0x1A4) = -1;
        fn_8053E8C8(self, 14);
    }
    fn_8047F984(lbl_8087F540, lbl_80887E24);
    if (mark != 0) {
        *reinterpret_cast<unsigned int*>(bytes + 0x98) |= 0x10000;
    }
}

// NONMATCHING: 81.67% - MWCC combines the bit extraction with a record-form instruction.
extern "C" void fn_8054FBC8(void* self) {
    if (((*reinterpret_cast<unsigned int*>(static_cast<unsigned char*>(self) + 0x98) >> 16) & 1) != 0) {
        fn_8053EF70(self);
    }
}

// NONMATCHING: 28.33% - handles active-action selection and timing; intrusive manager-list cleanup remains inferred.
extern "C" void fn_8054FBE0(void* self, int dispatch) {
    unsigned char* object = static_cast<unsigned char*>(self);
    int mode = *reinterpret_cast<int*>(object + 0x1B0);
    if (mode == 0) {
        return;
    }

    int previous = *reinterpret_cast<int*>(object + 0x1A0);
    int selected = *reinterpret_cast<int*>(object + 0x1B4);
    if (previous != selected) {
        if (dispatch != 0) {
            fn_80550A78(self);
        }

        int count = *reinterpret_cast<int*>(object + 0xBC);
        unsigned char** entries = *reinterpret_cast<unsigned char***>(object + 0xB8);
        unsigned char* oldEntry = previous >= 0 && previous < count ? entries[previous] : 0;
        unsigned char* newEntry = selected >= 0 && selected < count ? entries[selected] : 0;
        bool oldIsGroup = oldEntry != 0 && *reinterpret_cast<int*>(oldEntry + 0x10) == 3;
        bool newIsGroup = newEntry != 0 && *reinterpret_cast<int*>(newEntry + 0x10) == 3;
        if (!oldIsGroup || !newIsGroup) {
            fn_8047F994(lbl_8087F540, 1, 1);
        }
        *reinterpret_cast<int*>(object + 0x1A0) = selected;
    }

    if (mode == 1) {
        int frame = *reinterpret_cast<int*>(object + 0x1B8);
        *reinterpret_cast<float*>(object + 0x19C) = static_cast<float>(frame);
        int objectCount = *reinterpret_cast<int*>(object + 0x168);
        unsigned char* objects = *reinterpret_cast<unsigned char**>(object + 0x164);
        for (int i = 0; i < objectCount; ++i) {
            unsigned char* item = objects + i * 0x1C0;
            *reinterpret_cast<int*>(item + 0xB8) = 0;
            *reinterpret_cast<int*>(item + 0xBC) = 0;
            *reinterpret_cast<int*>(item + 0xC0) = 0;
        }

        int id = *reinterpret_cast<int*>(object + 0x190);
        bool clearFlag = id == 0x5E7 || id == 0x841;
        if (id == 0xE14 || id == 0x151E) {
            unsigned char* event = static_cast<unsigned char*>(fn_80541210(self));
            int type = event == 0 ? -1 : *reinterpret_cast<int*>(event + 0x0C);
            clearFlag = id == 0xE14 ? type == 0 : type == 3;
        }
        if (clearFlag) {
            *reinterpret_cast<unsigned int*>(object + 0x98) &= ~0x10000u;
        }
    } else {
        *reinterpret_cast<float*>(object + 0x19C) = lbl_80887E20;
    }

    *reinterpret_cast<float*>(object + 0x198) = static_cast<float>(*reinterpret_cast<int*>(object + 0x1B8));
    *reinterpret_cast<int*>(object + 0x1B0) = 0;
}

// NONMATCHING: 59.98% - camera transition timing and colors reconstructed from the event mode and frame checks.
extern "C" void fn_8054FFE0(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* event = static_cast<unsigned char*>(fn_80541210(self));
    if (event != 0) {
        int mode = *reinterpret_cast<int*>(object + 0x50);
        if (mode == 1 || mode == 3) {
            void* manager = lbl_8087F540;
            int start = *reinterpret_cast<int*>(object + 0x1A4);
            int current = *reinterpret_cast<int*>(object + 0x1A0);
            int eventStart = *reinterpret_cast<int*>(event + 0x1C);
            int changeMode = *reinterpret_cast<int*>(event + 0x18);
            if (start != current) {
                if (changeMode == 0) {
                    fn_8047F8B0(manager);
                } else if (changeMode == 1) {
                    fn_8047F850(manager, eventStart, static_cast<int>(0xFF000000), 0);
                } else if (changeMode == 2) {
                    fn_8047F850(manager, eventStart, -1, 0x00FFFFFF);
                } else if (changeMode == 3) {
                    fn_8047F88C(manager, eventStart);
                }
            }

            int frame = *reinterpret_cast<int*>(static_cast<unsigned char*>(manager) + 0x24);
            int end = *reinterpret_cast<int*>(static_cast<unsigned char*>(manager) + 0x2C);
            int remaining = end - frame;
            float firstWindow = *reinterpret_cast<float*>(object + 0x198);
            float secondWindow = *reinterpret_cast<float*>(object + 0x19C);
            if (firstWindow <= static_cast<float>(remaining) && secondWindow >= static_cast<float>(remaining)) {
                int transition = *reinterpret_cast<int*>(event + 0x20);
                if (transition == 1) {
                    fn_8047F850(manager, frame, 0, static_cast<int>(0xFF000000));
                } else if (transition == 2) {
                    fn_8047F850(manager, frame, 0x00FFFFFF, -1);
                }
            }

            if (frame != 0) {
                int midpoint = frame / 2;
                if (firstWindow <= static_cast<float>(end - midpoint) &&
                    secondWindow >= static_cast<float>(end - midpoint) &&
                    *reinterpret_cast<int*>(event + 0x20) == 3) {
                    fn_8047F88C(manager, frame);
                }
            }
            fn_8047F6E4(manager);
        }
    }
    *reinterpret_cast<int*>(object + 0x1A4) = *reinterpret_cast<int*>(object + 0x1A0);
}

extern "C" void fn_805501DC(void* self, int enabled) {
    int& state = *reinterpret_cast<int*>(static_cast<unsigned char*>(self) + 0x1C0);
    if (state == 0 && enabled != 0) {
        fn_8047F8C0(lbl_8087F540, 1);
    } else if (state != 0 && enabled == 0) {
        fn_8047F8C0(lbl_8087F540, 0);
    }
    state = enabled;
}

// NONMATCHING: 65.17% - manager gating, action windows, cut changes and timeouts are reconstructed;
// exact MWCC register allocation and nested action-state dispatch still differ.
extern "C" void fn_80550254(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned int& flags = *reinterpret_cast<unsigned int*>(object + 0x98);
    flags |= 0x00040000;
    if (SetConfig::sInstance != 0 && SetConfig::sInstance->mEventCameraControl == 0) {
        flags &= ~0x00040000;
    }

    void* manager = lbl_8087F540;
    void* current = *reinterpret_cast<void**>(object + 0x1C0);
    if (current != 0) {
        int type = *reinterpret_cast<int*>(static_cast<unsigned char*>(current) + 0x0C);
        void* handler;
        if (type == 0x1F) {
            unsigned char* first = *reinterpret_cast<int*>(static_cast<unsigned char*>(current) + 0x30) > 0
                ? *reinterpret_cast<unsigned char**>(static_cast<unsigned char*>(current) + 0x2C) : 0;
            handler = *reinterpret_cast<void**>(first + 4);
        } else {
            handler = lbl_807C9140[type];
        }
        if (handler == 0 || reinterpret_cast<int (*)(void*, void*)>(
                (*reinterpret_cast<void***>(handler))[2])(handler, current) != 3) {
            if (current != 0) {
                fn_8047F8C0(manager, 0);
            }
            *reinterpret_cast<void**>(object + 0x1C0) = 0;
        } else {
            return;
        }
    }

    fn_8054FBE0(self, 1);
    float& time = *reinterpret_cast<float*>(object + 0x198);
    time += *reinterpret_cast<float*>(static_cast<unsigned char*>(manager) + 0xB4);
    float& previousTime = *reinterpret_cast<float*>(object + 0x19C);
    if (previousTime > time) {
        previousTime = lbl_80887E20;
    }
    fn_8054FFE0(self);
    int mode = *reinterpret_cast<int*>(object + 0x50);
    if (mode == 1 || mode == 3) {
        fn_80481A20(manager);
        fn_80489A50(manager, 0, 0);
        fn_80488DB8(manager);
    }

    unsigned char* event = static_cast<unsigned char*>(fn_80541210(self));
    if (event != 0) {
        int count = *reinterpret_cast<int*>(event + 0x40);
        unsigned char** records = *reinterpret_cast<unsigned char***>(event + 0x3C);
        for (int i = 0; i < count; ++i) {
            unsigned char* record = records[i];
            if (record == 0 || *reinterpret_cast<int*>(record + 0x14) == 0) {
                break;
            }
            if (*reinterpret_cast<int*>(record + 0x0C) == 3) {
                int key = *reinterpret_cast<int*>(record + 0x10);
                int objectCount = *reinterpret_cast<int*>(object + 0x168);
                unsigned char* objects = *reinterpret_cast<unsigned char**>(object + 0x164);
                int selected = 0;
                for (; selected < objectCount; ++selected) {
                    if (*reinterpret_cast<int*>(objects + selected * 0x1C0 + 0x14) == key) {
                        break;
                    }
                }
                *reinterpret_cast<int*>(object + 0x1BC) = selected < objectCount ? selected * 0x1C0 : 0;
            } else {
                *reinterpret_cast<int*>(object + 0x1BC) = 0;
            }

            int groupCount = *reinterpret_cast<int*>(record + 0x1C);
            unsigned char** groups = *reinterpret_cast<unsigned char***>(record + 0x18);
            for (int j = 0; j < groupCount; ++j) {
                unsigned char* group = groups[j];
                if (group == 0 || *reinterpret_cast<int*>(group + 0x10) == 0) {
                    break;
                }
                int actionCount = *reinterpret_cast<int*>(group + 0x18);
                unsigned char** actions = *reinterpret_cast<unsigned char***>(group + 0x14);
                for (int k = 0; k < actionCount; ++k) {
                    unsigned char* action = actions[k];
                    int type = *reinterpret_cast<int*>(action + 0x0C);
                    void* handler;
                    if (type == 0x1F) {
                        unsigned char* entry = *reinterpret_cast<int*>(action + 0x30) > 0
                            ? *reinterpret_cast<unsigned char**>(action + 0x2C) : 0;
                        handler = *reinterpret_cast<void**>(entry + 4);
                    } else {
                        handler = lbl_807C9140[type];
                    }
                    if (handler == 0) {
                        continue;
                    }
                    float start = static_cast<float>(*reinterpret_cast<int*>(action + 0x10));
                    float end = static_cast<float>(*reinterpret_cast<int*>(action + 0x18) +
                                                   *reinterpret_cast<int*>(action + 0x10));
                    float lo = *reinterpret_cast<float*>(object + 0x19C);
                    float hi = *reinterpret_cast<float*>(object + 0x198);
                    bool starts = start >= lo - 1.0f && start < hi;
                    bool ends = end >= lo - 1.0f && end < hi;
                    int active = *reinterpret_cast<int*>(object + 0x1BC);
                    bool selected = active != 0 && start >= lo - 1.0f && end >= hi;
                    if (selected) {
                        void** table = *reinterpret_cast<void***>(handler);
                        reinterpret_cast<void (*)(void*, void*, int)>(table[3])(handler, action, 0);
                        reinterpret_cast<void (*)(void*, void*, int)>(table[3])(handler, action, 1);
                        reinterpret_cast<int (*)(void*, void*)>(table[2])(handler, action);
                        reinterpret_cast<void (*)(void*, void*, int)>(table[3])(handler, action, 2);
                        reinterpret_cast<void (*)(void*, void*, int)>(table[3])(handler, action, 3);
                    } else {
                        void** table = *reinterpret_cast<void***>(handler);
                        if (starts) reinterpret_cast<void (*)(void*, void*, int)>(table[3])(handler, action, 0);
                        if (ends) reinterpret_cast<void (*)(void*, void*, int)>(table[3])(handler, action, 1);
                        if (lo >= start && hi <= end) {
                            *reinterpret_cast<int*>(object + 0x1C0) =
                                reinterpret_cast<int (*)(void*, void*)>(table[2])(handler, action);
                        }
                        if (*reinterpret_cast<int*>(object + 0x1C0) == 3) {
                            fn_8047F8C0(manager, 1);
                        }
                    }
                }
            }
        }
    }

    fn_8054FBE0(self, 0);
    mode = *reinterpret_cast<int*>(object + 0x50);
    if (mode == 1 || mode == 3) {
        fn_8048F9B0(manager, event);
    }
    event = static_cast<unsigned char*>(fn_80541210(self));
    if (event != 0 && time > static_cast<float>(*reinterpret_cast<int*>(event + 0x2C))) {
        time = static_cast<float>(*reinterpret_cast<int*>(event + 0x2C));
        int cutId = reinterpret_cast<Event::Cut*>(event)->fn_8053B028();
        if (cutId != -1) {
            int count = *reinterpret_cast<int*>(object + 0xBC);
            unsigned char** entries = *reinterpret_cast<unsigned char***>(object + 0xB8);
            int i = 0;
            for (; i < count; ++i) {
                if (*reinterpret_cast<int*>(entries[i] + 0x0C) == cutId) break;
            }
            fn_80541520(self, fn_80541198(self), 0);
            if (mode == 1 || mode == 3) {
                fn_80484FBC(manager); fn_804870F4(manager); fn_80486F50(manager);
                fn_8048F184(manager, 0); fn_8048F184(manager, 1); fn_8048FC7C(manager);
            }
            if (reinterpret_cast<Event::Cut*>(event)->fn_8053B028() != *reinterpret_cast<int*>(event + 0x0C)) {
                fn_80550254(self);
            }
        } else if (lbl_8087F0A8 != 0 && *reinterpret_cast<int*>(static_cast<unsigned char*>(lbl_8087F0A8) + 0x194) != 0 &&
                   *reinterpret_cast<int*>(object + 0x190) == 0x19D) {
            fn_80370B78(lbl_8087F430, -1, lbl_80887E20, 0, 0);
            fn_8053E8C8(self, 11);
        } else {
            fn_8053E8C8(self, 13);
        }
    }
    *reinterpret_cast<float*>(object + 0x19C) = time;
}

// NONMATCHING: 52.41% - walks the action table and dispatches active window callbacks; table layout
// is inferred from the field accesses at 0x80550A78.
extern "C" void fn_80550A78(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* event = static_cast<unsigned char*>(fn_80541210(self));
    if (event == 0) {
        return;
    }

    int objectCount = *reinterpret_cast<int*>(object + 0x168);
    unsigned char* objectArray = *reinterpret_cast<unsigned char**>(object + 0x164);
    int eventCount = *reinterpret_cast<int*>(event + 0x40);
    unsigned char** eventArray = *reinterpret_cast<unsigned char***>(event + 0x3C);
    for (int i = 0; i < eventCount; ++i) {
        unsigned char* record = eventArray[i];
        if (record == 0 || *reinterpret_cast<int*>(record + 0x14) == 0) {
            return;
        }

        if (*reinterpret_cast<int*>(record + 0x0C) == 3) {
            int key = *reinterpret_cast<int*>(record + 0x10);
            int index = 0;
            for (; index < objectCount; ++index) {
                if (*reinterpret_cast<int*>(objectArray + index * 0x1C0 + 0x14) == key) {
                    break;
                }
            }
            *reinterpret_cast<int*>(object + 0x1BC) = index < objectCount ? index : 0;
        } else {
            *reinterpret_cast<int*>(object + 0x1BC) = 0;
        }

        int groupCount = *reinterpret_cast<int*>(record + 0x1C);
        unsigned char** groups = *reinterpret_cast<unsigned char***>(record + 0x18);
        for (int j = 0; j < groupCount; ++j) {
            unsigned char* group = groups[j];
            if (*reinterpret_cast<int*>(group + 0x10) == 0) {
                return;
            }
            int actionCount = *reinterpret_cast<int*>(group + 0x18);
            unsigned char** actions = *reinterpret_cast<unsigned char***>(group + 0x14);
            for (int k = 0; k < actionCount; ++k) {
                unsigned char* action = actions[k];
                int type = *reinterpret_cast<int*>(action + 0x0C);
                void* handler;
                if (type == 0x1F) {
                    int count = *reinterpret_cast<int*>(action + 0x30);
                    unsigned char* entries = count > 0 ? *reinterpret_cast<unsigned char**>(action + 0x2C) : 0;
                    handler = *reinterpret_cast<void**>(entries + 4);
                } else {
                    handler = lbl_807C9140[type];
                }
                if (handler == 0) {
                    continue;
                }

                int start = *reinterpret_cast<int*>(action + 0x10);
                int end = *reinterpret_cast<int*>(action + 0x14);
                float window = *reinterpret_cast<float*>(object + 0x19C);
                if (static_cast<float>(start) <= window && static_cast<float>(end) >= window) {
                    void** vtable = *reinterpret_cast<void***>(handler);
                    reinterpret_cast<void (*)(void*, void*, int)>(vtable[3])(handler, action, 4);
                }
            }
        }
    }
}

// NONMATCHING: 35.87% - per-event action dispatch and cut routing are reconstructed; interval codegen differs.
extern "C" void fn_80550CE0(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    int stop = 0;
    for (int eventIndex = 0; !stop; ++eventIndex) {
        unsigned char* event = static_cast<unsigned char*>(fn_80541210(self));
        if (event == 0 || eventIndex >= *reinterpret_cast<int*>(event + 0x40)) {
            break;
        }
        unsigned char** records = *reinterpret_cast<unsigned char***>(event + 0x3C);
        unsigned char* record = records[eventIndex];
        if (record == 0 || *reinterpret_cast<int*>(record + 0x14) == 0) {
            break;
        }

        int recordType = *reinterpret_cast<int*>(record + 0x0C);
        if (recordType == 3) {
            int key = *reinterpret_cast<int*>(record + 0x10);
            int found = 0;
            int objectCount = *reinterpret_cast<int*>(object + 0x168);
            unsigned char* objectArray = *reinterpret_cast<unsigned char**>(object + 0x164);
            for (; found < objectCount; ++found) {
                if (*reinterpret_cast<int*>(objectArray + found * 0x1C0 + 0x14) == key) {
                    break;
                }
            }
            *reinterpret_cast<int*>(object + 0x1BC) = found < objectCount ? found : 0;
        } else {
            *reinterpret_cast<int*>(object + 0x1BC) = 0;
        }

        int groupCount = *reinterpret_cast<int*>(record + 0x1C);
        unsigned char** groups = *reinterpret_cast<unsigned char***>(record + 0x18);
        for (int groupIndex = 0; groupIndex < groupCount; ++groupIndex) {
            unsigned char* group = groups[groupIndex];
            if (group == 0 || *reinterpret_cast<int*>(group + 0x10) == 0) {
                stop = 1;
                break;
            }
            int actionCount = *reinterpret_cast<int*>(group + 0x18);
            unsigned char** actions = *reinterpret_cast<unsigned char***>(group + 0x14);
            for (int actionIndex = 0; actionIndex < actionCount; ++actionIndex) {
                unsigned char* action = actions[actionIndex];
                int type = *reinterpret_cast<int*>(action + 0x0C);
                void* handler;
                if (type == 0x1F) {
                    int entryCount = *reinterpret_cast<int*>(action + 0x30);
                    unsigned char* firstEntry = entryCount > 0 ? *reinterpret_cast<unsigned char**>(action + 0x2C) : 0;
                    handler = *reinterpret_cast<void**>(firstEntry + 4);
                } else {
                    handler = lbl_807C9140[type];
                }
                if (handler == 0) {
                    continue;
                }

                float start = static_cast<float>(*reinterpret_cast<int*>(action + 0x10));
                float end = static_cast<float>(*reinterpret_cast<int*>(action + 0x14));
                float windowStart = *reinterpret_cast<float*>(object + 0x198);
                float windowEnd = *reinterpret_cast<float*>(object + 0x19C);
                if (start <= windowEnd && end >= windowStart) {
                    void** vtable = *reinterpret_cast<void***>(handler);
                    reinterpret_cast<void (*)(void*, void*, int)>(vtable[3])(handler, action, 5);
                }
            }
        }

        int cutId = reinterpret_cast<Event::Cut*>(record)->fn_8053B028();
        if (cutId != -1) {
            int count = *reinterpret_cast<int*>(object + 0xBC);
            unsigned char** entries = *reinterpret_cast<unsigned char***>(object + 0xB8);
            int selected = 0;
            for (; selected < count; ++selected) {
                if (*reinterpret_cast<int*>(entries[selected] + 0x0C) == cutId) {
                    break;
                }
            }
            int target = fn_80541198(self);
            fn_80541520(self, target, 0);
        }

        int mode = *reinterpret_cast<int*>(object + 0x1B0);
        if (mode == 0) {
            stop = 1;
        } else {
            int selected = *reinterpret_cast<int*>(object + 0x1B4);
            if (selected != *reinterpret_cast<int*>(object + 0x1A0)) {
                *reinterpret_cast<int*>(object + 0x1A0) = selected;
                if (mode != 1) {
                    *reinterpret_cast<float*>(object + 0x19C) = lbl_80887E20;
                }
            }
            float frame = static_cast<float>(*reinterpret_cast<int*>(object + 0x1B8));
            *reinterpret_cast<float*>(object + 0x198) = frame;
            if (mode == 1) {
                *reinterpret_cast<float*>(object + 0x19C) = frame;
                int count = *reinterpret_cast<int*>(object + 0x168);
                unsigned char* objects = *reinterpret_cast<unsigned char**>(object + 0x164);
                for (int i = 0; i < count; ++i) {
                    unsigned char* item = objects + i * 0x1C0;
                    *reinterpret_cast<int*>(item + 0xB8) = 0;
                    *reinterpret_cast<int*>(item + 0xBC) = 0;
                    *reinterpret_cast<int*>(item + 0xC0) = 0;
                }
            }
            *reinterpret_cast<int*>(object + 0x1B0) = 0;
        }
    }
    fn_8053E8C8(self, 13);
}

// The short status helpers are kept here in address order while the behaviour dispatchers are
// reconstructed from the original split.
extern "C" int fn_805510F8() {
    return 0;
}

extern "C" int fn_80551100() {
    return 0;
}

