#pragma once
#include "GUI.h"
#include "Controls.h"

/*
class CAntiAimTab : public CTab
{
public:
	void Setup();
	CDropBox disable_aa;

	////////////
	CSlider move_lag;
	CSlider standing_lag;
	CSlider air_lag;
	CSlider delay_shot;
	CSlider state_delay;
	CCheckBox	airstuck;
	CKeyBind airstuckkey;
	CKeyBind Double_key;
	CCheckBox Doubel_tap;
	CCheckBox Double_key_check;
	//anti aim
	CGroupBox AntiAimGroup;
	CGroupBox FakeLagOptions;
	CGroupBox OtherGroup;
	CComboBox AntiAimPitch;
	CCheckBox lag_bool;
	CComboBox AntiAimPitch2;
	CComboBox AntiAimPitch3;
	CSlider fakeyawoffset3;
	CComboBox AntiAimPitch4;
	CSlider fakeyawoffset4;

	CSlider fakeyawoffset2;
	CComboBox AntiAimYaw3;
	CComboBox AntiAimYaw4;

	CComboBox AntiAimYaw2;
	CSlider AntiAimYawoffset2;
	CSlider jitteryawawoffset2;
	CComboBox rbot_desyncstanding2;
	CKeyBind rbot_desyncstandingkey2;
	CSlider AntiAimYawoffset3;
	CSlider jitteryawawoffset3;
	CComboBox rbot_desyncstanding3;
	CKeyBind rbot_desyncstandingkey3;
	CSlider AntiAimYawoffset4;
	CSlider jitteryawawoffset4;
	CComboBox rbot_desyncstanding4;
	CKeyBind rbot_desyncstandingkey4;
	CComboBox rbot_desyncstanding;
	CKeyBind rbot_desyncstandingkey;
	CComboBox AntiAimYaw;
	CSlider AntiAimYawoffset;
	CSlider jitteryawawoffset;
	CComboBox edgeyaw;
	CComboBox edgeyaw2;
	CComboBox yawbase2;
	CSlider body_lean2;
	CSlider inverted_body_lean2;
	CComboBox edgeyaw3;
	CComboBox yawbase3;
	CSlider body_lean3;
	CSlider inverted_body_lean3;
	CComboBox edgeyaw4;
	CComboBox Double_keymethod;
	CComboBox yawbase4;
	CSlider body_lean4;
	CSlider inverted_body_lean4;
	CComboBox yawbase;
	CSlider body_lean;
	CSlider fakeyawoffset;
	CSlider inverted_body_lean;
	CCheckBox aaenabel;
	CKeyBind manualaakey;
	CCheckBox fake_crouch;
	CKeyBind fake_crouchkey;
	CCheckBox no_crouch_cooldown;
	CKeyBind fakewalkenabelkey;
	CCheckBox fakewalkenabel;
	CSlider FakeWalkSpeed;
	CSlider LbyAdd;
	CLabel manualaakeyinfo;
	CCheckBox lbybreaker;
	CCheckBox bFixLegMovement;
	CCheckBox static_lby;
};
*/
/*
class CLegitBotTab : public CTab
{
public:
	void Setup();
	// Master Switch

	CGroupBox AimbotGroup;

	CCheckBox AimbotEnable;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CCheckBox AimbotSmokeCheck;
	CCheckBox AimbotBacktrack;
	CCheckBox StandAloneRCS;
	CSlider StandAloneRCSamount;
	CCheckBox visualizefov;
	CCheckBox Legit_AA_visualize;
	CColorSelector visualizefov_col;
	CCheckBox Legit_AA;
	CCheckBox manual_Legit_AA;
	CCheckBox Trigger_bot;
	CCheckBox Trigger_bot_key;
	CComboBox Trigger_bot_keymethod;

	CKeyBind Trigger_key;
	CCheckBox Trigger_bot_deathmatch;
	CCheckBox Trigger_bot_smoke_check;
	CDropBox Trigger_Hitbox;
	CSlider Trigger_min_damage;
	CSlider Trigger_chance;
	CSlider Trigger_delay;
	CKeyBind Legit_AA_key;
	CColorSelector Legit_AA_col;
	CColorSelector Legit_AA_visualize_col;
	CCheckBox legitaa;
	CComboBox Legit_AA_type;
	CComboBox Legit_AA_visualize_material;
	CKeyBind legitaakey;
	CGroupBox WeaponMainGroup;
	CComboBox specific_weapon;
	CSlider   WeaponMainSpeed;
	CSlider   WeaponMainFoV;
	CSlider WeaponMainRecoil;
	CComboBox WeaponMainHitbox;
	CSlider   WeaponMainInacc;
	CSlider WeaponMaindmg;
	CSlider   WeaponPistSpeed;
	CSlider   WeaponPistFoV;
	CSlider WeaponPistRecoil;
	CComboBox WeaponPistHitbox;
	CSlider   WeaponPistInacc;
	CSlider WeaponPistdmg;
	CSlider   WeaponSnipSpeed;
	CSlider   WeaponSnipFoV;
	CSlider WeaponSnipRecoil;
	CComboBox WeaponSnipHitbox;
	CSlider   WeaponSnipInacc;
	CSlider WeaponSnipdmg;
	CSlider   WeaponMpSpeed;
	CSlider   WeaponMpFoV;
	CSlider WeaponMpRecoil;
	CComboBox WeaponMpHitbox;
	CSlider   WeaponMpInacc;
	CSlider WeaponMpdmg;
	CSlider   WeaponShotgunSpeed;
	CSlider   WeaponShotgunFoV;
	CSlider WeaponShotgunRecoil;
	CComboBox WeaponShotgunHitbox;
	CSlider   WeaponShotgunInacc;
	CSlider WeaponShotgundmg;
	CSlider   WeaponMGSpeed;
	CSlider   WeaponMGFoV;
	CSlider WeaponMGRecoil;
	CComboBox WeaponMGHitbox;
	CSlider   WeaponMGInacc;
	CGroupBox Other;
	CSlider Weaponheavydmg;
};
*/
/*
class CVisualTab : public CTab
{
public:
	void Setup();

	CGroupBox OptionsGroup;
	CCheckBox hitmarker;
	CCheckBox impacts;
	CCheckBox penetration_reticle;
	CCheckBox spectators;
	CCheckBox Indicators;
	CCheckBox LBYIndicator;
	CCheckBox killCounter;
	CCheckBox Wasd_indicator;
	CCheckBox hit_effect;
	CSlider IndicatorsY;
	CSlider IndicatorsX;

	CCheckBox glowlocal;
	CColorSelector glowlocal_col;
	CColorSelector	glowenemy_col;
	CColorSelector	glowteam_col;
	CDropBox m_world_glow;
	CCheckBox glowenemy;
	CColorSelector m_world_glow_col;
	CCheckBox glowteam;
	CCheckBox drop_box;
	CCheckBox droppedweaponammo;
	CCheckBox droppedweaponammobar;
	CCheckBox chickenbox;
	CComboBox grenadename;
	CCheckBox grenade;
	CComboBox droppedweaponname;
	CColorSelector drop_box_col;
	CColorSelector droppedweaponammobar_col;
	CSlider spectatorsX;
	CCheckBox Chocked_indicator;
	CCheckBox nightmode;
	CSlider spectatorsY;
	CCheckBox feet_circle;
	CCheckBox zeus_range;
	CComboBox hitmarkersound;
	CGroupBox MoreOptions;
	CGroupBox ChamsOptions;
	CGroupBox Effects;
	CColorSelector zeus_range_col;
	CCheckBox fov_arrows_enemy;
	CColorSelector fov_arrows_enemy_col;
	CCheckBox esp_bool;
	CComboBox scope_bool_option;
	CDropBox indicator;
	CDropBox bullettracers_type;
	CColorSelector skeleton_col;
	CColorSelector skeleton_col_team;
	CDropBox skeleton;
	CDropBox skeleton_team;

	CCheckBox esp_player_boxes;
	CCheckBox esp_player_boxes_team;

	CColorSelector esp_player_boxes_col;
	CColorSelector esp_player_boxes_col_team;

	CColorSelector esp_player_names_col;
	CColorSelector esp_player_names_col_team;
	CColorSelector esp_player_ammo_col_team;
	CColorSelector Helmet_info_col_team;
	CColorSelector Scoped_info_col_team;

	CColorSelector Fakeduck_info_col_team;
	CColorSelector esp_player_ammo_col;
	CColorSelector esp_esp_weapon_col;
	CColorSelector	money_info_col;
	CColorSelector esp_esp_weapon_col_team;
	CColorSelector money_info_col_team;
	CColorSelector Helmet_info_col;
	CCheckBox esp_player_names;
	CCheckBox esp_player_names_team;
	CColorSelector Scoped_info_col_local;
	CCheckBox esp_player_health;
	CCheckBox esp_player_health_team;
	CComboBox esp_player_health_type;
	CComboBox esp_player_health_type_team;
	CComboBox esp_player_boxes_type;
	CComboBox esp_player_boxes_type_team;
	CCheckBox esp_ammo_team;
	CColorSelector Fakeduck_info_col;
	CCheckBox esp_ammo;
	CCheckBox esp_weapon;
	CCheckBox money_info;
	CCheckBox Helmet_info;
	CCheckBox Scoped_info;
	CCheckBox esp_weapon_team;
	CCheckBox money_info_team;
	CCheckBox Helmet_info_team;
	CCheckBox Scoped_info_team;
	CCheckBox Fakeduck_info_team;

	CComboBox WeaponArray;
	CCheckBox Fakeduck_info;
	CComboBox WeaponArray_team;
	CDropBox skeleton_Local;
	CCheckBox esp_player_boxes_Local;
	CCheckBox DebugLagComp;
	CColorSelector DebugLagComp_col;
	CColorSelector esp_player_boxes_col_Local;
	CComboBox esp_player_boxes_type_Local;
	CComboBox esp_player_health_type_Local;
	CComboBox WeaponArray_Local;
	CCheckBox esp_player_names_Local;
	CCheckBox esp_player_health_Local;
	CCheckBox esp_ammo_Local;
	CCheckBox esp_weapon_Local;
	CColorSelector Fakeduck_info_col_local;
	CCheckBox money_info_local;
	CCheckBox Helmet_info_local;
	CCheckBox Scoped_info_local;
	CCheckBox Fakeduck_info_local;
	CColorSelector esp_player_names_col_Local;
	CColorSelector skeleton_col_Local;
	CColorSelector esp_player_ammo_col_Local;
	CColorSelector esp_esp_weapon_col_Local;
	CColorSelector money_info_col_local;
	CColorSelector Helmet_info_col_local;
	CColorSelector Scoped_info_col;
	CCheckBox esp_grenadeprediction;
	CComboBox chamsmaterial;
	CComboBox chamsmaterial_team;
	CComboBox chamsmaterial_local;
	CCheckBox chams_player_enabled;
	CCheckBox vis_localplayerchams_shadow;
	CCheckBox localplayerchams;
	CCheckBox vis_chams_enemy_visible;
	CCheckBox bullettracers;
	CCheckBox full_bright;
	CColorSelector esp_grenadepredictione_col;
	CColorSelector vis_localplayerchams_shadow_col;
	CColorSelector localplayerchams_col;
	CColorSelector vis_chams_enemy_visible_col;
	CColorSelector acc_vis_chams_enemy_invisible_col;
	CColorSelector vis_chams_friendly_invisible_col;
	CColorSelector vis_chams_friendly_visible_col;
	CCheckBox vis_chams_enemy_invisible;
	CComboBox HandCHAMS;
	CCheckBox manualaaindicator;
	CColorSelector manualaaindicator_col;
	CColorSelector BacktrackChams_col;
	CCheckBox BacktrackChams;
	CSlider beamsize;
	CColorSelector impacts_col;
	CCheckBox vis_chams_friendly_invisible;
	CCheckBox vis_chams_friendly_visible;

	CCheckBox fakelagchams;
	CColorSelector fakelagchams_col;
	CColorSelector HandCHAMS_col;

	CSlider chamsopacity_local;
	CCheckBox HandCHAMSenabel;
	CSlider bulletlife;
	CSlider chamsopacity;
	CSlider chamsopacity_team;
	CColorSelector bullettracers_col;
	CCheckBox scope_bool;
	CCheckBox noflash;
	CCheckBox NoZoom;
	CCheckBox norecoil;
	CCheckBox msc_remove_smoke;

	CCheckBox DisablePostProcessing;
	CCheckBox watermark_enable;
	CSlider viewmodel;
	CSlider viewmodel_x;
	CSlider viewmodel_y;
	CSlider viewmodel_z;
	CCheckBox SoundEsp;
	CSlider flTransparentWorld;
	CSlider flTransparentProps;
	CColorSelector SoundEsp_col;
	CSlider Fov;
	CSlider transparency_scoped_alpha;
};

class CVisualTab : public CTab
{
public:
	void Setup();

	CGroupBox OptionsGroup;
	CCheckBox hitmarker;
	CCheckBox impacts;
	CCheckBox penetration_reticle;
	CCheckBox spectators;
	CCheckBox Indicators;
	CCheckBox LBYIndicator;
	CCheckBox killCounter;
	CCheckBox Wasd_indicator;
	CCheckBox hit_effect;
	CSlider IndicatorsY;
	CSlider IndicatorsX;

	CCheckBox glowlocal;
	CColorSelector glowlocal_col;
	CColorSelector	glowenemy_col;
	CColorSelector	glowteam_col;
	CDropBox m_world_glow;
	CCheckBox glowenemy;
	CColorSelector m_world_glow_col;
	CCheckBox glowteam;
	CCheckBox drop_box;
	CCheckBox droppedweaponammo;
	CCheckBox droppedweaponammobar;
	CCheckBox chickenbox;
	CComboBox grenadename;
	CCheckBox grenade;
	CComboBox droppedweaponname;
	CColorSelector drop_box_col;
	CColorSelector droppedweaponammobar_col;
	CSlider spectatorsX;
	CCheckBox Chocked_indicator;
	CCheckBox nightmode;
	CSlider spectatorsY;
	CCheckBox feet_circle;
	CCheckBox zeus_range;
	CComboBox hitmarkersound;
	CGroupBox MoreOptions;
	CGroupBox ChamsOptions;
	CGroupBox Effects;
	CColorSelector zeus_range_col;
	CCheckBox fov_arrows_enemy;
	CColorSelector fov_arrows_enemy_col;
	CCheckBox esp_bool;
	CComboBox scope_bool_option;
	CDropBox indicator;
	CDropBox bullettracers_type;
	CColorSelector skeleton_col;
	CColorSelector skeleton_col_team;
	CDropBox skeleton;
	CDropBox skeleton_team;

	CCheckBox esp_player_boxes;
	CCheckBox esp_player_boxes_team;

	CColorSelector esp_player_boxes_col;
	CColorSelector esp_player_boxes_col_team;

	CColorSelector esp_player_names_col;
	CColorSelector esp_player_names_col_team;
	CColorSelector esp_player_ammo_col_team;
	CColorSelector Helmet_info_col_team;
	CColorSelector Scoped_info_col_team;

	CColorSelector Fakeduck_info_col_team;
	CColorSelector esp_player_ammo_col;
	CColorSelector esp_esp_weapon_col;
	CColorSelector	money_info_col;
	CColorSelector esp_esp_weapon_col_team;
	CColorSelector money_info_col_team;
	CColorSelector Helmet_info_col;
	CCheckBox esp_player_names;
	CCheckBox esp_player_names_team;
	CColorSelector Scoped_info_col_local;
	CCheckBox esp_player_health;
	CCheckBox esp_player_health_team;
	CComboBox esp_player_health_type;
	CComboBox esp_player_health_type_team;
	CComboBox esp_player_boxes_type;
	CComboBox esp_player_boxes_type_team;
	CCheckBox esp_ammo_team;
	CColorSelector Fakeduck_info_col;
	CCheckBox esp_ammo;
	CCheckBox esp_weapon;
	CCheckBox money_info;
	CCheckBox Helmet_info;
	CCheckBox Scoped_info;
	CCheckBox esp_weapon_team;
	CCheckBox money_info_team;
	CCheckBox Helmet_info_team;
	CCheckBox Scoped_info_team;
	CCheckBox Fakeduck_info_team;

	CComboBox WeaponArray;
	CCheckBox Fakeduck_info;
	CComboBox WeaponArray_team;
	CDropBox skeleton_Local;
	CCheckBox esp_player_boxes_Local;
	CCheckBox DebugLagComp;
	CColorSelector DebugLagComp_col;
	CColorSelector esp_player_boxes_col_Local;
	CComboBox esp_player_boxes_type_Local;
	CComboBox esp_player_health_type_Local;
	CComboBox WeaponArray_Local;
	CCheckBox esp_player_names_Local;
	CCheckBox esp_player_health_Local;
	CCheckBox esp_ammo_Local;
	CCheckBox esp_weapon_Local;
	CColorSelector Fakeduck_info_col_local;
	CCheckBox money_info_local;
	CCheckBox Helmet_info_local;
	CCheckBox Scoped_info_local;
	CCheckBox Fakeduck_info_local;
	CColorSelector esp_player_names_col_Local;
	CColorSelector skeleton_col_Local;
	CColorSelector esp_player_ammo_col_Local;
	CColorSelector esp_esp_weapon_col_Local;
	CColorSelector money_info_col_local;
	CColorSelector Helmet_info_col_local;
	CColorSelector Scoped_info_col;
	CCheckBox esp_grenadeprediction;
	CComboBox chamsmaterial;
	CComboBox chamsmaterial_team;
	CComboBox chamsmaterial_local;
	CCheckBox chams_player_enabled;
	CCheckBox vis_localplayerchams_shadow;
	CCheckBox localplayerchams;
	CCheckBox vis_chams_enemy_visible;
	CCheckBox bullettracers;
	CCheckBox full_bright;
	CColorSelector esp_grenadepredictione_col;
	CColorSelector vis_localplayerchams_shadow_col;
	CColorSelector localplayerchams_col;
	CColorSelector vis_chams_enemy_visible_col;
	CColorSelector acc_vis_chams_enemy_invisible_col;
	CColorSelector vis_chams_friendly_invisible_col;
	CColorSelector vis_chams_friendly_visible_col;
	CCheckBox vis_chams_enemy_invisible;
	CComboBox HandCHAMS;
	CCheckBox manualaaindicator;
	CColorSelector manualaaindicator_col;
	CColorSelector BacktrackChams_col;
	CCheckBox BacktrackChams;
	CSlider beamsize;
	CColorSelector impacts_col;
	CCheckBox vis_chams_friendly_invisible;
	CCheckBox vis_chams_friendly_visible;

	CCheckBox fakelagchams;
	CColorSelector fakelagchams_col;
	CColorSelector HandCHAMS_col;

	CSlider chamsopacity_local;
	CCheckBox HandCHAMSenabel;
	CSlider bulletlife;
	CSlider chamsopacity;
	CSlider chamsopacity_team;
	CColorSelector bullettracers_col;
	CCheckBox scope_bool;
	CCheckBox noflash;
	CCheckBox NoZoom;
	CCheckBox norecoil;
	CCheckBox msc_remove_smoke;

	CCheckBox DisablePostProcessing;
	CCheckBox watermark_enable;
	CSlider viewmodel;
	CSlider viewmodel_x;
	CSlider viewmodel_y;
	CSlider viewmodel_z;
	CCheckBox SoundEsp;
	CSlider flTransparentWorld;
	CSlider flTransparentProps;
	CColorSelector SoundEsp_col;
	CSlider Fov;
	CSlider transparency_scoped_alpha;
};

*/
/*
class CMiscTab : public CTab
{
public:
	void Setup();

	CComboBox ConfigBox;
	CGroupBox ConfigGroup;

	CListBox ConfigListBox;
	CButton SaveConfig;
	CButton LoadConfig;
	CButton RemoveConfig;
	CTextField NewConfigName;
	CButton AddConfig;

	CButton SaveButton;
	CButton LoadButton;
	CGroupBox OtherGroup;
	CCheckBox bhop_bool;
	CCheckBox rankreveal;
	CCheckBox edge_jump;
	CCheckBox duck_in_air;
	CCheckBox AutoStrafe;
	CCheckBox knifebot;
	CCheckBox walkbot;
	CCheckBox OtherSafeMode;
	CComboBox Menutheme;

	CCheckBox bypass_obs;
	CCheckBox FakePing;
	CGroupBox MoreOps;
	CCheckBox OtherThirdperson;
	CKeyBind ThirdPersonKeyBind;
	CKeyBind edge_jumpKeyBind;
	CComboBox clantag;
	CTextField customclan;
	CButton unloadbutton;
	CCheckBox NameChanger;
	CCheckBox autobuy;
	CDropBox buybot;
	CColorSelector MenuColor;
	CSlider thirdperson_distance;
	CSlider AmbientRed;
	CSlider AmbientGreen;
	CSlider AmbientBlue;
	CLabel MenuKeyLabel;
	CKeyBind MenuKey;
	CCheckBox ragdoll;
};
class CSkinTab : public CTab
{
public:
	void Setup();

	CGroupBox MainGroup;
	CGroupBox WeaponGroup;
	CCheckBox skins_enable;
	CCheckBox stacktrackknife_enable;
	CComboBox skins_knife_model;
	CListBox KnifeSkin;
	CListBox GloveSkin;

	CComboBox GloveModel;
	CButton SkinApply;
	CButton rankApply;
	CSlider player_level;
	CSlider player_level_xp;
	CSlider wins;
	CSlider cmd_friendly;
	CSlider cmd_teaching;
	CSlider cmd_leader;
	CComboBox rank_id;
	CCheckBox rankchanger;

	CComboBox M41SSkin;
	CComboBox M4A4Skin;
	CComboBox AK47Skin;
	CComboBox AUGSkin;
	CComboBox FAMASSkin;
	CComboBox GALILSkin;
	CComboBox SG553Skin;

	CComboBox SCAR20Skin;
	CComboBox G3SG1Skin;
	CComboBox SSG08Skin;
	CComboBox AWPSkin;

	CComboBox MAG7Skin;
	CComboBox XM1014Skin;
	CComboBox SAWEDOFFSkin;

	CComboBox NOVASkin;
	CComboBox NEGEVSkin;
	CComboBox M249Skin;

	CComboBox GLOCKSkin;
	CComboBox USPSSkin;
	CComboBox DEAGLESkin;
	CComboBox MAGNUMSkin;
	CComboBox DUALSSkin;
	CComboBox RevolverSkin;
	CComboBox FIVESEVENSkin;
	CComboBox TECNINESkin;
	CComboBox P2000Skin;
	CComboBox P250Skin;

	CComboBox MAC10Skin;
	CComboBox P90Skin;
	CComboBox UMP45Skin;
	CComboBox BIZONSkin;
	CComboBox MP7Skin;
	CComboBox MP9Skin;

	CSlider stacktrackknife;

};*/
/*
class CPlayersTab : public CTab
{
public:
	void Setup();

	CListBox PlayerListControl;

	CGroupBox PlayerSettingsGroup;
	CCheckBox PlayerFriendly;
	CCheckBox PlayerPreferBody;
	CCheckBox PlayerForcePitch;
	CComboBox PlayerForcePitch_Pitch;
	CCheckBox PlayerForceYaw;
	CComboBox PlayerForceYaw_Yaw;
};*/


class PhantomWindow : public CWindow
{
public:
	void Setup();
	//CLegitBotTab LegitBotTab;
	//CVisualTab VisualsTab;
	//CMiscTab MiscTab;
	//CSkinTab SkinTab;
	//CPlayersTab PlayersTab;

	CButton   SkinApply;
	CButton SaveButton1;
	CButton LoadButton1;
	CButton SaveButton;
	CButton LoadButton;
	CButton UnloadButton;
	CButton PanicButton;
	CButton ThirdButton;
	CButton ByPass;
	CButton FirstButton;
	CComboBox ConfigBox;
};
namespace Options
{
	void SetupMenu();
	void DoUIFrame();
	extern PhantomWindow Menu;
};
