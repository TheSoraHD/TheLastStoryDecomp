#include "MP/FuncMessage.h"
#include "atn/net/PacketPack.h"
#include "std/ios_init.h"

namespace MP {

// inferred: multiplayer packet packer (vtable 0x80792A28; the atn::SingletonNoStatic base is
// left out)
class PacketPackMP : public atn::net::PacketPack {
public:
    // inferred: vf10 returns 0x41 (0x80501054) and vf0C returns 0 (0x8050105C); they come after the
    // weak copies of the MP::func_* classes in the original, which is not reproduced yet, so they
    // are only declared
    virtual int vf0C();
    virtual unsigned char vf10();
    virtual unsigned char vf14(int code);
    virtual atn::net::FuncMessage* vf18(unsigned char id);
};

// inferred: message id of a packet code (0x804FF9BC)
unsigned char PacketPackMP::vf14(int code) {
    switch (code) {
    case 0x1016:
        return 0x1;
    case 0x1014:
        return 0x2;
    case 0x1015:
        return 0x3;
    case 0x1017:
        return 0x4;
    case 0x1024:
        return 0x5;
    case 0x1025:
        return 0x3A;
    case 0x1027:
        return 0x6;
    case 0x102E:
        return 0x7;
    case 0x102F:
        return 0x8;
    case 0x1049:
        return 0x40;
    case 0x1030:
        return 0x9;
    case 0x1022:
        return 0xA;
    case 0x1021:
        return 0xB;
    case 0x1001:
        return 0xC;
    case 0x1002:
        return 0xD;
    case 0x1003:
        return 0xE;
    case 0x1004:
        return 0xF;
    case 0x1005:
        return 0x10;
    case 0x1006:
        return 0x11;
    case 0x1007:
        return 0x12;
    case 0x1008:
        return 0x13;
    case 0x1009:
        return 0x14;
    case 0x100A:
        return 0x15;
    case 0x100B:
        return 0x16;
    case 0x1010:
        return 0x17;
    case 0x1013:
        return 0x18;
    case 0x101A:
        return 0x19;
    case 0x101B:
        return 0x1A;
    case 0x101C:
        return 0x1B;
    case 0x101D:
        return 0x1C;
    case 0x101E:
        return 0x1D;
    case 0x1023:
        return 0x1E;
    case 0x1031:
        return 0x1F;
    case 0x1032:
        return 0x20;
    case 0x6:
        return 0x21;
    case 0x8:
        return 0x22;
    case 0x9:
        return 0x23;
    case 0x10:
        return 0x24;
    case 0x13:
        return 0x25;
    case 0x14:
        return 0x26;
    case 0x1033:
        return 0x27;
    case 0x1034:
        return 0x28;
    case 0x1035:
        return 0x29;
    case 0x1036:
        return 0x2A;
    case 0x1037:
        return 0x2B;
    case 0x1038:
        return 0x2C;
    case 0x1039:
        return 0x2D;
    case 0x103A:
        return 0x2E;
    case 0x103B:
        return 0x2F;
    case 0x103C:
        return 0x30;
    case 0x103D:
        return 0x31;
    case 0x103E:
        return 0x32;
    case 0x103F:
        return 0x33;
    case 0x1011:
        return 0x34;
    case 0x1012:
        return 0x35;
    case 0x1040:
        return 0x36;
    case 0x1041:
        return 0x37;
    case 0x1042:
        return 0x38;
    case 0x1043:
        return 0x39;
    case 0x1044:
        return 0x3B;
    case 0x1045:
        return 0x3C;
    case 0x1046:
        return 0x3D;
    case 0x1047:
        return 0x3E;
    case 0x1048:
        return 0x3F;
    }
    fn_8050E62C(0, "invalid messageID\n");
    return 0;
}

// inferred: returns the message object of an id (0x804FFE04). This file is only a part of the
// translation unit (the other MP::func_* classes and PacketPackMP are around it), so it is not
// linked.
atn::net::FuncMessage* PacketPackMP::vf18(unsigned char id) {
    static func_player_update_loc s_func_player_update_loc;
    static func_player_status s_func_player_status;
    static func_player_sync s_func_player_sync;
    static func_player_update_loc_mot1 s_func_player_update_loc_mot1;
    static func_bullet_register s_func_bullet_register;
    static func_gimmick_trigger s_func_gimmick_trigger;
    static func_hit_force s_func_hit_force;
    static func_explode_fx s_func_explode_fx;
    static func_play_message s_func_play_message;
    static func_apply_skill s_func_apply_skill;
    static func_apply_item s_func_apply_item;
    static func_rule_stage s_func_rule_stage;
    static func_rule_time_limit s_func_rule_time_limit;
    static func_rule_score_limit s_func_rule_score_limit;
    static func_rule_gamestart s_func_rule_gamestart;
    static func_player_ready s_func_player_ready;
    static func_player_notready s_func_player_notready;
    static func_player_team s_func_player_team;
    static func_player_info s_func_player_info;
    static func_player_info_protector s_func_player_info_protector;
    static func_player_info_weapon s_func_player_info_weapon;
    static func_player_info_status s_func_player_info_status;
    static func_player_load_end s_func_player_load_end;
    static func_player_play_end s_func_player_play_end;
    static func_enemy_status s_func_enemy_status;
    static func_enemy_status_sp s_func_enemy_status_sp;
    static func_enemy_sync s_func_enemy_sync;
    static func_enemy_update_loc s_func_enemy_update_loc;
    static func_enemy_update_loc_mot1 s_func_enemy_update_loc_mot1;
    static func_damaged_enemy_sp s_func_damaged_enemy_sp;
    static func_player_weapon_temp s_func_player_weapon_temp;
    static func_resolve_team_balance s_func_resolve_team_balance;
    static func_client_update s_func_client_update;
    static func_game_start s_func_game_start;
    static func_game_end s_func_game_end;
    static func_client_timeout s_func_client_timeout;
    static func_game_close s_func_game_close;
    static func_game_open s_func_game_open;
    static func_rule_player_number s_func_rule_player_number;
    static func_bot_on_team s_func_bot_on_team;
    static func_ack s_func_ack;
    static func_query_team_request s_func_query_team_request;
    static func_request_character s_func_request_character;
    static func_player_info_for_host s_func_player_info_for_host;
    static func_notify_character s_func_notify_character;
    static func_player_beatable s_func_player_beatable;
    static func_player_life s_func_player_life;
    static func_rule_match_type s_func_rule_match_type;
    static func_player_warp s_func_player_warp;
    static func_chat s_func_chat;
    static func_enemy_reinforce s_func_enemy_reinforce;
    static func_player_score s_func_player_score;
    static func_player_result_item_id s_func_player_result_item_id;
    static func_syncallmessage_host s_func_syncallmessage_host;
    static func_sync_dummy s_func_sync_dummy;
    static func_sync_complete s_func_sync_complete;
    static func_start_result s_func_start_result;
    static func_homing_bullet_register s_func_homing_bullet_register;
    static func_request_game_start s_func_request_game_start;
    static func_responce_game_start s_func_responce_game_start;
    static func_request_time s_func_request_time;
    static func_enter_setting s_func_enter_setting;
    static func_syncallmessage_lobby_host s_func_syncallmessage_lobby_host;
    static func_explode_fx_reliable s_func_explode_fx_reliable;
    static atn::net::FuncMessage* sTable[] = {
        0,
        &s_func_player_update_loc,
        &s_func_player_status,
        &s_func_player_sync,
        &s_func_player_update_loc_mot1,
        &s_func_bullet_register,
        &s_func_gimmick_trigger,
        &s_func_hit_force,
        &s_func_explode_fx,
        &s_func_play_message,
        &s_func_apply_skill,
        &s_func_apply_item,
        &s_func_rule_stage,
        &s_func_rule_time_limit,
        &s_func_rule_score_limit,
        &s_func_rule_gamestart,
        &s_func_player_ready,
        &s_func_player_notready,
        &s_func_player_team,
        &s_func_player_info,
        &s_func_player_info_protector,
        &s_func_player_info_weapon,
        &s_func_player_info_status,
        &s_func_player_load_end,
        &s_func_player_play_end,
        &s_func_enemy_status,
        &s_func_enemy_status_sp,
        &s_func_enemy_sync,
        &s_func_enemy_update_loc,
        &s_func_enemy_update_loc_mot1,
        &s_func_damaged_enemy_sp,
        &s_func_player_weapon_temp,
        &s_func_resolve_team_balance,
        &s_func_client_update,
        &s_func_game_start,
        &s_func_game_end,
        &s_func_client_timeout,
        &s_func_game_close,
        &s_func_game_open,
        &s_func_rule_player_number,
        &s_func_bot_on_team,
        &s_func_ack,
        &s_func_query_team_request,
        &s_func_request_character,
        &s_func_player_info_for_host,
        &s_func_notify_character,
        &s_func_player_beatable,
        &s_func_player_life,
        &s_func_rule_match_type,
        &s_func_player_warp,
        &s_func_chat,
        &s_func_enemy_reinforce,
        &s_func_player_score,
        &s_func_player_result_item_id,
        &s_func_syncallmessage_host,
        &s_func_sync_dummy,
        &s_func_sync_complete,
        &s_func_start_result,
        &s_func_homing_bullet_register,
        &s_func_request_game_start,
        &s_func_responce_game_start,
        &s_func_request_time,
        &s_func_enter_setting,
        &s_func_syncallmessage_lobby_host,
        &s_func_explode_fx_reliable,
        0,
    };
    if (id >= 0x41) {
        id = 0;
    }
    return sTable[id];
}

} // namespace MP
