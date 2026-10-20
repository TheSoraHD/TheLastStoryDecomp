#ifndef MP_FUNCMESSAGE_H
#define MP_FUNCMESSAGE_H

// Multiplayer messages (atn::net::FuncMessage and the MP::func_* classes). Every class has six
// virtual functions; member names are invented. Only MP::func_enter_setting is defined so far.

extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

namespace atn {
namespace net {

// inferred: abstract base without a destructor in its vtable
class FuncMessage {
public:
    virtual int vf08() = 0;          // inferred: message id
    virtual unsigned int vf0C() = 0; // inferred: size of the payload
    virtual void vf10(void* dst, const void* src) = 0;
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode) = 0;
    virtual unsigned int vf18() = 0; // inferred unsigned: compared with cmplwi (0x8050E094)
    // inferred: compares a queued message with new data (atn::net::PacketPack 0x8050E094)
    virtual unsigned int vf1C(const char* data, const void* arg) = 0;
};

} // namespace net
} // namespace atn

namespace MP {

class func_player_update_loc : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_status : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_sync : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_update_loc_mot1 : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_bullet_register : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_gimmick_trigger : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_hit_force : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_explode_fx : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_play_message : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_apply_skill : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_apply_item : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_rule_stage : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_rule_time_limit : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_rule_score_limit : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_rule_gamestart : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_ready : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_notready : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_team : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_info : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_info_protector : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_info_weapon : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_info_status : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_load_end : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_play_end : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_enemy_status : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_enemy_status_sp : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_enemy_sync : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_enemy_update_loc : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_enemy_update_loc_mot1 : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_damaged_enemy_sp : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_weapon_temp : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_resolve_team_balance : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_client_update : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_game_start : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_game_end : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_client_timeout : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_game_close : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_game_open : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_rule_player_number : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_bot_on_team : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_ack : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_query_team_request : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_request_character : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_info_for_host : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_notify_character : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_beatable : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_life : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_rule_match_type : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_warp : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_chat : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_enemy_reinforce : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_score : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_player_result_item_id : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_syncallmessage_host : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_sync_dummy : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_sync_complete : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_start_result : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_homing_bullet_register : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_request_game_start : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_responce_game_start : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_request_time : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_enter_setting : public atn::net::FuncMessage {
public:
    virtual int vf08() { return 0x3E; }        // 0x80500724
    virtual unsigned int vf0C() { return 0; }  // 0x8050071C
    virtual void vf10(void* dst, const void* src) { // 0x805006C0
        if (src != 0) {
            memcpy(dst, src, vf0C());
        }
    }
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode) { // 0x8050062C
        if (mode == 1 && *pos != 0) {
            memcpy(dst, *pos, vf0C());
        }
        *pos += vf0C();
    }
    virtual unsigned int vf18() { return 0; } // 0x8050072C
    virtual unsigned int vf1C(const char* data, const void* arg) { return 1; } // 0x80500624
};

class func_syncallmessage_lobby_host : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

class func_explode_fx_reliable : public atn::net::FuncMessage {
public:
    virtual int vf08();
    virtual unsigned int vf0C();
    virtual void vf10(void* dst, const void* src);
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode);
    virtual unsigned int vf18();
    virtual unsigned int vf1C(const char* data, const void* arg);
};

} // namespace MP

#endif
