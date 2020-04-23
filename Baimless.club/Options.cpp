#include "Options.hpp"
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

namespace legit {
    namespace colors {
        float legit_aa_visualize_color[3] = { 0.f, 0.12f,1.f };
        float legit_aa_visualize_fov_color[3] = { 0.f, 0.12f,1.f };
        float zeus_range[3] = { 0.f, 0.12f,1.f };
        float aimpoint_col[3] = { 0.f,0.f,1.f };
    }
}

namespace esp {
    namespace colors {
        float nadecolorr[3] = { 0.f,0.f,1.f };
		float grenadehelper[3] = { 0.f,0.f,1.f };
        float sound_esp[3] = { 0.f,0.f,1.f };
        //OPTION(Color, sound_esp, Color(0, 128, 255));
        float zeus_range[3] = { 0.f, 0.12f,1.f };
        float bomb_timer_col[3] = { 0.f, 0.12f,1.f };

        float grenade_prediction[3] = { 0.f, 0.12f,1.f };
        float out_of_view_arrows[3] = { 0.f,0.f,1.f };
        //OPTION(Color, out_of_view_arrows, Color(0, 128, 255));
        float manual_aa[3] = { 0.f, 0.12f,1.f };
        //OPTION(Color, impacts, Color(0, 128, 255));
        float impacts[3] = { 0.f,0.f,1.f };
        float beam_color[3] = { 0.f,0.f,1.f };
        float shot_hitboxes[3] = { 0.f, 0.12f,1.f };
        //OPTION(Color, local_glow, Color(0, 128, 255));
        //OPTION(Color, enemy_glow, Color(0, 128, 255));
        //OPTION(Color, team_glow, Color(0, 128, 255));
        float world_glow[3] = { 0.f,0.f,1.f };
        float enemy_glow[3] = { 0.f,0.f,1.f };
        float local_glow[3] = { 0.f,0.f,1.f };
        float team_glow[3] = { 0.f,0.f,1.f };
        //OPTION(Color, world_glow, Color(0, 128, 255));
        float dropped_weapon_box[3] = { 0.f, 0.12f,1.f };
        float dropped_weapon_ammo[3] = { 0.f, 0.12f,1.f };
        float dropped_weapon_ammo_bar[3] = { 0.f, 0.12f,1.f };
        namespace enemy {
            float box_col[3] = { 0.f,0.f,1.f };
            float name_col[3] = { 1.f,1.f,1.f };
            float ammo[3] = { 1.f,1.f,1.f };
            float weapons[3] = { 1.f,1.f,1.f };
            float money[3] = { 1.f,1.f,1.f };
            float helmet[3] = { 1.f,1.f,1.f };
            float scoped[3] = { 1.f,1.f,1.f };
            float fakeduck[3] = { 1.f,1.f,1.f };
            float skeleton[3] = { 1.f,1.f,1.f };
        }
        namespace team {
            float box_col[3] = { 0.f,0.f,1.f };
            float name_col[3] = { 1.f,1.f,1.f };
            float ammo[3] = { 1.f,1.f,1.f };
            float weapons[3] = { 1.f,1.f,1.f };
            float money[3] = { 1.f,1.f,1.f };
            float helmet[3] = { 1.f,1.f,1.f };
            float scoped[3] = { 1.f,1.f,1.f };
            float fakeduck[3] = { 1.f,1.f,1.f };
            float skeleton[3] = { 1.f,1.f,1.f };
        }
        namespace local {
            float box_col[3] = { 0.f,0.f,1.f };
            float name_col[3] = { 1.f,1.f,1.f };
            float ammo[3] = { 1.f,1.f,1.f };
            float weapons[3] = { 1.f,1.f,1.f };
            float money[3] = { 1.f,1.f,1.f };
            float helmet[3] = { 1.f,1.f,1.f };
            float scoped[3] = { 1.f,1.f,1.f };
            float fakeduck[3] = { 1.f,1.f,1.f };
            float skeleton[3] = { 1.f,1.f,1.f };
        }
    }
}


namespace materials {
    namespace colors {
        namespace enemy {
            float player[3] = { 0.89f, 0.37f, 0.80f };
            float behind_wall[3] = { 0.91f, 0.35f, 0.36f };
            float backtrack[3] = { 0.71f, 0.15f, 0.72f };
        }
        namespace team {
            float player[3] = { 0.89f, 0.37f, 0.80f };
            float behind_wall[3] = { 0.91f, 0.35f, 0.36f };
            float backtrack[3] = { 0.71f, 0.15f, 0.72f };
        }
        namespace local {
            float chams[3] = { 0.89f, 0.37f, 0.80f };
            float desync[3] = { 0.f, 0.12f,1.f };
            float fakelag[3] = { 0.f, 0.12f,1.f };
            float hand_chams[3] = { 0.f, 0.12f,1.f };
        }
    }
}

namespace menu
{
    float color[3] = { 0.f,0.f,1.f };
    int frequency = 100;
}

