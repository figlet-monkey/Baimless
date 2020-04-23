#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include <deque>
#include "../../Singleton.h"
#include "..\..\SDK\IClientMode.h"

#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / g_pGlobalVars->intervalPerTick ) )
#define TICKS_TO_TIME( t )		( g_pGlobalVars->intervalPerTick *( t ) )

#define PI12 3.14159265358979323846f

inline Vector AngleV12ector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI12));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI12));

	auto sp = sin(meme.x / 180.f * static_cast<float>(PI12));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI12));

	return Vector(cp*cy, cp*sy, -sp);
}

inline float DistPointToLine(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}

struct lbyRecords
{
	int tick_count;
	float lby;
	Vector headPosition;

};
struct backtrackData
{
	float simtime;
	Vector hitboxPos;
	Vector origin;

};

class BackTrack
{
public:

	int latest_tick;
	bool IsTickValid(int tick);
	bool Lowerbody(int tick);
	void UpdateRecord(int i);
	void pejacofabacksadahdghasda();
	lbyRecords records[64];
	bool RunLBYBackTrack(int i, CUserCmd* cmd, Vector& aimPoint);
	void Update(int tick_count);
	void legitBackTrack(CUserCmd* cmd, C_BaseEntity* pLocal);
};

extern backtrackData headPositions[64][12];

extern BackTrack* backtracking;

struct PlayerRecords
{
	matrix3x4_t Matrix[128];
	float Velocity;
	EntityFlags m_flags;
	float SimTime;
	bool Shot;
	Vector m_abs_origin;
	Vector m_origin;
	Vector m_abs_ang;

	Vector m_vecOrigin;
};


class VarMapEntry_t
{
public:
	unsigned short type;
	unsigned short m_bNeedsToInterpolate; // Set to false when this var doesn't
										  // need Interpolate() called on it anymore.
	void* data;
	void* watcher;
};

struct VarMapping_t
{
	VarMapping_t()
	{
		m_nInterpolatedEntries = 0;
	}

	VarMapEntry_t* m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};
class LagComp
{
public:
	int ShotTick[65];
	std::deque<PlayerRecords> PlayerRecord[65] = {  };
	void StoreRecord(C_BaseEntity* pEnt);
	void ClearRecords(int i);
	static float get_lerp_time();
	static bool valid_simtime(const float& simtime);
	void apply(C_BaseEntity* player, bool backup = false) const;

	template<class T, class U>
	T clamp(T in, U low, U high);

	void DisableInterpolation(C_BaseEntity* pEntity)
	{
		VarMapping_t* map = GetVarMap(pEntity);
		if (!map) return;
		for (int i = 0; i < map->m_nInterpolatedEntries; i++)
		{
			VarMapEntry_t* e = &map->m_Entries[i];
			e->m_bNeedsToInterpolate = false;
		}
	}
private:
	VarMapping_t* GetVarMap(void* pBaseEntity)
	{
		return reinterpret_cast<VarMapping_t*>((DWORD)pBaseEntity + 0x24);
	}
};
extern LagComp g_LagComp;
