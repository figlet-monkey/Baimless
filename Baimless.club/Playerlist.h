#pragma once

#include "Utils\Interfaces.h"
#include "newMenu.h"
#include <iostream>
/////////////
extern double PlistArray[102][200];

class CPlayer
{
public:
	void Add_Player(C_BaseEntity *Ent)
	{
		entity = Ent;
		if (!entity)
			index = -1;
		else
			index = entity->EntIndex();
		g_pEngine->GetPlayerInfo(index, info);
		name = info->szName;
		is_valid_player = true;
	}

	void Reset_Player()
	{
		is_valid_player = false;
		name = "unamed";
		selected = false;
		resolver_data.last_hit_angle = Vector(0, 0, 0);
		resolver_data.has_hit_angle = false;
		resolver_data.missed_shots = 0;
		resolver_data.shots = 0;
		resolver_data.time_at_update = 0;
		resolver_data.stored_lby = 0;
		resolver_data.temp = 0;
		resolver_data.old_lowerbody_yaws = 0;
		resolver_data.resolved_yaw = 0;
		resolver_data.old_yaw_deltas = 0;
		resolver_data.newer_stored_lby = 0;
		resolver_data.stored_lby_two = 0;
		resolver_data.lowerbody_is_updated = false;
		possible_spinspeed = 0;
		dormancy_data.initialized = false;
		dormancy_data.last_health = 0;
		dormancy_data.last_armor = 0;
		dormancy_data.last_ammo = 0;
		dormancy_data.last_team = 0;
		dormancy_data.last_visible = false;

		last_origin = Vector(0, 0, 0);

		Friendly = false;
		PreferBodyAim = false;
		ForcePitch = false;
		ForcePitch_Pitch = 0;
		ForceYaw = false;
		ForceYaw_Yaw = 0;

		aimbot_bone = 0;

		if (index != -1) {
			for (int i = 0; i < 199; i++)
			{
				PlistArray[index][i] = 0;
			}
		}
		index = -1;
	}
public:
	int index = -1;
	C_BaseEntity* entity;
	PlayerInfo_t* info = new PlayerInfo_t;
	char* name = "unamed";
	bool is_valid_player = true;
	bool selected = false;
	float possible_spinspeed = 0;
	class DormanyData
	{
	public:
		float last_health = 0;
		float last_armor = 0;
		Vector last_min;
		Vector last_max;
		Vector last_origin;
		C_BaseCombatWeapon* last_weapon;
		int last_ammo = 0;
		int last_team = 0;
		bool last_visible = false;
		bool initialized = true;
	};
	DormanyData dormancy_data;

	class ResolverData
	{
	public:
		Vector last_hit_angle = Vector(0, 0, 0);
		bool has_hit_angle = false;
		int missed_shots = 0;
		int shots = 0;
		float time_at_update;
		float stored_lby;
		float temp;
		float old_lowerbody_yaws;
		float resolved_yaw;
		float old_yaw_deltas;
		float newer_stored_lby;
		float stored_lby_two;
		bool lowerbody_is_updated;
	};
	ResolverData resolver_data;

	Vector last_origin = Vector(0, 0, 0);

	bool Friendly = false;
	bool PreferBodyAim = false;
	bool ForcePitch = false;
	int ForcePitch_Pitch = 0;
	bool ForceYaw = false;
	int ForceYaw_Yaw = 0;

	int aimbot_bone = 0;
};
/*
class CPlist
{
private:
	bool Setup = false;
	CPlayer Players[100];
	int previous_player = 0;
public:
	int selected_player = 0;
public:
	void update()
	{
		if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
		{
			C_BaseEntity* me = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

			for (int i = 0; i < 99; i++)
			{
				C_BaseEntity* Ent = g_pEntityList->GetClientEntity(i);
				if (!Ent) { Players[i].Reset_Player(); continue; }

				if (!Ent->IsPlayer())
				{
					Players[i].Reset_Player(); continue;
				}

				if (Ent == me) { Players[i].Reset_Player(); continue; }
				Players[i].Add_Player(Ent);
			}

			Setup = true;
		}
		else if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame())
		{
			for (int i = 0; i < std::size(Players); i++)
			{
				Players[i].Reset_Player();
			}
		}
	}

	void reset()
	{
		for (int i = 0; i < 99; i++)
		{
			Players[i].Reset_Player();
		}
	}

	CPlayer* get_player(int i)
	{
		return &Players[i];
	}

	void paint()
	{
		static bool initialized_menu = false;
		if (!initialized_menu)
		{
			Options::Menu.PlayersTab.PlayerFriendly.SetState(false);
			Options::Menu.PlayersTab.PlayerPreferBody.SetState(false);
			Options::Menu.PlayersTab.PlayerForcePitch.SetState(false);
			Options::Menu.PlayersTab.PlayerForceYaw.SetState(false);
			initialized_menu = true;
		}

		if (g_pEngine->IsInGame() && g_pEngine->IsConnected())
		{
			Options::Menu.PlayersTab.PlayerListControl.ClearItems();
			std::vector <CPlayer*> draw_list;
			for (int i = 0; i < 99; i++)
			{
				if (Players[i].is_valid_player)
				{
					draw_list.push_back(&Players[i]);
				}
			}

			int players = 0;
			for (CPlayer* Player : draw_list)
			{
				std::string playername = "";
				for (int i = 0; i < 24; i++)
				{
					playername += std::string(Player->name)[i];
				}

				Options::Menu.PlayersTab.PlayerListControl.AddItem(playername, Player->index);

				if (Player->index == Options::Menu.PlayersTab.PlayerListControl.GetValue()) Player->selected = true;
				else Player->selected = false;

				players++;

				if (Player->selected) selected_player = Player->entity->EntIndex();
			}

			CPlayer* Player = get_player(Options::Menu.PlayersTab.PlayerListControl.GetValue());
			if (previous_player != Options::Menu.PlayersTab.PlayerListControl.GetValue())
			{
				Options::Menu.PlayersTab.PlayerFriendly.SetState(Player->Friendly);
				Options::Menu.PlayersTab.PlayerPreferBody.SetState(Player->PreferBodyAim);
				Options::Menu.PlayersTab.PlayerForcePitch.SetState(Player->ForcePitch);
				Options::Menu.PlayersTab.PlayerForcePitch_Pitch.SetIndex(Player->ForcePitch_Pitch);
				Options::Menu.PlayersTab.PlayerForceYaw.SetState(Player->ForceYaw);
				Options::Menu.PlayersTab.PlayerForceYaw_Yaw.SetIndex(Player->ForceYaw_Yaw);
				previous_player = Options::Menu.PlayersTab.PlayerListControl.GetValue();
			}
			Player->Friendly = Options::Menu.PlayersTab.PlayerFriendly.GetState();
			Player->PreferBodyAim = Options::Menu.PlayersTab.PlayerPreferBody.GetState();
			Player->ForcePitch = Options::Menu.PlayersTab.PlayerForcePitch.GetState();
			Player->ForcePitch_Pitch = Options::Menu.PlayersTab.PlayerForcePitch_Pitch.GetIndex();
			Player->ForceYaw = Options::Menu.PlayersTab.PlayerForceYaw.GetState();
			Player->ForceYaw_Yaw = Options::Menu.PlayersTab.PlayerForceYaw_Yaw.GetIndex();
		}
	}
};*/

//extern CPlist plist;

//////