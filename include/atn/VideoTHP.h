#ifndef ATN_VIDEOTHP_H
#define ATN_VIDEOTHP_H

namespace atn {

// inferred: THP movie player (atn/VideoTHP.cpp, 0x98 bytes, bases atn::IRender and atn::Video);
// only what Event::MovieTask uses is declared, names invented
class VideoTHP {
public:
    VideoTHP(const char* path); // 0x800DCA70
    virtual ~VideoTHP();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28(); // inferred: draws the movie

    int m04;  // inferred: kind (0, or 2/3 for RTMV movies)
    int m08;  // inferred: nonzero once playing
    unsigned char m0C[0x98 - 0x0C];
};

} // namespace atn

#endif
