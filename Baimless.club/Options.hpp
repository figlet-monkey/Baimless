#pragma once
#include <string>
#include <memory>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include "Utils/Color.h"
#include <Windows.h>
#include "SDK/Definitions.h"
#include "imgui/imgui.h"
#define A( s ) #s
#define OPTION(type, var, val) Var<type> var = {A(var), val}
template <typename T = bool>
class Var {
public:
    std::string name;
    std::shared_ptr<T> value;
    int32_t size;
    Var(std::string name, T v) : name(name) {
        value = std::make_shared<T>(v);
        size = sizeof(T);
    }
    operator T() { return *value; }
    operator T* () { return &*value; }
    operator T() const { return *value; }
    //operator T*() const { return value; }
};

enum MenuKnife
{
    Off,
    Bayonet,
    M9,
    Karam,
    Gut,
    Flip,
    Butter,
    Shadow,
    Flachion,
    Hunts,
    Bowei,
    Navja,
    Stiletto,
    Ursus,
    Talon
};

namespace legit
{
    namespace colors {
        extern float legit_aa_visualize_color[3];
        extern float legit_aa_visualize_fov_color[3];
        extern float zeus_range[3];
        extern float aimpoint_col[3];
    }
}

namespace esp
{
    namespace colors {
        extern  float nadecolorr[3];
		extern float grenadehelper[3];
        extern  float sound_esp[3];
        extern  float zeus_range[3];
        extern  float bomb_timer_col[3];

        extern  float grenade_prediction[3];
        extern  float out_of_view_arrows[3];
        extern  float manual_aa[3];
        extern  float impacts[3];
        extern  float beam_color[3];
        extern  float shot_hitboxes[3];
        extern  float world_glow[3];
        extern  float enemy_glow[3];
        extern  float local_glow[3];
        extern  float team_glow[3];
        extern float dropped_weapon_box[3];
        extern float dropped_weapon_ammo[3];
        extern float dropped_weapon_ammo_bar[3];
        namespace enemy {
            extern float box_col[3];
            extern float name_col[3];
            extern float weapons[3];
            extern float ammo[3];
            extern float helmet[3];
            extern float money[3];
            extern float scoped[3];
            extern float fakeduck[3];
            extern float skeleton[3];
        }
        namespace team {
            extern float box_col[3];
            extern float name_col[3];
            extern float weapons[3];
            extern float ammo[3];
            extern float helmet[3];
            extern float money[3];
            extern float scoped[3];
            extern float fakeduck[3];
            extern float skeleton[3];
        }
        namespace local {
            extern float box_col[3];
            extern float name_col[3];
            extern float weapons[3];
            extern float ammo[3];
            extern float helmet[3];
            extern float money[3];
            extern float scoped[3];
            extern float fakeduck[3];
            extern float skeleton[3];
        }
    }
}

namespace materials {
    namespace colors {
        namespace enemy {
            extern float player[3];
            extern float behind_wall[3];
            extern float backtrack[3];
        }
        namespace team {
            extern float player[3];
            extern float behind_wall[3];
            extern float backtrack[3];
        }
        namespace local {
            extern float chams[3];
            extern float desync[3];
            extern float fakelag[3];
            extern float hand_chams[3];
        }
    }
}
namespace menu {
    extern float color[3];
    extern int frequency;
}

class Options2
{
public:
    bool GLOVE;
    bool remember = false;
    float rainbow;
    OPTION(Color, color_esp_ally_visible, Color(0, 128, 255, 190));
    OPTION(Color, color_esp_enemy_visible, Color(111, 227, 40, 190));
    OPTION(Color, color_esp_ally_occluded, Color(0, 128, 255, 190));
    OPTION(Color, color_esp_enemy_occluded, Color(180, 7, 233, 190));
    OPTION(Color, color_esp_weapons, Color(250, 250, 250, 190));
    OPTION(Color, color_esp_defuse, Color(0, 128, 255, 190));
    OPTION(Color, color_esp_c4, Color(255, 255, 0, 190));
    OPTION(Color, color_esp_crosshair, Color(255, 255, 0, 190));
    OPTION(Color, color_esp_item, Color(255, 255, 0, 190));
    // 
    // ESP
    //
    bool esp_streamproof = false;
    bool esp_enabled = false;
    bool esp_enemies_only= false;
    bool esp_player_boxes= false;
    bool esp_player_names= false;
    bool esp_player_health= false;
    bool esp_player_armour= false;
    bool esp_player_weapons= false;
    bool esp_player_snaplines= false;
    bool esp_crosshair= false;
    bool esp_dropped_weapons= false;
    bool esp_defuse_kit= false;
    bool esp_planted_c4= false;
    bool esp_items= false;

    //
    // New Skin changer
    //
    bool skins_enable = false;
    int skins_knife_model = 0;
    int skins_glove_model= 0;
    int skins_glove_skin= 0;
    bool stattrak = false;
    int stacktrak = 0;
    int skin_changer_tab = 0;
    std::string weaponname = "";
    int currentWeapon;
    struct
    {
        bool ChangerEnabled = false;
        int ChangerSkin = 0;
        char ChangerName[32] = "";
        bool stat;
        int statamount = 0;
        bool customstat;
        int customstatamount;
        char Name;
        float wear = 0.0001f;
    } W[519];
    //
    // New menu
    //
    ImColor bar_col = ImColor(0,0,255,255);
    //
    // Ragebot
    //
    struct Ragebot
    {
       
        bool fakelagfix,
            extrapolation,
            prediction,
            autoscope,
            resolver,
            backtrack,
            shotbacktrack,
            autostop,
            autozeus,
            autorevolver,
            removespread,
            norecoil,
            silentaim,
            autofire,
            show_aimpoint,
            enable = false;
        int auto_stop_select = 0;
        float zeus_hitchance = 0.f;
        std::string hitscan_str = "";
        bool hitscan_selected[6];
        float min_hitchance = 0.f;
        float autowall_damage_hitchance = 0.f;
        float avoid_head = 0.f;
        std::string avoid_head_str = "";
        bool avoid_head_selected[7];
        float avoid_head_x = 0.f;
        float head_scale = 0.f;
        float body_scale = 0.f;
        int baimkey = 0;
        int rage_tab = 0;
    }rage;
   
    //
    // Anti aim
    //
    struct Antiaim
    {
        bool enable,
            fakelag,
            air_stuck,
            double_tap,
            slow_walk,
            static_lby,
            lby_break,
            infinite_duck,
            fake_duck,
            fix_leg_mov,
            double_tap_on_key = false;

        int aa_tab_current,
            air_stuck_key,
            double_tap_key,
            slow_walk_key,
            invert_key,
            fake_duck_key,
            double_tap_method = 0;

        float fake_lag_standing,
            fake_lag_moving,
            fake_lag_air,
            double_tap_shot,
            double_tap_delay,
            slow_speed,
            lby_break_add = 0.f;
        std::string disable_aa = "";
        bool disable_aa_selected[2];
        struct Stand {
            
            int pitch_selected,
                yaw_base_selected,
                yaw_selected,
                body_yaw_selected,
                edge_type_selected,
                body_yaw_key = 0;
            float offset,
                jitter,
                desync,
                body_lean,
                inverted_body_lean = 0.f;
        }stand;

        struct Moving {

            int pitch_selected,
                yaw_base_selected,
                yaw_selected,
                body_yaw_selected,
                edge_type_selected,
                body_yaw_key = 0;
            float offset,
                jitter,
                desync,
                body_lean,
                inverted_body_lean = 0.f;
        }moving;

        struct Air {

            int pitch_selected,
                yaw_base_selected,
                yaw_selected,
                body_yaw_selected,
                edge_type_selected,
                body_yaw_key = 0;
            float offset,
                jitter,
                desync,
                body_lean,
                inverted_body_lean = 0.f;
        }air;

        struct Slow {

            int pitch_selected,
                yaw_base_selected,
                yaw_selected,
                body_yaw_selected,
                edge_type_selected,
                body_yaw_key = 0;
            float offset,
                jitter,
                desync,
                body_lean,
                inverted_body_lean = 0.f;
        }slow;
    }antiaa;
    //
    // Legit
    //
    int current_legit_wep;
    struct
    {
        float speed,
            fov,
            smooth,
            min_dmg = 0.f;
        int hitscan = 0;
        int rcs = 70;

    } advanced_legit[519];
    struct Legitbot{
        float LegitAA_Offset = 90.f;
        bool recoil_crosshair = false;
        bool air_shot = false;
        bool flash_check = false;
        bool jump_check = false;
        bool silentaim = false;
        int legit_extra,
            legit_main,
            key,
            material,
            guns_int,
            legit_aa_key= 0;
        int legit_type = 0;
        bool enable,
            smoke_check,
            on_key,
            backtrack,
            rcs_standalone,
            visualize_fov ,
            legit_aa ,
            legit_aa_indicator ,
            legit_aa_visualize = false;

        float rcs_amount = 0.f;
        struct Trigger {
            float trigger_min,
                trigger_min_chance ,
                trigger_delay = 0.f;
            bool trigger_hitscan_bool[5];
            std::string hitscan = "";
            int key,
                method = 0;
            bool enable,
                on_key,
                deathmatch ,
                smoke_check = false;
            bool internal = false;
        }trigger;

        struct Rifles {
            float speed,
                fov ,
                smooth ,
                min_dmg  = 0.f;
            int rcs = 70;
            int hitscan = 0;
        }rifles;
        struct Pistols {
            float speed,
                fov,
                smooth,
                min_dmg = 0.f;
            int rcs = 70;
            int hitscan = 0;
        }pistols;
        struct Snipers {
            float speed,
                fov,
                smooth,
                min_dmg = 0.f;
            int rcs = 70;
            int hitscan = 0;
        }snipers;
        struct Sub {
            float speed,
                fov,
                smooth,
                min_dmg = 0.f;
            int rcs = 70;
            int hitscan = 0;
        }sub;
        struct Shotguns {
            float speed,
                fov,
                smooth,
                min_dmg = 0.f;
            int rcs = 70;
            int hitscan = 0;
        }shotguns;
        struct Heavys {
            float speed,
                fov,
                smooth,
                min_dmg = 0.f;
            int rcs = 70;
            int hitscan = 0;
        }heavys;
        struct Hitscan {
           /* bool hitscan[4] = {false,false,false,false};
            ImVec4 head_color = ImVec4(0, 0, 0, 1);
            ImVec4 body_color = ImVec4(0, 0, 0, 1);
            ImVec4 pelvis_color = ImVec4(0, 0, 0, 1);
            ImVec4 legs_color = ImVec4(0, 0, 0, 1);
            ImVec4 foots_color = ImVec4(0, 0, 0, 1);*/
    }hitscan;
    }legit;
    //
    // Visuals
    //
   
    ///
   
    //
   
    //
   
    struct Esp {
        bool nade_esp_smoke[3];
        std::string smoke = "";
        bool nade_esp_molotov[3];
        std::string molotov = "";
        bool helper = false;
        bool enable = false;
        int player_esp, colored_models,other_esp, effects_esp = 0;
        struct Effects {
            bool night_mode,
                fullbright ,
                removescopeoverlay,
                remove_zoom ,
                remove_flash ,
                remove_recoil ,
                remove_smoke ,
                disable_post,
                beam,
                shot_hitboxes ,
                impacts ,
                manual_aa ,
                outofviewarrows ,
                grenade_prediction ,
                autowall_crosshair,
                local_glow ,
                enemy_glow ,
                team_glow ,
                dropped_weapon_box ,
                dropped_weapon_ammo ,
                dropped_weapon_ammo_bar,
                grenade_box ,
                chicken_Box = false;
            bool watermark = true;
            std::string indicators = "";
            std::string beams = "";
            std::string world_glow = "";
            bool world_selected[3];
            bool indicator_selected[3];
            bool beam_selected[3];
            int scope_type, dropped_weapon_name_type, grenade_box_type = 0;
            float beam_life,
                beam_size = 0.f;
            int watermark_option = 0;

        }effects;
        struct Other {
            bool hitmarker, sound_esp,
                feet_circle,
                zeus_range,
                indicators ,
                fake_indicator ,
                choke_indicator ,
                kill_counter,
                spectator_list,
                wasd,
                bomb_timer,

                hit_effect= false;
            bool disable_backdrop = false;
            int hitmark_sound = 0;
            float override_viewangle_fov = 50.f;
            float override_viewmodel_fov,              
                fov_x,
                fov_y,
                fov_z,
                indicators_x ,
                indicators_y,
                spectator_x ,
                spectator_y ,
                scoped_alpha = 0.f;
            int world_transparency = 100;
            float prop_transp = 100.f;
        }other;
        struct Enemy {
            int player_esp,
                box_type,
                health_type,
                skeleton_type,
                weapon_type = 0;
            bool enable,
                box,
                name,
                health,
                ammo,
                weapon,
                money,
                scoped,
                helmet,
                fakeduck = false;
        }enemy;

        struct Team {
            int player_esp,
                box_type,
                health_type,
                skeleton_type,
                weapon_type = 0;
            bool enable,
                box,
                name,
                health,
                ammo,
                weapon,
                money,
                scoped,
                helmet,
                fakeduck = false;
        }team;
        struct Local {
            int player_esp,
                box_type,
                health_type,
                skeleton_type,
                weapon_type = 0;
            bool enable,
                box,
                name,
                health,
                ammo,
                weapon,
                money,
                scoped,
                helmet,
                fakeduck = false;
        }local;    
       

    }esp;
   
    struct Colored_materials {
        struct Other {
            int hand_chams_material = 0;
            bool hand_chams = false;
        }other;
        struct Enemy {
            int material = 0;
            bool player,
                behind_wall,
                backtrack = false;
            float chams_opactiy = 1.f;
        }enemy;
        struct Team {
            int material = 0;
            bool player,
                behind_wall,
                backtrack = false;
            float chams_opactiy = 1.f;
        }team;
        struct Local {
            int material = 0;
            bool player,
                behind_wall,
                backtrack,
                chams,
                desync,
                fakelag = false;
            float chams_opactiy = 1.f;
        }local;

        
    }cmaterials;
    //
    // Misc
    //
    std::string clantag = "Baimless 2.0";
    //char custom_clan_tag[255];
    struct Misc {
		bool minijump = false;
		int minijump_key = 0;
		bool wasdstrafe = false;
		bool autostrafe = false;
        bool unlockinventory = false;
        bool bhop,
            edge_jump ,
            duck_in_air ,
            comp_ranks ,
            air_strafe ,ragdoll ,
            name_stealer,
            knife_bot ,
            walk_bot ,
            thirdperson,
            chroma_menu,
            ant_untrusted = true,
            key_menu = false;
        int air_strafe_option;

        int clantag_type, config = 0;
        int menu_key = 0x2D;
        float thirdpeson_distance,
            ambient_red ,
            ambient_green ,
            ambient_blue = 0.f;
        std::string autobuy_str = "";
        bool autobuy = false;
        int thirdperson_key = 0;
        int edge_jump_key = 0;
        int playerModelT = 0;
        int playerModelCT = 0;
        bool autobuy_selected[16];
        int clantag_type_ = 0;
        bool fakeprime = false;
        bool autoaccpet = false;
        bool radar = false;
        bool jumpbug = false;
        int jumpbug_key = 0;
        bool quickReload = false;
        bool killMessage = false;
        std::string killMessageString = "Baimless 2.0";
        bool revealSuspect = false;
        bool recoilCrosshair = false;
        bool bJumpBug = false;
        int bJumpKey = 0;
        bool bDidJump;
        bool movement_recording = false;
        bool recordBool = false;
        bool playBool = false;
        bool autoAccept = false;
        bool misc_radar = false;
        bool misc_knife_switch = false;
		bool eventlogs = false;
        bool extend_backtrack = false;
    }misc;
    //
    // Changers
    //
    
    struct Changers {
        int playerModel = 0;
        struct Rank
        {
           bool enable = false;
           int selected = 0;
           int lvl = 1;
           int xp = 1;
           int wins = 1;
           int friend_ = 1;
           int teach = 1;
           int leader = 1;
        }rank;
    }changers;

    //
    // Fake ban
    //
    struct Ban
    {
        int time = 0;
        int type = 0;
   }ban;
    //
    // Medal changer
    //
    struct Medals
    {
        bool enable = false;
        std::vector<uint32_t> medals = {};
    }medals;
};
inline Options2 g_Options;
