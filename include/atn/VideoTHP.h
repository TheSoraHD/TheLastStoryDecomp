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
    virtual void vf2C();
    virtual int vf30();  // inferred: current frame (TutorialTask::vf10)
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();

    int m04;  // inferred: kind (0, or 2/3 for RTMV movies)
    int m08;  // inferred: nonzero once playing
    int m0C;  // inferred: set to 1 by TutorialTask::fn_805BA934 (loop?)
    float m10; // inferred: depth of the movie (TutorialTask::fn_805BA934)
    unsigned char m14[0x1C - 0x14];
    int m1C;  // inferred: nonzero at the end of the movie (TutorialTask::vf10)
    unsigned char m20[0x98 - 0x20];
};

} // namespace atn

#endif
