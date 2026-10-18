#ifndef GAME_ARMS_H
#define GAME_ARMS_H

// inferred: weapon object (class name from RTTI, vtable at 0x80777494). Members unknown;
// parameters named by the members they are stored in.
class Arms {
public:
    Arms(void* arg04, int arg08, int arg0C, int arg270); // 0x80043EAC
    virtual ~Arms();                                     // 0x80044134
    virtual int vf0C();                                  // 0x8004424C
    virtual void vf10();                                 // 0x80044780
    virtual void vf14();                                 // 0x80044BB0
    virtual void vf18();                                 // 0x800457A0

    void* m04;
    int m08;
    int m0C;
};

#endif
