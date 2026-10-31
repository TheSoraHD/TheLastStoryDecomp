#ifndef MP_FUNCMESSAGE_H
#define MP_FUNCMESSAGE_H

// Multiplayer messages (atn::net::FuncMessage and the MP::func_* classes). Every class has six
// virtual functions; member names are invented. vf08, vf0C and vf18 return constants (inline).

extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" void* memset(void* dst, int c, unsigned long n);

namespace atn {
namespace net {

// inferred: abstract base without a destructor in its vtable
class FuncMessage {
public:
    virtual int vf08() = 0;          // inferred: message id
    virtual unsigned int vf0C() = 0; // inferred: size of the payload
    // inferred: vf10, vf14 and vf1C are shared by the vtables of all the MP::func_* classes, so they
    // are inline members of the base (weak copies 0x805006C0, 0x8050062C, 0x80500624)
    virtual void vf10(void* dst, const void* src) {
        if (src != 0) {
            memcpy(dst, src, vf0C());
        }
    }
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode) {
        if (mode == 1 && *pos != 0) {
            memcpy(dst, *pos, vf0C());
        }
        *pos += vf0C();
    }
    virtual unsigned int vf18() = 0; // inferred unsigned: compared with cmplwi (0x8050E094)
    // inferred: compares a queued message with new data (atn::net::PacketPack 0x8050E094)
    virtual unsigned int vf1C(const char* data, const void* arg) { return 1; }
};

} // namespace net
} // namespace atn

namespace MP {

class func_player_update_loc : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x1; }
    virtual unsigned int vf0C() { return 0x2c; }
};

class func_player_status : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x2; }
    virtual unsigned int vf0C() { return 0x15; }
};

class func_player_sync : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x3; }
    virtual unsigned int vf0C() { return 0xa; }
};

class func_player_update_loc_mot1 : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x4; }
    virtual unsigned int vf0C() { return 0x35; }
};

class func_bullet_register : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x5; }
    virtual unsigned int vf0C() { return 0x26; }
};

class func_homing_bullet_register : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x3a; }
    virtual unsigned int vf0C() { return 0x44; }
};

class func_gimmick_trigger : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x6; }
    virtual unsigned int vf0C() { return 0x1f; }
};

class func_hit_force : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x7; }
    virtual unsigned int vf0C() { return 0x26; }
};

class func_explode_fx : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x8; }
    virtual unsigned int vf0C() { return 0x1f; }
};

class func_explode_fx_reliable : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x40; }
    virtual unsigned int vf0C() { return 0x1f; }
};

class func_play_message : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x9; }
    virtual unsigned int vf0C() { return 0x8; }
};

class func_apply_skill : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0xa; }
    virtual unsigned int vf0C() { return 0x8c; }
};

class func_apply_item : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0xb; }
    virtual unsigned int vf0C() { return 0x8c; }
};

class func_rule_stage : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0xc; }
    virtual unsigned int vf0C() { return 0x4; }
};

class func_rule_time_limit : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0xd; }
    virtual unsigned int vf0C() { return 0x4; }
};

class func_rule_score_limit : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0xe; }
    virtual unsigned int vf0C() { return 0x4; }
};

class func_rule_gamestart : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0xf; }
    virtual unsigned int vf0C() { return 0x3c; }
};

class func_player_ready : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x10; }
    virtual unsigned int vf0C() { return 0x0; }
};

class func_player_notready : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x11; }
    virtual unsigned int vf0C() { return 0x0; }
};

class func_player_team : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x12; }
    virtual unsigned int vf0C() { return 0x4; }
};

class func_player_info : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x13; }
    virtual unsigned int vf0C() { return 0x20; }
};

class func_player_info_for_host : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x2c; }
    virtual unsigned int vf0C() { return 0x5b; }
};

class func_player_info_protector : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x14; }
    virtual unsigned int vf0C() { return 0x50; }
};

class func_player_info_weapon : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x15; }
    virtual unsigned int vf0C() { return 0x8; }
};

class func_player_info_status : public atn::net::FuncMessage {
public:
    // inferred: status of a player; the two 0x20-byte blocks are not sent (names invented)
    struct Status_unk {
        int m00[9];
        char m24[0x20];
        char m44[0x10];
        char m54[8];
        char m5C[0x20];
    };
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x16; }
    virtual void vf10(void* dst, const void* src) {
        if (src != 0) {
            const Status_unk* s = (const Status_unk*)src;
            char* d = (char*)dst;
            memcpy(d, s, 4);
            memcpy(d + 0x04, &s->m00[1], 4);
            memcpy(d + 0x08, &s->m00[2], 4);
            memcpy(d + 0x0C, &s->m00[3], 4);
            memcpy(d + 0x10, &s->m00[4], 4);
            memcpy(d + 0x14, &s->m00[5], 4);
            memcpy(d + 0x18, &s->m00[6], 4);
            memcpy(d + 0x1C, &s->m00[7], 4);
            memcpy(d + 0x20, &s->m00[8], 4);
            memcpy(d + 0x24, s->m44, 0x10);
            memcpy(d + 0x34, s->m54, 8);
        }
    }
    virtual unsigned int vf0C() { return 0x3c; }
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode) {
        if (mode == 1 && *pos != 0) {
            Status_unk* s = (Status_unk*)dst;
            memcpy(&s->m00[0], *pos, 4);
            *pos += 4;
            memcpy(&s->m00[1], *pos, 4);
            *pos += 4;
            memcpy(&s->m00[2], *pos, 4);
            *pos += 4;
            memcpy(&s->m00[3], *pos, 4);
            *pos += 4;
            memcpy(&s->m00[4], *pos, 4);
            *pos += 4;
            memcpy(&s->m00[5], *pos, 4);
            *pos += 4;
            memcpy(&s->m00[6], *pos, 4);
            *pos += 4;
            memcpy(&s->m00[7], *pos, 4);
            *pos += 4;
            memcpy(&s->m00[8], *pos, 4);
            *pos += 4;
            memset(s->m24, 0, 0x20);
            memcpy(s->m44, *pos, 0x10);
            *pos += 0x10;
            memcpy(s->m54, *pos, 8);
            *pos += 8;
            memset(s->m5C, 0, 0x20);
        } else {
            *pos += vf0C();
        }
    }
};

class func_player_load_end : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x17; }
    virtual unsigned int vf0C() { return 0x0; }
};

class func_player_play_end : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x18; }
    virtual unsigned int vf0C() { return 0x0; }
};

class func_enemy_status : public atn::net::FuncMessage {
public:
    // inferred: payload (names invented)
    struct Payload_unk {
        unsigned char m00[0x12];
        unsigned char m12_hi : 6;
        unsigned char m12_1 : 1;
        unsigned char m12_0 : 1;
        unsigned char m13;
        unsigned char m14;
    };
    virtual unsigned int vf18() { return 0x1; }
    virtual unsigned int vf1C(const char* data, const void* arg) {
        const Payload_unk* p = (const Payload_unk*)(data + 4);
        const Payload_unk* a = (const Payload_unk*)arg;
        if (p->m12_1 != a->m12_1) {
            return 0;
        }
        return p->m14 == a->m14;
    }
    virtual int vf08() { return 0x19; }
    virtual unsigned int vf0C() { return 0x15; }
};

class func_enemy_status_sp : public atn::net::FuncMessage {
public:
    // inferred: payload (names invented)
    struct Payload_unk {
        unsigned char m00[0x14];
        unsigned char m14;
    };
    virtual unsigned int vf18() { return 0x1; }
    virtual unsigned int vf1C(const char* data, const void* arg) {
        const Payload_unk* p = (const Payload_unk*)(data + 4);
        const Payload_unk* a = (const Payload_unk*)arg;
        return p->m14 == a->m14;
    }
    virtual int vf08() { return 0x1a; }
    virtual unsigned int vf0C() { return 0x25; }
};

class func_enemy_sync : public atn::net::FuncMessage {
public:
    // inferred: payload (names invented)
    struct Payload_unk {
        unsigned char m00[0x08];
        unsigned char m08_7 : 1;
        unsigned char m08_6 : 1;
        unsigned char m08_5 : 1;
        unsigned char m08_4 : 1;
        unsigned char m08_lo : 4;
        unsigned char m09;
    };
    virtual unsigned int vf18() { return 0x1; }
    virtual unsigned int vf1C(const char* data, const void* arg) {
        const Payload_unk* p = (const Payload_unk*)(data + 4);
        const Payload_unk* a = (const Payload_unk*)arg;
        if (p->m08_4 != a->m08_4) {
            return 0;
        }
        return p->m09 == a->m09;
    }
    virtual int vf08() { return 0x1b; }
    virtual unsigned int vf0C() { return 0xa; }
};

class func_enemy_update_loc : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x1c; }
    virtual unsigned int vf0C() { return 0x31; }
};

class func_enemy_update_loc_mot1 : public atn::net::FuncMessage {
public:
    // inferred: payload (names invented)
    struct Payload_unk {
        unsigned char m00[0x2C];
        int m2C;
        unsigned char m30;
    };
    virtual unsigned int vf18() { return 0x1; }
    // inferred: the queued message is replaced when the byte at +0x30 and the word at +0x2C match
    virtual unsigned int vf1C(const char* data, const void* arg) {
        const Payload_unk* p = (const Payload_unk*)(data + 4);
        const Payload_unk* a = (const Payload_unk*)arg;
        if (p->m30 != a->m30) {
            return 0;
        }
        return p->m2C == a->m2C;
    }
    virtual int vf08() { return 0x1d; }
    virtual unsigned int vf0C() { return 0x3a; }
};

class func_damaged_enemy_sp : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x1e; }
    virtual unsigned int vf0C() { return 0xf; }
};

class func_player_weapon_temp : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x1f; }
    virtual unsigned int vf0C() { return 0x5; }
};

class func_resolve_team_balance : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x20; }
    virtual unsigned int vf0C() { return 0x1; }
};

class func_client_update : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x21; }
    virtual unsigned int vf0C() { return 0x34; }
};

class func_game_start : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x22; }
    virtual unsigned int vf0C() { return 0x0; }
};

class func_game_end : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x23; }
    virtual unsigned int vf0C() { return 0x0; }
};

class func_client_timeout : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x24; }
    virtual unsigned int vf0C() { return 0x1; }
};

class func_game_close : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x25; }
    virtual unsigned int vf0C() { return 0x0; }
};

class func_game_open : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x26; }
    virtual unsigned int vf0C() { return 0x0; }
};

class func_rule_player_number : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x27; }
    virtual unsigned int vf0C() { return 0x1; }
};

class func_bot_on_team : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x28; }
    virtual unsigned int vf0C() { return 0x10; }
};

class func_ack : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x29; }
    virtual unsigned int vf0C() { return 0x9; }
};

class func_query_team_request : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x2a; }
    virtual unsigned int vf0C() { return 0x2; }
};

class func_request_character : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x2b; }
    virtual unsigned int vf0C() { return 0x0; }
};

class func_notify_character : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x2d; }
    virtual unsigned int vf0C() { return 0x1; }
};

class func_player_beatable : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x2e; }
    virtual unsigned int vf0C() { return 0x2; }
};

class func_player_life : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x2f; }
    virtual unsigned int vf0C() { return 0x3; }
};

class func_rule_match_type : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x1; }
    virtual int vf08() { return 0x30; }
    virtual unsigned int vf0C() { return 0x1; }
};

class func_player_warp : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x31; }
    virtual unsigned int vf0C() { return 0x1; }
};

class func_chat : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x32; }
    virtual unsigned int vf0C() { return 0x4; }
};

class func_enemy_reinforce : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x33; }
    virtual unsigned int vf0C() { return 0x4; }
};

class func_player_score : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x34; }
    virtual unsigned int vf0C() { return 0x2; }
};

class func_player_result_item_id : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x35; }
    virtual unsigned int vf0C() { return 0x8; }
};

class func_syncallmessage_host : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x36; }
    virtual unsigned int vf0C() { return 0x6d; }
};

class func_syncallmessage_lobby_host : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x3f; }
    virtual unsigned int vf0C() { return 0x6d; }
};

class func_sync_dummy : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x37; }
    virtual unsigned int vf0C() { return 0x1; }
};

class func_sync_complete : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x38; }
    virtual unsigned int vf0C() { return 0x0; }
};

class func_start_result : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x39; }
    virtual unsigned int vf0C() { return 0x14; }
};

class func_request_game_start : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x3b; }
    virtual unsigned int vf0C() { return 0x0; }
};

class func_responce_game_start : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x3c; }
    virtual unsigned int vf0C() { return 0x0; }
};

class func_request_time : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0x0; }
    virtual int vf08() { return 0x3d; }
    virtual unsigned int vf0C() { return 0x1; }
};

class func_enter_setting : public atn::net::FuncMessage {
public:
    virtual unsigned int vf18() { return 0; } // 0x8050072C
    virtual int vf08() { return 0x3E; }        // 0x80500724
    virtual unsigned int vf0C() { return 0; }  // 0x8050071C
};

} // namespace MP

#endif
