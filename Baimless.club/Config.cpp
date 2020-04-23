#include "Config.h"
#include "Options.hpp"


#include <fstream>
#include <filesystem>
#include "json.h"
#include <iomanip>
#include "Definitions.h"
nlohmann::json config;

void ConfigSys::CreateConfigFolder(std::string path)
{
    if (!std::filesystem::create_directory(path)) return;
}

bool ConfigSys::FileExists(std::string file)
{
    return std::filesystem::exists(file);
}
const char* GetWeaponName(int iWeaponID)
{
    switch (iWeaponID)
    {
    default:
        return "none";
    case (int)ItemDefinitionIndex::WEAPON_MP5SD:
        return "mp5-sd";
    case (int)ItemDefinitionIndex::WEAPON_DEAGLE:
        return "deagle";
    case (int)ItemDefinitionIndex::WEAPON_ELITE:
        return "duals";
    case (int)ItemDefinitionIndex::WEAPON_FIVESEVEN:
        return "five7";
    case (int)ItemDefinitionIndex::WEAPON_GLOCK:
        return "glock";
    case (int)ItemDefinitionIndex::WEAPON_AK47:
        return "ak47";
    case (int)ItemDefinitionIndex::WEAPON_AUG:
        return "aug";
    case (int)ItemDefinitionIndex::WEAPON_AWP:
        return "awp";
    case (int)ItemDefinitionIndex::WEAPON_FAMAS:
        return "famas";
    case (int)ItemDefinitionIndex::WEAPON_G3SG1:
        return "g3sg1";
    case (int)ItemDefinitionIndex::WEAPON_GALIL:
        return "galil";
    case (int)ItemDefinitionIndex::WEAPON_M249:
        return "m249";
    case (int)ItemDefinitionIndex::WEAPON_M4A4:
        return "m4a4";
    case (int)ItemDefinitionIndex::WEAPON_MAC10:
        return "mac10";
    case (int)ItemDefinitionIndex::WEAPON_P90:
        return "p90";
    case (int)ItemDefinitionIndex::WEAPON_UMP45:
        return "ump45";
    case (int)ItemDefinitionIndex::WEAPON_XM1014:
        return "xm1014";
    case (int)ItemDefinitionIndex::WEAPON_BIZON:
        return "bizon";
    case (int)ItemDefinitionIndex::WEAPON_MAG7:
        return "mag7";
    case (int)ItemDefinitionIndex::WEAPON_NEGEV:
        return "negev";
    case (int)ItemDefinitionIndex::WEAPON_SAWEDOFF:
        return "sawedoff";
    case (int)ItemDefinitionIndex::WEAPON_TEC9:
        return "tec9";
    case (int)ItemDefinitionIndex::WEAPON_ZEUS:
        return "taser";
    case (int)ItemDefinitionIndex::WEAPON_USPS:
        return "usp-s";
    case (int)ItemDefinitionIndex::WEAPON_MP7:
        return "mp7";
    case (int)ItemDefinitionIndex::WEAPON_MP9:
        return "mp9";
    case (int)ItemDefinitionIndex::WEAPON_NOVA:
        return "nova";
    case (int)ItemDefinitionIndex::WEAPON_P250:
        return "p250";
    case (int)ItemDefinitionIndex::WEAPON_SCAR20:
        return "scar20";
    case (int)ItemDefinitionIndex::WEAPON_SG553:
        return "sg556";
    case (int)ItemDefinitionIndex::WEAPON_SSG08:
        return "ssg08";
    case (int)ItemDefinitionIndex::WEAPON_M4A1S:
        return "m4a1-s";
    case (int)ItemDefinitionIndex::WEAPON_P2000:
        return "p2000";
    case (int)ItemDefinitionIndex::WEAPON_CZ75:
        return "cz75";
    case (int)ItemDefinitionIndex::WEAPON_REVOLVER:
        return "revolver";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_BAYONET:
        return "bayonet";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY:
        return "butterfly";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_FALCHION:
        return "falshion";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_FLIP:
        return "flip";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_GUT:
        return "gut";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT:
        return "karambit";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET:
        return "m9";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL:
        return "huntsman";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE:
        return "bowie";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_PUSH:
        return "daggers";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE:
        return "jackknife";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_STILETTO:
        return "stiletto";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_URSUS:
        return "ursos";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER:
        return "widowmaker";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_SKELETON:
        return "skeleton";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_OUTDOOR:
        return "outdoor";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_CANIS:
        return "canis";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_CORD:
        return "cord";
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_CSS:
        return "css";
    }
    return "none";
}


#define ESPCOL esp::colors
#define ENCOL esp::colors::enemy
#define TECOL esp::colors::team
#define LOCOL esp::colors::team
void ConfigSys::SaveConfig(const std::string path)
{
    std::ofstream output_file(path);

    if (!output_file.good())
        return;
    //config["VERSION"] = "v1";
    SaveArray(legit::colors::legit_aa_visualize_color, "legit_aa_visualize_color");
    SaveArray(legit::colors::legit_aa_visualize_fov_color, "legit_aa_visualize_fov_color");
    SaveArray(legit::colors::zeus_range, "zeus_range");
    SaveArray(legit::colors::aimpoint_col, "aimpoint_col");
    SaveArray(ESPCOL::sound_esp, "sound_esp");
    SaveArray(ESPCOL::zeus_range, "zeus_range");

    SaveArray(ESPCOL::grenade_prediction, "grenade_prediction");
    SaveArray(ESPCOL::out_of_view_arrows, "out_of_view_arrows");
    SaveArray(ESPCOL::manual_aa, "zeus_range");
    SaveArray(ESPCOL::zeus_range, "manual_aa");
    SaveArray(ESPCOL::impacts, "impacts");
    SaveArray(ESPCOL::beam_color, "beam_color");
    SaveArray(ESPCOL::shot_hitboxes, "shot_hitboxes");
    SaveArray(ESPCOL::world_glow, "world_glow");
    SaveArray(ESPCOL::enemy_glow, "enemy_glow");
    SaveArray(ESPCOL::local_glow, "local_glow");
    SaveArray(ESPCOL::team_glow, "team_glow");
    SaveArray(ESPCOL::dropped_weapon_box, "dropped_weapon_box");
    SaveArray(ESPCOL::dropped_weapon_ammo, "dropped_weapon_ammo");
    SaveArray(ESPCOL::dropped_weapon_ammo_bar, "dropped_weapon_ammo_bar");

    SaveArray(ENCOL::box_col, "enemy_box_col");
    SaveArray(ENCOL::name_col, "enemy_name_col");
    SaveArray(ENCOL::weapons, "enemy_weapons");
    SaveArray(ENCOL::ammo, "enemy_ammo");
    SaveArray(ENCOL::helmet, "enemy_helmet");
    SaveArray(ENCOL::money, "enemy_money");
    SaveArray(ENCOL::scoped, "enemy_scoped");
    SaveArray(ENCOL::fakeduck, "enemy_fakeduck");
    SaveArray(ENCOL::skeleton, "enemy_skeleton");

    SaveArray(TECOL::box_col, "team_box_col");
    SaveArray(TECOL::name_col, "team_name_col");
    SaveArray(TECOL::weapons, "team_weapons");
    SaveArray(TECOL::ammo, "team_ammo");
    SaveArray(TECOL::helmet, "team_helmet");
    SaveArray(TECOL::money, "team_money");
    SaveArray(TECOL::scoped, "team_scoped");
    SaveArray(TECOL::fakeduck, "team_fakeduck");
    SaveArray(TECOL::skeleton, "team_skeleton");

    SaveArray(LOCOL::box_col, "local_box_col");
    SaveArray(LOCOL::name_col, "local_name_col");
    SaveArray(LOCOL::weapons, "local_weapons");
    SaveArray(LOCOL::ammo, "local_ammo");
    SaveArray(LOCOL::helmet, "local_helmet");
    SaveArray(LOCOL::money, "local_money");
    SaveArray(LOCOL::scoped, "local_scoped");
    SaveArray(LOCOL::fakeduck, "local_fakeduck");
    SaveArray(LOCOL::skeleton, "local_skeleton");
    SaveArray(esp::colors::nadecolorr, "nadecolorr");

    SaveArray(materials::colors::enemy::backtrack, "material_enemy_backtrack");
    SaveArray(materials::colors::enemy::behind_wall, "material_enemy_behind_wall");
    SaveArray(materials::colors::enemy::player, "material_enemy_player");
    SaveArray(materials::colors::team::backtrack, "material_team_backtrack");
    SaveArray(materials::colors::team::behind_wall, "material_tean_behind_wall");
    SaveArray(materials::colors::team::player, "material_team_player");

    SaveArray(materials::colors::local::chams, "material_local_chams");
    SaveArray(materials::colors::local::desync, "material_local_desync");
    SaveArray(materials::colors::local::fakelag, "material_local_fakelag");
    SaveArray(materials::colors::local::hand_chams, "material_local_hand_chams");
    SaveArray(menu::color, "menu_color");
    SaveArray(esp::colors::nadecolorr, "nadecolorr");

    SaveColor(g_Options.color_esp_ally_visible, "obs_color_esp_ally_visible");
    SaveColor(g_Options.color_esp_enemy_visible, "obs_color_esp_enemy_visible");
    SaveColor(g_Options.color_esp_ally_occluded, "obs_color_esp_ally_occluded");
    SaveColor(g_Options.color_esp_enemy_occluded, "obs_color_esp_enemy_occluded");
    SaveColor(g_Options.color_esp_weapons, "obs_color_esp_weapons");
    SaveColor(g_Options.color_esp_defuse, "obs_color_esp_defuse");
    SaveColor(g_Options.color_esp_c4, "obs_color_esp_c4");
    SaveColor(g_Options.color_esp_crosshair, "obs_color_esp_crosshair");
    SaveColor(g_Options.color_esp_item, "obs_color_esp_item");
    ////////////////////////////////////////////////////////////////
    Save(g_Options.esp_enabled, "esp_enable");
    Save(g_Options.esp_enemies_only, "esp_enemies_only");
    Save(g_Options.esp_player_boxes, "esp_player_boxes");
    Save(g_Options.esp_player_names, "esp_player_names");
    Save(g_Options.esp_player_health, "esp_player_health");
    Save(g_Options.esp_player_armour, "esp_player_armour");
    Save(g_Options.esp_player_weapons, "esp_player_weapons");
    Save(g_Options.esp_player_snaplines, "esp_player_snaplines");
    Save(g_Options.esp_dropped_weapons, "esp_dropped_weapons");
    Save(g_Options.esp_defuse_kit, "esp_defuse_kit");
    Save(g_Options.esp_planted_c4, "esp_planted_c4");
	Save(g_Options.esp.other.bomb_timer, "esp_bomb_timer_");
    /////////////////////////////////////////////////////////////////
    Save(g_Options.skins_enable, "skins_enable");
    Save(g_Options.skins_knife_model, "skins_knife_model");
    Save(g_Options.skins_glove_model, "skins_glove_model");
    Save(g_Options.skins_glove_skin, "skins_glove_skin");
	Load(g_Options.legit.flash_check, "legit_flash_check");
    for (auto val : guns)
    {
        int id = (int)val.first;
        Save(g_Options.W[id].ChangerSkin, std::string(GetWeaponName(id)) + std::string("_skin"));
        Save(g_Options.W[id].statamount, std::string(GetWeaponName(id)) + std::string("_statrak"));
        Save(g_Options.W[id].wear, std::string(GetWeaponName(id)) + std::string("_wear"));
        Save(g_Options.advanced_legit[id].fov, std::string(GetWeaponName(id) + std::string("_fov")).c_str());
        Save(g_Options.advanced_legit[id].speed, std::string(GetWeaponName(id) + std::string("_speed")).c_str());
        Save(g_Options.advanced_legit[id].min_dmg, std::string(GetWeaponName(id) + std::string("_min_dmg")).c_str());
        Save(g_Options.advanced_legit[id].smooth, std::string(GetWeaponName(id) + std::string("_smooth")).c_str());
        Save(g_Options.advanced_legit[id].rcs, std::string(GetWeaponName(id) + std::string("_rcs")).c_str());
        Save(g_Options.advanced_legit[id].smooth, std::string(GetWeaponName(id) + std::string("_smooth")).c_str());
        Save(g_Options.advanced_legit[id].hitscan, std::string(GetWeaponName(id) + std::string("_hitscan")).c_str());
    }
    Save(g_Options.rage.fakelagfix, "rage_fakelagfix");
    Save(g_Options.rage.extrapolation, "rage_extrapolation");
    Save(g_Options.rage.prediction, "rage_prediction");
    Save(g_Options.rage.autoscope, "rage_autoscope");
    Save(g_Options.rage.resolver, "rage_resolver");
    Save(g_Options.rage.backtrack, "rage_backtrack");
    Save(g_Options.rage.shotbacktrack, "rage_shotbacktrack");
    Save(g_Options.rage.autostop, "rage_autostop");
    Save(g_Options.rage.autozeus, "rage_autozeus");
    Save(g_Options.rage.autorevolver, "rage_autorevolver");
    Save(g_Options.rage.removespread, "rage_removespread");
    Save(g_Options.rage.norecoil, "rage_norecoil");
    Save(g_Options.rage.silentaim, "rage_silentaim");
    Save(g_Options.rage.autofire, "rage_autofire");
    Save(g_Options.rage.show_aimpoint, "rage_show_aimpoint");
    Save(g_Options.rage.enable, "rage_enable");
    Save(g_Options.rage.auto_stop_select, "rage_auto_stop_select");
    Save(g_Options.rage.zeus_hitchance, "rage_zeus_hitchance");
    SaveArray(g_Options.rage.hitscan_selected, "rage_hitscan_selected");
    Save(g_Options.rage.min_hitchance, "rage_min_hitchance");
    Save(g_Options.rage.autowall_damage_hitchance, "rage_autowall_damage_hitchance");
    Save(g_Options.rage.avoid_head, "rage_avoid_head");
    SaveArray(g_Options.rage.avoid_head_selected, "rage_avoid_head_selected");
    Save(g_Options.rage.avoid_head_x, "rage_avoid_head_x");
    Save(g_Options.rage.head_scale, "rage_head_scale");
    Save(g_Options.rage.body_scale, "rage_body_scale");
    Save(g_Options.rage.baimkey, "rage_baimkey");
    Save(g_Options.rage.rage_tab, "rage_rage_tab");
    /////////////////////////////////////////////////////////////////////
    Save(g_Options.antiaa.enable, "antiaa_enable");
    Save(g_Options.antiaa.fakelag, "antiaa_fakelag");
    Save(g_Options.antiaa.air_stuck, "antiaa_air_stuck");
    Save(g_Options.antiaa.double_tap, "antiaa_double_tap");
    Save(g_Options.antiaa.slow_walk, "antiaa_slow_walk");
    Save(g_Options.antiaa.static_lby, "antiaa_static_lby");
    Save(g_Options.antiaa.lby_break, "antiaa_lby_break");
    Save(g_Options.antiaa.infinite_duck, "antiaa_infinite_duck");
    Save(g_Options.antiaa.fake_duck, "antiaa_fake_duck");
    Save(g_Options.antiaa.fix_leg_mov, "antiaa_fix_leg_mov");
    Save(g_Options.antiaa.double_tap_on_key, "antiaa_double_tap_on_key");
    Save(g_Options.antiaa.aa_tab_current, "antiaa_aa_tab_current");
    Save(g_Options.antiaa.air_stuck_key, "antiaa_air_stuck_key");
    Save(g_Options.antiaa.double_tap_key, "antiaa_double_tap_key");
    Save(g_Options.antiaa.slow_walk_key, "antiaa_slow_walk_key");
    Save(g_Options.antiaa.invert_key, "antiaa_invert_key");
    Save(g_Options.antiaa.fake_duck_key, "antiaa_fake_duck_key");
    Save(g_Options.antiaa.double_tap_method, "antiaa_double_tap_method");
    Save(g_Options.antiaa.fake_lag_standing, "antiaa_fake_lag_standing");
    Save(g_Options.antiaa.fake_lag_moving, "antiaa_fake_lag_moving");
    Save(g_Options.antiaa.fake_lag_air, "antiaa_fake_lag_air");
    Save(g_Options.antiaa.double_tap_shot, "antiaa_double_tap_shot");
    Save(g_Options.antiaa.double_tap_delay, "antiaa_double_tap_delay");
    Save(g_Options.antiaa.slow_speed, "antiaa_slow_speed");
    Save(g_Options.antiaa.lby_break_add, "antiaa_lby_break_add");
    SaveArray(g_Options.antiaa.disable_aa_selected, "antiaa_disable_aa_selected");
    /**/
    Save(g_Options.antiaa.stand.pitch_selected, "antiaa_stand_pitch_selected");
    Save(g_Options.antiaa.stand.yaw_base_selected, "antiaa_stand_yaw_base_selected");
    Save(g_Options.antiaa.stand.yaw_selected, "antiaa_stand_yaw_selected");
    Save(g_Options.antiaa.stand.body_yaw_selected, "antiaa_stand_body_yaw_selected");
    Save(g_Options.antiaa.stand.edge_type_selected, "antiaa_stand_edge_type_selected");
    Save(g_Options.antiaa.stand.body_yaw_key, "antiaa_stand_body_yaw_key");
    Save(g_Options.antiaa.stand.offset, "antiaa_stand_offset");
    Save(g_Options.antiaa.stand.jitter, "antiaa_stand_jitter");
    Save(g_Options.antiaa.stand.desync, "antiaa_stand_desync");
    Save(g_Options.antiaa.stand.body_lean, "antiaa_stand_body_lean");
    Save(g_Options.antiaa.stand.inverted_body_lean, "antiaa_stand_inverted_body_lean");
    /**/
    Save(g_Options.antiaa.moving.pitch_selected, "antiaa_moving_pitch_selected");
    Save(g_Options.antiaa.moving.yaw_base_selected, "antiaa_moving_yaw_base_selected");
    Save(g_Options.antiaa.moving.yaw_selected, "antiaa_moving_yaw_selected");
    Save(g_Options.antiaa.moving.body_yaw_selected, "antiaa_moving_body_yaw_selected");
    Save(g_Options.antiaa.moving.edge_type_selected, "antiaa_moving_edge_type_selected");
    Save(g_Options.antiaa.moving.body_yaw_key, "antiaa_moving_body_yaw_key");
    Save(g_Options.antiaa.moving.offset, "antiaa_moving_offset");
    Save(g_Options.antiaa.moving.jitter, "antiaa_moving_jitter");
    Save(g_Options.antiaa.moving.desync, "antiaa_moving_desync");
    Save(g_Options.antiaa.moving.body_lean, "antiaa_moving_body_lean");
    Save(g_Options.antiaa.moving.inverted_body_lean, "antiaa_moving_inverted_body_lean");
    /**/
    Save(g_Options.antiaa.air.pitch_selected, "antiaa_air_pitch_selected");
    Save(g_Options.antiaa.air.yaw_base_selected, "antiaa_air_yaw_base_selected");
    Save(g_Options.antiaa.air.yaw_selected, "antiaa_air_yaw_selected");
    Save(g_Options.antiaa.air.body_yaw_selected, "antiaa_air_body_yaw_selected");
    Save(g_Options.antiaa.air.edge_type_selected, "antiaa_air_edge_type_selected");
    Save(g_Options.antiaa.air.body_yaw_key, "antiaa_air_body_yaw_key");
    Save(g_Options.antiaa.air.offset, "antiaa_air_offset");
    Save(g_Options.antiaa.air.jitter, "antiaa_air_jitter");
    Save(g_Options.antiaa.air.desync, "antiaa_air_desync");
    Save(g_Options.antiaa.air.body_lean, "antiaa_air_body_lean");
    Save(g_Options.antiaa.air.inverted_body_lean, "antiaa_air_inverted_body_lean");
    /**/
    Save(g_Options.antiaa.slow.pitch_selected, "antiaa_slow_pitch_selected");
    Save(g_Options.antiaa.slow.yaw_base_selected, "antiaa_slow_yaw_base_selected");
    Save(g_Options.antiaa.slow.yaw_selected, "antiaa_slow_yaw_selected");
    Save(g_Options.antiaa.slow.body_yaw_selected, "antiaa_slow_body_yaw_selected");
    Save(g_Options.antiaa.slow.edge_type_selected, "antiaa_slow_edge_type_selected");
    Save(g_Options.antiaa.slow.body_yaw_key, "antiaa_slow_body_yaw_key");
    Save(g_Options.antiaa.slow.offset, "antiaa_slow_offset");
    Save(g_Options.antiaa.slow.jitter, "antiaa_slow_jitter");
    Save(g_Options.antiaa.slow.desync, "antiaa_slow_desync");
    Save(g_Options.antiaa.slow.body_lean, "antiaa_slow_body_lean");
    Save(g_Options.antiaa.slow.inverted_body_lean, "antiaa_slow_inverted_body_lean");
    /////////////////////////////////////////////////////////////////////
    Save(g_Options.legit.legit_extra, "legit_legit_extra");
    Save(g_Options.legit.legit_main, "legit_legit_main");
    Save(g_Options.legit.key, "legit_key");
    Save(g_Options.legit.material, "legit_material");
    Save(g_Options.legit.guns_int, "legit_guns_int");
    Save(g_Options.legit.legit_aa_key, "legit_legit_aa_key");
    Save(g_Options.legit.enable, "legit_enable");
    Save(g_Options.legit.smoke_check, "legit_smoke_check");
    Save(g_Options.legit.on_key, "legit_on_key");
    Save(g_Options.legit.backtrack, "legit_backtrack");
    Save(g_Options.legit.rcs_standalone, "legit_rcs_standalone");
    Save(g_Options.legit.visualize_fov, "legit_visualize_fov");
    Save(g_Options.legit.legit_aa, "legit_legit_aa");
    Save(g_Options.legit.legit_aa_indicator, "legit_legit_aa_indicator");
    Save(g_Options.legit.legit_aa_visualize, "legit_legit_aa_visualize");
    Save(g_Options.legit.rcs_amount, "legit_rcs_amount");
    /**/
    Save(g_Options.legit.trigger.trigger_min, "legit_trigger_trigger_min");
    Save(g_Options.legit.trigger.trigger_min_chance, "legit_trigger_trigger_min_chance");
    Save(g_Options.legit.trigger.trigger_delay, "legit_trigger_trigger_delay");
    SaveArray(g_Options.legit.trigger.trigger_hitscan_bool, "legit_trigger_trigger_hitscan_bool");
    Save(g_Options.legit.trigger.key, "legit_trigger_key");
    Save(g_Options.legit.trigger.method, "legit_trigger_method");
    Save(g_Options.legit.trigger.enable, "legit_trigger_enable");
    Save(g_Options.legit.trigger.on_key, "legit_trigger_on_key");
    Save(g_Options.legit.trigger.deathmatch, "legit_trigger_deathmatch");
    Save(g_Options.legit.trigger.smoke_check, "legit_trigger_smoke_check");
    /**/
    Save(g_Options.legit.rifles.speed, "legit_rifles_speed");
    Save(g_Options.legit.rifles.fov, "legit_rifles_fov");
    Save(g_Options.legit.rifles.smooth, "legit_rifles_smooth");
    Save(g_Options.legit.rifles.min_dmg, "legit_rifles_min_dmg");
    Save(g_Options.legit.rifles.rcs, "legit_rifles_rcs");
    Save(g_Options.legit.rifles.hitscan, "legit_rifles_hitscan");
    /**/
    Save(g_Options.legit.pistols.speed, "legit_pistols_speed");
    Save(g_Options.legit.pistols.fov, "legit_pistols_fov");
    Save(g_Options.legit.pistols.smooth, "legit_pistols_smooth");
    Save(g_Options.legit.pistols.min_dmg, "legit_pistols_min_dmg");
    Save(g_Options.legit.pistols.rcs, "legit_pistols_rcs");
    Save(g_Options.legit.pistols.hitscan, "legit_pistols_hitscan");
    /**/
    Save(g_Options.legit.snipers.speed, "legit_snipers_speed");
    Save(g_Options.legit.snipers.fov, "legit_snipers_fov");
    Save(g_Options.legit.snipers.smooth, "legit_snipers_smooth");
    Save(g_Options.legit.snipers.min_dmg, "legit_snipers_min_dmg");
    Save(g_Options.legit.snipers.rcs, "legit_snipers_rcs");
    Save(g_Options.legit.snipers.hitscan, "legit_snipers_hitscan");
    /**/
    Save(g_Options.legit.sub.speed, "legit_sub_speed");
    Save(g_Options.legit.sub.fov, "legit_sub_fov");
    Save(g_Options.legit.sub.smooth, "legit_sub_smooth");
    Save(g_Options.legit.sub.min_dmg, "legit_sub_min_dmg");
    Save(g_Options.legit.sub.rcs, "legit_sub_rcs");
    Save(g_Options.legit.sub.hitscan, "legit_sub_hitscan");
    /**/
    Save(g_Options.legit.shotguns.speed, "legit_shotguns_speed");
    Save(g_Options.legit.shotguns.fov, "legit_shotguns_fov");
    Save(g_Options.legit.shotguns.smooth, "legit_shotguns_smooth");
    Save(g_Options.legit.shotguns.min_dmg, "legit_shotguns_min_dmg");
    Save(g_Options.legit.shotguns.rcs, "legit_shotguns_rcs");
    Save(g_Options.legit.shotguns.hitscan, "legit_shotguns_hitscan");
    /**/
    Save(g_Options.legit.heavys.speed, "legit_heavys_speed");
    Save(g_Options.legit.heavys.fov, "legit_heavys_fov");
    Save(g_Options.legit.heavys.smooth, "legit_heavys_smooth");
    Save(g_Options.legit.heavys.min_dmg, "legit_heavys_min_dmg");
    Save(g_Options.legit.heavys.rcs, "legit_heavys_rcs");
    Save(g_Options.legit.heavys.rcs, "legit_heavys_hitscan");
    /**/
    Save(g_Options.legit.legit_type, "legit_legit_type");
    Save(g_Options.legit.silentaim, "legit_silentaim");
    Save(g_Options.legit.air_shot, "legit_air_shot");
    /////////////////////////////////////////////////////////////////////
    Save(g_Options.esp.enable, "esp_enable");
    Save(g_Options.esp.colored_models, "esp_colored_models");
    Save(g_Options.esp.other_esp, "esp_other_esp");
    Save(g_Options.esp.effects_esp, "esp_effects_esp");
    /**/
    Save(g_Options.esp.effects.night_mode, "esp_effects_night_mode");
    Save(g_Options.esp.effects.fullbright, "esp_effects_fullbright");
    Save(g_Options.esp.effects.removescopeoverlay, "esp_effects_removescopeoverlay");
    Save(g_Options.esp.effects.remove_zoom, "esp_effects_remove_zoom");
    Save(g_Options.esp.effects.remove_flash, "esp_effects_remove_flash");
    Save(g_Options.esp.effects.remove_recoil, "esp_effects_remove_recoil");
    Save(g_Options.esp.effects.remove_smoke, "esp_effects_remove_smoke");
    Save(g_Options.esp.effects.disable_post, "esp_effects_disable_post");
    Save(g_Options.esp.effects.watermark, "esp_effects_watermark");
    Save(g_Options.esp.effects.beam, "esp_effects_beam");
    Save(g_Options.esp.effects.shot_hitboxes, "esp_effects_shot_hitboxes");
    Save(g_Options.esp.effects.impacts, "esp_effects_impacts");
    Save(g_Options.esp.effects.manual_aa, "esp_effects_manual_aa");
    Save(g_Options.esp.effects.outofviewarrows, "esp_effects_outofviewarrows");
    Save(g_Options.esp.effects.grenade_prediction, "esp_effects_grenade_prediction");
    Save(g_Options.esp.effects.autowall_crosshair, "esp_effects_autowall_crosshair");
    Save(g_Options.esp.effects.local_glow, "esp_effects_local_glow");
    Save(g_Options.esp.effects.enemy_glow, "esp_effects_enemy_glow");
    Save(g_Options.esp.effects.team_glow, "esp_effects_team_glow");
    Save(g_Options.esp.effects.dropped_weapon_box, "esp_effects_dropped_weapon_box");
    Save(g_Options.esp.effects.dropped_weapon_ammo, "esp_effects_dropped_weapon_ammo");
    Save(g_Options.esp.effects.dropped_weapon_ammo_bar, "esp_effects_dropped_weapon_ammo_bar");
    Save(g_Options.esp.effects.grenade_box, "esp_effects_grenade_box");
    Save(g_Options.esp.effects.chicken_Box, "esp_effects_chicken_Box");
    SaveArray(g_Options.esp.effects.world_selected, "esp_effects_world_selected");
    SaveArray(g_Options.esp.effects.indicator_selected, "esp_effects_indicator_selected");
    SaveArray(g_Options.esp.effects.beam_selected, "esp_effects_beam_selected");
    Save(g_Options.esp.effects.scope_type, "esp_effects_scope_type");
    Save(g_Options.esp.effects.dropped_weapon_name_type, "esp_effects_dropped_weapon_name_type");
    Save(g_Options.esp.effects.grenade_box_type, "esp_effects_grenade_box_type");
    Save(g_Options.esp.effects.beam_life, "esp_effects_beam_life");
    Save(g_Options.esp.effects.beam_size, "esp_effects_beam_size");
    Save(g_Options.esp.effects.watermark_option, "esp_effects_watermark_option");
    /**/
    Save(g_Options.esp.other.hitmarker, "esp_other_hitmarker");
    Save(g_Options.esp.other.sound_esp, "esp_other_sound_esp");
    Save(g_Options.esp.other.feet_circle, "esp_other_feet_circle");
    Save(g_Options.esp.other.zeus_range, "esp_other_zeus_range");
    Save(g_Options.esp.other.indicators, "esp_other_indicators");
    Save(g_Options.esp.other.fake_indicator, "esp_other_fake_indicator");
    Save(g_Options.esp.other.choke_indicator, "esp_other_choke_indicator");
    Save(g_Options.esp.other.kill_counter, "esp_other_kill_counter");
    Save(g_Options.esp.other.spectator_list, "esp_other_spectator_list");
    Save(g_Options.esp.other.wasd, "esp_other_wasd");
    Save(g_Options.esp.other.wasd, "esp_other_bomb_timer");

    Save(g_Options.esp.other.hit_effect, "esp_other_hit_effect");
    Save(g_Options.esp.other.hitmark_sound, "esp_other_hitmark_sound");
    Save(g_Options.esp.other.override_viewangle_fov, "esp_other_override_viewangle_fov");
    Save(g_Options.esp.other.override_viewmodel_fov, "esp_other_");
    Save(g_Options.esp.other.fov_x, "esp_other_fov_x");
    Save(g_Options.esp.other.fov_y, "esp_other_fov_y");
    Save(g_Options.esp.other.fov_z, "esp_other_fov_z");
    Save(g_Options.esp.other.indicators_x, "esp_other_indicators_x");
    Save(g_Options.esp.other.indicators_y, "esp_other_indicators_y");
    Save(g_Options.esp.other.spectator_x, "esp_other_spectator_x");
    Save(g_Options.esp.other.spectator_y, "esp_other_spectator_y");
    Save(g_Options.esp.other.prop_transp, "esp_other_prop_transp");
    Save(g_Options.esp.other.scoped_alpha, "esp_other_scoped_alpha");
    Save(g_Options.esp.other.world_transparency, "esp_other_world_transparency");
    /**/
    Save(g_Options.esp.enemy.player_esp, "esp_enemy_player_esp");
    Save(g_Options.esp.enemy.box_type, "esp_enemy_box_type");
    Save(g_Options.esp.enemy.health_type, "esp_enemy_health_type");
    Save(g_Options.esp.enemy.skeleton_type, "esp_enemy_skeleton_type");
    Save(g_Options.esp.enemy.weapon_type, "esp_enemy_weapon_type");
    Save(g_Options.esp.enemy.enable, "esp_enemy_enable");
    Save(g_Options.esp.enemy.box, "esp_enemy_box");
    Save(g_Options.esp.enemy.name, "esp_enemy_name");
    Save(g_Options.esp.enemy.health, "esp_enemy_health");
    Save(g_Options.esp.enemy.ammo, "esp_enemy_ammo");
    Save(g_Options.esp.enemy.weapon, "esp_enemy_weapon");
    Save(g_Options.esp.enemy.money, "esp_enemy_money");
    Save(g_Options.esp.enemy.scoped, "esp_enemy_scoped");
    Save(g_Options.esp.enemy.helmet, "esp_enemy_helmet");
    Save(g_Options.esp.enemy.fakeduck, "esp_enemy_fakeduck");
    /**/
    Save(g_Options.esp.team.player_esp, "esp_team_player_esp");
    Save(g_Options.esp.team.box_type, "esp_team_box_type");
    Save(g_Options.esp.team.health_type, "esp_team_health_type");
    Save(g_Options.esp.team.skeleton_type, "esp_team_skeleton_type");
    Save(g_Options.esp.team.weapon_type, "esp_team_weapon_type");
    Save(g_Options.esp.team.enable, "esp_team_enable");
    Save(g_Options.esp.team.box, "esp_team_box");
    Save(g_Options.esp.team.name, "esp_team_name");
    Save(g_Options.esp.team.health, "esp_team_health");
    Save(g_Options.esp.team.ammo, "esp_team_ammo");
    Save(g_Options.esp.team.weapon, "esp_team_weapon");
    Save(g_Options.esp.team.money, "esp_team_money");
    Save(g_Options.esp.team.scoped, "esp_team_scoped");
    Save(g_Options.esp.team.helmet, "esp_team_helmet");
    Save(g_Options.esp.team.fakeduck, "esp_team_fakeduck");
    /**/
    Save(g_Options.esp.local.player_esp, "esp_local_player_esp");
    Save(g_Options.esp.local.box_type, "esp_local_box_type");
    Save(g_Options.esp.local.health_type, "esp_local_health_type");
    Save(g_Options.esp.local.skeleton_type, "esp_local_skeleton_type");
    Save(g_Options.esp.local.weapon_type, "esp_local_weapon_type");
    Save(g_Options.esp.local.enable, "esp_local_enable");
    Save(g_Options.esp.local.box, "esp_local_box");
    Save(g_Options.esp.local.name, "esp_local_name");
    Save(g_Options.esp.local.health, "esp_local_health");
    Save(g_Options.esp.local.ammo, "esp_local_ammo");
    Save(g_Options.esp.local.weapon, "esp_local_weapon");
    Save(g_Options.esp.local.money, "esp_local_money");
    Save(g_Options.esp.local.scoped, "esp_local_scoped");
    Save(g_Options.esp.local.helmet, "esp_local_helmet");
    Save(g_Options.esp.local.fakeduck, "esp_local_fakeduck");
    Save(g_Options.esp.other.disable_backdrop, "esp_disable_backdrop");
    SaveArray(g_Options.esp.nade_esp_smoke, "esp_nade_esp_smoke");
    SaveArray(g_Options.esp.nade_esp_molotov, "esp_nade_esp_molotov");
    /////////////////////////////////////////////////////////////////////
    Save(g_Options.cmaterials.other.hand_chams_material, "cmaterials_other_hand_chams_material");
    Save(g_Options.cmaterials.other.hand_chams, "cmaterials_other_hand_chams");
    /**/
    Save(g_Options.cmaterials.enemy.material, "cmaterials_enemy_material");
    Save(g_Options.cmaterials.enemy.player, "cmaterials_enemy_player");
    Save(g_Options.cmaterials.enemy.behind_wall, "cmaterials_enemy_behind_wall");
    Save(g_Options.cmaterials.enemy.backtrack, "cmaterials_enemy_backtrack");
    Save(g_Options.cmaterials.enemy.chams_opactiy, "cmaterials_enemy_chams_opactiy");
    /**/
    Save(g_Options.cmaterials.team.material, "cmaterials_team_material");
    Save(g_Options.cmaterials.team.player, "cmaterials_team_player");
    Save(g_Options.cmaterials.team.behind_wall, "cmaterials_team_behind_wall");
    Save(g_Options.cmaterials.team.backtrack, "cmaterials_team_backtrack");
    Save(g_Options.cmaterials.team.chams_opactiy, "cmaterials_team_chams_opactiy");
    /**/
    Save(g_Options.cmaterials.local.material, "cmaterials_local_material");
    Save(g_Options.cmaterials.local.player, "cmaterials_local_player");
    Save(g_Options.cmaterials.local.behind_wall, "cmaterials_local_behind_wall");
    Save(g_Options.cmaterials.local.backtrack, "cmaterials_local_backtrack");
    Save(g_Options.cmaterials.local.chams_opactiy, "cmaterials_local_chams_opactiy");
    Save(g_Options.cmaterials.local.chams, "cmaterials_local_chams");
    Save(g_Options.cmaterials.local.desync, "cmaterials_local_desync");
    Save(g_Options.cmaterials.local.fakelag, "cmaterials_local_fakelag");
    /////////////////////////////////////////////////////////////////////
    Save(g_Options.clantag, "custom_clan_tag");
    Save(g_Options.misc.bhop, "misc_bhop");
    Save(g_Options.misc.edge_jump, "misc_edge_jump");
    Save(g_Options.misc.duck_in_air, "misc_duck_in_air");
    Save(g_Options.misc.comp_ranks, "misc_comp_ranks");
    Save(g_Options.misc.air_strafe, "misc_air_strafe");
    Save(g_Options.misc.name_stealer, "misc_name_stealer");
    Save(g_Options.misc.knife_bot, "misc_knife_bot");
    Save(g_Options.misc.walk_bot, "misc_walk_bot");
    Save(g_Options.misc.thirdperson, "misc_thirdperson");
    Save(g_Options.misc.chroma_menu, "misc_chroma_menu");
    Save(g_Options.misc.ant_untrusted, "misc_ant_untrusted");
    Save(g_Options.misc.key_menu, "misc_key_menu");
    Save(g_Options.misc.clantag_type, "misc_clantag_type");
    Save(g_Options.misc.air_strafe_option, "misc_air_strafe_type");

    Save(g_Options.misc.config, "misc_config");
    Save(g_Options.misc.menu_key, "misc_menu_key");
    Save(g_Options.misc.thirdpeson_distance, "misc_thirdpeson_distance");
    Save(g_Options.misc.ambient_red, "misc_ambient_red");
    Save(g_Options.misc.ambient_green, "misc_ambient_green");
    Save(g_Options.misc.ambient_blue, "misc_ambient_blue");
    Save(g_Options.misc.autobuy, "misc_autobuy");
    Save(g_Options.misc.thirdperson_key, "misc_thirdperson_key");
    Save(g_Options.misc.edge_jump_key, "misc_edge_jump_key");
    SaveArray(g_Options.misc.autobuy_selected, "misc_autobuy_selected");
    Save(g_Options.misc.playerModelCT, "misc_playerModelCT");
    Save(g_Options.misc.playerModelT, "misc_playerModelT");
    Save(g_Options.misc.fakeprime, "misc_fakeprime");
    Save(g_Options.misc.autoaccpet, "misc_autoaccpet");
    Save(g_Options.misc.radar, "misc_radar");
    Save(g_Options.misc.recoilCrosshair, "misc_recoilCrosshair");
    Save(g_Options.misc.autoaccpet, "misc_autoaccpet");
    Save(g_Options.misc.unlockinventory, "misc_unlockinventory");
	Save(g_Options.misc.eventlogs, "misc_event_logs");
    /////////////////////////////////////////////////////////////////////
    Save(g_Options.changers.rank.enable, "changers_enable");
    Save(g_Options.changers.rank.selected, "changers_selected");
    Save(g_Options.changers.rank.lvl, "changers_lvl");
    Save(g_Options.changers.rank.xp, "changers_xp");
    Save(g_Options.changers.rank.wins, "changers_wins");
    Save(g_Options.changers.rank.friend_, "changers_friend");
    Save(g_Options.changers.rank.teach, "changers_teach");
    Save(g_Options.changers.rank.leader, "changers_leader");
    Save(g_Options.ban.time, "ban_time");
    Save(g_Options.ban.type, "ban_type");
    Save(g_Options.changers.playerModel, "local_player_model");
    output_file << std::setw(4) << config << std::endl;
    output_file.close();
}


void ConfigSys::LoadConfig(const std::string path)
{
    std::ifstream input_file(path);

    if (!input_file.good())
        return;

    try
    {
        config << input_file;
    }
    catch (...)
    {
        input_file.close();
        return;
    }
   //if (config["VERSION"] != "v1")
        //return;
    LoadArray(legit::colors::legit_aa_visualize_color, "legit_aa_visualize_color");
    LoadArray(legit::colors::legit_aa_visualize_fov_color, "legit_aa_visualize_fov_color");
    LoadArray(legit::colors::zeus_range, "zeus_range");
    LoadArray(legit::colors::aimpoint_col, "aimpoint_col");
    LoadArray(ESPCOL::sound_esp, "sound_esp");
    LoadArray(ESPCOL::zeus_range, "zeus_range");

    LoadArray(ESPCOL::grenade_prediction, "grenade_prediction");
    LoadArray(ESPCOL::out_of_view_arrows, "out_of_view_arrows");
    LoadArray(ESPCOL::manual_aa, "zeus_range");
    LoadArray(ESPCOL::zeus_range, "manual_aa");
    LoadArray(ESPCOL::impacts, "impacts");
    LoadArray(ESPCOL::beam_color, "beam_color");
    LoadArray(ESPCOL::shot_hitboxes, "shot_hitboxes");
    LoadArray(ESPCOL::world_glow, "world_glow");
    LoadArray(ESPCOL::enemy_glow, "enemy_glow");
    LoadArray(ESPCOL::local_glow, "local_glow");
    LoadArray(ESPCOL::team_glow, "team_glow");
    LoadArray(ESPCOL::dropped_weapon_box, "dropped_weapon_box");
    LoadArray(ESPCOL::dropped_weapon_ammo, "dropped_weapon_ammo");
    LoadArray(ESPCOL::dropped_weapon_ammo_bar, "dropped_weapon_ammo_bar");

    LoadArray(ENCOL::box_col, "enemy_box_col");
    LoadArray(ENCOL::name_col, "enemy_name_col");
    LoadArray(ENCOL::weapons, "enemy_weapons");
    LoadArray(ENCOL::ammo, "enemy_ammo");
    LoadArray(ENCOL::helmet, "enemy_helmet");
    LoadArray(ENCOL::money, "enemy_money");
    LoadArray(ENCOL::scoped, "enemy_scoped");
    LoadArray(ENCOL::fakeduck, "enemy_fakeduck");
    LoadArray(ENCOL::skeleton, "enemy_skeleton");

    LoadArray(TECOL::box_col, "team_box_col");
    LoadArray(TECOL::name_col, "team_name_col");
    LoadArray(TECOL::weapons, "team_weapons");
    LoadArray(TECOL::ammo, "team_ammo");
    LoadArray(TECOL::helmet, "team_helmet");
    LoadArray(TECOL::money, "team_money");
    LoadArray(TECOL::scoped, "team_scoped");
    LoadArray(TECOL::fakeduck, "team_fakeduck");
    LoadArray(TECOL::skeleton, "team_skeleton");

    LoadArray(LOCOL::box_col, "local_box_col");
    LoadArray(LOCOL::name_col, "local_name_col");
    LoadArray(LOCOL::weapons, "local_weapons");
    LoadArray(LOCOL::ammo, "local_ammo");
    LoadArray(LOCOL::helmet, "local_helmet");
    LoadArray(LOCOL::money, "local_money");
    LoadArray(LOCOL::scoped, "local_scoped");
    LoadArray(LOCOL::fakeduck, "local_fakeduck");
    LoadArray(LOCOL::skeleton, "local_skeleton");
    LoadArray(esp::colors::nadecolorr , "nadecolorr");

    LoadArray(materials::colors::enemy::backtrack, "material_enemy_backtrack");
    LoadArray(materials::colors::enemy::behind_wall, "material_enemy_behind_wall");
    LoadArray(materials::colors::enemy::player, "material_enemy_player");
    LoadArray(materials::colors::team::backtrack, "material_team_backtrack");
    LoadArray(materials::colors::team::behind_wall, "material_tean_behind_wall");
    LoadArray(materials::colors::team::player, "material_team_player");

    LoadArray(materials::colors::local::chams, "material_local_chams");
    LoadArray(materials::colors::local::desync, "material_local_desync");
    LoadArray(materials::colors::local::fakelag, "material_local_fakelag");
    LoadArray(materials::colors::local::hand_chams, "material_local_hand_chams");
    LoadArray(menu::color, "menu_color");


    LoadColor(g_Options.color_esp_ally_visible, "obs_color_esp_ally_visible");
    LoadColor(g_Options.color_esp_enemy_visible, "obs_color_esp_enemy_visible");
    LoadColor(g_Options.color_esp_ally_occluded, "obs_color_esp_ally_occluded");
    LoadColor(g_Options.color_esp_enemy_occluded, "obs_color_esp_enemy_occluded");
    LoadColor(g_Options.color_esp_weapons, "obs_color_esp_weapons");
    LoadColor(g_Options.color_esp_defuse, "obs_color_esp_defuse");
    LoadColor(g_Options.color_esp_c4, "obs_color_esp_c4");
    LoadColor(g_Options.color_esp_crosshair, "obs_color_esp_crosshair");
    LoadColor(g_Options.color_esp_item, "obs_color_esp_item");

    ////////////////////////////////////////////////////////////////
    Load(g_Options.esp_enabled, "esp_enable");
    Load(g_Options.esp_enemies_only, "esp_enemies_only");
    Load(g_Options.esp_player_boxes, "esp_player_boxes");
    Load(g_Options.esp_player_names, "esp_player_names");
    Load(g_Options.esp_player_health, "esp_player_health");
    Load(g_Options.esp_player_armour, "esp_player_armour");
    Load(g_Options.esp_player_weapons, "esp_player_weapons");
    Load(g_Options.esp_player_snaplines, "esp_player_snaplines");
    Load(g_Options.esp_dropped_weapons, "esp_dropped_weapons");
    Load(g_Options.esp_defuse_kit, "esp_defuse_kit");
    Load(g_Options.esp_planted_c4, "esp_planted_c4");
	Load(g_Options.esp.other.bomb_timer, "esp_bomb_timer_");
    /////////////////////////////////////////////////////////////////
    Load(g_Options.skins_enable, "skins_enable");
    Load(g_Options.skins_knife_model, "skins_knife_model");
    Load(g_Options.skins_glove_model, "skins_glove_model");
    Load(g_Options.skins_glove_skin, "skins_glove_skin");
	Load(g_Options.legit.flash_check, "legit_flash_check");
	for (auto val : guns)
	{
		int id = (int)val.first;
        Load(g_Options.W[id].ChangerSkin, std::string(GetWeaponName(id)) + std::string("_skin"));
        Load(g_Options.W[id].statamount, std::string(GetWeaponName(id)) + std::string("_statrak"));
        Load(g_Options.W[id].wear, std::string(GetWeaponName(id)) + std::string("_wear"));
		Load(g_Options.advanced_legit[id].fov, std::string(GetWeaponName(id) + std::string("_fov")).c_str());
		Load(g_Options.advanced_legit[id].speed, std::string(GetWeaponName(id) + std::string("_speed")).c_str());
		Load(g_Options.advanced_legit[id].min_dmg, std::string(GetWeaponName(id) + std::string("_min_dmg")).c_str());
		Load(g_Options.advanced_legit[id].smooth, std::string(GetWeaponName(id) + std::string("_smooth")).c_str());
		Load(g_Options.advanced_legit[id].rcs, std::string(GetWeaponName(id) + std::string("_rcs")).c_str());
		Load(g_Options.advanced_legit[id].smooth, std::string(GetWeaponName(id) + std::string("_smooth")).c_str());
		Load(g_Options.advanced_legit[id].hitscan, std::string(GetWeaponName(id) + std::string("_hitscan")).c_str());		
	}
    Load(g_Options.rage.fakelagfix, "rage_fakelagfix");
    Load(g_Options.rage.extrapolation, "rage_extrapolation");
    Load(g_Options.rage.prediction, "rage_prediction");
    Load(g_Options.rage.autoscope, "rage_autoscope");
    Load(g_Options.rage.resolver, "rage_resolver");
    Load(g_Options.rage.backtrack, "rage_backtrack");
    Load(g_Options.rage.shotbacktrack, "rage_shotbacktrack");
    Load(g_Options.rage.autostop, "rage_autostop");
    Load(g_Options.rage.autozeus, "rage_autozeus");
    Load(g_Options.rage.autorevolver, "rage_autorevolver");
    Load(g_Options.rage.removespread, "rage_removespread");
    Load(g_Options.rage.norecoil, "rage_norecoil");
    Load(g_Options.rage.silentaim, "rage_silentaim");
    Load(g_Options.rage.autofire, "rage_autofire");
    Load(g_Options.rage.show_aimpoint, "rage_show_aimpoint");
    Load(g_Options.rage.enable, "rage_enable");
    Load(g_Options.rage.auto_stop_select, "rage_auto_stop_select");
    Load(g_Options.rage.zeus_hitchance, "rage_zeus_hitchance");
    LoadArray(g_Options.rage.hitscan_selected, "rage_hitscan_selected");
    Load(g_Options.rage.min_hitchance, "rage_min_hitchance");
    Load(g_Options.rage.autowall_damage_hitchance, "rage_autowall_damage_hitchance");
    Load(g_Options.rage.avoid_head, "rage_avoid_head");
    LoadArray(g_Options.rage.avoid_head_selected, "rage_avoid_head_selected");
    Load(g_Options.rage.avoid_head_x, "rage_avoid_head_x");
    Load(g_Options.rage.head_scale, "rage_head_scale");
    Load(g_Options.rage.body_scale, "rage_body_scale");
    Load(g_Options.rage.baimkey, "rage_baimkey");
    Load(g_Options.rage.rage_tab, "rage_rage_tab");
    /////////////////////////////////////////////////////////////////////
    Load(g_Options.antiaa.enable, "antiaa_enable");
    Load(g_Options.antiaa.fakelag, "antiaa_fakelag");
    Load(g_Options.antiaa.air_stuck, "antiaa_air_stuck");
    Load(g_Options.antiaa.double_tap, "antiaa_double_tap");
    Load(g_Options.antiaa.slow_walk, "antiaa_slow_walk");
    Load(g_Options.antiaa.static_lby, "antiaa_static_lby");
    Load(g_Options.antiaa.lby_break, "antiaa_lby_break");
    Load(g_Options.antiaa.infinite_duck, "antiaa_infinite_duck");
    Load(g_Options.antiaa.fake_duck, "antiaa_fake_duck");
    Load(g_Options.antiaa.fix_leg_mov, "antiaa_fix_leg_mov");
    Load(g_Options.antiaa.double_tap_on_key, "antiaa_double_tap_on_key");
    Load(g_Options.antiaa.aa_tab_current, "antiaa_aa_tab_current");
    Load(g_Options.antiaa.air_stuck_key, "antiaa_air_stuck_key");
    Load(g_Options.antiaa.double_tap_key, "antiaa_double_tap_key");
    Load(g_Options.antiaa.slow_walk_key, "antiaa_slow_walk_key");
    Load(g_Options.antiaa.invert_key, "antiaa_invert_key");
    Load(g_Options.antiaa.fake_duck_key, "antiaa_fake_duck_key");
    Load(g_Options.antiaa.double_tap_method, "antiaa_double_tap_method");
    Load(g_Options.antiaa.fake_lag_standing, "antiaa_fake_lag_standing");
    Load(g_Options.antiaa.fake_lag_moving, "antiaa_fake_lag_moving");
    Load(g_Options.antiaa.fake_lag_air, "antiaa_fake_lag_air");
    Load(g_Options.antiaa.double_tap_shot, "antiaa_double_tap_shot");
    Load(g_Options.antiaa.double_tap_delay, "antiaa_double_tap_delay");
    Load(g_Options.antiaa.slow_speed, "antiaa_slow_speed");
    Load(g_Options.antiaa.lby_break_add, "antiaa_lby_break_add");
    LoadArray(g_Options.antiaa.disable_aa_selected, "antiaa_disable_aa_selected");
    /**/
    Load(g_Options.antiaa.stand.pitch_selected, "antiaa_stand_pitch_selected");
    Load(g_Options.antiaa.stand.yaw_base_selected, "antiaa_stand_yaw_base_selected");
    Load(g_Options.antiaa.stand.yaw_selected, "antiaa_stand_yaw_selected");
    Load(g_Options.antiaa.stand.body_yaw_selected, "antiaa_stand_body_yaw_selected");
    Load(g_Options.antiaa.stand.edge_type_selected, "antiaa_stand_edge_type_selected");
    Load(g_Options.antiaa.stand.body_yaw_key, "antiaa_stand_body_yaw_key");
    Load(g_Options.antiaa.stand.offset, "antiaa_stand_offset");
    Load(g_Options.antiaa.stand.jitter, "antiaa_stand_jitter");
    Load(g_Options.antiaa.stand.desync, "antiaa_stand_desync");
    Load(g_Options.antiaa.stand.body_lean, "antiaa_stand_body_lean");
    Load(g_Options.antiaa.stand.inverted_body_lean, "antiaa_stand_inverted_body_lean");
    /**/
    Load(g_Options.antiaa.moving.pitch_selected, "antiaa_moving_pitch_selected");
    Load(g_Options.antiaa.moving.yaw_base_selected, "antiaa_moving_yaw_base_selected");
    Load(g_Options.antiaa.moving.yaw_selected, "antiaa_moving_yaw_selected");
    Load(g_Options.antiaa.moving.body_yaw_selected, "antiaa_moving_body_yaw_selected");
    Load(g_Options.antiaa.moving.edge_type_selected, "antiaa_moving_edge_type_selected");
    Load(g_Options.antiaa.moving.body_yaw_key, "antiaa_moving_body_yaw_key");
    Load(g_Options.antiaa.moving.offset, "antiaa_moving_offset");
    Load(g_Options.antiaa.moving.jitter, "antiaa_moving_jitter");
    Load(g_Options.antiaa.moving.desync, "antiaa_moving_desync");
    Load(g_Options.antiaa.moving.body_lean, "antiaa_moving_body_lean");
    Load(g_Options.antiaa.moving.inverted_body_lean, "antiaa_moving_inverted_body_lean");
    /**/
    Load(g_Options.antiaa.air.pitch_selected, "antiaa_air_pitch_selected");
    Load(g_Options.antiaa.air.yaw_base_selected, "antiaa_air_yaw_base_selected");
    Load(g_Options.antiaa.air.yaw_selected, "antiaa_air_yaw_selected");
    Load(g_Options.antiaa.air.body_yaw_selected, "antiaa_air_body_yaw_selected");
    Load(g_Options.antiaa.air.edge_type_selected, "antiaa_air_edge_type_selected");
    Load(g_Options.antiaa.air.body_yaw_key, "antiaa_air_body_yaw_key");
    Load(g_Options.antiaa.air.offset, "antiaa_air_offset");
    Load(g_Options.antiaa.air.jitter, "antiaa_air_jitter");
    Load(g_Options.antiaa.air.desync, "antiaa_air_desync");
    Load(g_Options.antiaa.air.body_lean, "antiaa_air_body_lean");
    Load(g_Options.antiaa.air.inverted_body_lean, "antiaa_air_inverted_body_lean");
    /**/
    Load(g_Options.antiaa.slow.pitch_selected, "antiaa_slow_pitch_selected");
    Load(g_Options.antiaa.slow.yaw_base_selected, "antiaa_slow_yaw_base_selected");
    Load(g_Options.antiaa.slow.yaw_selected, "antiaa_slow_yaw_selected");
    Load(g_Options.antiaa.slow.body_yaw_selected, "antiaa_slow_body_yaw_selected");
    Load(g_Options.antiaa.slow.edge_type_selected, "antiaa_slow_edge_type_selected");
    Load(g_Options.antiaa.slow.body_yaw_key, "antiaa_slow_body_yaw_key");
    Load(g_Options.antiaa.slow.offset, "antiaa_slow_offset");
    Load(g_Options.antiaa.slow.jitter, "antiaa_slow_jitter");
    Load(g_Options.antiaa.slow.desync, "antiaa_slow_desync");
    Load(g_Options.antiaa.slow.body_lean, "antiaa_slow_body_lean");
    Load(g_Options.antiaa.slow.inverted_body_lean, "antiaa_slow_inverted_body_lean");
    /////////////////////////////////////////////////////////////////////
    Load(g_Options.legit.legit_extra, "legit_legit_extra");
    Load(g_Options.legit.legit_main, "legit_legit_main");
    Load(g_Options.legit.key, "legit_key");
    Load(g_Options.legit.material, "legit_material");
    Load(g_Options.legit.guns_int, "legit_guns_int");
    Load(g_Options.legit.legit_aa_key, "legit_legit_aa_key");
    Load(g_Options.legit.enable, "legit_enable");
    Load(g_Options.legit.smoke_check, "legit_smoke_check");
    Load(g_Options.legit.on_key, "legit_on_key");
    Load(g_Options.legit.backtrack, "legit_backtrack");
    Load(g_Options.legit.rcs_standalone, "legit_rcs_standalone");
    Load(g_Options.legit.visualize_fov, "legit_visualize_fov");
    Load(g_Options.legit.legit_aa, "legit_legit_aa");
    Load(g_Options.legit.legit_aa_indicator, "legit_legit_aa_indicator");
    Load(g_Options.legit.legit_aa_visualize, "legit_legit_aa_visualize");
    Load(g_Options.legit.rcs_amount, "legit_rcs_amount");
    /**/
    Load(g_Options.legit.trigger.trigger_min, "legit_trigger_trigger_min");
    Load(g_Options.legit.trigger.trigger_min_chance, "legit_trigger_trigger_min_chance");
    Load(g_Options.legit.trigger.trigger_delay, "legit_trigger_trigger_delay");
    LoadArray(g_Options.legit.trigger.trigger_hitscan_bool, "legit_trigger_trigger_hitscan_bool");
    Load(g_Options.legit.trigger.key, "legit_trigger_key");
    Load(g_Options.legit.trigger.method, "legit_trigger_method");
    Load(g_Options.legit.trigger.enable, "legit_trigger_enable");
    Load(g_Options.legit.trigger.on_key, "legit_trigger_on_key");
    Load(g_Options.legit.trigger.deathmatch, "legit_trigger_deathmatch");
    Load(g_Options.legit.trigger.smoke_check, "legit_trigger_smoke_check");
    /**/
    Load(g_Options.legit.rifles.speed, "legit_rifles_speed");
    Load(g_Options.legit.rifles.fov, "legit_rifles_fov");
    Load(g_Options.legit.rifles.smooth, "legit_rifles_smooth");
    Load(g_Options.legit.rifles.min_dmg, "legit_rifles_min_dmg");
    Load(g_Options.legit.rifles.rcs, "legit_rifles_rcs");
    Load(g_Options.legit.rifles.hitscan, "legit_rifles_hitscan");
    /**/
    Load(g_Options.legit.pistols.speed, "legit_pistols_speed");
    Load(g_Options.legit.pistols.fov, "legit_pistols_fov");
    Load(g_Options.legit.pistols.smooth, "legit_pistols_smooth");
    Load(g_Options.legit.pistols.min_dmg, "legit_pistols_min_dmg");
    Load(g_Options.legit.pistols.rcs, "legit_pistols_rcs");
    Load(g_Options.legit.pistols.hitscan, "legit_pistols_hitscan");
    /**/
    Load(g_Options.legit.snipers.speed, "legit_snipers_speed");
    Load(g_Options.legit.snipers.fov, "legit_snipers_fov");
    Load(g_Options.legit.snipers.smooth, "legit_snipers_smooth");
    Load(g_Options.legit.snipers.min_dmg, "legit_snipers_min_dmg");
    Load(g_Options.legit.snipers.rcs, "legit_snipers_rcs");
    Load(g_Options.legit.snipers.hitscan, "legit_snipers_hitscan");
    /**/
    Load(g_Options.legit.sub.speed, "legit_sub_speed");
    Load(g_Options.legit.sub.fov, "legit_sub_fov");
    Load(g_Options.legit.sub.smooth, "legit_sub_smooth");
    Load(g_Options.legit.sub.min_dmg, "legit_sub_min_dmg");
    Load(g_Options.legit.sub.rcs, "legit_sub_rcs");
    Load(g_Options.legit.sub.hitscan, "legit_sub_hitscan");
    /**/
    Load(g_Options.legit.shotguns.speed, "legit_shotguns_speed");
    Load(g_Options.legit.shotguns.fov, "legit_shotguns_fov");
    Load(g_Options.legit.shotguns.smooth, "legit_shotguns_smooth");
    Load(g_Options.legit.shotguns.min_dmg, "legit_shotguns_min_dmg");
    Load(g_Options.legit.shotguns.rcs, "legit_shotguns_rcs");
    Load(g_Options.legit.shotguns.hitscan, "legit_shotguns_hitscan");
    /**/
    Load(g_Options.legit.heavys.speed, "legit_heavys_speed");
    Load(g_Options.legit.heavys.fov, "legit_heavys_fov");
    Load(g_Options.legit.heavys.smooth, "legit_heavys_smooth");
    Load(g_Options.legit.heavys.min_dmg, "legit_heavys_min_dmg");
    Load(g_Options.legit.heavys.rcs, "legit_heavys_rcs");
    Load(g_Options.legit.heavys.rcs, "legit_heavys_hitscan");
    /**/
    Load(g_Options.legit.legit_type, "legit_legit_type");
    Load(g_Options.legit.silentaim, "legit_silentaim");
    Load(g_Options.legit.air_shot, "legit_air_shot");
    /////////////////////////////////////////////////////////////////////
    Load(g_Options.esp.enable, "esp_enable");
    Load(g_Options.esp.colored_models, "esp_colored_models");
    Load(g_Options.esp.other_esp, "esp_other_esp");
    Load(g_Options.esp.effects_esp, "esp_effects_esp");
    /**/
    Load(g_Options.esp.effects.night_mode, "esp_effects_night_mode");
    Load(g_Options.esp.effects.fullbright, "esp_effects_fullbright");
    Load(g_Options.esp.effects.removescopeoverlay, "esp_effects_removescopeoverlay");
    Load(g_Options.esp.effects.remove_zoom, "esp_effects_remove_zoom");
    Load(g_Options.esp.effects.remove_flash, "esp_effects_remove_flash");
    Load(g_Options.esp.effects.remove_recoil, "esp_effects_remove_recoil");
    Load(g_Options.esp.effects.remove_smoke, "esp_effects_remove_smoke");
    Load(g_Options.esp.effects.disable_post, "esp_effects_disable_post");
    Load(g_Options.esp.effects.watermark, "esp_effects_watermark");
    Load(g_Options.esp.effects.beam, "esp_effects_beam");
    Load(g_Options.esp.effects.shot_hitboxes, "esp_effects_shot_hitboxes");
    Load(g_Options.esp.effects.impacts, "esp_effects_impacts");
    Load(g_Options.esp.effects.manual_aa, "esp_effects_manual_aa");
    Load(g_Options.esp.effects.outofviewarrows, "esp_effects_outofviewarrows");
    Load(g_Options.esp.effects.grenade_prediction, "esp_effects_grenade_prediction");
    Load(g_Options.esp.effects.autowall_crosshair, "esp_effects_autowall_crosshair");
    Load(g_Options.esp.effects.local_glow, "esp_effects_local_glow");
    Load(g_Options.esp.effects.enemy_glow, "esp_effects_enemy_glow");
    Load(g_Options.esp.effects.team_glow, "esp_effects_team_glow");
    Load(g_Options.esp.effects.dropped_weapon_box, "esp_effects_dropped_weapon_box");
    Load(g_Options.esp.effects.dropped_weapon_ammo, "esp_effects_dropped_weapon_ammo");
    Load(g_Options.esp.effects.dropped_weapon_ammo_bar, "esp_effects_dropped_weapon_ammo_bar");
    Load(g_Options.esp.effects.grenade_box, "esp_effects_grenade_box");
    Load(g_Options.esp.effects.chicken_Box, "esp_effects_chicken_Box");
    LoadArray(g_Options.esp.effects.world_selected, "esp_effects_world_selected");
    LoadArray(g_Options.esp.effects.indicator_selected, "esp_effects_indicator_selected");
    LoadArray(g_Options.esp.effects.beam_selected, "esp_effects_beam_selected");
    Load(g_Options.esp.effects.scope_type, "esp_effects_scope_type");
    Load(g_Options.esp.effects.dropped_weapon_name_type, "esp_effects_dropped_weapon_name_type");
    Load(g_Options.esp.effects.grenade_box_type, "esp_effects_grenade_box_type");
    Load(g_Options.esp.effects.beam_life, "esp_effects_beam_life");
    Load(g_Options.esp.effects.beam_size, "esp_effects_beam_size");
    Load(g_Options.esp.effects.watermark_option, "esp_effects_watermark_option");
    /**/
    Load(g_Options.esp.other.hitmarker, "esp_other_hitmarker");
    Load(g_Options.esp.other.sound_esp, "esp_other_sound_esp");
    Load(g_Options.esp.other.feet_circle, "esp_other_feet_circle");
    Load(g_Options.esp.other.zeus_range, "esp_other_zeus_range");
    Load(g_Options.esp.other.indicators, "esp_other_indicators");
    Load(g_Options.esp.other.fake_indicator, "esp_other_fake_indicator");
    Load(g_Options.esp.other.choke_indicator, "esp_other_choke_indicator");
    Load(g_Options.esp.other.kill_counter, "esp_other_kill_counter");
    Load(g_Options.esp.other.spectator_list, "esp_other_spectator_list");
    Load(g_Options.esp.other.wasd, "esp_other_wasd");
    Load(g_Options.esp.other.wasd, "esp_other_bomb_timer");

    Load(g_Options.esp.other.hit_effect, "esp_other_hit_effect");
    Load(g_Options.esp.other.hitmark_sound, "esp_other_hitmark_sound");
    Load(g_Options.esp.other.override_viewangle_fov, "esp_other_override_viewangle_fov");
    Load(g_Options.esp.other.override_viewmodel_fov, "esp_other_");
    Load(g_Options.esp.other.fov_x, "esp_other_fov_x");
    Load(g_Options.esp.other.fov_y, "esp_other_fov_y");
    Load(g_Options.esp.other.fov_z, "esp_other_fov_z");
    Load(g_Options.esp.other.indicators_x, "esp_other_indicators_x");
    Load(g_Options.esp.other.indicators_y, "esp_other_indicators_y");
    Load(g_Options.esp.other.spectator_x, "esp_other_spectator_x");
    Load(g_Options.esp.other.spectator_y, "esp_other_spectator_y");
    Load(g_Options.esp.other.prop_transp, "esp_other_prop_transp");
    Load(g_Options.esp.other.scoped_alpha, "esp_other_scoped_alpha");
    Load(g_Options.esp.other.world_transparency, "esp_other_world_transparency");
    /**/
    Load(g_Options.esp.enemy.player_esp, "esp_enemy_player_esp");
    Load(g_Options.esp.enemy.box_type, "esp_enemy_box_type");
    Load(g_Options.esp.enemy.health_type, "esp_enemy_health_type");
    Load(g_Options.esp.enemy.skeleton_type, "esp_enemy_skeleton_type");
    Load(g_Options.esp.enemy.weapon_type, "esp_enemy_weapon_type");
    Load(g_Options.esp.enemy.enable, "esp_enemy_enable");
    Load(g_Options.esp.enemy.box, "esp_enemy_box");
    Load(g_Options.esp.enemy.name, "esp_enemy_name");
    Load(g_Options.esp.enemy.health, "esp_enemy_health");
    Load(g_Options.esp.enemy.ammo, "esp_enemy_ammo");
    Load(g_Options.esp.enemy.weapon, "esp_enemy_weapon");
    Load(g_Options.esp.enemy.money, "esp_enemy_money");
    Load(g_Options.esp.enemy.scoped, "esp_enemy_scoped");
    Load(g_Options.esp.enemy.helmet, "esp_enemy_helmet");
    Load(g_Options.esp.enemy.fakeduck, "esp_enemy_fakeduck");
    /**/
    Load(g_Options.esp.team.player_esp, "esp_team_player_esp");
    Load(g_Options.esp.team.box_type, "esp_team_box_type");
    Load(g_Options.esp.team.health_type, "esp_team_health_type");
    Load(g_Options.esp.team.skeleton_type, "esp_team_skeleton_type");
    Load(g_Options.esp.team.weapon_type, "esp_team_weapon_type");
    Load(g_Options.esp.team.enable, "esp_team_enable");
    Load(g_Options.esp.team.box, "esp_team_box");
    Load(g_Options.esp.team.name, "esp_team_name");
    Load(g_Options.esp.team.health, "esp_team_health");
    Load(g_Options.esp.team.ammo, "esp_team_ammo");
    Load(g_Options.esp.team.weapon, "esp_team_weapon");
    Load(g_Options.esp.team.money, "esp_team_money");
    Load(g_Options.esp.team.scoped, "esp_team_scoped");
    Load(g_Options.esp.team.helmet, "esp_team_helmet");
    Load(g_Options.esp.team.fakeduck, "esp_team_fakeduck");
    /**/
    Load(g_Options.esp.local.player_esp, "esp_local_player_esp");
    Load(g_Options.esp.local.box_type, "esp_local_box_type");
    Load(g_Options.esp.local.health_type, "esp_local_health_type");
    Load(g_Options.esp.local.skeleton_type, "esp_local_skeleton_type");
    Load(g_Options.esp.local.weapon_type, "esp_local_weapon_type");
    Load(g_Options.esp.local.enable, "esp_local_enable");
    Load(g_Options.esp.local.box, "esp_local_box");
    Load(g_Options.esp.local.name, "esp_local_name");
    Load(g_Options.esp.local.health, "esp_local_health");
    Load(g_Options.esp.local.ammo, "esp_local_ammo");
    Load(g_Options.esp.local.weapon, "esp_local_weapon");
    Load(g_Options.esp.local.money, "esp_local_money");
    Load(g_Options.esp.local.scoped, "esp_local_scoped");
    Load(g_Options.esp.local.helmet, "esp_local_helmet");
    Load(g_Options.esp.local.fakeduck, "esp_local_fakeduck");
    Load(g_Options.esp.other.disable_backdrop, "esp_disable_backdrop");
    LoadArray(g_Options.esp.nade_esp_smoke, "esp_nade_esp_smoke");
    LoadArray(g_Options.esp.nade_esp_molotov, "esp_nade_esp_molotov");
    /////////////////////////////////////////////////////////////////////
    Load(g_Options.cmaterials.other.hand_chams_material, "cmaterials_other_hand_chams_material");
    Load(g_Options.cmaterials.other.hand_chams, "cmaterials_other_hand_chams");
    /**/
    Load(g_Options.cmaterials.enemy.material, "cmaterials_enemy_material");
    Load(g_Options.cmaterials.enemy.player, "cmaterials_enemy_player");
    Load(g_Options.cmaterials.enemy.behind_wall, "cmaterials_enemy_behind_wall");
    Load(g_Options.cmaterials.enemy.backtrack, "cmaterials_enemy_backtrack");
    Load(g_Options.cmaterials.enemy.chams_opactiy, "cmaterials_enemy_chams_opactiy");
    /**/
    Load(g_Options.cmaterials.team.material, "cmaterials_team_material");
    Load(g_Options.cmaterials.team.player, "cmaterials_team_player");
    Load(g_Options.cmaterials.team.behind_wall, "cmaterials_team_behind_wall");
    Load(g_Options.cmaterials.team.backtrack, "cmaterials_team_backtrack");
    Load(g_Options.cmaterials.team.chams_opactiy, "cmaterials_team_chams_opactiy");
    /**/
    Load(g_Options.cmaterials.local.material, "cmaterials_local_material");
    Load(g_Options.cmaterials.local.player, "cmaterials_local_player");
    Load(g_Options.cmaterials.local.behind_wall, "cmaterials_local_behind_wall");
    Load(g_Options.cmaterials.local.backtrack, "cmaterials_local_backtrack");
    Load(g_Options.cmaterials.local.chams_opactiy, "cmaterials_local_chams_opactiy");
    Load(g_Options.cmaterials.local.chams, "cmaterials_local_chams");
    Load(g_Options.cmaterials.local.desync, "cmaterials_local_desync");
    Load(g_Options.cmaterials.local.fakelag, "cmaterials_local_fakelag");
    /////////////////////////////////////////////////////////////////////
    Load(g_Options.clantag, "custom_clan_tag");
    Load(g_Options.misc.bhop, "misc_bhop");
    Load(g_Options.misc.edge_jump, "misc_edge_jump");
    Load(g_Options.misc.duck_in_air, "misc_duck_in_air");
    Load(g_Options.misc.comp_ranks, "misc_comp_ranks");
    Load(g_Options.misc.air_strafe, "misc_air_strafe");
    Load(g_Options.misc.name_stealer, "misc_name_stealer");
    Load(g_Options.misc.knife_bot, "misc_knife_bot");
    Load(g_Options.misc.walk_bot, "misc_walk_bot");
    Load(g_Options.misc.thirdperson, "misc_thirdperson");
    Load(g_Options.misc.chroma_menu, "misc_chroma_menu");
    Load(g_Options.misc.ant_untrusted, "misc_ant_untrusted");
    Load(g_Options.misc.key_menu, "misc_key_menu");
    Load(g_Options.misc.clantag_type, "misc_clantag_type");
    Load(g_Options.misc.air_strafe_option, "misc_air_strafe_type");
    Load(g_Options.misc.config, "misc_config");
    Load(g_Options.misc.menu_key, "misc_menu_key");
    Load(g_Options.misc.thirdpeson_distance, "misc_thirdpeson_distance");
    Load(g_Options.misc.ambient_red, "misc_ambient_red");
    Load(g_Options.misc.ambient_green, "misc_ambient_green");
    Load(g_Options.misc.ambient_blue, "misc_ambient_blue");
    Load(g_Options.misc.autobuy, "misc_autobuy");
    Load(g_Options.misc.thirdperson_key, "misc_thirdperson_key");
    Load(g_Options.misc.edge_jump_key, "misc_edge_jump_key");
    LoadArray(g_Options.misc.autobuy_selected, "misc_autobuy_selected");
    Load(g_Options.misc.playerModelCT, "misc_playerModelCT");
    Load(g_Options.misc.playerModelT, "misc_playerModelT");
    Load(g_Options.misc.fakeprime, "misc_fakeprime");
    Load(g_Options.misc.autoaccpet, "misc_autoaccpet");
    Load(g_Options.misc.radar, "misc_radar");
    Load(g_Options.misc.recoilCrosshair, "misc_recoilCrosshair");
    Load(g_Options.misc.autoaccpet, "misc_autoaccpet");
    Load(g_Options.misc.unlockinventory, "misc_unlockinventory");
	Load(g_Options.misc.eventlogs, "misc_event_logs");
    /////////////////////////////////////////////////////////////////////
    Load(g_Options.changers.rank.enable, "changers_enable");
    Load(g_Options.changers.rank.selected, "changers_selected");
    Load(g_Options.changers.rank.lvl, "changers_lvl");
    Load(g_Options.changers.rank.xp, "changers_xp");
    Load(g_Options.changers.rank.wins, "changers_wins");
    Load(g_Options.changers.rank.friend_, "changers_friend");
    Load(g_Options.changers.rank.teach, "changers_teach");
    Load(g_Options.changers.rank.leader, "changers_leader");
    Load(g_Options.changers.playerModel, "local_player_model");
    input_file.close();
}


std::vector<std::string> ConfigSys::GetAllConfigs()
{

    namespace fs = std::experimental::filesystem;

    std::string fPath = std::string(my_documents_folder) + "\\baimless.xyz\\";

    std::vector<ConfigFile> config_files = GetAllConfigsInFolder(fPath, ".xyz");
    std::vector<std::string> config_file_names;

    for (auto config = config_files.begin(); config != config_files.end(); config++)
        config_file_names.emplace_back(config->GetName());

    std::sort(config_file_names.begin(), config_file_names.end());

    return config_file_names;
}

std::vector<ConfigFile> ConfigSys::GetAllConfigsInFolder(const std::string path, const std::string ext)
{
    namespace fs = std::filesystem;

    std::vector<ConfigFile> config_files;

    if (fs::exists(path) && fs::is_directory(path))
    {
        for (auto it = fs::recursive_directory_iterator(path); it != fs::recursive_directory_iterator(); it++)
        {
            if (fs::is_regular_file(*it) && it->path().extension() == ext)
            {
                std::string fPath = path + it->path().filename().string();

                std::string tmp_f_name = it->path().filename().string();
                size_t pos = tmp_f_name.find(".");
                std::string fName = (std::string::npos == pos) ? tmp_f_name : tmp_f_name.substr(0, pos);

                ConfigFile new_config(fName, fPath);
                config_files.emplace_back(new_config);
            }
        }
    }
    return config_files;
}

template<typename T>
void ConfigSys::Load(T& value, std::string str)
{
    if (config[str].empty())
    {
        //value = nullptr;
        return;
    }
    value = config[str].get<T>();
}

void ConfigSys::LoadArray(float_t value[4], std::string str)
{
    if (config[str]["0"].empty() || config[str]["1"].empty() || config[str]["2"].empty() || config[str]["3"].empty())
        return;

    value[0] = config[str]["0"].get<float_t>();
    value[1] = config[str]["1"].get<float_t>();
    value[2] = config[str]["2"].get<float_t>();
    value[3] = config[str]["3"].get<float_t>();
}

void ConfigSys::LoadArray(bool value[14], std::string str)
{
    if (config[str]["0"].empty() || config[str]["1"].empty() || config[str]["2"].empty() || config[str]["3"].empty()
        || config[str]["4"].empty() || config[str]["5"].empty() || config[str]["6"].empty() || config[str]["7"].empty()
        || config[str]["8"].empty() || config[str]["9"].empty() || config[str]["10"].empty() || config[str]["11"].empty()
        || config[str]["12"].empty() || config[str]["13"].empty())
        return;

    value[0] = config[str]["0"].get<bool>();
    value[1] = config[str]["1"].get<bool>();
    value[2] = config[str]["2"].get<bool>();
    value[3] = config[str]["3"].get<bool>();
    value[4] = config[str]["4"].get<bool>();
    value[5] = config[str]["5"].get<bool>();
    value[6] = config[str]["6"].get<bool>();
    value[7] = config[str]["7"].get<bool>();
    value[8] = config[str]["8"].get<bool>();
    value[9] = config[str]["9"].get<bool>();
    value[10] = config[str]["10"].get<bool>();
    value[11] = config[str]["11"].get<bool>();
    value[12] = config[str]["12"].get<bool>();
    value[13] = config[str]["13"].get<bool>();
}


template<typename T>
void ConfigSys::Save(T& value, std::string str)
{
    config[str] = value;
}

void ConfigSys::SaveArray(float_t value[4], std::string str)
{
    config[str]["0"] = value[0];
    config[str]["1"] = value[1];
    config[str]["2"] = value[2];
    config[str]["3"] = value[3];
}

void ConfigSys::SaveArray(bool value[14], std::string str)
{
    config[str]["0"] = value[0];
    config[str]["1"] = value[1];
    config[str]["2"] = value[2];
    config[str]["3"] = value[3];
    config[str]["4"] = value[4];
    config[str]["5"] = value[5];
    config[str]["6"] = value[6];
    config[str]["7"] = value[7];
    config[str]["8"] = value[8];
    config[str]["9"] = value[9];
    config[str]["10"] = value[10];
    config[str]["11"] = value[11];
    config[str]["12"] = value[12];
    config[str]["13"] = value[13];
}

void ConfigSys::SaveColor(Color col, std::string str)
{
    config[str]["0"] = col[0];
    config[str]["1"] = col[1];
    config[str]["2"] = col[2];
    config[str]["3"] = col[3];
}

void ConfigSys::LoadColor(Color* col, std::string str)
{
    if (config[str]["0"].empty() || config[str]["1"].empty(), config[str]["2"].empty(), config[str]["3"].empty())
        return;

    int red = config[str]["0"].get<int>();
    int green = config[str]["1"].get<int>();
    int blue = config[str]["2"].get<int>();
    int alpha = config[str]["3"].get<int>();

    col->SetColor(red, green, blue, alpha);
}
