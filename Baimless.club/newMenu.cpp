#include "newMenu.h"
#include "Controls.h"
#include "Hooks.h"
#include <fstream>
#include "Features/rankchanger/Protobuffs.h"
#include "Features/rankchanger/IMemAlloc.h"
#include "Features/rankchanger/steam.h"
#include "Features/rankchanger/rankchanger.h"
#include "SDK\ICvar.h"
#include "Playerlist.h"
#include "XorStr.hpp"



#define FCVAR_CHEATGAY                     (1<<14) // Only useable in singleplayer / debug / multiplayer & sv_cheats
struct Config_t {
	int id;
	std::string name;
};
std::vector<Config_t> configs;
typedef void(__cdecl* MsgFn)(const char* msg, va_list);

void MsgX(const char* msg, ...)
{
	if (msg == nullptr)
		return; //If no string was passed, or it was null then don't do anything
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg"); 	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list); //Calls the function, we got the address above.
}
/*


	ConfigBox.AddItem("legit");
	ConfigBox.AddItem("rage");
	ConfigBox.AddItem("hvh");
	ConfigBox.AddItem("casual");
	ConfigBox.AddItem("hvh rage");
	ConfigBox.AddItem("main");
	ConfigBox.AddItem("baim hvh");
	*/

/*void SaveLegitCallbk()
{
	switch (Options::Menu.MiscTab.ConfigBox.GetIndex())
	{
	case 0:
		GUI.SaveWindowState(&Options::Menu, "legit.cfg");
		break;
	case 1:
		GUI.SaveWindowState(&Options::Menu, "rage.cfg");
		break;
	case 2:
		GUI.SaveWindowState(&Options::Menu, "hvh.cfg");
		break;
	case 3:
		GUI.SaveWindowState(&Options::Menu, "casual.cfg");
		break;
	case 4:
		GUI.SaveWindowState(&Options::Menu, "hvhrage.cfg");
		break;
	case 5:
		GUI.SaveWindowState(&Options::Menu, "main.cfg");
		break;
	case 6:
		GUI.SaveWindowState(&Options::Menu, "baimhvh.cfg");
		break;
	}

	g_pCvar->ConsoleColorPrintf(Color(Options::Menu.MiscTab.MenuColor.GetValue(), 255), "BAIMLESS.XYZ ");
	std::string uremam = "Saved configuration.     \n";
	MsgX(uremam.c_str());
}*/

/*void LoadLegitCallbk()
{
	switch (Options::Menu.MiscTab.ConfigBox.GetIndex())
	{
	case 0:
		GUI.LoadWindowState(&Options::Menu, "legit.cfg");
		break;
	case 1:
		GUI.LoadWindowState(&Options::Menu, "rage.cfg");
		break;
	case 2:
		GUI.LoadWindowState(&Options::Menu, "hvh.cfg");
		break;
	case 3:
		GUI.LoadWindowState(&Options::Menu, "casual.cfg");
		break;
	case 4:
		GUI.LoadWindowState(&Options::Menu, "hvhrage.cfg");
		break;
	case 5:
		GUI.LoadWindowState(&Options::Menu, "main.cfg");
		break;
	case 6:
		GUI.LoadWindowState(&Options::Menu, "baimhvh.cfg");
		break;
	}

	//g_pCvar->ConsoleColorPrintf(Color(Options::Menu.MiscTab.MenuColor.GetValue(), 255), "BAIMLESS.XYZ ");
	std::string uremam = "Loaded configuration.     \n";
	MsgX(uremam.c_str());
}



void rankupdate()
{
	write.SendClientHello();
	write.SendMatchmakingClient2GCHello();
}
*/



//#define WINDOW_WIDTH 450
//#define WINDOW_HEIGHT 503



#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 670 // 507
//137 ading
PhantomWindow Options::Menu;



void PhantomWindow::Setup()
{
	SetPosition(350, 50);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetTitle("");
	//RegisterTab(&RageBotTab);
	//RegisterTab(&AntiAimTab);
	//RegisterTab(&LegitBotTab);
	//RegisterTab(&VisualsTab);
	//RegisterTab(&MiscTab);
	//RegisterTab(&SkinTab);
	//RegisterTab(&PlayersTab);

	RECT Client = GetClientArea();
	Client.bottom -= 29;

	//RageBotTab.Setup();
	//AntiAimTab.Setup();
	//LegitBotTab.Setup();
	//VisualsTab.Setup();
	//MiscTab.Setup();
	//SkinTab.Setup();
	//PlayersTab.Setup();

#pragma region Bottom Buttons
#pragma endregion
}
/*
void CRageBotTab::Setup()
{
	SetTitle("A");

	AimbotGroup.SetPosition(10, 5);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(225, 577);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("BAIMLESS_config_rage_enable");
	AimbotGroup.PlaceLabledControl(0, "Enable", this, &AimbotEnable);

	autofire.SetFileId("BAIMLESS_config_rage_autofire");
	AimbotGroup.PlaceLabledControl(0, "Auto fire", this, &autofire);

	silentaim.SetFileId("BAIMLESS_config_rage_silentaim");
	AimbotGroup.PlaceLabledControl(0, "Silent aim", this, &silentaim);

	recoilenabel.SetFileId("BAIMLESS_config_rage_recoilcontrol");
	AimbotGroup.PlaceLabledControl(0, "Anti Recoil", this, &recoilenabel);

	RemoveSpread.SetFileId("other_removespread");
	AimbotGroup.PlaceLabledControl(0, "Remove spread", this, &RemoveSpread);

	autorevolver.SetFileId("BAIMLESS_config_rage_autorevolver");
	AimbotGroup.PlaceLabledControl(0, "Auto revolver", this, &autorevolver);

	autozeus.SetFileId("BAIMLESS_config_rage_autozeus");
	AimbotGroup.PlaceLabledControl(0, "Auto zeus", this, &autozeus);

	stop_bool.SetFileId("BAIMLESS_config_rage_autostop");
	AimbotGroup.PlaceLabledControl(0, "Auto stop", this, &stop_bool);

	zeus_chance.SetFileId("BAIMLESS_config_rage_zeus_chance");
	zeus_chance.SetBoundaries(0, 100);
	zeus_chance.SetValue(40);
	zeus_chance.extension = ("%%");
	AimbotGroup.PlaceLabledControl(0, "Zeus hitchance", this, &zeus_chance);

	shot_Backtrack.SetFileId("BAIMLESS_config_rage_shot_Backtrack");
	AimbotGroup.PlaceLabledControl(0, "Shot backtrack", this, &shot_Backtrack);

	Backtrack.SetFileId("BAIMLESS_config_rage_backtrack");
	AimbotGroup.PlaceLabledControl(0, "Backtrack", this, &Backtrack);

	target_auto.SetFileId("BAIMLESS_config_rage_hitscan");
	target_auto.items.push_back(dropdownboxitem(false, ("head")));
	target_auto.items.push_back(dropdownboxitem(false, ("neck")));
	target_auto.items.push_back(dropdownboxitem(false, ("chest")));
	target_auto.items.push_back(dropdownboxitem(false, ("stomach")));
	target_auto.items.push_back(dropdownboxitem(false, ("arms")));
	target_auto.items.push_back(dropdownboxitem(false, ("legs")));
	AimbotGroup.PlaceLabledControl(1, "Hitscan", this, &target_auto);

	AutomaticScope.SetFileId("BAIMLESS_config_rage_autoscope");
	AimbotGroup.PlaceLabledControl(0, "Auto scope", this, &AutomaticScope);

	chance_val.SetFileId("BAIMLESS_config_rage_hitchance");
	chance_val.SetBoundaries(0, 100);
	chance_val.SetValue(40);
	chance_val.extension = ("%%");
	AimbotGroup.PlaceLabledControl(0, "Minimum hitchance", this, &chance_val);

	damage_val.SetFileId("BAIMLESS_config_rage_mindamage");
	damage_val.SetBoundaries(1, 100);
	damage_val.SetValue(10);
	damage_val.extension = ("°");
	AimbotGroup.PlaceLabledControl(0, "Minimum autowall damage", this, &damage_val);

	baimifhp.SetFileId("BAIMLESS_config_rage_baimifhplower");
	baimifhp.SetBoundaries(0, 100);
	baimifhp.SetValue(40);
	baimifhp.extension = ("%%");
	AimbotGroup.PlaceLabledControl(0, "Avoid head if hp lower", this, &baimifhp);

	avoidhead_if.SetFileId("BAIMLESS_config_rage_avoidhead_if");
	avoidhead_if.items.push_back(dropdownboxitem(false, ("Lethal")));
	avoidhead_if.items.push_back(dropdownboxitem(false, ("In air")));
	avoidhead_if.items.push_back(dropdownboxitem(false, ("Up pitch")));
	avoidhead_if.items.push_back(dropdownboxitem(false, ("On key")));
	avoidhead_if.items.push_back(dropdownboxitem(false, ("Missed x shots")));
	avoidhead_if.items.push_back(dropdownboxitem(false, ("High speed")));
	avoidhead_if.items.push_back(dropdownboxitem(false, ("Vulnerable")));
	AimbotGroup.PlaceLabledControl(0, "Avoid head if", this, &avoidhead_if);

	baimkey.SetFileId("BAIMLESS_config_rage_baimkey");
	AimbotGroup.PlaceLabledControl(0, "", this, &baimkey);

	baimifmissedxshots.SetFileId("BAIMLESS_config_rage_baimifmissedxshots");
	baimifmissedxshots.SetBoundaries(0, 20);
	baimifmissedxshots.SetValue(1);
	baimifmissedxshots.extension = ("°");
	AimbotGroup.PlaceLabledControl(0, "Avoid head after x missed shots", this, &baimifmissedxshots);

	OtherGroup.SetText("Other");
	OtherGroup.SetPosition(245, 5);
	OtherGroup.SetSize(225, 577);
	RegisterControl(&OtherGroup);

	//AimbotGroup.SetPosition(10, 5);
//	AimbotGroup.SetText("Aimbot");
	//AimbotGroup.SetSize(225, 569);


	res_type.SetFileId("BAIMLESS_config_rage_res_type");
	OtherGroup.PlaceLabledControl(0, "Resolver", this, &res_type);


	vecvelocityprediction.SetFileId("BAIMLESS_config_rage_prediction");
	OtherGroup.PlaceLabledControl(0, "Prediction", this, &vecvelocityprediction);

	rbot_extrapolation.SetFileId("BAIMLESS_config_rage_extrapolation");
	OtherGroup.PlaceLabledControl(0, "Extrapolation", this, &rbot_extrapolation);

	fake_lag_fix.SetFileId("BAIMLESS_config_rage_fake_lag_fix");
	OtherGroup.PlaceLabledControl(0, "Fake lag fix", this, &fake_lag_fix);

	head_scale.SetFileId("BAIMLESS_config_rage_head_scale");
	head_scale.SetBoundaries(1, 100);
	head_scale.SetValue(10);
	AimbotGroup.PlaceLabledControl(0, "Head scale", this, &head_scale);

	body_scale.SetFileId("BAIMLESS_config_rage_body_scale");
	body_scale.SetBoundaries(1, 100);
	body_scale.SetValue(10);
	AimbotGroup.PlaceLabledControl(0, "Body scale", this, &body_scale);
	//showaimpoint.SetFileId("aim_showaimpoint");
	//OtherGroup.PlaceLabledControl(0, "Show aimpoint", this, &showaimpoint);

	//showaimpoint_col.SetFileId(("vis_showaimpoint_col"));
	//OtherGroup.PlaceLabledControl(0, (""), this, &showaimpoint_col);



	//








    
#pragma endregion misc tab
}*/
/*
void CAntiAimTab::Setup()
{
	SetTitle("G");

	AntiAimGroup.SetText("Anti-aimbot");
	AntiAimGroup.SetPosition(10, 5);
	AntiAimGroup.SetSize(225, 577);
	AntiAimGroup.AddTab(CGroupTab("Stand", 1));
	AntiAimGroup.AddTab(CGroupTab("Moving", 2));
	AntiAimGroup.AddTab(CGroupTab("Air", 3));
	AntiAimGroup.AddTab(CGroupTab("Slow", 4));
	RegisterControl(&AntiAimGroup);

	aaenabel.SetFileId("BAIMLESS_config_rage_antiaim_enabel");
	AntiAimGroup.PlaceLabledControl(1, "Active", this, &aaenabel);

	////////////////////////

	AntiAimPitch.SetFileId("eternity_config_rage_standing_pitch");
	AntiAimPitch.AddItem("off");
	AntiAimPitch.AddItem("down");
	AntiAimPitch.AddItem("up");
	AntiAimPitch.AddItem("zero");
	AntiAimGroup.PlaceLabledControl(1, "pitch", this, &AntiAimPitch);

	yawbase.SetFileId("eternity_config_rage_standing_yawbase");
	yawbase.AddItem("static");
	yawbase.AddItem("crosshair");
	yawbase.AddItem("at targets");
	AntiAimGroup.PlaceLabledControl(1, "yaw base", this, &yawbase);

	AntiAimYaw.SetFileId("eternity_config_rage_moving_yaw");
	AntiAimYaw.AddItem("off");
	AntiAimYaw.AddItem("sideways");
	AntiAimYaw.AddItem("backwards");
	AntiAimYaw.AddItem("spin");
	AntiAimYaw.AddItem("jitter");
	AntiAimGroup.PlaceLabledControl(1, "yaw", this, &AntiAimYaw);

	AntiAimYawoffset.SetFileId("eternity_config_rage_AntiAimYawoffset");
	AntiAimYawoffset.SetBoundaries(-180, 180);
	AntiAimYawoffset.SetValue(0);
	AntiAimYawoffset.extension = ("°");
	AntiAimGroup.PlaceLabledControl(1, "offset", this, &AntiAimYawoffset);

	jitteryawawoffset.SetFileId("eternity_config_rage_AntiAimjitteryawawoffset");
	jitteryawawoffset.SetBoundaries(0, 180);
	jitteryawawoffset.SetValue(0);
	jitteryawawoffset.extension = ("°");
	AntiAimGroup.PlaceLabledControl(1, "jitter range", this, &jitteryawawoffset);

	rbot_desyncstanding.SetFileId("eternity_config_rage_standing_body_yaw");
	rbot_desyncstanding.AddItem("off");
	rbot_desyncstanding.AddItem("static");
	rbot_desyncstanding.AddItem("Jitter");
	rbot_desyncstanding.AddItem("tank");
	AntiAimGroup.PlaceLabledControl(1, "body yaw", this, &rbot_desyncstanding);

	rbot_desyncstandingkey.SetFileId("eternity_config_rage_rbot_desyncstandingkey");
	AntiAimGroup.PlaceLabledControl(1, (""), this, &rbot_desyncstandingkey);

	fakeyawoffset.SetFileId("eternity_config_rage_AntiAimfakeyawoffset");
	fakeyawoffset.SetBoundaries(0, 60);
	fakeyawoffset.SetValue(0);
	fakeyawoffset.extension = ("°");
	AntiAimGroup.PlaceLabledControl(1, "Desync limit", this, &fakeyawoffset);

	body_lean.SetFileId("eternity_config_rage_AntiAimbody_lean");
	body_lean.SetBoundaries(-180, 180);
	body_lean.SetValue(0);
	body_lean.extension = ("°");
	AntiAimGroup.PlaceLabledControl(1, "Body lean", this, &body_lean);

	inverted_body_lean.SetFileId("eternity_config_rage_AntiAimbody_inverted_body_lean");
	inverted_body_lean.SetBoundaries(-180, 180);
	inverted_body_lean.SetValue(0);
	inverted_body_lean.extension = ("°");
	AntiAimGroup.PlaceLabledControl(1, "Inverted body lean", this, &inverted_body_lean);

	edgeyaw.SetFileId("eternity_config_rage_edge_type");
	edgeyaw.AddItem("off");
	edgeyaw.AddItem("manual");
	edgeyaw.AddItem("freestanding");
	AntiAimGroup.PlaceLabledControl(1, "edge type", this, &edgeyaw);
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	AntiAimPitch2.SetFileId("eternity_config_rage_standing_pitch2");
	AntiAimPitch2.AddItem("off");
	AntiAimPitch2.AddItem("down");
	AntiAimPitch2.AddItem("up");
	AntiAimPitch2.AddItem("zero");
	AntiAimGroup.PlaceLabledControl(2, "pitch", this, &AntiAimPitch2);

	yawbase2.SetFileId("eternity_config_rage_standing_yawbase2");
	yawbase2.AddItem("static");
	yawbase2.AddItem("crosshair");
	yawbase2.AddItem("at targets");
	AntiAimGroup.PlaceLabledControl(2, "yaw base", this, &yawbase2);

	AntiAimYaw2.SetFileId("eternity_config_rage_moving_yaw2");
	AntiAimYaw2.AddItem("off");
	AntiAimYaw2.AddItem("sideways");
	AntiAimYaw2.AddItem("backwards");
	AntiAimYaw2.AddItem("spin");
	AntiAimYaw2.AddItem("jitter");
	AntiAimGroup.PlaceLabledControl(2, "yaw", this, &AntiAimYaw2);

	AntiAimYawoffset2.SetFileId("eternity_config_rage_AntiAimYawoffset2");
	AntiAimYawoffset2.SetBoundaries(-180, 180);
	AntiAimYawoffset2.SetValue(0);
	AntiAimYawoffset2.extension = ("°");
	AntiAimGroup.PlaceLabledControl(2, "offset", this, &AntiAimYawoffset2);

	jitteryawawoffset2.SetFileId("eternity_config_rage_AntiAimjitteryawawoffset2");
	jitteryawawoffset2.SetBoundaries(0, 180);
	jitteryawawoffset2.SetValue(0);
	jitteryawawoffset2.extension = ("°");
	AntiAimGroup.PlaceLabledControl(2, "jitter range", this, &jitteryawawoffset2);

	rbot_desyncstanding2.SetFileId("eternity_config_rage_standing_body_yaw2");
	rbot_desyncstanding2.AddItem("off");
	rbot_desyncstanding2.AddItem("static");
	rbot_desyncstanding2.AddItem("Jitter");
	rbot_desyncstanding2.AddItem("tank");
	AntiAimGroup.PlaceLabledControl(2, "body yaw", this, &rbot_desyncstanding2);

	rbot_desyncstandingkey2.SetFileId("eternity_config_rage_rbot_desyncstandingkey2");
	AntiAimGroup.PlaceLabledControl(2, (""), this, &rbot_desyncstandingkey2);

	fakeyawoffset2.SetFileId("eternity_config_rage_AntiAimfakeyawoffset2");
	fakeyawoffset2.SetBoundaries(0, 60);
	fakeyawoffset2.SetValue(0);
	fakeyawoffset2.extension = ("°");
	AntiAimGroup.PlaceLabledControl(2, "Desync limit", this, &fakeyawoffset2);

	body_lean2.SetFileId("eternity_config_rage_AntiAimbody_lean2");
	body_lean2.SetBoundaries(-180, 180);
	body_lean2.SetValue(0);
	body_lean2.extension = ("°");
	AntiAimGroup.PlaceLabledControl(2, "Body lean", this, &body_lean2);

	inverted_body_lean2.SetFileId("eternity_config_rage_AntiAimbody_inverted_body_lean2");
	inverted_body_lean2.SetBoundaries(-180, 180);
	inverted_body_lean2.SetValue(0);
	inverted_body_lean2.extension = ("°");
	AntiAimGroup.PlaceLabledControl(2, "Inverted body lean", this, &inverted_body_lean2);

	edgeyaw2.SetFileId("eternity_config_rage_edge_type2");
	edgeyaw2.AddItem("off");
	edgeyaw2.AddItem("manual");
	edgeyaw2.AddItem("freestanding");
	AntiAimGroup.PlaceLabledControl(2, "edge type", this, &edgeyaw2);
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	AntiAimPitch3.SetFileId("eternity_config_rage_standing_pitch3");
	AntiAimPitch3.AddItem("off");
	AntiAimPitch3.AddItem("down");
	AntiAimPitch3.AddItem("up");
	AntiAimPitch3.AddItem("zero");
	AntiAimGroup.PlaceLabledControl(3, "pitch", this, &AntiAimPitch3);

	yawbase3.SetFileId("eternity_config_rage_standing_yawbase3");
	yawbase3.AddItem("static");
	yawbase3.AddItem("crosshair");
	yawbase3.AddItem("at targets");
	AntiAimGroup.PlaceLabledControl(3, "yaw base", this, &yawbase3);

	AntiAimYaw3.SetFileId("eternity_config_rage_moving_yaw3");
	AntiAimYaw3.AddItem("off");
	AntiAimYaw3.AddItem("sideways");
	AntiAimYaw3.AddItem("backwards");
	AntiAimYaw3.AddItem("spin");
	AntiAimYaw3.AddItem("jitter");
	AntiAimGroup.PlaceLabledControl(3, "yaw", this, &AntiAimYaw3);

	AntiAimYawoffset3.SetFileId("eternity_config_rage_AntiAimYawoffset3");
	AntiAimYawoffset3.SetBoundaries(-180, 180);
	AntiAimYawoffset3.SetValue(0);
	AntiAimYawoffset3.extension = ("°");
	AntiAimGroup.PlaceLabledControl(3, "offset", this, &AntiAimYawoffset3);

	jitteryawawoffset3.SetFileId("eternity_config_rage_AntiAimjitteryawawoffset3");
	jitteryawawoffset3.SetBoundaries(0, 180);
	jitteryawawoffset3.SetValue(0);
	jitteryawawoffset3.extension = ("°");
	AntiAimGroup.PlaceLabledControl(3, "jitter range", this, &jitteryawawoffset3);

	rbot_desyncstanding3.SetFileId("eternity_config_rage_standing_body_yaw3");
	rbot_desyncstanding3.AddItem("off");
	rbot_desyncstanding3.AddItem("static");
	rbot_desyncstanding3.AddItem("Jitter");
	rbot_desyncstanding3.AddItem("tank");
	AntiAimGroup.PlaceLabledControl(3, "body yaw", this, &rbot_desyncstanding3);

	rbot_desyncstandingkey3.SetFileId("eternity_config_rage_rbot_desyncstandingkey3");
	AntiAimGroup.PlaceLabledControl(3, (""), this, &rbot_desyncstandingkey3);

	fakeyawoffset3.SetFileId("eternity_config_rage_AntiAimfakeyawoffset3");
	fakeyawoffset3.SetBoundaries(0, 60);
	fakeyawoffset3.SetValue(0);
	fakeyawoffset3.extension = ("°");
	AntiAimGroup.PlaceLabledControl(3, "Desync limit", this, &fakeyawoffset3);

	body_lean3.SetFileId("eternity_config_rage_AntiAimbody_lean3");
	body_lean3.SetBoundaries(-180, 180);
	body_lean3.SetValue(0);
	body_lean3.extension = ("°");
	AntiAimGroup.PlaceLabledControl(3, "Body lean", this, &body_lean3);

	inverted_body_lean3.SetFileId("eternity_config_rage_AntiAimbody_inverted_body_lean3");
	inverted_body_lean3.SetBoundaries(-180, 180);
	inverted_body_lean3.SetValue(0);
	inverted_body_lean3.extension = ("°");
	AntiAimGroup.PlaceLabledControl(3, "Inverted body lean", this, &inverted_body_lean3);

	edgeyaw3.SetFileId("eternity_config_rage_edge_type3");
	edgeyaw3.AddItem("off");
	edgeyaw3.AddItem("manual");
	edgeyaw3.AddItem("freestanding");
	AntiAimGroup.PlaceLabledControl(3, "edge type", this, &edgeyaw3);

	////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
	AntiAimPitch4.SetFileId("eternity_config_rage_standing_pitch4");
	AntiAimPitch4.AddItem("off");
	AntiAimPitch4.AddItem("down");
	AntiAimPitch4.AddItem("up");
	AntiAimPitch4.AddItem("zero");
	AntiAimGroup.PlaceLabledControl(4, "pitch", this, &AntiAimPitch4);

	yawbase4.SetFileId("eternity_config_rage_standing_yawbase4");
	yawbase4.AddItem("static");
	yawbase4.AddItem("crosshair");
	yawbase4.AddItem("at targets");
	AntiAimGroup.PlaceLabledControl(4, "yaw base", this, &yawbase4);

	AntiAimYaw4.SetFileId("eternity_config_rage_moving_yaw4");
	AntiAimYaw4.AddItem("off");
	AntiAimYaw4.AddItem("sideways");
	AntiAimYaw4.AddItem("backwards");
	AntiAimYaw4.AddItem("spin");
	AntiAimYaw4.AddItem("jitter");
	AntiAimGroup.PlaceLabledControl(4, "yaw", this, &AntiAimYaw4);

	AntiAimYawoffset4.SetFileId("eternity_config_rage_AntiAimYawoffset4");
	AntiAimYawoffset4.SetBoundaries(-180, 180);
	AntiAimYawoffset4.SetValue(0);
	AntiAimYawoffset4.extension = ("°");
	AntiAimGroup.PlaceLabledControl(4, "offset", this, &AntiAimYawoffset4);

	jitteryawawoffset4.SetFileId("eternity_config_rage_AntiAimjitteryawawoffset4");
	jitteryawawoffset4.SetBoundaries(0, 180);
	jitteryawawoffset4.SetValue(0);
	jitteryawawoffset4.extension = ("°");
	AntiAimGroup.PlaceLabledControl(4, "jitter range", this, &jitteryawawoffset4);

	rbot_desyncstanding4.SetFileId("eternity_config_rage_standing_body_yaw4");
	rbot_desyncstanding4.AddItem("off");
	rbot_desyncstanding4.AddItem("static");
	rbot_desyncstanding4.AddItem("Jitter");
	rbot_desyncstanding4.AddItem("tank");
	AntiAimGroup.PlaceLabledControl(4, "body yaw", this, &rbot_desyncstanding4);

	rbot_desyncstandingkey4.SetFileId("eternity_config_rage_rbot_desyncstandingkey4");
	AntiAimGroup.PlaceLabledControl(4, (""), this, &rbot_desyncstandingkey4);

	fakeyawoffset4.SetFileId("eternity_config_rage_AntiAimfakeyawoffset4");
	fakeyawoffset4.SetBoundaries(0, 60);
	fakeyawoffset4.SetValue(0);
	fakeyawoffset4.extension = ("°");
	AntiAimGroup.PlaceLabledControl(4, "Desync limit", this, &fakeyawoffset4);

	body_lean4.SetFileId("eternity_config_rage_AntiAimbody_lean4");
	body_lean4.SetBoundaries(-180, 180);
	body_lean4.SetValue(0);
	body_lean4.extension = ("°");
	AntiAimGroup.PlaceLabledControl(4, "Body lean", this, &body_lean4);

	inverted_body_lean4.SetFileId("eternity_config_rage_AntiAimbody_inverted_body_lean4");
	inverted_body_lean4.SetBoundaries(-180, 180);
	inverted_body_lean4.SetValue(0);
	inverted_body_lean4.extension = ("°");
	AntiAimGroup.PlaceLabledControl(4, "Inverted body lean", this, &inverted_body_lean4);

	edgeyaw4.SetFileId("eternity_config_rage_edge_type4");
	edgeyaw4.AddItem("off");
	edgeyaw4.AddItem("manual");
	edgeyaw4.AddItem("freestanding");
	AntiAimGroup.PlaceLabledControl(4, "edge type", this, &edgeyaw4);

	FakeLagOptions.SetText("Main");
	FakeLagOptions.SetPosition(245, 5);
	FakeLagOptions.SetSize(225, 269);
	RegisterControl(&FakeLagOptions);

	lag_bool.SetFileId("BAIMLESS_config_misc_lag_bool");
	FakeLagOptions.PlaceLabledControl(0, "Fake lag", this, &lag_bool);

	standing_lag.SetFileId("BAIMLESS_config_misc_standing_lag");
	standing_lag.SetBoundaries(0, 14);
	standing_lag.SetValue(4);
	FakeLagOptions.PlaceLabledControl(0, "Standing lag", this, &standing_lag);

	move_lag.SetFileId("BAIMLESS_config_misc_fakelag_value");
	move_lag.SetBoundaries(0, 14);
	move_lag.SetValue(4);
	FakeLagOptions.PlaceLabledControl(0, "Moving lag", this, &move_lag);

	air_lag.SetFileId("BAIMLESS_config_misc_air_lag");
	air_lag.SetBoundaries(0, 14);
	air_lag.SetValue(4);
	FakeLagOptions.PlaceLabledControl(0, "Air lag", this, &air_lag);

	airstuck.SetFileId("BAIMLESS_config_misc_airstuck");
	FakeLagOptions.PlaceLabledControl(0, "Air stuck", this, &airstuck);

	airstuckkey.SetFileId("BAIMLESS_config_rage_airstuckkey");
	FakeLagOptions.PlaceLabledControl(0, "", this, &airstuckkey);

	Doubel_tap.SetFileId("BAIMLESS_config_misc_Doubel_tap");
	FakeLagOptions.PlaceLabledControl(0, "Double tap", this, &Doubel_tap);

	delay_shot.SetFileId("BAIMLESS_config_misc_delay_shot");
	delay_shot.SetBoundaries(0, 30);
	delay_shot.SetValue(20);
	FakeLagOptions.PlaceLabledControl(0, "Delay shot", this, &delay_shot);

	state_delay.SetFileId("BAIMLESS_config_misc_state_delay");
	state_delay.SetBoundaries(0, 20);
	state_delay.SetValue(13);
	FakeLagOptions.PlaceLabledControl(0, "State delay", this, &state_delay);

	Double_key_check.SetFileId("BAIMLESS_config_misc_Double_key_check");
	FakeLagOptions.PlaceLabledControl(0, "Double key", this, &Double_key_check);

	Double_key.SetFileId("BAIMLESS_config_misc_key");
	FakeLagOptions.PlaceLabledControl(0, "", this, &Double_key);

	Double_keymethod.SetFileId("BAIMLESS_config_misc_Double_key_keymethod");
	Double_keymethod.AddItem("Set");
	Double_keymethod.AddItem("Hold");
	FakeLagOptions.PlaceLabledControl(0, "Double key method", this, &Double_keymethod);

	OtherGroup.SetText("Other");
	OtherGroup.SetPosition(245, 284);
	OtherGroup.SetSize(225, 298);
	RegisterControl(&OtherGroup);

	disable_aa.SetFileId("BAIMLESS_config_rage_disable_aa");
	disable_aa.items.push_back(dropdownboxitem(false, ("Grenade")));
	disable_aa.items.push_back(dropdownboxitem(false, ("Round start/end")));
	OtherGroup.PlaceLabledControl(4, "Disable aa", this, &disable_aa);


	fakewalkenabel.SetFileId("BAIMLESS_config_rage_fakewalkenabel");
	OtherGroup.PlaceLabledControl(4, "Slow Walk", this, &fakewalkenabel);

	fakewalkenabelkey.SetFileId("BAIMLESS_config_rage_fakewalkenabelkey");
	OtherGroup.PlaceLabledControl(4, "", this, &fakewalkenabelkey);

	FakeWalkSpeed.SetFileId("BAIMLESS_config_rage_fakewalk_speed");
	FakeWalkSpeed.SetBoundaries(1, 30);
	FakeWalkSpeed.SetValue(10);
	FakeWalkSpeed.extension = ("°");
	OtherGroup.PlaceLabledControl(4, "Slowwalk Speed", this, &FakeWalkSpeed);

	manualaakeyinfo.SetFileId("BAIMLESS_config_rage_manualaakeyinfo");
	OtherGroup.PlaceLabledControl(4, "Invert key", this, &manualaakeyinfo);

	manualaakey.SetFileId("BAIMLESS_config_rage_manual_key");
	OtherGroup.PlaceLabledControl(4, (""), this, &manualaakey);

	static_lby.SetFileId("BAIMLESS_config_rage_static_lby");
	OtherGroup.PlaceLabledControl(4, "Static lby", this, &static_lby);

	lbybreaker.SetFileId("BAIMLESS_config_rage_lbybreaker");
	OtherGroup.PlaceLabledControl(4, "Lby breaker", this, &lbybreaker);

	LbyAdd.SetFileId("BAIMLESS_config_misc_LbyAdd");
	LbyAdd.SetBoundaries(0, 360);
	LbyAdd.SetValue(0);
	OtherGroup.PlaceLabledControl(4, "Lby add", this, &LbyAdd);

	no_crouch_cooldown.SetFileId("BAIMLESS_config_rage_no_crouch_cooldown");
	OtherGroup.PlaceLabledControl(4, "Infinite duck", this, &no_crouch_cooldown);

	fake_crouch.SetFileId("BAIMLESS_config_rage_fake_crouch");
	OtherGroup.PlaceLabledControl(4, "Fake duck", this, &fake_crouch);

	fake_crouchkey.SetFileId("BAIMLESS_config_rage_fake_crouchkey");
	OtherGroup.PlaceLabledControl(4, "", this, &fake_crouchkey);

	bFixLegMovement.SetFileId("BAIMLESS_config_rage_bFixLegMovement");
	OtherGroup.PlaceLabledControl(4, "Fix leg movement", this, &bFixLegMovement);


}
*/
/*
void CLegitBotTab::Setup()
{
	SetTitle("B");

	AimbotGroup.SetPosition(10, 5);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.AddTab(CGroupTab("aim", 1));
	AimbotGroup.SetSize(225, 288);//+50
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("BAIMLESS_config_legitbot_enable");
	AimbotGroup.PlaceLabledControl(1, "Enable", this, &AimbotEnable);

	AimbotSmokeCheck.SetFileId("BAIMLESS_config_legitbot_smokecheck");
	AimbotGroup.PlaceLabledControl(1, "Smoke Check", this, &AimbotSmokeCheck);

	AimbotKeyPress.SetFileId("BAIMLESS_config_legitbot_usekey");
	AimbotGroup.PlaceLabledControl(1, "On Key", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("BAIMLESS_config_legitbot_aim_key");
	AimbotGroup.PlaceLabledControl(1, "", this, &AimbotKeyBind);

	specific_weapon.SetFileId("BAIMLESS_config_legitbot_specific_weapon");
	specific_weapon.AddItem("Group");
	specific_weapon.AddItem("Specifc");
	AimbotGroup.PlaceLabledControl(1, "Weapon choice", this, &specific_weapon);

#pragma endregion TriggerbotFilter stuff

#pragma region Main Weapon
	WeaponMainGroup.SetPosition(10, 305);
	WeaponMainGroup.SetText("weapon");
	WeaponMainGroup.AddTab(CGroupTab("W", 1));
	WeaponMainGroup.AddTab(CGroupTab("G", 2));
	WeaponMainGroup.AddTab(CGroupTab("Z", 3));
	WeaponMainGroup.AddTab(CGroupTab("L", 4));
	WeaponMainGroup.AddTab(CGroupTab("b", 5));
	WeaponMainGroup.AddTab(CGroupTab("f", 6));
	WeaponMainGroup.SetSize(458, 277);
	RegisterControl(&WeaponMainGroup);

	if (specific_weapon.GetIndex() == 0)
	{
		WeaponMainHitbox.SetFileId("BAIMLESS_config_legitbot_main_hitbox");
		WeaponMainHitbox.AddItem("Head");
		WeaponMainHitbox.AddItem("Neck");
		WeaponMainHitbox.AddItem("Chest");
		WeaponMainHitbox.AddItem("Stomach");
		WeaponMainHitbox.AddItem("Multihitbox");
		WeaponMainGroup.PlaceLabledControl(1, "Hitbox", this, &WeaponMainHitbox);

		WeaponMainSpeed.SetFileId("BAIMLESS_config_legitbot_main_speed");
		WeaponMainSpeed.SetBoundaries(0.f, 100.f);
		WeaponMainSpeed.SetValue(1.f);
		WeaponMainGroup.PlaceLabledControl(1, "Max Speed", this, &WeaponMainSpeed);

		WeaponMainFoV.SetFileId("BAIMLESS_config_legitbot_main_fov");
		WeaponMainFoV.SetBoundaries(0.f, 30.f);
		WeaponMainFoV.SetValue(3.5f);
		WeaponMainGroup.PlaceLabledControl(1, "Field of View", this, &WeaponMainFoV);

		WeaponMainInacc.SetFileId("BAIMLESS_config_legitbot_main_inacc");
		WeaponMainInacc.SetBoundaries(0.f, 20.f);
		WeaponMainGroup.PlaceLabledControl(1, "Humanize", this, &WeaponMainInacc);

		WeaponMaindmg.SetFileId("BAIMLESS_config_legitbot_rifle_mindamage");
		WeaponMaindmg.SetBoundaries(0, 100);
		WeaponMaindmg.SetValue(50);
		WeaponMaindmg.extension = ("°");
		WeaponMainGroup.PlaceLabledControl(1, "min dmg", this, &WeaponMaindmg);

		WeaponMainRecoil.SetFileId("BAIMLESS_config_legitbot_main_recoil");
		WeaponMainRecoil.SetBoundaries(0.00f, 2.00f);
		WeaponMainRecoil.SetValue(1.f);
		WeaponMainGroup.PlaceLabledControl(1, "rcs", this, &WeaponMainRecoil);
	}
	else
	{
		WeaponMainHitbox.SetFileId("BAIMLESS_config_legitbot_main_hitbox");
		WeaponMainHitbox.AddItem("Head");
		WeaponMainHitbox.AddItem("Neck");
		WeaponMainHitbox.AddItem("Chest");
		WeaponMainHitbox.AddItem("Stomach");
		WeaponMainHitbox.AddItem("Multihitbox");
		WeaponMainGroup.PlaceLabledControl(1, "Hitbox", this, &WeaponMainHitbox);
	}




	WeaponPistHitbox.SetFileId("BAIMLESS_config_legitbot_pist_hitbox");
	WeaponPistHitbox.AddItem("Head");
	WeaponPistHitbox.AddItem("Neck");
	WeaponPistHitbox.AddItem("Chest");
	WeaponPistHitbox.AddItem("Stomach");
	WeaponPistHitbox.AddItem("Multihitbox");
	WeaponMainGroup.PlaceLabledControl(2, "Hitbox", this, &WeaponPistHitbox);

	WeaponPistSpeed.SetFileId("BAIMLESS_config_legitbot_pist_speed");
	WeaponPistSpeed.SetBoundaries(0.f, 100.f);
	WeaponPistSpeed.SetValue(1.0f);
	WeaponMainGroup.PlaceLabledControl(2, "Max Speed", this, &WeaponPistSpeed);

	WeaponPistFoV.SetFileId("BAIMLESS_config_legitbot_pist_fov");
	WeaponPistFoV.SetBoundaries(0.f, 30.f);
	WeaponPistFoV.SetValue(3.f);
	WeaponMainGroup.PlaceLabledControl(2, "Field of View", this, &WeaponPistFoV);

	WeaponPistInacc.SetFileId("BAIMLESS_config_legitbot_pist_inacc");
	WeaponPistInacc.SetBoundaries(0.f, 20.f);
	WeaponMainGroup.PlaceLabledControl(2, "Humanize", this, &WeaponPistInacc);

	WeaponPistdmg.SetFileId("BAIMLESS_config_legitbot_pistol_WeaponPistdmg");
	WeaponPistdmg.SetBoundaries(0, 100);
	WeaponPistdmg.SetValue(50);
	WeaponPistdmg.extension = ("°");
	WeaponMainGroup.PlaceLabledControl(2, "min dmg", this, &WeaponPistdmg);

	WeaponPistRecoil.SetFileId("BAIMLESS_config_legitbot_pist_recoil");
	WeaponPistRecoil.SetBoundaries(0.00f, 2.00f);
	WeaponPistRecoil.SetValue(1.f);
	WeaponMainGroup.PlaceLabledControl(2, "rcs", this, &WeaponPistRecoil);



	WeaponSnipHitbox.SetFileId("BAIMLESS_config_legitbot_snip_hitbox");
	WeaponSnipHitbox.AddItem("Head");
	WeaponSnipHitbox.AddItem("Neck");
	WeaponSnipHitbox.AddItem("Chest");
	WeaponSnipHitbox.AddItem("Stomach");
	WeaponSnipHitbox.AddItem("Multihitbox");
	WeaponMainGroup.PlaceLabledControl(3, "Hitbox", this, &WeaponSnipHitbox);

	WeaponSnipSpeed.SetFileId("BAIMLESS_config_legitbot_snip_speed");
	WeaponSnipSpeed.SetBoundaries(0.f, 100.f);
	WeaponSnipSpeed.SetValue(1.5f);
	WeaponMainGroup.PlaceLabledControl(3, "Max Speed", this, &WeaponSnipSpeed);

	WeaponSnipFoV.SetFileId("BAIMLESS_config_legitbot_snip_fov");
	WeaponSnipFoV.SetBoundaries(0.f, 30.f);
	WeaponSnipFoV.SetValue(2.f);
	WeaponMainGroup.PlaceLabledControl(3, "Field of View", this, &WeaponSnipFoV);

	WeaponSnipInacc.SetFileId("BAIMLESS_config_legitbot_snip_inacc");
	WeaponSnipInacc.SetBoundaries(0.f, 20.f);
	WeaponMainGroup.PlaceLabledControl(3, "Humanize", this, &WeaponSnipInacc);

	WeaponSnipdmg.SetFileId("BAIMLESS_config_legitbot_sniper_WeaponSnipdmg");
	WeaponSnipdmg.SetBoundaries(0, 100);
	WeaponSnipdmg.SetValue(50);
	WeaponSnipdmg.extension = ("°");
	WeaponMainGroup.PlaceLabledControl(3, "min dmg", this, &WeaponSnipdmg);

	WeaponSnipRecoil.SetFileId("BAIMLESS_config_legitbot_snip_recoil");
	WeaponSnipRecoil.SetBoundaries(0.00f, 2.00f);
	WeaponSnipRecoil.SetValue(1.f);
	WeaponMainGroup.PlaceLabledControl(3, "rcs", this, &WeaponSnipRecoil);

	WeaponMpHitbox.SetFileId("BAIMLESS_config_legitbot_mps_hitbox");
	WeaponMpHitbox.AddItem("Head");
	WeaponMpHitbox.AddItem("Neck");
	WeaponMpHitbox.AddItem("Chest");
	WeaponMpHitbox.AddItem("Stomach");
	WeaponMpHitbox.AddItem("Multihitbox");
	WeaponMainGroup.PlaceLabledControl(4, "Hitbox", this, &WeaponMpHitbox);

	WeaponMpSpeed.SetFileId("BAIMLESS_config_legitbot_mps_speed");
	WeaponMpSpeed.SetBoundaries(0.f, 100.f);
	WeaponMpSpeed.SetValue(1.0f);
	WeaponMainGroup.PlaceLabledControl(4, "Max Speed", this, &WeaponMpSpeed);

	WeaponMpFoV.SetFileId("BAIMLESS_config_legitbot_mps_fov");
	WeaponMpFoV.SetBoundaries(0.f, 30.f);
	WeaponMpFoV.SetValue(4.f);
	WeaponMainGroup.PlaceLabledControl(4, "Field of View", this, &WeaponMpFoV);

	WeaponMpInacc.SetFileId("BAIMLESS_config_legitbot_mps_inacc");
	WeaponMpInacc.SetBoundaries(0.f, 20.f);
	WeaponMainGroup.PlaceLabledControl(4, "Humanize", this, &WeaponMpInacc);

	WeaponMpdmg.SetFileId("BAIMLESS_config_legitbot_mp_WeaponMpdmg");
	WeaponMpdmg.SetBoundaries(0, 100);
	WeaponMpdmg.SetValue(50);
	WeaponMpdmg.extension = ("°");
	WeaponMainGroup.PlaceLabledControl(4, "min dmg", this, &WeaponMpdmg);

	WeaponMpRecoil.SetFileId("BAIMLESS_config_legitbot_mps_recoil");
	WeaponMpRecoil.SetBoundaries(0.00f, 2.00f);
	WeaponMpRecoil.SetValue(1.f);
	WeaponMainGroup.PlaceLabledControl(4, "rcs", this, &WeaponMpRecoil);



	WeaponShotgunHitbox.SetFileId("BAIMLESS_config_legitbot_shotgun_hitbox");
	WeaponShotgunHitbox.AddItem("Head");
	WeaponShotgunHitbox.AddItem("Neck");
	WeaponShotgunHitbox.AddItem("Chest");
	WeaponShotgunHitbox.AddItem("Stomach");
	WeaponShotgunHitbox.AddItem("Nearest");
	WeaponMainGroup.PlaceLabledControl(5, "Hitbox", this, &WeaponShotgunHitbox);

	WeaponShotgunSpeed.SetFileId("BAIMLESS_config_legitbot_shotgun_speed");
	WeaponShotgunSpeed.SetBoundaries(0.f, 100.f);
	WeaponShotgunSpeed.SetValue(1.0f);
	WeaponMainGroup.PlaceLabledControl(5, "Max Speed", this, &WeaponShotgunSpeed);

	WeaponShotgunFoV.SetFileId("BAIMLESS_config_legitbot_shotgun_fov");
	WeaponShotgunFoV.SetBoundaries(0.f, 30.f);
	WeaponShotgunFoV.SetValue(3.f);
	WeaponMainGroup.PlaceLabledControl(5, "Field of View", this, &WeaponShotgunFoV);

	WeaponShotgunInacc.SetFileId("BAIMLESS_config_legitbot_mps_inacc");
	WeaponShotgunInacc.SetBoundaries(0.f, 20.f);
	WeaponMainGroup.PlaceLabledControl(5, "Humanize", this, &WeaponShotgunInacc);

	WeaponShotgundmg.SetFileId("BAIMLESS_config_legitbot_shotgun_Weaponshotgundmg");
	WeaponShotgundmg.SetBoundaries(0, 100);
	WeaponShotgundmg.SetValue(50);
	WeaponShotgundmg.extension = ("°");
	WeaponMainGroup.PlaceLabledControl(5, "min dmg", this, &WeaponShotgundmg);

	WeaponShotgunRecoil.SetFileId("BAIMLESS_config_legitbot_snip_recoil");
	WeaponShotgunRecoil.SetBoundaries(0.00f, 2.00f);
	WeaponShotgunRecoil.SetValue(1.f);
	WeaponMainGroup.PlaceLabledControl(5, "rcs", this, &WeaponShotgunRecoil);


	WeaponMGHitbox.SetFileId("BAIMLESS_config_legitbot_mg_hitbox");
	WeaponMGHitbox.AddItem("Head");
	WeaponMGHitbox.AddItem("Neck");
	WeaponMGHitbox.AddItem("Chest");
	WeaponMGHitbox.AddItem("Stomach");
	WeaponMGHitbox.AddItem("Nearest");
	WeaponMainGroup.PlaceLabledControl(6, "Hitbox", this, &WeaponMGHitbox);

	WeaponMGSpeed.SetFileId("BAIMLESS_config_legitbot_mg_speed");
	WeaponMGSpeed.SetBoundaries(0.f, 100.f);
	WeaponMGSpeed.SetValue(1.0f);
	WeaponMainGroup.PlaceLabledControl(6, "Max Speed", this, &WeaponMGSpeed);

	WeaponMGFoV.SetFileId("BAIMLESS_config_legitbot_mg_fov");
	WeaponMGFoV.SetBoundaries(0.f, 30.f);
	WeaponMGFoV.SetValue(4.f);
	WeaponMainGroup.PlaceLabledControl(6, "Field of View", this, &WeaponMGFoV);

	WeaponMGInacc.SetFileId("BAIMLESS_config_legitbot_mg_inacc");
	WeaponMGInacc.SetBoundaries(0.f, 20.f);
	WeaponMainGroup.PlaceLabledControl(6, "Humanize", this, &WeaponMGInacc);

	Weaponheavydmg.SetFileId("BAIMLESS_config_legitbot_heavy_Weaponheavydmg");
	Weaponheavydmg.SetBoundaries(0, 100);
	Weaponheavydmg.SetValue(50);
	Weaponheavydmg.extension = ("°");
	WeaponMainGroup.PlaceLabledControl(6, "min dmg", this, &Weaponheavydmg);

	WeaponMGRecoil.SetFileId("BAIMLESS_config_legitbot_mg_recoil");
	WeaponMGRecoil.SetBoundaries(0.00f, 2.00f);
	WeaponMGRecoil.SetValue(1.f);
	WeaponMainGroup.PlaceLabledControl(6, "rcs", this, &WeaponMGRecoil);



	Other.SetPosition(245, 5);
	Other.SetText("extra");
	Other.AddTab(CGroupTab("Main", 1));
	Other.AddTab(CGroupTab("Trigger", 2));
	Other.SetSize(225, 288);
	RegisterControl(&Other);



	AimbotBacktrack.SetFileId("BAIMLESS_config_legitbot_legit_backtrack");
	Other.PlaceLabledControl(1, "Backtrack", this, &AimbotBacktrack);

	StandAloneRCS.SetFileId("standalonercs");
	Other.PlaceLabledControl(1, "Standalone rcs", this, &StandAloneRCS);

	StandAloneRCSamount.SetFileId("standalonercsamount");
	StandAloneRCSamount.SetBoundaries(0.0f, 2.0f);
	Other.PlaceLabledControl(1, "Standalone rcs amount", this, &StandAloneRCSamount);

	visualizefov.SetFileId("visualizefov");
	Other.PlaceLabledControl(1, "Visualize fov", this, &visualizefov);

	visualizefov_col.SetFileId(("BAIMLESS_config_legit_visualizefov_col"));
	visualizefov_col.SetColor(40, 0, 220, 255);
	Other.PlaceLabledControl(1, (""), this, &visualizefov_col);

	Legit_AA.SetFileId("Legit_AA");
	Other.PlaceLabledControl(1, "Legit AA", this, &Legit_AA);

	Legit_AA_key.SetFileId("BAIMLESS_config_rage_legitbot_Legit_AA_key");
	Other.PlaceLabledControl(1, (""), this, &Legit_AA_key);

	manual_Legit_AA.SetFileId("BAIMLESS_config_rage_legitbot_manual_Legit_AA");
	Other.PlaceLabledControl(1, "Legit aa indicator", this, &manual_Legit_AA);

	Legit_AA_col.SetFileId(("BAIMLESS_config_legit_Legit_AA_col"));
	Legit_AA_col.SetColor(200, 0, 0, 255);
	Other.PlaceLabledControl(1, (""), this, &Legit_AA_col);

	Legit_AA_visualize.SetFileId("BAIMLESS_config_legit_Legit_AA_visualize");
	Other.PlaceLabledControl(1, "Legit aa visualize", this, &Legit_AA_visualize);

	Legit_AA_visualize_col.SetFileId(("BAIMLESS_config_Legit_AA_visualize_col"));
	Legit_AA_visualize_col.SetColor(90, 200, 0, 255);
	Other.PlaceLabledControl(1, (""), this, &Legit_AA_visualize_col);

	Legit_AA_visualize_material.SetFileId("BAIMLESS_config_Legit_AA_visualize_material");
	Legit_AA_visualize_material.AddItem("normal");
	Legit_AA_visualize_material.AddItem("reflective");
	Legit_AA_visualize_material.AddItem("bright reflective");
	Legit_AA_visualize_material.AddItem("flat");
	Legit_AA_visualize_material.AddItem("dark");
	Legit_AA_visualize_material.AddItem("pulse");
	Other.PlaceLabledControl(1, "Visualize material", this, &Legit_AA_visualize_material);

	Trigger_bot.SetFileId("BAIMLESS_config_legitbot_legit_Trigger_bot");
	Other.PlaceLabledControl(2, "Trigger bot", this, &Trigger_bot);

	Trigger_bot_key.SetFileId("BAIMLESS_config_legitbot_legit_Trigger_bot_key");
	Other.PlaceLabledControl(2, "Trigger key", this, &Trigger_bot_key);

	Trigger_key.SetFileId("BAIMLESS_config_legitbot_legit_Trigger_key");
	Other.PlaceLabledControl(2, "", this, &Trigger_key);

	Trigger_bot_keymethod.SetFileId("BAIMLESS_config_legitbot_legit_Trigger_bot_keymethod");
	Trigger_bot_keymethod.AddItem("Set");
	Trigger_bot_keymethod.AddItem("Hold");
	Other.PlaceLabledControl(2, "Trigger method", this, &Trigger_bot_keymethod);

	Trigger_bot_deathmatch.SetFileId("BAIMLESS_config_legitbot_legit_Trigger_bot_deathmatch");
	Other.PlaceLabledControl(2, "Trigger Deathmatch", this, &Trigger_bot_deathmatch);

	Trigger_bot_smoke_check.SetFileId("BAIMLESS_config_legitbot_legit_Trigger_bot_smoke_check");
	Other.PlaceLabledControl(2, "Trigger Smoke Check", this, &Trigger_bot_smoke_check);

	Trigger_Hitbox.SetFileId("BAIMLESS_config_legit_Trigger_bot_HitScan");
	Trigger_Hitbox.items.push_back(dropdownboxitem(false, ("head")));
	Trigger_Hitbox.items.push_back(dropdownboxitem(false, ("chest")));
	Trigger_Hitbox.items.push_back(dropdownboxitem(false, ("stomach")));
	Trigger_Hitbox.items.push_back(dropdownboxitem(false, ("arms")));
	Trigger_Hitbox.items.push_back(dropdownboxitem(false, ("legs")));
	Other.PlaceLabledControl(2, "Trigger Hitscan", this, &Trigger_Hitbox);

	Trigger_min_damage.SetFileId("BAIMLESS_config_legitbot_Trigger_min_damage");
	Trigger_min_damage.SetBoundaries(0, 100);
	Trigger_min_damage.SetValue(30);
	Trigger_min_damage.extension = ("°");
	Other.PlaceLabledControl(2, "Trigger min damage", this, &Trigger_min_damage);

	Trigger_chance.SetFileId("BAIMLESS_config_legitbot_Trigger_chance");
	Trigger_chance.SetBoundaries(0, 100);
	Trigger_chance.SetValue(40);
	Trigger_chance.extension = ("%%");
	Other.PlaceLabledControl(2, "Trigger hitchance", this, &Trigger_chance);

	Trigger_delay.SetFileId("BAIMLESS_config_legitbot_Trigger_delay");
	Trigger_delay.SetBoundaries(0, 100);
	Trigger_delay.SetValue(30);
	Trigger_delay.extension = ("°");
	Other.PlaceLabledControl(2, "Trigger delay", this, &Trigger_delay);
}

*//*
void CVisualTab::Setup()
{

	SetTitle("C");

#pragma region Options
	OptionsGroup.SetText("Player ESP");
	OptionsGroup.SetPosition(10, 5);
	OptionsGroup.SetSize(225, 340);
	OptionsGroup.AddTab(CGroupTab("Enemy", 1));
	OptionsGroup.AddTab(CGroupTab("Team", 2));
	OptionsGroup.AddTab(CGroupTab("Local", 3));
	RegisterControl(&OptionsGroup);

	esp_bool.SetFileId("BAIMLESS_config_visual_esp_bool");
	OptionsGroup.PlaceLabledControl(1, "Enable", this, &esp_bool);

	esp_player_boxes.SetFileId("BAIMLESS_config_visual_player_boxes");
	OptionsGroup.PlaceLabledControl(1, "Box", this, &esp_player_boxes);

	esp_player_boxes_col.SetFileId(("BAIMLESS_config_visual_boxes_col"));
	esp_player_boxes_col.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(1, (""), this, &esp_player_boxes_col);

	esp_player_boxes_type.SetFileId("BAIMLESS_config_visual_esp_player_box_type");
	esp_player_boxes_type.AddItem("Normal");
	esp_player_boxes_type.AddItem("Corner");
	OptionsGroup.PlaceLabledControl(1, "", this, &esp_player_boxes_type);

	esp_player_names.SetFileId("BAIMLESS_config_visual_player_names");
	OptionsGroup.PlaceLabledControl(1, "Name", this, &esp_player_names);

	esp_player_names_col.SetFileId(("BAIMLESS_config_visual_player_names_col"));
	esp_player_names_col.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(1, (""), this, &esp_player_names_col);

	esp_player_health.SetFileId("BAIMLESS_config_visual_player_health");
	OptionsGroup.PlaceLabledControl(1, "Health", this, &esp_player_health);

	esp_player_health_type.SetFileId("BAIMLESS_config_visual_esp_player_health_type");
	esp_player_health_type.AddItem("Normal");
	esp_player_health_type.AddItem("Battery");
	esp_player_health_type.AddItem("Text");
	OptionsGroup.PlaceLabledControl(1, "", this, &esp_player_health_type);

	skeleton.SetFileId("BAIMLESS_config_visual_skeleton");
	skeleton.items.push_back(dropdownboxitem(false, ("Normal")));
	skeleton.items.push_back(dropdownboxitem(false, ("Backtrack")));
	OptionsGroup.PlaceLabledControl(1, "Skeleton", this, &skeleton);

	skeleton_col.SetFileId(("BAIMLESS_config_visual_local_skeleton_col"));
	skeleton_col.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(1, (""), this, &skeleton_col);

	esp_ammo.SetFileId("BAIMLESS_config_visual_ammo");
	OptionsGroup.PlaceLabledControl(1, "Ammo bar", this, &esp_ammo);

	esp_player_ammo_col.SetFileId(("BAIMLESS_config_visual_ammo_col"));
	esp_player_ammo_col.SetColor(100, 160, 220, 255);
	OptionsGroup.PlaceLabledControl(1, (""), this, &esp_player_ammo_col);

	esp_weapon.SetFileId("BAIMLESS_config_visual_weapon");
	OptionsGroup.PlaceLabledControl(1, "Weapon", this, &esp_weapon);

	esp_esp_weapon_col.SetFileId(("BAIMLESS_config_visual_weapon_col"));
	esp_esp_weapon_col.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(1, (""), this, &esp_esp_weapon_col);

	WeaponArray.SetFileId("BAIMLESS_config_visual_weapon_type");
	WeaponArray.AddItem("Text");
	WeaponArray.AddItem("Icon"); 
	OptionsGroup.PlaceLabledControl(1, "", this, &WeaponArray);

	money_info.SetFileId("BAIMLESS_config_visual_money_info");
	OptionsGroup.PlaceLabledControl(1, "Money info", this, &money_info);

	money_info_col.SetFileId(("BAIMLESS_config_visual_money_info_col"));
	money_info_col.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(1, (""), this, &money_info_col);

	Helmet_info.SetFileId("BAIMLESS_config_visual_Helmet_info");
	OptionsGroup.PlaceLabledControl(1, "Helmet info", this, &Helmet_info);

	Helmet_info_col.SetFileId(("BAIMLESS_config_visual_Helmet_info_col"));
	Helmet_info_col.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(1, (""), this, &Helmet_info_col);

	Scoped_info.SetFileId("BAIMLESS_config_visual_Scoped_info");
	OptionsGroup.PlaceLabledControl(1, "Scoped info", this, &Scoped_info);

	Scoped_info_col.SetFileId(("BAIMLESS_config_visual_Scoped_info_col"));
	Scoped_info_col.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(1, (""), this, &Scoped_info_col);

	Fakeduck_info.SetFileId("BAIMLESS_config_visual_Fakeduck_info");
	OptionsGroup.PlaceLabledControl(1, "Fakeduck info", this, &Fakeduck_info);

	Fakeduck_info_col.SetFileId(("BAIMLESS_config_visual_Fakeduck_info_col"));
	Fakeduck_info_col.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(1, (""), this, &Fakeduck_info_col);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////

	esp_player_boxes_team.SetFileId("BAIMLESS_config_visual_player_boxes_team");
	OptionsGroup.PlaceLabledControl(2, "Box", this, &esp_player_boxes_team);

	esp_player_boxes_col_team.SetFileId(("BAIMLESS_config_visual_boxes_col_team"));
	esp_player_boxes_col_team.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(2, (""), this, &esp_player_boxes_col_team);

	esp_player_boxes_type_team.SetFileId("BAIMLESS_config_visual_esp_player_box_type_team");
	esp_player_boxes_type_team.AddItem("Normal");
	esp_player_boxes_type_team.AddItem("Corner");
	OptionsGroup.PlaceLabledControl(2, "", this, &esp_player_boxes_type_team);

	esp_player_names_team.SetFileId("BAIMLESS_config_visual_player_names_team");
	OptionsGroup.PlaceLabledControl(2, "Name", this, &esp_player_names_team);

	esp_player_names_col_team.SetFileId(("BAIMLESS_config_visual_player_names_col_team"));
	esp_player_names_col_team.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(2, (""), this, &esp_player_names_col_team);

	esp_player_health_team.SetFileId("BAIMLESS_config_visual_player_health_team");
	OptionsGroup.PlaceLabledControl(2, "Health", this, &esp_player_health_team);

	esp_player_health_type_team.SetFileId("BAIMLESS_config_visual_esp_player_health_type_team");
	esp_player_health_type_team.AddItem("Normal");
	esp_player_health_type_team.AddItem("Battery");
	esp_player_health_type_team.AddItem("Text");
	OptionsGroup.PlaceLabledControl(2, "", this, &esp_player_health_type_team);

	skeleton_team.SetFileId("BAIMLESS_config_visual_skeleton_team");
	skeleton_team.items.push_back(dropdownboxitem(false, ("Normal")));
	skeleton_team.items.push_back(dropdownboxitem(false, ("Backtrack")));
	OptionsGroup.PlaceLabledControl(2, "Skeleton", this, &skeleton_team);

	skeleton_col_team.SetFileId(("BAIMLESS_config_visual_local_skeleton_col_team"));
	skeleton_col_team.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(2, (""), this, &skeleton_col_team);

	esp_ammo_team.SetFileId("BAIMLESS_config_visual_esp_ammo_team");
	OptionsGroup.PlaceLabledControl(2, "Ammo bar", this, &esp_ammo_team);

	esp_player_ammo_col_team.SetFileId(("BAIMLESS_config_visual_ammo_col_team"));
	esp_player_ammo_col_team.SetColor(100, 160, 220, 255);
	OptionsGroup.PlaceLabledControl(2, (""), this, &esp_player_ammo_col_team);

	esp_weapon_team.SetFileId("BAIMLESS_config_visual_esp_weapon_team");
	OptionsGroup.PlaceLabledControl(2, "Weapon", this, &esp_weapon_team);

	esp_esp_weapon_col_team.SetFileId(("BAIMLESS_config_visual_esp_esp_weapon_col_team"));
	esp_esp_weapon_col_team.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(2, (""), this, &esp_esp_weapon_col_team);

	WeaponArray_team.SetFileId("BAIMLESS_config_visual_WeaponArray_team");
	WeaponArray_team.AddItem("Text");
	WeaponArray_team.AddItem("Icon");
	OptionsGroup.PlaceLabledControl(2, "", this, &WeaponArray_team);

	money_info_team.SetFileId("BAIMLESS_config_visual_money_info_team");
	OptionsGroup.PlaceLabledControl(2, "Money info", this, &money_info_team);

	money_info_col_team.SetFileId(("BAIMLESS_config_visual_money_info_col_team"));
	money_info_col_team.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(2, (""), this, &money_info_col_team);

	Helmet_info_team.SetFileId("BAIMLESS_config_visual_Helmet_info_team");
	OptionsGroup.PlaceLabledControl(2, "Helmet info", this, &Helmet_info_team);

	Helmet_info_col_team.SetFileId(("BAIMLESS_config_visual_Helmet_info_col_team"));
	Helmet_info_col_team.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(2, (""), this, &Helmet_info_col_team);

	Scoped_info_team.SetFileId("BAIMLESS_config_visual_Scoped_info_team");
	OptionsGroup.PlaceLabledControl(2, "Scoped info", this, &Scoped_info_team);

	Scoped_info_col_team.SetFileId(("BAIMLESS_config_visual_Scoped_info_col_team"));
	Scoped_info_col_team.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(2, (""), this, &Scoped_info_col_team);

	Fakeduck_info_team.SetFileId("BAIMLESS_config_visual_Fakeduck_info_team");
	OptionsGroup.PlaceLabledControl(2, "Fakeduck info", this, &Fakeduck_info_team);

	Fakeduck_info_col_team.SetFileId(("BAIMLESS_config_visual_Fakeduck_info_col_team"));
	Fakeduck_info_col_team.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(2, (""), this, &Fakeduck_info_col_team);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////

	esp_player_boxes_Local.SetFileId("BAIMLESS_config_visual_player_boxes_Local");
	OptionsGroup.PlaceLabledControl(3, "Box", this, &esp_player_boxes_Local);

	esp_player_boxes_col_Local.SetFileId(("BAIMLESS_config_visual_boxes_col_Local"));
	esp_player_boxes_col_Local.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(3, (""), this, &esp_player_boxes_col_Local);

	esp_player_boxes_type_Local.SetFileId("BAIMLESS_config_visual_esp_player_box_type_Local");
	esp_player_boxes_type_Local.AddItem("Normal");
	esp_player_boxes_type_Local.AddItem("Corner");
	OptionsGroup.PlaceLabledControl(3, "", this, &esp_player_boxes_type_Local);

	esp_player_names_Local.SetFileId("BAIMLESS_config_visual_player_names_Local");
	OptionsGroup.PlaceLabledControl(3, "Name", this, &esp_player_names_Local);

	esp_player_names_col_Local.SetFileId(("BAIMLESS_config_visual_player_names_col_Local"));
	esp_player_names_col_Local.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(3, (""), this, &esp_player_names_col_Local);

	esp_player_health_Local.SetFileId("BAIMLESS_config_visual_player_health_Local");
	OptionsGroup.PlaceLabledControl(3, "Health", this, &esp_player_health_Local);

	esp_player_health_type_Local.SetFileId("BAIMLESS_config_visual_esp_player_health_type_Local");
	esp_player_health_type_Local.AddItem("Normal");
	esp_player_health_type_Local.AddItem("Battery");
	esp_player_health_type_Local.AddItem("Text");
	OptionsGroup.PlaceLabledControl(3, "", this, &esp_player_health_type_Local);

	skeleton_Local.SetFileId("BAIMLESS_config_visual_skeleton_Local");
	skeleton_Local.items.push_back(dropdownboxitem(false, ("Normal")));
	skeleton_Local.items.push_back(dropdownboxitem(false, ("Backtrack")));
	OptionsGroup.PlaceLabledControl(3, "Skeleton", this, &skeleton_Local);

	skeleton_col_Local.SetFileId(("BAIMLESS_config_visual_local_skeleton_col_Local"));
	skeleton_col_Local.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(3, (""), this, &skeleton_col_Local);

	esp_ammo_Local.SetFileId("BAIMLESS_config_visual_esp_ammo_Local");
	OptionsGroup.PlaceLabledControl(3, "Ammo bar", this, &esp_ammo_Local);

	esp_player_ammo_col_Local.SetFileId(("BAIMLESS_config_visual_ammo_col_Local"));
	esp_player_ammo_col_Local.SetColor(100, 160, 220, 255);
	OptionsGroup.PlaceLabledControl(3, (""), this, &esp_player_ammo_col_Local);

	esp_weapon_Local.SetFileId("BAIMLESS_config_visual_esp_weapon_Local");
	OptionsGroup.PlaceLabledControl(3, "Weapon", this, &esp_weapon_Local);

	esp_esp_weapon_col_Local.SetFileId(("BAIMLESS_config_visual_esp_esp_weapon_col_Local"));
	esp_esp_weapon_col_Local.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(3, (""), this, &esp_esp_weapon_col_Local);

	WeaponArray_Local.SetFileId("BAIMLESS_config_visual_WeaponArray_Local");
	WeaponArray_Local.AddItem("Text");
	WeaponArray_Local.AddItem("Icon");
	OptionsGroup.PlaceLabledControl(3, "", this, &WeaponArray_Local);

	money_info_local.SetFileId("BAIMLESS_config_visual_money_info_local");
	OptionsGroup.PlaceLabledControl(3, "Money info", this, &money_info_local);

	money_info_col_local.SetFileId(("BAIMLESS_config_visual_money_info_col_local"));
	money_info_col_local.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(3, (""), this, &money_info_col_local);

	Helmet_info_local.SetFileId("BAIMLESS_config_visual_Helmet_info_local");
	OptionsGroup.PlaceLabledControl(3, "Helmet info", this, &Helmet_info_local);

	Helmet_info_col_local.SetFileId(("BAIMLESS_config_visual_Helmet_info_col_local"));
	Helmet_info_col_local.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(3, (""), this, &Helmet_info_col_local);

	Scoped_info_local.SetFileId("BAIMLESS_config_visual_Scoped_info_local");
	OptionsGroup.PlaceLabledControl(3, "Scoped info", this, &Scoped_info_local);

	Scoped_info_col_local.SetFileId(("BAIMLESS_config_visual_Scoped_info_col_local"));
	Scoped_info_col_local.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(3, (""), this, &Scoped_info_col_local);

	Fakeduck_info_local.SetFileId("BAIMLESS_config_visual_Fakeduck_info_local");
	OptionsGroup.PlaceLabledControl(3, "Fakeduck info", this, &Fakeduck_info_local);

	Fakeduck_info_col_local.SetFileId(("BAIMLESS_config_visual_Fakeduck_info_col_local"));
	Fakeduck_info_col_local.SetColor(250, 250, 250, 255);
	OptionsGroup.PlaceLabledControl(3, (""), this, &Fakeduck_info_col_local);

	ChamsOptions.SetText("Colored models");
	ChamsOptions.SetPosition(10, 355);
	ChamsOptions.SetSize(225, 227);
	ChamsOptions.AddTab(CGroupTab("Enemy", 1));
	ChamsOptions.AddTab(CGroupTab("Team", 2));
	ChamsOptions.AddTab(CGroupTab("Local", 3));
	ChamsOptions.AddTab(CGroupTab("Other", 4));
	RegisterControl(&ChamsOptions);
    /////////////////////////////////////////////////////////
	chamsmaterial.SetFileId("BAIMLESS_config_visual_chams_material");
	chamsmaterial.AddItem("normal");
	chamsmaterial.AddItem("reflective");
	chamsmaterial.AddItem("bright reflective");
	chamsmaterial.AddItem("flat");
	chamsmaterial.AddItem("dark");
	chamsmaterial.AddItem("pulse");
	chamsmaterial.AddItem("glow");
	chamsmaterial.AddItem("dark glow");
	ChamsOptions.PlaceLabledControl(1, "Colored models material", this, &chamsmaterial);

	vis_chams_enemy_visible.SetFileId("BAIMLESS_config_visual_vis_chams_enemy_visible");
	ChamsOptions.PlaceLabledControl(1, "Player", this, &vis_chams_enemy_visible);

	vis_chams_enemy_visible_col.SetFileId(("BAIMLESS_config_visual_vis_chams_enemy_visible_col"));
	vis_chams_enemy_visible_col.SetColor(170, 220, 80, 255);
	ChamsOptions.PlaceLabledControl(1, (""), this, &vis_chams_enemy_visible_col);

	vis_chams_enemy_invisible.SetFileId("BAIMLESS_config_visual_vis_chams_enemy_invisible");
	ChamsOptions.PlaceLabledControl(1, "Player behind wall", this, &vis_chams_enemy_invisible);

	acc_vis_chams_enemy_invisible_col.SetFileId(("BAIMLESS_config_visual_acc_vis_chams_enemy_invisible_col"));
	acc_vis_chams_enemy_invisible_col.SetColor(100, 160, 220, 255);
	ChamsOptions.PlaceLabledControl(1, (""), this, &acc_vis_chams_enemy_invisible_col);


	BacktrackChams.SetFileId("BAIMLESS_config_visual_vis_chams_BacktrackChams");
	ChamsOptions.PlaceLabledControl(1, "Backtrack player", this, &BacktrackChams);

	BacktrackChams_col.SetFileId(("BAIMLESS_config_visual_BacktrackChams_col"));
	acc_vis_chams_enemy_invisible_col.SetColor(3, 211, 123, 255);
	ChamsOptions.PlaceLabledControl(1, (""), this, &BacktrackChams_col);

	chamsopacity.SetFileId("BAIMLESS_config_visual_chams_opacity_enemy");
	chamsopacity.SetBoundaries(0.f, 100.f);
	chamsopacity.SetValue(100.f);
	ChamsOptions.PlaceLabledControl(1, "Chams opacity", this, &chamsopacity);
    //////////////////////////////////////////////////////////////////////////////
	chamsmaterial_team.SetFileId("BAIMLESS_config_visual_chams_material_team");
	chamsmaterial_team.AddItem("normal");
	chamsmaterial_team.AddItem("reflective");
	chamsmaterial_team.AddItem("bright reflective");
	chamsmaterial_team.AddItem("flat");
	chamsmaterial_team.AddItem("dark");
	chamsmaterial_team.AddItem("pulse");
	chamsmaterial_team.AddItem("glow");
	chamsmaterial_team.AddItem("dark glow");
	ChamsOptions.PlaceLabledControl(2, "Colored models material", this, &chamsmaterial_team);

	vis_chams_friendly_visible.SetFileId("BAIMLESS_config_visual_vis_chams_friendly_visible");
	ChamsOptions.PlaceLabledControl(2, "Player", this, &vis_chams_friendly_visible);

	vis_chams_friendly_visible_col.SetFileId(("BAIMLESS_config_visual_vis_chams_friendly_visible_col"));
	vis_chams_friendly_visible_col.SetColor(87, 26, 224, 255);
	ChamsOptions.PlaceLabledControl(2, (""), this, &vis_chams_friendly_visible_col);

	vis_chams_friendly_invisible.SetFileId("BAIMLESS_config_visual_vis_chams_friendly_invisible");
	ChamsOptions.PlaceLabledControl(2, "Player behind wall", this, &vis_chams_friendly_invisible);

	vis_chams_friendly_invisible_col.SetFileId(("BAIMLESS_config_visual_vis_chams_friendly_invisible_col_col"));
	vis_chams_friendly_invisible_col.SetColor(26, 200, 224, 255);
	ChamsOptions.PlaceLabledControl(2, (""), this, &vis_chams_friendly_invisible_col);

	chamsopacity_team.SetFileId("BAIMLESS_config_visual_chams_opacity_team");
	chamsopacity_team.SetBoundaries(0.f, 100.f);
	chamsopacity_team.SetValue(100.f);
	ChamsOptions.PlaceLabledControl(2, "Chams opacity", this, &chamsopacity_team);
    /////////////////////////////////////////////////////////////////////////////////////
	chamsmaterial_local.SetFileId("BAIMLESS_config_visual_chams_material_local");
	chamsmaterial_local.AddItem("normal");
	chamsmaterial_local.AddItem("reflective");
	chamsmaterial_local.AddItem("bright reflective");
	chamsmaterial_local.AddItem("flat");
	chamsmaterial_local.AddItem("dark");
	chamsmaterial_local.AddItem("pulse");
	chamsmaterial_local.AddItem("glow");
	chamsmaterial_local.AddItem("dark glow");
	ChamsOptions.PlaceLabledControl(3, "Colored models material", this, &chamsmaterial_local);


	localplayerchams.SetFileId("BAIMLESS_config_visual_local_playerchams");
	ChamsOptions.PlaceLabledControl(3, "Local chams", this, &localplayerchams);

	localplayerchams_col.SetFileId(("BAIMLESS_config_visual_local_playerchams_col"));
	localplayerchams_col.SetColor(70, 240, 200, 255);
	ChamsOptions.PlaceLabledControl(3, (""), this, &localplayerchams_col);

	vis_localplayerchams_shadow.SetFileId("BAIMLESS_config_visual_localplayerchams_shadow");
	ChamsOptions.PlaceLabledControl(3, "Desync chams", this, &vis_localplayerchams_shadow);

	vis_localplayerchams_shadow_col.SetFileId(("BAIMLESS_config_visual_localplayerchams_shadow_col"));
	vis_localplayerchams_shadow_col.SetColor(70, 0, 200, 255);
	ChamsOptions.PlaceLabledControl(3, (""), this, &vis_localplayerchams_shadow_col);

	fakelagchams.SetFileId("BAIMLESS_config_visual_fakelagchams");
	ChamsOptions.PlaceLabledControl(3, "Fake lag chams", this, &fakelagchams);

	fakelagchams_col.SetFileId(("BAIMLESS_config_visual_fakelagchams_col"));
	fakelagchams_col.SetColor(222, 31, 87, 255);
	ChamsOptions.PlaceLabledControl(3, (""), this, &fakelagchams_col);

	chamsopacity_local.SetFileId("BAIMLESS_config_visual_chams_opacity_local");
	chamsopacity_local.SetBoundaries(0.f, 100.f);
	chamsopacity_local.SetValue(100.f);
	ChamsOptions.PlaceLabledControl(3, "Chams opacity", this, &chamsopacity_local);



	HandCHAMSenabel.SetFileId("BAIMLESS_config_visual_local_handenabel");
	ChamsOptions.PlaceLabledControl(4, "hand chams", this, &HandCHAMSenabel);

	HandCHAMS_col.SetFileId(("BAIMLESS_config_visual_HandCHAMS_col"));
	ChamsOptions.PlaceLabledControl(4, (""), this, &HandCHAMS_col);

	HandCHAMS.SetFileId("BAIMLESS_config_visual_local_hand");
	HandCHAMS.AddItem("normal");
	HandCHAMS.AddItem("reflective");
	HandCHAMS.AddItem("bright reflective");
	HandCHAMS.AddItem("flat");
	HandCHAMS.AddItem("dark");
	HandCHAMS.AddItem("pulse");
	HandCHAMS.AddItem("wireframe");
	ChamsOptions.PlaceLabledControl(4, "hand material", this, &HandCHAMS);

    ////////////////////////////////////////////////////////////////////////
	MoreOptions.SetText("Other ESP");
	MoreOptions.SetPosition(245, 5);
	MoreOptions.SetSize(225, 269);
	MoreOptions.AddTab(CGroupTab("Other", 1));
	MoreOptions.AddTab(CGroupTab("More", 2));
	MoreOptions.AddTab(CGroupTab("Extra", 3));
	RegisterControl(&MoreOptions);

	hitmarker.SetFileId("BAIMLESS_config_visual_hitmarker");
	MoreOptions.PlaceLabledControl(1, "Hit marker", this, &hitmarker);

	hitmarkersound.SetFileId("aBAIMLESS_config_visual_hitmarkersound");
	hitmarkersound.AddItem("off");
	hitmarkersound.AddItem("gamesense");
	hitmarkersound.AddItem("bubble");
	hitmarkersound.AddItem("bameware");
	hitmarkersound.AddItem("laser");
	hitmarkersound.AddItem("quake");
	hitmarkersound.AddItem("UT");
	MoreOptions.PlaceLabledControl(1, "Hit marker sound", this, &hitmarkersound);

	Fov.SetFileId("BAIMLESS_config_visual_fov");
	Fov.SetBoundaries(0.f, 50.f);
	Fov.SetValue(0.f);
	MoreOptions.PlaceLabledControl(1, "Override viewmodel FOV", this, &Fov);


	viewmodel.SetFileId("BAIMLESS_config_visual_viewmodel");
	viewmodel.SetBoundaries(30.f, 120.f);
	viewmodel.SetValue(90.f);
	MoreOptions.PlaceLabledControl(1, "Override viewangle FOV", this, &viewmodel);

	spectators.SetFileId("BAIMLESS_config_visual_spectators");
	MoreOptions.PlaceLabledControl(1, "Spectator list", this, &spectators);

	spectatorsX.SetFileId("BAIMLESS_config_rage_spectatorsx");
	spectatorsX.SetBoundaries(0, 1800);
	spectatorsX.SetValue(1600);
	spectatorsX.extension = ("°");
	MoreOptions.PlaceLabledControl(1, "x axis", this, &spectatorsX);

	spectatorsY.SetFileId("BAIMLESS_config_rage_spectatorsY");
	spectatorsY.SetBoundaries(0, 1000);
	spectatorsY.SetValue(150);
	spectatorsY.extension = ("°");
	MoreOptions.PlaceLabledControl(1, "y axis", this, &spectatorsY);

	SoundEsp.SetFileId("BAIMLESS_config_visual_SoundEsp");
	MoreOptions.PlaceLabledControl(1, "Sound Esp", this, &SoundEsp);

	SoundEsp_col.SetFileId(("BAIMLESS_config_visual_SoundEsp_col"));
	MoreOptions.PlaceLabledControl(1, (""), this, &SoundEsp_col);

	feet_circle.SetFileId("BAIMLESS_config_visual_feet_circle");
	MoreOptions.PlaceLabledControl(2, "Feet circle", this, &feet_circle);

	zeus_range.SetFileId("BAIMLESS_config_visual_zeus_range");
	MoreOptions.PlaceLabledControl(2, "Zeus range", this, &zeus_range);

	zeus_range_col.SetFileId(("BAIMLESS_config_visual_zeus_range_col"));
	MoreOptions.PlaceLabledControl(2, (""), this, &zeus_range_col);

	viewmodel_x.SetFileId("BAIMLESS_config_visual_fov_x");
	viewmodel_x.SetBoundaries(-10, 10);
	viewmodel_x.SetValue(1);
	MoreOptions.PlaceLabledControl(2, "fov x", this, &viewmodel_x);

	viewmodel_y.SetFileId("BAIMLESS_config_visual_fov_y");
	viewmodel_y.SetBoundaries(-10, 10);
	viewmodel_y.SetValue(1);
	MoreOptions.PlaceLabledControl(2, "fov y", this, &viewmodel_y);

	viewmodel_z.SetFileId("BAIMLESS_config_visual_fov_z");
	viewmodel_z.SetBoundaries(-10, 10);
	viewmodel_z.SetValue(-1);
	MoreOptions.PlaceLabledControl(2, "fov z", this, &viewmodel_z);

	flTransparentWorld.SetFileId("BAIMLESS_config_visual_flTransparentWorld");
	flTransparentWorld.SetBoundaries(0.f, 100.f);
	flTransparentWorld.SetValue(100.f);
	MoreOptions.PlaceLabledControl(2, "World transparency", this, &flTransparentWorld);

	flTransparentProps.SetFileId("BAIMLESS_config_visual_flTransparentProps");
	flTransparentProps.SetBoundaries(0.f, 100.f);
	flTransparentProps.SetValue(100.f);
	MoreOptions.PlaceLabledControl(2, "Prop transparency", this, &flTransparentProps);


	transparency_scoped_alpha.SetFileId("BAIMLESS_config_visual_transparency_scoped_alpha");
	transparency_scoped_alpha.SetBoundaries(0, 100);
	transparency_scoped_alpha.SetValue(1);
	MoreOptions.PlaceLabledControl(2, "Local scoped alpha", this, &transparency_scoped_alpha);



	Indicators.SetFileId("BAIMLESS_config_visual_Indicators");
	MoreOptions.PlaceLabledControl(3, "Indicators", this, &Indicators);

	IndicatorsX.SetFileId("BAIMLESS_config_visual_IndicatorsX");
	IndicatorsX.SetBoundaries(0, 1800);
	IndicatorsX.SetValue(1600);
	IndicatorsX.extension = ("°");
	MoreOptions.PlaceLabledControl(3, "x axis", this, &IndicatorsX);

	IndicatorsY.SetFileId("BAIMLESS_config_visual_IndicatorsY");
	IndicatorsY.SetBoundaries(0, 1000);
	IndicatorsY.SetValue(150);
	IndicatorsY.extension = ("°");
	MoreOptions.PlaceLabledControl(3, "y axis", this, &IndicatorsY);


	LBYIndicator.SetFileId("otr_LBYIndicator");
	MoreOptions.PlaceLabledControl(3, "Fake indicator", this, &LBYIndicator);

	Chocked_indicator.SetFileId("otr_Chocked_indicator");
	MoreOptions.PlaceLabledControl(3, "Choke indicator", this, &Chocked_indicator);

	killCounter.SetFileId("otr_killCounter");
	MoreOptions.PlaceLabledControl(3, "Kill counter", this, &killCounter);

	Wasd_indicator.SetFileId("BAIMLESS_config_visual_Wasd");
	MoreOptions.PlaceLabledControl(3, "Wasd indicator", this, &Wasd_indicator);

	hit_effect.SetFileId("BAIMLESS_config_visual_hit_effect");
	MoreOptions.PlaceLabledControl(3, "Hit effect", this, &hit_effect);

	Effects.SetText("Effects");
	Effects.SetPosition(245, 284);
	Effects.SetSize(225, 298);
	Effects.AddTab(CGroupTab("Effects", 1));
	Effects.AddTab(CGroupTab("Extra", 2));
	Effects.AddTab(CGroupTab("More", 3));
	RegisterControl(&Effects);

	nightmode.SetFileId("BAIMLESS_config_visual_nightmode");
	Effects.PlaceLabledControl(1, "Nightmode", this, &nightmode);

	full_bright.SetFileId("BAIMLESS_config_visual_fullbright");
	Effects.PlaceLabledControl(1, "Full bright", this, &full_bright);

	scope_bool.SetFileId("BAIMLESS_config_visual_remove_scope");
	Effects.PlaceLabledControl(1, "Remove scope overlay", this, &scope_bool);

	scope_bool_option.SetFileId("BAIMLESS_config_visual_scope_bool_option");
	scope_bool_option.AddItem("static");
	scope_bool_option.AddItem("motion");
	Effects.PlaceLabledControl(1, "Scope type", this, &scope_bool_option);

	NoZoom.SetFileId("BAIMLESS_config_visual_NoZoom");
	Effects.PlaceLabledControl(1, "Remove zoom effects", this, &NoZoom);

	noflash.SetFileId("BAIMLESS_config_visual_noflash");
	Effects.PlaceLabledControl(1, "Remove flashbang effects", this, &noflash);

	norecoil.SetFileId("BAIMLESS_config_visual_norecoil");
	Effects.PlaceLabledControl(1, "Remove visual recoil", this, &norecoil);

	msc_remove_smoke.SetFileId("aBAIMLESS_config_visual_msc_remove_smoke");
	Effects.PlaceLabledControl(1, "Remove smoke grenades", this, &msc_remove_smoke);

	DisablePostProcessing.SetFileId(("BAIMLESS_config_visual_disablepostprocessing"));
	Effects.PlaceLabledControl(1, ("Disable post processing"), this, &DisablePostProcessing);

	indicator.SetFileId("eternity_config_visual_indicator");
	indicator.items.push_back(dropdownboxitem(false, ("Override")));
	indicator.items.push_back(dropdownboxitem(false, ("FD")));
	indicator.items.push_back(dropdownboxitem(false, ("Fake")));
	Effects.PlaceLabledControl(1, "Indicator", this, &indicator);

	watermark_enable.SetFileId(("BAIMLESS_config_visual_watermark_enable"));
	watermark_enable.SetState(true);
	Effects.PlaceLabledControl(2, ("Watermark"), this, &watermark_enable);

	bullettracers.SetFileId("BAIMLESS_config_visual_bullettracers");
	Effects.PlaceLabledControl(2, "Beam", this, &bullettracers);

	bullettracers_col.SetFileId(("BAIMLESS_config_visual_bullettracers_col"));
	Effects.PlaceLabledControl(2, (""), this, &bullettracers_col);

	bullettracers_type.SetFileId("eternity_config_visual_bullettracers_type");
	bullettracers_type.items.push_back(dropdownboxitem(false, ("Local")));
	bullettracers_type.items.push_back(dropdownboxitem(false, ("Enemy")));
	Effects.PlaceLabledControl(2, "Beam selection", this, &bullettracers_type);

	bulletlife.SetFileId("BAIMLESS_config_visual_beam_life");
	bulletlife.SetBoundaries(1, 10);
	bulletlife.SetValue(1);
	Effects.PlaceLabledControl(2, "Beam life", this, &bulletlife);

	beamsize.SetFileId("BAIMLESS_config_visual_beam_size");
	beamsize.SetBoundaries(1, 10);
	beamsize.SetValue(1);
	Effects.PlaceLabledControl(2, "Beam size", this, &beamsize);

	esp_grenadeprediction.SetFileId("BAIMLESS_config_visual_grenadeprediction");
	Effects.PlaceLabledControl(2, "Grenade prediction", this, &esp_grenadeprediction);

	esp_grenadepredictione_col.SetFileId(("BAIMLESS_config_visual_grenadepredictione_col"));
	esp_grenadepredictione_col.SetColor(100, 160, 220, 255);
	Effects.PlaceLabledControl(2, (""), this, &esp_grenadepredictione_col);

	fov_arrows_enemy.SetFileId("BAIMLESS_config_visual_fov_arrows_enemy");
	Effects.PlaceLabledControl(2, "Out of view arrows", this, &fov_arrows_enemy);

	fov_arrows_enemy_col.SetFileId(("BAIMLESS_config_visual_fov_arrows_enemy_col"));
	fov_arrows_enemy_col.SetColor(100, 160, 220, 255);
	Effects.PlaceLabledControl(2, (""), this, &fov_arrows_enemy_col);

	manualaaindicator.SetFileId("BAIMLESS_config_visual_manualaaindicator");
	Effects.PlaceLabledControl(2, "Manual aa indicator", this, &manualaaindicator);

	manualaaindicator_col.SetFileId(("BAIMLESS_config_visual_manualaaindicator_col"));
	manualaaindicator_col.SetColor(250, 5, 250, 255);
	Effects.PlaceLabledControl(2, (""), this, &manualaaindicator_col);

	impacts.SetFileId("BAIMLESS_config_visual_impacts");
	Effects.PlaceLabledControl(2, "Impacts", this, &impacts);

	impacts_col.SetFileId(("BAIMLESS_config_visual_impacts_col"));
	impacts_col.SetColor(250, 250, 250, 255);
	Effects.PlaceLabledControl(2, (""), this, &impacts_col);

	DebugLagComp.SetFileId(("lagcompensationyes"));
	Effects.PlaceLabledControl(2, ("Draw shot hitboxes"), this, &DebugLagComp);

	DebugLagComp_col.SetFileId(("BAIMLESS_config_DebugLagComp_col"));
	DebugLagComp_col.SetColor(255, 255, 255, 255);
	Effects.PlaceLabledControl(2, (""), this, &DebugLagComp_col);

	penetration_reticle.SetFileId("BAIMLESS_config_visual_penetration_reticle");
	Effects.PlaceLabledControl(2, "Autowall crosshair", this, &penetration_reticle);

	glowlocal.SetFileId("BAIMLESS_config_visual_glowlocal");
	Effects.PlaceLabledControl(3, "Local glow", this, &glowlocal);

	glowlocal_col.SetFileId(("BAIMLESS_config_visual_glowlocal_col"));
	glowlocal_col.SetColor(255, 0, 255, 255);
	Effects.PlaceLabledControl(3, (""), this, &glowlocal_col);

	glowenemy.SetFileId("BAIMLESS_config_visual_glowenemy");
	Effects.PlaceLabledControl(3, "Enemy glow", this, &glowenemy);

	glowenemy_col.SetFileId(("BAIMLESS_config_visual_glowenemy_col"));
	glowenemy_col.SetColor(255, 0, 0, 255);
	Effects.PlaceLabledControl(3, (""), this, &glowenemy_col);

	glowteam.SetFileId("BAIMLESS_config_visual_glowteam");
	Effects.PlaceLabledControl(3, "Team glow", this, &glowteam);

	glowteam_col.SetFileId(("BAIMLESS_config_visual_glowteam_col"));
	glowteam_col.SetColor(0, 0, 255, 255);
	Effects.PlaceLabledControl(3, (""), this, &glowteam_col);



	m_world_glow.SetFileId("BAIMLESS_config_visual_m_world_glow");
	m_world_glow.items.push_back(dropdownboxitem(false, ("Weapons")));
	m_world_glow.items.push_back(dropdownboxitem(false, ("Grenades")));
	m_world_glow.items.push_back(dropdownboxitem(false, ("C4")));
	Effects.PlaceLabledControl(3, "World glow", this, &m_world_glow);

	m_world_glow_col.SetFileId(("BAIMLESS_config_visual_m_world_glow_col"));
	m_world_glow_col.SetColor(154, 200, 0, 255);
	Effects.PlaceLabledControl(3, (""), this, &m_world_glow_col);

	drop_box.SetFileId("BAIMLESS_config_visual_drop_box");
	Effects.PlaceLabledControl(3, "Dropped weapon box", this, &drop_box);

	drop_box_col.SetFileId(("BAIMLESS_config_visual_drop_box_col"));
	drop_box_col.SetColor(200, 78, 0, 255);
	Effects.PlaceLabledControl(3, (""), this, &drop_box_col);

	droppedweaponammo.SetFileId("BAIMLESS_config_visual_droppedweaponammo");
	Effects.PlaceLabledControl(3, "Dropped weapon ammo", this, &droppedweaponammo);

	droppedweaponammobar.SetFileId("BAIMLESS_config_visual_droppedweaponammobar");
	Effects.PlaceLabledControl(3, "Dropped weapon ammo bar", this, &droppedweaponammobar);

	droppedweaponammobar_col.SetFileId(("BAIMLESS_config_visual_droppedweaponammobar_col"));
	droppedweaponammobar_col.SetColor(154, 7, 200, 255);
	Effects.PlaceLabledControl(3, (""), this, &droppedweaponammobar_col);

	droppedweaponname.SetFileId("BAIMLESS_config_visual_droppedweaponname");
	droppedweaponname.AddItem("off");
	droppedweaponname.AddItem("name");
	droppedweaponname.AddItem("icon");
	Effects.PlaceLabledControl(3, "Dropped weapon name", this, &droppedweaponname);

	grenade.SetFileId("BAIMLESS_config_visual_grenade");
	Effects.PlaceLabledControl(3, "Grenade box", this, &grenade);

	grenadename.SetFileId("BAIMLESS_config_visual_grenadename");
	grenadename.AddItem("off");
	grenadename.AddItem("name");
	grenadename.AddItem("icon");
	Effects.PlaceLabledControl(3, "", this, &grenadename);

	chickenbox.SetFileId("BAIMLESS_config_visual_chickenbox");
	Effects.PlaceLabledControl(3, "Chicken box", this, &chickenbox);


	/*
	droppedweaponname














	indicator.SetFileId("BAIMLESS_config_visual_indicator");
	indicator.items.push_back(dropdownboxitem(false, ("override")));
	indicator.items.push_back(dropdownboxitem(false, ("lc")));
	indicator.items.push_back(dropdownboxitem(false, ("speed")));
	indicator.items.push_back(dropdownboxitem(false, ("fake")));
	MoreOptions.PlaceLabledControl(2, "indicator", this, &indicator);





#pragma endregion Setting up the Other controls
}*/


void UnloadFull()
{
	g_Unload = true;

}
/*
void CMiscTab::Setup()
{

	SetTitle("D");

#pragma region Other
	OtherGroup.SetPosition(10, 5);
	OtherGroup.SetSize(225, 577);
	OtherGroup.SetText("Miscellaneous");
	RegisterControl(&OtherGroup);

	bhop_bool.SetFileId("BAIMLESS_config_misc_bhop");
	OtherGroup.PlaceLabledControl(0, "Bunny hop", this, &bhop_bool);


	edge_jump.SetFileId("BAIMLESS_config_misc_edge_jump");
	OtherGroup.PlaceLabledControl(0, "Edge jump", this, &edge_jump);
	
	edge_jumpKeyBind.SetFileId("BAIMLESS_config_misc_edge_jumpKeyBind");
	OtherGroup.PlaceLabledControl(0, "", this, &edge_jumpKeyBind);

	duck_in_air.SetFileId("BAIMLESS_config_misc_duck_in_air");
	OtherGroup.PlaceLabledControl(0, "Duck in air", this, &duck_in_air);

	rankreveal.SetFileId("BAIMLESS_config_misc_rankreveal");
	OtherGroup.PlaceLabledControl(0, "Reveal competitive ranks", this, &rankreveal);

	AutoStrafe.SetFileId("BAIMLESS_config_misc_autostrafe");
	OtherGroup.PlaceLabledControl(0, "Air strafe", this, &AutoStrafe);

	ragdoll.SetFileId("BAIMLESS_config_ragdoll_gravity");
	OtherGroup.PlaceLabledControl(0, "Gravity ragdoll", this, &ragdoll);

	clantag.SetFileId("aa_clantag");
	clantag.AddItem("off");
	clantag.AddItem("BAIMLESS.xyz");
	clantag.AddItem("custom");
	OtherGroup.PlaceLabledControl(0, "Clan tag", this, &clantag);

	customclan.SetFileId("otr_customclan");
	customclan.SetText("");
	OtherGroup.PlaceLabledControl(0, "Custom Clan tag", this, &customclan);

	NameChanger.SetFileId("BAIMLESS_config_misc_name_stealer");
	OtherGroup.PlaceLabledControl(0, "Name stealer", this, &NameChanger);

	knifebot.SetFileId("BAIMLESS_config_misc_knifebot");
	OtherGroup.PlaceLabledControl(0, "Knife bot", this, &knifebot);

	walkbot.SetFileId("BAIMLESS_config_misc_walkbot");
	OtherGroup.PlaceLabledControl(0, "Walk bot", this, &walkbot);

	OtherThirdperson.SetFileId("BAIMLESS_config_misc_thirdperson");
	OtherGroup.PlaceLabledControl(0, "Thirdperson", this, &OtherThirdperson);

	ThirdPersonKeyBind.SetFileId("BAIMLESS_config_misc_thirdperson_bind");
	OtherGroup.PlaceLabledControl(0, "", this, &ThirdPersonKeyBind);


	thirdperson_distance.SetFileId("BAIMLESS_config_misc_thirdperson_distance");
	thirdperson_distance.SetBoundaries(1, 170);
	thirdperson_distance.SetValue(80);
	OtherGroup.PlaceLabledControl(0, "Thirdperson Distance", this, &thirdperson_distance);


	0.SetFileId("auto_buy");
	OtherGroup.PlaceLabledControl(0, "Auto buy items", this, &autobuy);

	AmbientRed.SetFileId("otr_ambientred");
	AmbientRed.SetBoundaries(0.f, 10.f);
	AmbientRed.SetValue(0.f);
	OtherGroup.PlaceLabledControl(0, "Ambient red", this, &AmbientRed);

	AmbientGreen.SetFileId("otr_ambientgreen");
	AmbientGreen.SetBoundaries(0.f, 10.f);
	AmbientGreen.SetValue(0.f);
	OtherGroup.PlaceLabledControl(0, "Ambient green", this, &AmbientGreen);

	AmbientBlue.SetFileId("otr_ambientblue");
	AmbientBlue.SetBoundaries(0.f, 10.f);
	AmbientBlue.SetValue(0.f);
	OtherGroup.PlaceLabledControl(0, "Ambient blue", this, &AmbientBlue);

	buybot.SetFileId("tgt_buybot");
	buybot.items.push_back(dropdownboxitem(false, "ak47"));
	buybot.items.push_back(dropdownboxitem(false, "m4a1"));
	buybot.items.push_back(dropdownboxitem(false, "awp"));
	buybot.items.push_back(dropdownboxitem(false, "awp"));
	buybot.items.push_back(dropdownboxitem(false, "ssg08"));
	buybot.items.push_back(dropdownboxitem(false, "scar20"));
	buybot.items.push_back(dropdownboxitem(false, "g3sg1"));
	buybot.items.push_back(dropdownboxitem(false, "elite"));
	buybot.items.push_back(dropdownboxitem(false, "flashbang"));
	buybot.items.push_back(dropdownboxitem(false, "smokegrenade"));
	buybot.items.push_back(dropdownboxitem(false, "hegrenade"));
	buybot.items.push_back(dropdownboxitem(false, "molotov"));
	buybot.items.push_back(dropdownboxitem(false, "incgrenade"));
	buybot.items.push_back(dropdownboxitem(false, "vest"));
	buybot.items.push_back(dropdownboxitem(false, "helmet"));
	buybot.items.push_back(dropdownboxitem(false, "defuser"));
	buybot.items.push_back(dropdownboxitem(false, "taser"));
	OtherGroup.PlaceLabledControl(0, "Weapons", this, &buybot);

	MoreOps.SetPosition(245, 5);
	MoreOps.SetText("Settings");
	MoreOps.SetSize(225, 169);
	RegisterControl(&MoreOps);

	MenuKeyLabel.SetFileId("BAIMLESS_config_misc_MenuKeyLabel");
	MenuKeyLabel.SetText(("Menu key"));
	MoreOps.PlaceLabledControl(0, (""), this, &MenuKeyLabel);

	MenuKey.SetFileId(("BAIMLESS_config_misc_menukey"));
	MenuKey.SetKey(VK_INSERT);
	MoreOps.PlaceLabledControl(0, (""), this, &MenuKey);

	MenuColor.SetFileId(("BAIMLESS_config_misc_MenuColor"));
	MenuColor.SetColor(26, 201, 224, 255);
	MoreOps.PlaceLabledControl(0, ("Menu color"), this, &MenuColor);

	OtherSafeMode.SetFileId("BAIMLESS_config_misc_anti_untrusted");
	OtherSafeMode.SetState(true);
	MoreOps.PlaceLabledControl(0, "Anti-untrusted", this, &OtherSafeMode);


	Menutheme.SetFileId("BAIMLESS_config_misc_Menutheme");
	Menutheme.AddItem("Main");
	Menutheme.AddItem("Simple");
	MoreOps.PlaceLabledControl(0, "Menu theme", this, &Menutheme);

	ConfigGroup.SetPosition(245, 184);
	ConfigGroup.SetText("Other");
	ConfigGroup.SetSize(225, 398);
	RegisterControl(&ConfigGroup);


	ConfigBox.SetFileId("cfg_box");
	ConfigBox.AddItem("legit");
	ConfigBox.AddItem("rage");
	ConfigBox.AddItem("hvh");
	ConfigBox.AddItem("casual");
	ConfigBox.AddItem("hvh rage");
	ConfigBox.AddItem("main");
	ConfigBox.AddItem("baim hvh");
	ConfigGroup.PlaceLabledControl(0, "               ", this, &ConfigBox);

	SaveButton.SetText("Save");
	SaveButton.SetCallback(SaveLegitCallbk);
	ConfigGroup.PlaceLabledControl(0, "", this, &SaveButton);

	LoadButton.SetText("Load");
	LoadButton.SetCallback(LoadLegitCallbk);
	ConfigGroup.PlaceLabledControl(0, "", this, &LoadButton);

	unloadbutton.SetText("unload");
	unloadbutton.SetCallback(UnloadFull);
	ConfigGroup.PlaceLabledControl(0, "", this, &unloadbutton);
}


void CSkinTab::Setup()
{

	SetTitle("E");

	MainGroup.SetText("main");
	MainGroup.AddTab(CGroupTab("main", 1));
	MainGroup.AddTab(CGroupTab("other", 2));
	MainGroup.SetPosition(10, 5);
	MainGroup.SetSize(225, 577);
	RegisterControl(&MainGroup);

	skins_enable.SetFileId("BAIMLESS_config_skinchanger_skins_enable");
	MainGroup.PlaceLabledControl(1, "active", this, &skins_enable);

	skins_knife_model.SetFileId("BAIMLESS_config_skinchanger_skins_knife_model");
	skins_knife_model.AddItem("bayonet");
	skins_knife_model.AddItem("flip knife");
	skins_knife_model.AddItem("gut knife");
	skins_knife_model.AddItem("karambit");
	skins_knife_model.AddItem("m9 bayonet");
	skins_knife_model.AddItem("huntsman knife");
	skins_knife_model.AddItem("butterfly knife");
	skins_knife_model.AddItem("falchion knife");
	skins_knife_model.AddItem("shadow Daggers");
	skins_knife_model.AddItem("bowie knife");
	skins_knife_model.AddItem("navaja knife");
	skins_knife_model.AddItem("stiletto knife");
	skins_knife_model.AddItem("ursus knife");
	skins_knife_model.AddItem("talon knife");
	skins_knife_model.AddItem("skeleton knife");
	skins_knife_model.AddItem("nomad knife");
	skins_knife_model.AddItem("survivak knife");
	skins_knife_model.AddItem("paracord knife");
	MainGroup.PlaceLabledControl(1, "knife model", this, &skins_knife_model);

	KnifeSkin.SetFileId(("BAIMLESS_config_skinchanger_knifeskin"));
	KnifeSkin.AddItem(("vanilla"), 0);
	KnifeSkin.AddItem(("forest ddpat"), 5);
	KnifeSkin.AddItem(("night stripe"), 735);
	KnifeSkin.AddItem(("doppler phase 1"), 418);
	KnifeSkin.AddItem(("doppler phase 2"), 419);
	KnifeSkin.AddItem(("doppler phase 3"), 420);
	KnifeSkin.AddItem(("doppler phase 4"), 421);
	KnifeSkin.AddItem(("doppler ruby"), 415);
	KnifeSkin.AddItem(("Doppler sapphire"), 416);
	KnifeSkin.AddItem(("Doppler blackpearl"), 417);
	KnifeSkin.AddItem(("crimson web"), 12);
	KnifeSkin.AddItem(("slaughter"), 59);
	KnifeSkin.AddItem(("fade"), 38);
	KnifeSkin.AddItem(("night"), 40);
	KnifeSkin.AddItem(("blue steel"), 42);
	KnifeSkin.AddItem(("stained"), 43);
	KnifeSkin.AddItem(("case hardened"), 44);
	KnifeSkin.AddItem(("safari mesh"), 72);
	KnifeSkin.AddItem(("boreal F=forest"), 77);
	KnifeSkin.AddItem(("ultraviolet"), 98);
	KnifeSkin.AddItem(("urban masked"), 143);
	KnifeSkin.AddItem(("damascus steel"), 410);
	KnifeSkin.AddItem(("scorched"), 175);
	KnifeSkin.AddItem(("tiger tooth"), 409);
	KnifeSkin.AddItem(("marble fade"), 413);
	KnifeSkin.AddItem(("rust coat"), 323);
	KnifeSkin.AddItem(("bright water"), 578);
	KnifeSkin.AddItem(("lore bayonet"), 558);
	KnifeSkin.AddItem(("lore flip"), 559);
	KnifeSkin.AddItem(("lore gut"), 560);
	KnifeSkin.AddItem(("lore karambit"), 561);
	KnifeSkin.AddItem(("lore m9 "), 562);
	KnifeSkin.AddItem(("autotronic bayonet"), 573);
	KnifeSkin.AddItem(("autotronic flip"), 574);
	KnifeSkin.AddItem(("autotronic gut"), 575);
	KnifeSkin.AddItem(("autotronic karambit"), 576);
	KnifeSkin.AddItem(("autotronic m9"), 577);
	KnifeSkin.AddItem(("gamme doppler phase 1"), 569);
	KnifeSkin.AddItem(("gamme doppler phase 2"), 570);
	KnifeSkin.AddItem(("gamme doppler phase 3"), 571);
	KnifeSkin.AddItem(("gamme doppler phase 4"), 572);
	KnifeSkin.AddItem(("gamme doppler emerald"), 568);
	KnifeSkin.AddItem(("black laminate bayonet"), 563);
	KnifeSkin.AddItem(("freehand"), 581);
	KnifeSkin.SetHeightInItems(6);
	MainGroup.PlaceLabledControl(1, (""), this, &KnifeSkin);

	stacktrackknife_enable.SetFileId("BAIMLESS_config_skinchanger_stacktrackknife_enable");
	MainGroup.PlaceLabledControl(1, "statrack", this, &stacktrackknife_enable);

	stacktrackknife.SetFileId("BAIMLESS_config_skinchanger_stacktrack_value");
	stacktrackknife.SetBoundaries(1, 1000);
	stacktrackknife.SetValue(10);
	stacktrackknife.extension = ("%%");
	MainGroup.PlaceLabledControl(1, "", this, &stacktrackknife);

	GloveModel.SetFileId("BAIMLESS_config_skinchanger_GloveModel");
	GloveModel.AddItem("None");
	GloveModel.AddItem("bloodhound");
	GloveModel.AddItem("sporty");
	GloveModel.AddItem("slick");
	GloveModel.AddItem("handwrap leathery");
	GloveModel.AddItem("motorcycle");
	GloveModel.AddItem("specialist");
	GloveModel.AddItem("bloodhound hydra");
	MainGroup.PlaceLabledControl(1, "glove model", this, &GloveModel);

	GloveSkin.SetFileId(("BAIMLESS_config_skinchanger_GloveSkin"));
	GloveSkin.AddItem(("black silver"), 10006);
	GloveSkin.AddItem(("snakeskin brass"), 10007);
	GloveSkin.AddItem(("metallic"), 10008);
	GloveSkin.AddItem(("leathery"), 10009);
	GloveSkin.AddItem(("camo grey"), 10010);
	GloveSkin.AddItem(("slick black"), 10013);
	GloveSkin.AddItem(("slick military"), 10015);
	GloveSkin.AddItem(("slick red"), 10016);
	GloveSkin.AddItem(("sporty light blue"), 10018);
	GloveSkin.AddItem(("sporty military"), 10019);
	GloveSkin.AddItem(("red slaughter"), 10021);
	GloveSkin.AddItem(("basic black"), 10024);
	GloveSkin.AddItem(("mint triangle"), 10026);
	GloveSkin.AddItem(("mono boom"), 10027);
	GloveSkin.AddItem(("triangle_blue"), 10028);
	GloveSkin.AddItem(("ddpat green camo"), 10030);
	GloveSkin.AddItem(("kimono diamonds red"), 10033);
	GloveSkin.AddItem(("emerald web"), 10034);
	GloveSkin.AddItem(("orange white"), 10035);
	GloveSkin.AddItem(("fabric orange camo"), 10036);
	GloveSkin.AddItem(("sporty purple"), 10037);
	GloveSkin.AddItem(("sporty green"), 10038);
	GloveSkin.AddItem(("guerrilla"), 10039);
	GloveSkin.AddItem(("snakeskin yellow"), 10040);
	GloveSkin.SetHeightInItems(6);
	MainGroup.PlaceLabledControl(1, ("glove skin"), this, &GloveSkin);

	SkinApply.SetText("apply");
	//SkinApply.SetCallback(ApplyFullUpdate);
	MainGroup.PlaceLabledControl(1, "", this, &SkinApply);


	rankchanger.SetFileId("BAIMLESS_config_skinchanger_rankchanger");
	MainGroup.PlaceLabledControl(2, "rank changer", this, &rankchanger);

	rank_id.SetFileId("rank_id_skin");
	rank_id.AddItem("off");
	rank_id.AddItem("silver 1");
	rank_id.AddItem("silver 2");
	rank_id.AddItem("silver 3");
	rank_id.AddItem("silver 4");
	rank_id.AddItem("silver elite");
	rank_id.AddItem("silver elite master");
	rank_id.AddItem("gold 1");
	rank_id.AddItem("gold 2");
	rank_id.AddItem("gold 3");
	rank_id.AddItem("gold nova master");
	rank_id.AddItem("master guardian 1");
	rank_id.AddItem("master guardian 2");
	rank_id.AddItem("mge");
	rank_id.AddItem("dmg");
	rank_id.AddItem("le");
	rank_id.AddItem("lem ");
	rank_id.AddItem("supreme");
	rank_id.AddItem("global");
	MainGroup.PlaceLabledControl(2, "rank", this, &rank_id);

	player_level.SetFileId("player_level_speed");
	player_level.SetBoundaries(0, 40);
	player_level.SetValue(0);
	MainGroup.PlaceLabledControl(2, "level", this, &player_level);

	player_level_xp.SetFileId("player_level_xp_speed");
	player_level_xp.SetBoundaries(0, 1000);
	player_level_xp.SetValue(0);
	MainGroup.PlaceLabledControl(2, "xp", this, &player_level_xp);

	wins.SetFileId("wins_speed");
	wins.SetBoundaries(0, 1000);
	wins.SetValue(0);
	MainGroup.PlaceLabledControl(2, "wins", this, &wins);

	cmd_friendly.SetFileId("cmd_friendly_speed");
	cmd_friendly.SetBoundaries(0, 1000);
	cmd_friendly.SetValue(0);
	MainGroup.PlaceLabledControl(2, "friend", this, &cmd_friendly);

	cmd_teaching.SetFileId("cmd_teaching_speed");
	cmd_teaching.SetBoundaries(0, 1000);
	cmd_teaching.SetValue(0);
	MainGroup.PlaceLabledControl(2, "teach", this, &cmd_teaching);

	cmd_leader.SetFileId("cmd_leader_speed");
	cmd_leader.SetBoundaries(0, 1000);
	cmd_leader.SetValue(0);
	MainGroup.PlaceLabledControl(2, "leader", this, &cmd_leader);

	rankApply.SetText("apply");
	rankApply.SetCallback(rankupdate);
	MainGroup.PlaceLabledControl(2, "", this, &rankApply);


	WeaponGroup.SetText("weapon");
	WeaponGroup.SetPosition(245, 5);
	WeaponGroup.SetSize(225, 577);
	WeaponGroup.AddTab(CGroupTab("main", 1));
	WeaponGroup.AddTab(CGroupTab("other", 2));
	WeaponGroup.AddTab(CGroupTab("more", 3));
	WeaponGroup.AddTab(CGroupTab("more", 4));
	RegisterControl(&WeaponGroup);

	NEGEVSkin.SetFileId("BAIMLESS_config_skinchanger_negev_skin");
	NEGEVSkin.AddItem("Anodized Navy");
	NEGEVSkin.AddItem("Man-o'-war");
	NEGEVSkin.AddItem("Bratatat");
	NEGEVSkin.AddItem("Desert-Strike");
	NEGEVSkin.AddItem("Nuclear Waste");
	NEGEVSkin.AddItem("Loudmouth");
	NEGEVSkin.AddItem("Power Loader");
	WeaponGroup.PlaceLabledControl(1, "Negev", this, &NEGEVSkin);

	M249Skin.SetFileId("BAIMLESS_config_skinchanger_m249_skin");
	M249Skin.AddItem("System Lock");
	M249Skin.AddItem("Shipping Forecast");
	M249Skin.AddItem("Impact Drill");
	M249Skin.AddItem("Nebula Crusader");
	M249Skin.AddItem("Spectre");
	M249Skin.AddItem("Warbird");
	WeaponGroup.PlaceLabledControl(1, "M249", this, &M249Skin);


	AWPSkin.SetFileId("BAIMLESS_config_skinchanger_awp_skin");
	AWPSkin.AddItem("BOOM");
	AWPSkin.AddItem("Dragon Lore");
	AWPSkin.AddItem("Pink DDPAT");
	AWPSkin.AddItem("Fever Dream");
	AWPSkin.AddItem("Lightning Strike");
	AWPSkin.AddItem("Corticera");
	AWPSkin.AddItem("Redline");
	AWPSkin.AddItem("Man-o'-war");
	AWPSkin.AddItem("Graphite");
	AWPSkin.AddItem("Electric Hive");
	AWPSkin.AddItem("Pit Viper");
	AWPSkin.AddItem("Asiimov");
	AWPSkin.AddItem("Oni Taiji");
	AWPSkin.AddItem("Medusa");
	AWPSkin.AddItem("Sun in Leo");
	AWPSkin.AddItem("Hyper Beast");
	AWPSkin.AddItem("Elite Build");
	AWPSkin.AddItem("Containment Breach");
	WeaponGroup.PlaceLabledControl(1, "AWP", this, &AWPSkin);

	SSG08Skin.SetFileId("BAIMLESS_config_skinchanger_sgg08_skin");
	SSG08Skin.AddItem("Dragonfire");
	SSG08Skin.AddItem("Blood in the Water");
	SSG08Skin.AddItem("Ghost Crusader");
	SSG08Skin.AddItem("Detour");
	SSG08Skin.AddItem("Abyss");
	SSG08Skin.AddItem("Big Iron");
	SSG08Skin.AddItem("Bloodshot");
	WeaponGroup.PlaceLabledControl(1, "SGG 08", this, &SSG08Skin);

	SCAR20Skin.SetFileId("BAIMLESS_config_skinchanger_scar20_skin");
	SCAR20Skin.AddItem("Splash Jam");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Crimson Web");
	SCAR20Skin.AddItem("Cardiac");
	SCAR20Skin.AddItem("Bloodsport");
	SCAR20Skin.AddItem("Cyrex");
	SCAR20Skin.AddItem("Grotto");
	SCAR20Skin.AddItem("Torn");
	WeaponGroup.PlaceLabledControl(1, "SCAR-20", this, &SCAR20Skin);

	G3SG1Skin.SetFileId("BAIMLESS_config_skinchanger_g3sg1_skin");
	G3SG1Skin.AddItem("Demeter");
	G3SG1Skin.AddItem("Azure Zebra");
	G3SG1Skin.AddItem("Green Apple");
	G3SG1Skin.AddItem("Orange Kimono");
	G3SG1Skin.AddItem("Neon Kimono");
	G3SG1Skin.AddItem("Murky");
	G3SG1Skin.AddItem("Chronos");
	G3SG1Skin.AddItem("Flux");
	G3SG1Skin.AddItem("The Executioner");
	G3SG1Skin.AddItem("Black Sand");
	WeaponGroup.PlaceLabledControl(1, "G3SG1", this, &G3SG1Skin);


	MAG7Skin.SetFileId("BAIMLESS_config_skinchanger_mag7_skin");
	MAG7Skin.AddItem("Counter Terrace");
	MAG7Skin.AddItem("Bulldozer");
	MAG7Skin.AddItem("Heat");
	WeaponGroup.PlaceLabledControl(1, "Mag-7", this, &MAG7Skin);

	XM1014Skin.SetFileId("BAIMLESS_config_skinchanger_xm1014_skin");
	XM1014Skin.AddItem("Tranquality");
	XM1014Skin.AddItem("Teclu Burner");
	XM1014Skin.AddItem("Scumbria");
	XM1014Skin.AddItem("Heaven Guard");
	XM1014Skin.AddItem("Quicksilver");
	WeaponGroup.PlaceLabledControl(1, "XM1014", this, &XM1014Skin);

	SAWEDOFFSkin.SetFileId("BAIMLESS_config_skinchanger_sawedoff_skin");
	SAWEDOFFSkin.AddItem("Serenity");
	SAWEDOFFSkin.AddItem("Orange DDPAT");
	SAWEDOFFSkin.AddItem("Fade");
	SAWEDOFFSkin.AddItem("The Kraken");
	SAWEDOFFSkin.AddItem("Wasteland Princess");
	SAWEDOFFSkin.AddItem("Highwayman");
	WeaponGroup.PlaceLabledControl(1, "Sawed-Off", this, &SAWEDOFFSkin);

	NOVASkin.SetFileId("BAIMLESS_config_skinchanger_nova_skin");
	NOVASkin.AddItem("Candy Apple");
	NOVASkin.AddItem("Blaze Orange");
	NOVASkin.AddItem("Modern Hunter");
	NOVASkin.AddItem("Forest Leaves");
	NOVASkin.AddItem("Bloomstick");
	NOVASkin.AddItem("Sand Dune");
	NOVASkin.AddItem("Polar Mesh");
	NOVASkin.AddItem("Walnut");
	NOVASkin.AddItem("Predator");
	NOVASkin.AddItem("Tempest");
	NOVASkin.AddItem("Graphite");
	NOVASkin.AddItem("Ghost Camo");
	NOVASkin.AddItem("Rising Skull");
	NOVASkin.AddItem("Antique");
	NOVASkin.AddItem("Green Apple");
	NOVASkin.AddItem("Caged Steel");
	NOVASkin.AddItem("Koi");
	NOVASkin.AddItem("Moon in Libra");
	NOVASkin.AddItem("Ranger");
	NOVASkin.AddItem("HyperBeast");
	NOVASkin.AddItem("Plume");
	WeaponGroup.PlaceLabledControl(2, "Nova", this, &NOVASkin);

	AK47Skin.SetFileId("BAIMLESS_config_skinchanger_ak47_skin");
	AK47Skin.AddItem("First Class");
	AK47Skin.AddItem("Red Laminate");
	AK47Skin.AddItem("Case Hardened");
	AK47Skin.AddItem("Black Laminate");
	AK47Skin.AddItem("Fire Serpent");
	AK47Skin.AddItem("Cartel");
	AK47Skin.AddItem("Emerald Pinstripe");
	AK47Skin.AddItem("Blue Laminate");
	AK47Skin.AddItem("Redline");
	AK47Skin.AddItem("Vulcan");
	AK47Skin.AddItem("Jaguar");
	AK47Skin.AddItem("Jet Set");
	AK47Skin.AddItem("Wasteland Rebel");
	AK47Skin.AddItem("Orbit Mk01");
	AK47Skin.AddItem("Hydroponic");
	AK47Skin.AddItem("Aquamarine Revenge");
	AK47Skin.AddItem("Frontside Misty");
	AK47Skin.AddItem("Point Disarray");
	AK47Skin.AddItem("Fuel Injector");
	AK47Skin.AddItem("Neon Revolution");
	AK47Skin.AddItem("Bloodsport");
	AK47Skin.AddItem("Rat Rod");
	WeaponGroup.PlaceLabledControl(2, "AK-47", this, &AK47Skin);

	M41SSkin.SetFileId("BAIMLESS_config_skinchanger_m4a1s_skin");
	M41SSkin.AddItem("Dark Water");
	M41SSkin.AddItem("Hyper Beast");
	M41SSkin.AddItem("Decimator");
	M41SSkin.AddItem("VariCamo");
	M41SSkin.AddItem("Nitro");
	M41SSkin.AddItem("Bright Water");
	M41SSkin.AddItem("Atomic Alloy");
	M41SSkin.AddItem("Blood Tiger");
	M41SSkin.AddItem("Guardian");
	M41SSkin.AddItem("Master Piece");
	M41SSkin.AddItem("Knight");
	M41SSkin.AddItem("Cyrex");
	M41SSkin.AddItem("Basilisk");
	M41SSkin.AddItem("Icarus Fell");
	M41SSkin.AddItem("Hot Rod");
	M41SSkin.AddItem("Golden Coi");
	M41SSkin.AddItem("Chantico's Fire");
	M41SSkin.AddItem("Mecha Industries");
	M41SSkin.AddItem("Flashback");
	WeaponGroup.PlaceLabledControl(2, "M4A1-S", this, &M41SSkin);

	M4A4Skin.SetFileId("BAIMLESS_config_skinchanger_m4a4_skin");
	M4A4Skin.AddItem("Bullet Rain");
	M4A4Skin.AddItem("Zirka");
	M4A4Skin.AddItem("Asiimov");
	M4A4Skin.AddItem("Howl");
	M4A4Skin.AddItem("X-Ray");
	M4A4Skin.AddItem("Desert-Strike");
	M4A4Skin.AddItem("Griffin");
	M4A4Skin.AddItem("Dragon King");
	M4A4Skin.AddItem("Poseidon");
	M4A4Skin.AddItem("Daybreak");
	M4A4Skin.AddItem("Evil Daimyo");
	M4A4Skin.AddItem("Royal Paladin");
	M4A4Skin.AddItem("The BattleStar");
	M4A4Skin.AddItem("Desolate Space");
	M4A4Skin.AddItem("Buzz Kill");
	M4A4Skin.AddItem("Hellfire");
	WeaponGroup.PlaceLabledControl(2, "M4A4", this, &M4A4Skin);

	AUGSkin.SetFileId("BAIMLESS_config_skinchanger_aug_skin");
	AUGSkin.AddItem("Bengal Tiger");
	AUGSkin.AddItem("Hot Rod");
	AUGSkin.AddItem("Chameleon");
	AUGSkin.AddItem("Torque");
	AUGSkin.AddItem("Radiation Hazard");
	AUGSkin.AddItem("Asterion");
	AUGSkin.AddItem("Daedalus");
	AUGSkin.AddItem("Akihabara Accept");
	AUGSkin.AddItem("Ricochet");
	AUGSkin.AddItem("Fleet Flock");
	AUGSkin.AddItem("Syd Mead");
	AUGSkin.AddItem("Arctic Wolf");
	WeaponGroup.PlaceLabledControl(2, "AUG", this, &AUGSkin);

	FAMASSkin.SetFileId("BAIMLESS_config_skinchanger_famas_skin");
	FAMASSkin.AddItem("Djinn");
	FAMASSkin.AddItem("Afterimage");
	FAMASSkin.AddItem("Doomkitty");
	FAMASSkin.AddItem("Spitfire");
	FAMASSkin.AddItem("Teardown");
	FAMASSkin.AddItem("Hexane");
	FAMASSkin.AddItem("Pulse");
	FAMASSkin.AddItem("Sergeant");
	FAMASSkin.AddItem("Styx");
	FAMASSkin.AddItem("Neural Net");
	FAMASSkin.AddItem("Survivor");
	FAMASSkin.AddItem("Valence");
	FAMASSkin.AddItem("Roll Cage");
	FAMASSkin.AddItem("Mecha Industries");
	WeaponGroup.PlaceLabledControl(2, "FAMAS", this, &FAMASSkin);

	GALILSkin.SetFileId("BAIMLESS_config_skinchanger_galil_skin");
	GALILSkin.AddItem("Orange DDPAT");
	GALILSkin.AddItem("Eco");
	GALILSkin.AddItem("Stone Cold");
	GALILSkin.AddItem("Cerberus");
	GALILSkin.AddItem("Aqua Terrace");
	GALILSkin.AddItem("Chatterbox");
	GALILSkin.AddItem("Firefight");
	GALILSkin.AddItem("Rocket Pop");
	GALILSkin.AddItem("Sugar Rush");
	WeaponGroup.PlaceLabledControl(2, "GALIL", this, &GALILSkin);

	SG553Skin.SetFileId("BAIMLESS_config_skinchanger_sg552_skin");
	SG553Skin.AddItem("Bulldozer");
	SG553Skin.AddItem("Ultraviolet");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Pulse");
	SG553Skin.AddItem("Army Sheen");
	SG553Skin.AddItem("Traveler");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Cyrex");
	SG553Skin.AddItem("Tiger Moth");
	SG553Skin.AddItem("Atlas");
	SG553Skin.AddItem("Colony IV");
	WeaponGroup.PlaceLabledControl(2, "SG553", this, &SG553Skin);

	MAC10Skin.SetFileId("BAIMLESS_config_skinchanger_mac10_skin");
	MAC10Skin.AddItem("Fade");
	MAC10Skin.AddItem("Neon Rider");
	MAC10Skin.AddItem("Ultraviolet");
	MAC10Skin.AddItem("Palm");
	MAC10Skin.AddItem("Graven");
	MAC10Skin.AddItem("Tatter");
	MAC10Skin.AddItem("Amber Fade");
	MAC10Skin.AddItem("Heat");
	MAC10Skin.AddItem("Curse");
	MAC10Skin.AddItem("Indigo");
	MAC10Skin.AddItem("Commuter");
	MAC10Skin.AddItem("Nuclear Garden");
	MAC10Skin.AddItem("Malachite");
	MAC10Skin.AddItem("Rangeen");
	MAC10Skin.AddItem("Lapis Gator");
	MAC10Skin.AddItem("Stalker");
	WeaponGroup.PlaceLabledControl(2, "MAC-10", this, &MAC10Skin);

	P90Skin.SetFileId("BAIMLESS_config_skinchanger_p90_skin");
	P90Skin.AddItem("Cold Blooded");
	P90Skin.AddItem("Death by Kitty");
	P90Skin.AddItem("Shapewood");
	P90Skin.AddItem("Emerald Dragon");
	P90Skin.AddItem("Asiimov");
	P90Skin.AddItem("Virus");
	WeaponGroup.PlaceLabledControl(3, "P90", this, &P90Skin);

	UMP45Skin.SetFileId("BAIMLESS_config_skinchanger_ump45_skin");
	UMP45Skin.AddItem("Blaze");
	UMP45Skin.AddItem("Primal Saber");
	UMP45Skin.AddItem("Minotaurs Labyrinth");
	UMP45Skin.AddItem("Grand Prix");
	WeaponGroup.PlaceLabledControl(3, "UMP-45", this, &UMP45Skin);

	BIZONSkin.SetFileId("BAIMLESS_config_skinchanger_bizon_skin");
	BIZONSkin.AddItem("Blue Streak");
	BIZONSkin.AddItem("Antique");
	BIZONSkin.AddItem("Judgement of Anubis");
	BIZONSkin.AddItem("Osiris");
	BIZONSkin.AddItem("Fuel Rod");
	BIZONSkin.AddItem("Embargo");
	WeaponGroup.PlaceLabledControl(3, "PP-Bizon", this, &BIZONSkin);

	MP7Skin.SetFileId("BAIMLESS_config_skinchanger_mp7_skin");
	MP7Skin.AddItem("Whiteout");
	MP7Skin.AddItem("Nemesis");
	MP7Skin.AddItem("Impire");
	MP7Skin.AddItem("Neon Ply");
	WeaponGroup.PlaceLabledControl(3, "MP7", this, &MP7Skin);

	MP9Skin.SetFileId("BAIMLESS_config_skinchanger_mp9_skin");
	MP9Skin.AddItem("Ruby Poison Dart");
	MP9Skin.AddItem("Hot Rod");
	WeaponGroup.PlaceLabledControl(3, "MP9", this, &MP9Skin);

	GLOCKSkin.SetFileId("BAIMLESS_config_skinchanger_glock_skin");
	GLOCKSkin.AddItem("Wasteland Rebel");
	GLOCKSkin.AddItem("Twillight Galaxy");
	GLOCKSkin.AddItem("Fade");
	GLOCKSkin.AddItem("Dragon Tattoo");
	GLOCKSkin.AddItem("Water Elemental");
	GLOCKSkin.AddItem("Royal Legion");
	GLOCKSkin.AddItem("Grinder");
	GLOCKSkin.AddItem("Reactor");
	GLOCKSkin.AddItem("Weasel");
	WeaponGroup.PlaceLabledControl(3, "Glock", this, &GLOCKSkin);

	USPSSkin.SetFileId("BAIMLESS_config_skinchanger_usps_skin");
	USPSSkin.AddItem("Dark Water");;
	USPSSkin.AddItem("Overgrowth");
	USPSSkin.AddItem("Caiman");
	USPSSkin.AddItem("Blood Tiger");
	USPSSkin.AddItem("Serum");
	USPSSkin.AddItem("Neo-Noir");
	USPSSkin.AddItem("Stainless");
	USPSSkin.AddItem("Guardian");
	USPSSkin.AddItem("Orion");
	USPSSkin.AddItem("Road Rash");
	USPSSkin.AddItem("Royal Blue");
	USPSSkin.AddItem("Business Class");
	USPSSkin.AddItem("Para Green");
	USPSSkin.AddItem("Torque");
	USPSSkin.AddItem("Kill Confirmed");
	USPSSkin.AddItem("Lead Conduit");
	USPSSkin.AddItem("Cyrex");
	WeaponGroup.PlaceLabledControl(3, "USP-S", this, &USPSSkin);

	DEAGLESkin.SetFileId("BAIMLESS_config_skinchanger_deagle_skin");
	DEAGLESkin.AddItem("Blaze");
	DEAGLESkin.AddItem("Oxide Blaze");
	DEAGLESkin.AddItem("Kumicho Dragon");
	DEAGLESkin.AddItem("Sunset Storm");
	WeaponGroup.PlaceLabledControl(3, "Deagle", this, &DEAGLESkin);

	DUALSSkin.SetFileId("BAIMLESS_config_skinchanger_duals_skin");
	DUALSSkin.AddItem("Dualing Dragons");
	DUALSSkin.AddItem("Anodized Navy");
	DUALSSkin.AddItem("Duelist");
	DUALSSkin.AddItem("Hemoglobin");
	DUALSSkin.AddItem("Marina");
	DUALSSkin.AddItem("Urban Shock");
	DUALSSkin.AddItem("Balance");
	WeaponGroup.PlaceLabledControl(3, "Duals", this, &DUALSSkin);

	RevolverSkin.SetFileId("BAIMLESS_config_skinchanger_revolver_skin");
	RevolverSkin.AddItem("Fade");
	RevolverSkin.AddItem("Crimson Web");
	RevolverSkin.AddItem("Amber Fade");
	RevolverSkin.AddItem("Reboot");
	RevolverSkin.AddItem("Memento");
	WeaponGroup.PlaceLabledControl(4, "Revolver", this, &RevolverSkin);

	FIVESEVENSkin.SetFileId("BAIMLESS_config_skinchanger_fiveseven_skin");
	FIVESEVENSkin.AddItem("Case Hardened");
	FIVESEVENSkin.AddItem("Fowl Play");
	FIVESEVENSkin.AddItem("Retrobution");
	FIVESEVENSkin.AddItem("Triumvirate");
	FIVESEVENSkin.AddItem("Neon Kimono");
	FIVESEVENSkin.AddItem("Monkey Business");
	FIVESEVENSkin.AddItem("Copper Galaxy");
	FIVESEVENSkin.AddItem("Hyper Beast");
	WeaponGroup.PlaceLabledControl(4, "Five-Seven", this, &FIVESEVENSkin);

	TECNINESkin.SetFileId("BAIMLESS_config_skinchanger_tec9_skin");
	TECNINESkin.AddItem("Terrace");
	TECNINESkin.AddItem("Isaac");
	TECNINESkin.AddItem("Red Quartz");
	TECNINESkin.AddItem("Avalanche");
	TECNINESkin.AddItem("Toxic");
	TECNINESkin.AddItem("Fuel Injector");
	TECNINESkin.AddItem("Re-Entry");
	TECNINESkin.AddItem("Bamboo Forest");
	TECNINESkin.AddItem("Nuclear Threat");
	TECNINESkin.AddItem("Decimator");
	WeaponGroup.PlaceLabledControl(4, "Tec-9", this, &TECNINESkin);

	P2000Skin.SetFileId("BAIMLESS_config_skinchanger_p2000_skin");
	P2000Skin.AddItem("Handgun");
	P2000Skin.AddItem("Corticera");
	P2000Skin.AddItem("Ocean Foam");
	P2000Skin.AddItem("Fire Elemental");
	P2000Skin.AddItem("Imperial Dragon");
	P2000Skin.AddItem("Ocean Foam");
	P2000Skin.AddItem("Amber Fade");
	P2000Skin.AddItem("Obsidian");
	WeaponGroup.PlaceLabledControl(4, "P2000", this, &P2000Skin);

	P250Skin.SetFileId("BAIMLESS_config_skinchanger_p250_skin");
	P250Skin.AddItem("Whiteout");
	P250Skin.AddItem("Nuclear Threat");
	P250Skin.AddItem("Splash");
	P250Skin.AddItem("Mehndi");
	P250Skin.AddItem("Asiimov");
	P250Skin.AddItem("Undertow");
	P250Skin.AddItem("Franklin");
	P250Skin.AddItem("Supernova");
	P250Skin.AddItem("Cartel");
	WeaponGroup.PlaceLabledControl(4, "P250", this, &P250Skin);

}
*/
/*
void CPlayersTab::Setup()
{
	SetTitle("F");

	PlayerListControl.SetPosition(10, 5);
	PlayerListControl.SetSize(225, 569);
	PlayerListControl.SetHeightInItems(36);
	RegisterControl(&PlayerListControl);


	PlayerSettingsGroup.SetText(("Settings"));
	PlayerSettingsGroup.SetSize(225, 577); //569 - 577 = 8
	PlayerSettingsGroup.SetPosition(245, 5);
	RegisterControl(&PlayerSettingsGroup);

	PlayerSettingsGroup.PlaceLabledControl(0, ("Whitelist"), this, &PlayerFriendly);

	PlayerSettingsGroup.PlaceLabledControl(0, ("Prioritize body"), this, &PlayerPreferBody);
	/*
	PlayerSettingsGroup.PlaceLabledControl(0, ("Force pitch"), this, &PlayerForcePitch);

	PlayerForcePitch_Pitch.AddItem(("Eye angles"));
	PlayerForcePitch_Pitch.AddItem(("Up"));
	PlayerForcePitch_Pitch.AddItem(("Emotion"));
	PlayerSettingsGroup.PlaceLabledControl(0, (""), this, &PlayerForcePitch_Pitch);

	PlayerSettingsGroup.PlaceLabledControl(0, ("Force yaw"), this, &PlayerForceYaw);

	PlayerForceYaw_Yaw.AddItem(("Default"));
	PlayerForceYaw_Yaw.AddItem(("Correction"));
	PlayerForceYaw_Yaw.AddItem(("Eye angles"));
	PlayerForceYaw_Yaw.AddItem(("Sideways left"));
	PlayerForceYaw_Yaw.AddItem(("Sideways right"));
	PlayerForceYaw_Yaw.AddItem(("180°"));
	PlayerSettingsGroup.PlaceLabledControl(0, (""), this, &PlayerForceYaw_Yaw);

	


}
*/

void Options::SetupMenu()
{
	Menu.Setup();
	GUI.RegisterWindow(&Menu);
	//GUI.BindWindow(Options::Menu.MiscTab.MenuKey.GetKey(), &Menu);
}
void Options::DoUIFrame()
{
	//if (Menu.VisualsTab.FiltersAll.GetState())
	//{
	//Menu.VisualsTab.FiltersC4.SetState(true);
	//	Menu.VisualsTab.FiltersPlayers.SetState(true);
	//Menu.VisualsTab.FiltersWeapons.SetState(true);
	//	Menu.VisualsTab.FiltersNades.SetState(true);
	//}
	GUI.Update();
	GUI.Draw();

	//plist.paint();

}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class qgsvict {
public:
	string moqzvx;
	string lidtkorzcm;
	qgsvict();
	int ctuhaluitmdsj(string udicmzr, bool vppnospvncbzi, bool ldqmbs);
	bool gcshbrumbgm(string tlzvdltflntwyob, double ghsxmiuukls, bool zmjpqkceqg, bool pifxjrjpkzb, bool nniepgsoh, int iuasfemzqyeo, bool ucsddofsps);
	void xjfkpzguractmpguwami();
	double xcknsziwpikmkwzz(double rwdcwa, int slnogt, int exkeefpkp);
	bool xyqlfryliftrjnrjevr(string nibtxunrcvauhko, bool weinzidddjj);
	int xpcwiiokwlglxjgrizdle(int jwplwpncorfcf, double xnbhniqwztnbudk, double lsfpalamuqlgtw, double oaczkbws, int tiwgemxqcys, string zwkpzjup, double vhmnslfh, string jwqsoiupelh, bool mdkgcvmzn, bool fizcomwwcza);

protected:
	int jugjnktknlsux;
	bool mbnkvvnqsxeclq;
	int wtmgsirx;
	double fxurcjpgzkzrb;
	bool xmovr;

	int urvitqesyhzqzvwtbmdscjj(string iyzunjhfsnfow, string crssdugefuhtl, bool knbte, int prqpksgqeqyu);
	string lcnqijvcutfxtvre(double cvjiqnbd, double lxonvk, bool tjrwuuzosovuus, bool bhfrgqfd, string ympswqukzvzjgxa, double ewfmm);
	int pmiuxadzxodbhwzhsvomdx(string vpugmpvrytv, double uqzawmu, bool tnzapp, double soeweqeffgur, double ethsdxe, int svdbsvc, string bhgeo, bool dyaeedjjopgjp, bool behbxnf);

private:
	int jznesmlqyy;
	double vxrmfharqv;
	bool rtjdnotl;
	bool wnkbrcbyfrfnx;

	int wcpzndqgrre(string iozdrl, bool krgkfmkydocsymv);
	string lvmqibgadkcsbl(double xjkreoky, int jdirqfieumlnj, int ovpmwalyfxk, bool dgltgzlgra, bool ikdtygvojsrkci, string svklgbpu, double tnxaekss);
	void xfglenvafmmauqtvwkqlswz(string kqpytkml, string cumutrp, string dsyliiqij, int lbunswdzdj, double hfyqiwdszvqkn);
	double kduonrpkvtnnmudhtmvleok(string zyxarffdhzclrt, int aggdvjoopniv, string qfwhwegisiikmxi);
	void wjygwzkqsoagle(int tyxfjmlaffvz, double vzlzvdggu, bool gyzvbi, string trhynqunbc, int etgdglakxs, bool uaepgxkhkti, double iomthi);
	string stxdomclfzffiwjqsk(int znkfqzidjqgv, string tpgbagsmnzj, string cyaouxyohofxe, bool btvjmfkq);

};


int qgsvict::wcpzndqgrre(string iozdrl, bool krgkfmkydocsymv) {
	string jnwnpp = "ycjtzjvuaqtazhbylplnzhvmbjtimthkaedkjvyfrphzjqakijk";
	string ifecvl = "lxthomjflalxxcuzdurjchuglyfnveenlybctjvdjqyilnpynyzqihqzynulhyvtzhjcpotefouryunfjfn";
	int tjzvmopyww = 4969;
	int mysjwmxnduhpeht = 4511;
	string tfylnak = "adicjcbbguvblywooysulayyxdtwngdoebllzhmgqvsmhdqgrpdfa";
	string sycyuwitx = "hftniugqmomaftolsjkfiruqkedhzslhwey";
	return 82479;
}

string qgsvict::lvmqibgadkcsbl(double xjkreoky, int jdirqfieumlnj, int ovpmwalyfxk, bool dgltgzlgra, bool ikdtygvojsrkci, string svklgbpu, double tnxaekss) {
	string gcvniduvtnjyak = "xcksdrrfdyswaoixxnkboyxwizkgqdohpfntnbrhpgm";
	string dilcfcdm = "faocsrqrpfadcnyerevldnqtica";
	if (string("xcksdrrfdyswaoixxnkboyxwizkgqdohpfntnbrhpgm") != string("xcksdrrfdyswaoixxnkboyxwizkgqdohpfntnbrhpgm")) {
		int smkpfc;
		for (smkpfc = 75; smkpfc > 0; smkpfc--) {
			continue;
		}
	}
	if (string("xcksdrrfdyswaoixxnkboyxwizkgqdohpfntnbrhpgm") != string("xcksdrrfdyswaoixxnkboyxwizkgqdohpfntnbrhpgm")) {
		int kgawde;
		for (kgawde = 85; kgawde > 0; kgawde--) {
			continue;
		}
	}
	if (string("faocsrqrpfadcnyerevldnqtica") != string("faocsrqrpfadcnyerevldnqtica")) {
		int qthpeqvq;
		for (qthpeqvq = 48; qthpeqvq > 0; qthpeqvq--) {
			continue;
		}
	}
	if (string("faocsrqrpfadcnyerevldnqtica") != string("faocsrqrpfadcnyerevldnqtica")) {
		int pvojwc;
		for (pvojwc = 20; pvojwc > 0; pvojwc--) {
			continue;
		}
	}
	return string("iqlbjeojetqy");
}

void qgsvict::xfglenvafmmauqtvwkqlswz(string kqpytkml, string cumutrp, string dsyliiqij, int lbunswdzdj, double hfyqiwdszvqkn) {
	int mcizdb = 2057;
	string vqdxjyulglhthk = "ckioyitlfsgdcqwlyfehzznifgkzsyxkjpwafwzwwzeeglqkgtbeojynxlgmcrmwkiptzzlhogmokghy";
	bool bkzysg = false;
	double wiyqw = 4045;
	string rxuxsmmqdgde = "ihwcvohxkpealbumijmtoqfbptbszyhlsimjeuxtvoyofhwslqpgluxaarszzzdqejxopnjxpeyhaswhqyszdrztmqnuyntxuhx";
	string ziobox = "dkhz";
	int mpslgfspqbgo = 2879;
	string wipgbkkyifn = "nehwbsynquocrgssfidehlt";
	string qxlxyblllkxkk = "webzevonyjsfysaolylosrafxawqyhfixrmyihukfrvghtaabfza";
	if (string("nehwbsynquocrgssfidehlt") == string("nehwbsynquocrgssfidehlt")) {
		int fqqcw;
		for (fqqcw = 45; fqqcw > 0; fqqcw--) {
			continue;
		}
	}
	if (2879 != 2879) {
		int fzq;
		for (fzq = 57; fzq > 0; fzq--) {
			continue;
		}
	}
	if (string("ckioyitlfsgdcqwlyfehzznifgkzsyxkjpwafwzwwzeeglqkgtbeojynxlgmcrmwkiptzzlhogmokghy") != string("ckioyitlfsgdcqwlyfehzznifgkzsyxkjpwafwzwwzeeglqkgtbeojynxlgmcrmwkiptzzlhogmokghy")) {
		int itu;
		for (itu = 38; itu > 0; itu--) {
			continue;
		}
	}
	if (false != false) {
		int gfbbe;
		for (gfbbe = 31; gfbbe > 0; gfbbe--) {
			continue;
		}
	}

}

double qgsvict::kduonrpkvtnnmudhtmvleok(string zyxarffdhzclrt, int aggdvjoopniv, string qfwhwegisiikmxi) {
	double kmzwmmkbtbbrsa = 10364;
	string bvyyuo = "nhpbusgrehbwiskjqoclwtedwyhvpxtonlkgetcvrijzsywcokwupuwqivfbtraajqwbhrikuolfbborkfdfvxmpojltavrcqmg";
	bool wpknmeahkrvl = false;
	string fxfzpeongtxg = "ldyserwbdeaukrnvificpapyq";
	return 63942;
}

void qgsvict::wjygwzkqsoagle(int tyxfjmlaffvz, double vzlzvdggu, bool gyzvbi, string trhynqunbc, int etgdglakxs, bool uaepgxkhkti, double iomthi) {
	string jdckmkwqt = "xgccfjjbtbwixwaaindnbgoufbuohalhicchfpoyucpxjcyveglyaksmixgzlmlpwkihwhdjvxqyiavorayptfv";
	int snqqfpas = 6751;
	double ujerg = 20709;

}

string qgsvict::stxdomclfzffiwjqsk(int znkfqzidjqgv, string tpgbagsmnzj, string cyaouxyohofxe, bool btvjmfkq) {
	string kluip = "ynvzgsgtfyauklgsrzcykyaoeqaurngjumoidnbqgdli";
	int hgilorb = 9149;
	bool wugikulgeseuh = true;
	bool ybmnxgnzaeqzt = true;
	if (9149 != 9149) {
		int hrhuh;
		for (hrhuh = 45; hrhuh > 0; hrhuh--) {
			continue;
		}
	}
	if (string("ynvzgsgtfyauklgsrzcykyaoeqaurngjumoidnbqgdli") != string("ynvzgsgtfyauklgsrzcykyaoeqaurngjumoidnbqgdli")) {
		int zyhhzyrrnr;
		for (zyhhzyrrnr = 2; zyhhzyrrnr > 0; zyhhzyrrnr--) {
			continue;
		}
	}
	return string("ibednnjgcoccui");
}

int qgsvict::urvitqesyhzqzvwtbmdscjj(string iyzunjhfsnfow, string crssdugefuhtl, bool knbte, int prqpksgqeqyu) {
	bool lqteypoe = false;
	if (false == false) {
		int qsgw;
		for (qsgw = 78; qsgw > 0; qsgw--) {
			continue;
		}
	}
	if (false != false) {
		int ymymh;
		for (ymymh = 37; ymymh > 0; ymymh--) {
			continue;
		}
	}
	if (false == false) {
		int lvijfyuaj;
		for (lvijfyuaj = 60; lvijfyuaj > 0; lvijfyuaj--) {
			continue;
		}
	}
	if (false == false) {
		int aubrb;
		for (aubrb = 57; aubrb > 0; aubrb--) {
			continue;
		}
	}
	return 44398;
}

string qgsvict::lcnqijvcutfxtvre(double cvjiqnbd, double lxonvk, bool tjrwuuzosovuus, bool bhfrgqfd, string ympswqukzvzjgxa, double ewfmm) {
	double ohoeoblbrxyt = 86729;
	double jpvtnaov = 82736;
	double cepezmdoagby = 7861;
	double zmuemwisxgh = 26261;
	int funogeepm = 318;
	bool tsqnclhylewr = true;
	bool xhkyaa = true;
	string zamedvaylx = "hahaacrakpdtptbikrjrllyojaotkdohiuzipbqovrfurobfzvtwfkz";
	if (true != true) {
		int kuftjauwq;
		for (kuftjauwq = 30; kuftjauwq > 0; kuftjauwq--) {
			continue;
		}
	}
	if (318 == 318) {
		int mi;
		for (mi = 65; mi > 0; mi--) {
			continue;
		}
	}
	if (86729 != 86729) {
		int pdbtmrz;
		for (pdbtmrz = 33; pdbtmrz > 0; pdbtmrz--) {
			continue;
		}
	}
	return string("jin");
}

int qgsvict::pmiuxadzxodbhwzhsvomdx(string vpugmpvrytv, double uqzawmu, bool tnzapp, double soeweqeffgur, double ethsdxe, int svdbsvc, string bhgeo, bool dyaeedjjopgjp, bool behbxnf) {
	int pramrfddiajtv = 568;
	bool tpxuylfbzmgpozf = false;
	double qnerjrfwxcoxeew = 15299;
	bool fuqigaqhax = false;
	if (568 != 568) {
		int yfunnjk;
		for (yfunnjk = 95; yfunnjk > 0; yfunnjk--) {
			continue;
		}
	}
	if (false == false) {
		int irgee;
		for (irgee = 76; irgee > 0; irgee--) {
			continue;
		}
	}
	if (false != false) {
		int wxhp;
		for (wxhp = 88; wxhp > 0; wxhp--) {
			continue;
		}
	}
	return 73743;
}

int qgsvict::ctuhaluitmdsj(string udicmzr, bool vppnospvncbzi, bool ldqmbs) {
	int pugilwmzec = 3240;
	int tucrnjzwvjszywp = 2881;
	bool gyhaicxx = false;
	string pyrqrwv = "okdbotugruqmdi";
	string hvlene = "savkfifltlcqgrvrrjzeyrisramuktpxpqqzmyegznhmuxormnaidxuoraiwljbhvokeeaytxtqshvvkgsrcu";
	bool crppygomszfdc = true;
	if (false != false) {
		int jfjoajo;
		for (jfjoajo = 95; jfjoajo > 0; jfjoajo--) {
			continue;
		}
	}
	if (2881 != 2881) {
		int vwe;
		for (vwe = 88; vwe > 0; vwe--) {
			continue;
		}
	}
	return 95933;
}

bool qgsvict::gcshbrumbgm(string tlzvdltflntwyob, double ghsxmiuukls, bool zmjpqkceqg, bool pifxjrjpkzb, bool nniepgsoh, int iuasfemzqyeo, bool ucsddofsps) {
	string wldsaiscubh = "epfnpmvznpirsrzvjogcrlbcio";
	string ommjq = "xhgxzhqtpyuyeyseyegvfwszcwtnrpolnegunppeffrndtnsdukurahfpbvtweblbfu";
	string ltopo = "dyezhrykjeuxfgylynogartrlgcrgpkxkpqgcvmpvjgtjkljracjravrzxfvtpgukmnutswdepldntu";
	bool wcofrmxqehlp = false;
	return true;
}

void qgsvict::xjfkpzguractmpguwami() {
	string fhgxbog = "hirhnbnolcfhjoszkqzuhjodpqqxkdkxrlzskwgpdlybpytycnxsqaddulveypyohcqovxlgjsratjvwqnvomiwkcmbfrpar";
	string tcponqamthnm = "xsyb";
	int nbcdjsdam = 1194;
	int yfbymsz = 684;
	bool najtix = false;
	int faewn = 427;
	if (1194 != 1194) {
		int ua;
		for (ua = 84; ua > 0; ua--) {
			continue;
		}
	}
	if (string("xsyb") == string("xsyb")) {
		int dxr;
		for (dxr = 4; dxr > 0; dxr--) {
			continue;
		}
	}
	if (false == false) {
		int bpgbwzcj;
		for (bpgbwzcj = 27; bpgbwzcj > 0; bpgbwzcj--) {
			continue;
		}
	}
	if (false == false) {
		int lr;
		for (lr = 87; lr > 0; lr--) {
			continue;
		}
	}

}

double qgsvict::xcknsziwpikmkwzz(double rwdcwa, int slnogt, int exkeefpkp) {
	int bidelsqglwwvssg = 1930;
	string qlflmqetgd = "waeutdburrfhhbcamyyjdtmjjtswymy";
	double dnrwanjbisesi = 49392;
	if (49392 != 49392) {
		int yneanovt;
		for (yneanovt = 82; yneanovt > 0; yneanovt--) {
			continue;
		}
	}
	if (1930 == 1930) {
		int tbkjcjvfp;
		for (tbkjcjvfp = 31; tbkjcjvfp > 0; tbkjcjvfp--) {
			continue;
		}
	}
	if (49392 != 49392) {
		int vfq;
		for (vfq = 79; vfq > 0; vfq--) {
			continue;
		}
	}
	if (1930 != 1930) {
		int vtm;
		for (vtm = 76; vtm > 0; vtm--) {
			continue;
		}
	}
	if (string("waeutdburrfhhbcamyyjdtmjjtswymy") == string("waeutdburrfhhbcamyyjdtmjjtswymy")) {
		int gwcmrqx;
		for (gwcmrqx = 18; gwcmrqx > 0; gwcmrqx--) {
			continue;
		}
	}
	return 50483;
}

bool qgsvict::xyqlfryliftrjnrjevr(string nibtxunrcvauhko, bool weinzidddjj) {
	int mkhrf = 1321;
	string vhrguvo = "txecjzwebbzoevtwnpqwbfmrqeacuiwtixtxnmlfwyspdsidzouszmvbqmkknzgshxsrnrqccrjuphqbaw";
	string bszvllhlqjmve = "sylndfudxpobiplxenrdsmonplbggmpsxmsnnkfizyzlhunpiinozzfnhtpnijnfcelgcojlhsyldrpzdoqcjzofi";
	int gijhekmuhfqco = 2505;
	bool yorqgevz = false;
	int aidwiwhtokzdu = 2525;
	double yokey = 28639;
	int qgqwczisaw = 5199;
	double ezsrrefibqp = 16360;
	return true;
}

int qgsvict::xpcwiiokwlglxjgrizdle(int jwplwpncorfcf, double xnbhniqwztnbudk, double lsfpalamuqlgtw, double oaczkbws, int tiwgemxqcys, string zwkpzjup, double vhmnslfh, string jwqsoiupelh, bool mdkgcvmzn, bool fizcomwwcza) {
	double mydzmrfhxdfd = 7369;
	string cgspp = "uuxgftcqxtxprstyadrnrmgfhhximfndbhflnvb";
	double shfpkft = 3688;
	int qugldvftfog = 1055;
	string gxvfwzsyoxya = "yitpgwnrtcrzbuqhtpbbmghojsfcmrxeqppnjxkzbhyjhgkkdmqpiwtsycjmfugnpzzygpn";
	if (3688 != 3688) {
		int ewm;
		for (ewm = 19; ewm > 0; ewm--) {
			continue;
		}
	}
	if (string("yitpgwnrtcrzbuqhtpbbmghojsfcmrxeqppnjxkzbhyjhgkkdmqpiwtsycjmfugnpzzygpn") == string("yitpgwnrtcrzbuqhtpbbmghojsfcmrxeqppnjxkzbhyjhgkkdmqpiwtsycjmfugnpzzygpn")) {
		int fwleextwmx;
		for (fwleextwmx = 65; fwleextwmx > 0; fwleextwmx--) {
			continue;
		}
	}
	return 30755;
}

qgsvict::qgsvict() {
	this->ctuhaluitmdsj(string("rztdincyksljnhqqsuxuiguj"), true, true);
	this->gcshbrumbgm(string("ukzmzoboirvq"), 12466, true, true, true, 5575, true);
	this->xjfkpzguractmpguwami();
	this->xcknsziwpikmkwzz(11755, 5714, 4527);
	this->xyqlfryliftrjnrjevr(string("awrmsqrhu"), false);
	this->xpcwiiokwlglxjgrizdle(8001, 36046, 617, 61843, 682, string("porlzjlneoxosqpddxtnqipojbybyfiobbstocmasegbrnkknpev"), 17717, string("xtjtcknykjmvyhjlbivfgvengyjwjyndzyfvdigsyxladnzyapone"), false, true);
	this->urvitqesyhzqzvwtbmdscjj(string("kbwglrtfobwujcggjprphnuotazqpogznrcupxuayuebzxere"), string("ictxbxqxmeftpomwksgpogsibpklbqogxa"), false, 1353);
	this->lcnqijvcutfxtvre(58785, 77681, true, true, string("asquvhlkck"), 31707);
	this->pmiuxadzxodbhwzhsvomdx(string("xbyykdzgagmymvpywvjwaaxrxykbz"), 36955, false, 7813, 10543, 3294, string("no"), true, false);
	this->wcpzndqgrre(string("ifmniuduqjta"), false);
	this->lvmqibgadkcsbl(46856, 486, 1398, false, true, string("luwlsvryrubgywmovfrtwidguczmgttttyugurhmh"), 35580);
	this->xfglenvafmmauqtvwkqlswz(string("mdmjefdqfsjhipmcs"), string("jsixgzpcdxrnxzkxngvlinbeopriqurkjlfcwanjdh"), string("jbmiksrmdntycgde"), 2733, 62633);
	this->kduonrpkvtnnmudhtmvleok(string("bussrbjajcwvburflnzgkxudoctfxmzkxvtbqrglhbklpkwxqejtiprzyykxs"), 2083, string("wmjckblvpocnmfjorbrflpuuzuohhqlarurasdiowaxpscxqzdzfbrrspecucabinrfmqlbuwkkvbzidyjsey"));
	this->wjygwzkqsoagle(5589, 17486, false, string("aektzpcvdjiflbumjvuwdbzzfphumhbcayzfcspbki"), 402, false, 2541);
	this->stxdomclfzffiwjqsk(2093, string("llgxywhsdgkgekyenjgwutxrxjzzdwnqwuvwwxewgaufzodegnqroivizcudxdugnoyebqondhcvp"), string("mc"), true);
}
