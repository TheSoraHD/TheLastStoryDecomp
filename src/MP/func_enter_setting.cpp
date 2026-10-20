#include "MP/FuncMessage.h"
#include "atn/net/PacketPack.h"

namespace MP {

// inferred: multiplayer packet packer (vtable 0x80792A28; the atn::SingletonNoStatic base is
// left out)
class PacketPackMP : public atn::net::PacketPack {
public:
    virtual atn::net::FuncMessage* vf18(unsigned char id);
};

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
