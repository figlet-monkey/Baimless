#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "..\SDK\Recv.h"

struct inputdata_t;
typedef enum _fieldtypes
{
	FIELD_VOID = 0,			// No type or value
	FIELD_FLOAT,			// Any floating point value
	FIELD_STRING,			// A string ID (return from ALLOC_STRING)
	FIELD_VECTOR,			// Any vector, QAngle, or AngularImpulse
	FIELD_QUATERNION,		// A quaternion
	FIELD_INTEGER,			// Any integer or enum
	FIELD_BOOLEAN,			// boolean, implemented as an int, I may use this as a hint for compression
	FIELD_SHORT,			// 2 byte integer
	FIELD_CHARACTER,		// a byte
	FIELD_COLOR32,			// 8-bit per channel r,g,b,a (32bit color)
	FIELD_EMBEDDED,			// an embedded object with a datadesc, recursively traverse and embedded class/structure based on an additional typedescription
	FIELD_CUSTOM,			// special type that contains function pointers to it's read/write/parse functions

	FIELD_CLASSPTR,			// CBaseEntity *
	FIELD_EHANDLE,			// Entity handle
	FIELD_EDICT,			// edict_t *

	FIELD_POSITION_VECTOR,	// A world coordinate (these are fixed up across level transitions automagically)
	FIELD_TIME,				// a floating point time (these are fixed up automatically too!)
	FIELD_TICK,				// an integer tick count( fixed up similarly to time)
	FIELD_MODELNAME,		// Engine string that is a model name (needs precache)
	FIELD_SOUNDNAME,		// Engine string that is a sound name (needs precache)

	FIELD_INPUT,			// a list of inputed data fields (all derived from CMultiInputVar)
	FIELD_FUNCTION,			// A class function pointer (Think, Use, etc)

	FIELD_VMATRIX,			// a vmatrix (output coords are NOT worldspace)

							// NOTE: Use float arrays for local transformations that don't need to be fixed up.
							FIELD_VMATRIX_WORLDSPACE,// A VMatrix that maps some local space to world space (translation is fixed up on level transitions)
							FIELD_MATRIX3X4_WORLDSPACE,	// matrix3x4_t that maps some local space to world space (translation is fixed up on level transitions)

							FIELD_INTERVAL,			// a start and range floating point interval ( e.g., 3.2->3.6 == 3.2 and 0.4 )
							FIELD_MODELINDEX,		// a model index
							FIELD_MATERIALINDEX,	// a material index (using the material precache string table)

							FIELD_VECTOR2D,			// 2 floats

							FIELD_TYPECOUNT,		// MUST BE LAST
} fieldtype_t;

class ISaveRestoreOps;
class C_BaseEntity;
//
// Function prototype for all input handlers.
//
typedef void (C_BaseEntity::*inputfunc_t)(inputdata_t &data);


struct datamap_t;
class typedescription_t;
enum
{
	TD_OFFSET_NORMAL = 0,
	TD_OFFSET_PACKED = 1,

	// Must be last
	TD_OFFSET_COUNT,
};
class typedescription_t
{
public:
	int32_t fieldType; //0x0000
	char* fieldName; //0x0004
	int fieldOffset[TD_OFFSET_COUNT]; //0x0008
	int16_t fieldSize_UNKNWN; //0x0010
	int16_t flags_UNKWN; //0x0012
	char pad_0014[12]; //0x0014
	datamap_t* td; //0x0020
	char pad_0024[24]; //0x0024
}; //Size: 0x003C
struct datamap_t
{
	typedescription_t    *dataDesc;
	int                    dataNumFields;
	char const            *dataClassName;
	datamap_t            *baseMap;

	bool                chains_validated;
	// Have the "packed" offsets been computed
	bool                packed_offsets_computed;
	int                    packed_size;
};

#ifdef _DEBUG
#define FUNCTION_GUARD Global::szLastFunction = __FUNCTION__
#else
#define FUNCTION_GUARD std::runtime_error("ERROR! Module shipped in release with debug information.\n");
#endif
namespace OFFSETS
{
	extern uintptr_t m_iHealth;
	extern uintptr_t m_fFlags;
	extern uintptr_t m_vecVelocity;
	extern uintptr_t m_flLowerBodyYawTarget;
	extern uintptr_t deadflag;
	extern uintptr_t m_vecOrigin;
	extern uintptr_t m_iTeamNum;
	extern uintptr_t m_flDuckAmount;
	extern uintptr_t m_nTickBase;
	extern uintptr_t animstate;
	extern uintptr_t m_angEyeAngles;
	extern uintptr_t m_flSimulationTime;
	extern uintptr_t m_vecViewOffset;
	extern uintptr_t m_dwBoneMatrix;
	extern uintptr_t m_bGunGameImmunity;
	extern uintptr_t m_flPoseParameter;
	extern uintptr_t m_flNextPrimaryAttack;
	extern uintptr_t m_flNextAttack;
	extern uintptr_t m_ArmorValue;
	extern uintptr_t m_bHasHelmet;
	extern uintptr_t m_bIsScoped;
	extern uintptr_t m_iPlayerC4;
	extern uintptr_t dwPlayerResource;
	extern uintptr_t m_iItemDefinitionIndex;
	extern uintptr_t m_flPostponeFireReadyTime;
	extern uintptr_t invalidateBoneCache;

	extern uintptr_t m_bPinPulled;
	extern uintptr_t m_MoveType;
	extern uintptr_t m_iItemIDHigh;
	extern uintptr_t m_iEntityQuality;
	extern uintptr_t m_flFallbackWear;
	extern uintptr_t m_nFallbackPaintKit;
	extern uintptr_t m_hMyWearables;
	extern uintptr_t m_nModelIndex;
	extern uintptr_t m_iViewModelIndex;
	extern uintptr_t m_hWeaponWorldModel;
	extern uintptr_t m_OriginalOwnerXuidLow;
	extern uintptr_t m_OriginalOwnerXuidHigh;
	extern uintptr_t m_iClip1;
	extern uintptr_t animlayer;
	extern uintptr_t m_nFallbackSeed;

	void InitOffsets();
}

//Forward declaration. The class is defined on the cpp file
struct NetvarTable;
class NetvarDatabase;

using fnv_t = unsigned;

class FnvHash
{
	static const fnv_t FNV_PRIME = 16777619u;
	static const fnv_t OFFSET_BASIS = 2166136261u;

	template <unsigned int N>
	static constexpr fnv_t fnvHashConst(const char(&str)[N], unsigned int I = N)
	{
		return static_cast<fnv_t>(1ULL * (I == 1 ? (OFFSET_BASIS ^ str[0]) : (fnvHashConst(str, I - 1) ^ str[I - 1])) * FNV_PRIME);
	}

	static fnv_t fnvHash(const char* str)
	{
		const auto length = strlen(str) + 1;
		auto hash = OFFSET_BASIS;
		for (size_t i = 0; i < length; ++i)
		{
			hash ^= *str++;
			hash *= FNV_PRIME;
		}
		return hash;
	}

	struct Wrapper
	{
		Wrapper(const char* str) : str(str) { }
		const char* str;
	};

	fnv_t hash_value;
public:
	FnvHash(Wrapper wrapper) : hash_value(fnvHash(wrapper.str)) { }

	template <unsigned int N>
	constexpr FnvHash(const char(&str)[N]) : hash_value(fnvHashConst(str)) { }

	constexpr operator fnv_t() const { return this->hash_value; }
};

class NetvarManager {
private:
	static NetvarManager* instance;

	NetvarManager();
	~NetvarManager();

	NetvarManager(const NetvarManager&) = delete;

public:
	static NetvarManager* Instance() {
		if (!instance) instance = new NetvarManager;
		return instance;
	}

	void CreateDatabase();
	void DestroyDatabase();

	//////////////////////
	void Dump(std::ostream& stream) const;
	static RecvTable* GetTable(std::vector<RecvTable*> tables, const char* tableName);
	int GetProp2(std::vector<RecvTable*> tables, const char* tableName, const char* propName, RecvProp** prop) const;
	int GetProp2(std::vector<RecvTable*> tables, RecvTable* recvTable, const char* propName, RecvProp** prop) const;
	static std::vector<RecvTable*> GetTables();
	bool HookProp(const char* tableName, const char* propName, RecvVarProxyFn f) const;
	////////////////////


	void Dump(const std::string& file) const;

	int GetNetvarCount() const { return m_netvarCount; }
	int GetTableCount() const { return m_tableCount; }

	template<typename ...Args>
	uint32_t GetOffset(const std::string& szTableName, Args&&... args) {
		return GetOffset(szTableName, { std::forward<Args>(args)... });
	}

	unsigned int FindInDataMap(datamap_t *pMap, const char *name)
	{
		//FUNCTION_GUARD;

		while (pMap)
		{
			for (int i = 0; i < pMap->dataNumFields; i++)
			{
				if (pMap->dataDesc[i].fieldName == NULL)
					continue;

				if (strcmp(name, pMap->dataDesc[i].fieldName) == 0)
					return pMap->dataDesc[i].fieldOffset[TD_OFFSET_NORMAL];

				if (pMap->dataDesc[i].fieldType == FIELD_EMBEDDED)
				{
					if (pMap->dataDesc[i].td)
					{
						unsigned int offset;

						if ((offset = FindInDataMap(pMap->dataDesc[i].td, name)) != 0)
							return offset;
					}
				}
			}
			pMap = pMap->baseMap;
		}

		return 0;
	}


	uint32_t GetOffset(const std::string& szTableName, const std::initializer_list<std::string>& props) const;

	template<typename ...Args>
	static void HookVar(RecvVarProxyFn& OriginalFunction, void* Hooked_Func, const std::string& szTableName, Args&&... args)
	{
		HookVar(OriginalFunction, Hooked_Func, szTableName, { std::forward<Args>(args)... });
	}

	static void HookVar(RecvVarProxyFn& OriginalFunction, void* Hooked_Func, const std::string& szTableName, const std::initializer_list<std::string>& props);
	static void UnhookRecvProxies();

private:
	std::unique_ptr<NetvarTable> InternalLoadTable(RecvTable* pRecvTable, uint32_t offset);
	void Dump(std::ostream& output, NetvarTable& table, int level) const;

	std::unique_ptr<NetvarDatabase>    m_pDatabase = nullptr;
	uint32_t                           m_tableCount = 0;
	uint32_t                           m_netvarCount = 0;
};


#define GET_NETVAR(table, ...) NetvarManager::Instance()->GetOffset(table, __VA_ARGS__)
#define HOOK_NETVAR(OriginalFunction, Hooked_Func, table, ...) NetvarManager::HookVar(OriginalFunction, Hooked_Func, table, __VA_ARGS__)
