#pragma once
#include "IClientEntity.h"

class IClientEntityList
{
public:
    virtual IClientNetworkable*   GetClientNetworkable(int entnum) = 0;
    virtual void*                 vtablepad0x1(void) = 0;
    virtual void*                 vtablepad0x2(void) = 0;
    virtual C_BaseEntity*         GetClientEntity(int entNum) = 0;
    virtual C_BaseEntity*         GetClientEntityFromHandle(CBaseHandle hEnt) = 0;
    virtual IClientEntity*        GetClientEntity2(int entNum) = 0;
    virtual IClientEntity*        GetClientEntityFromHandle2(CBaseHandle hEnt) = 0;
    virtual int                   NumberOfEntities(bool bIncludeNonNetworkable) = 0;
    virtual int                   GetHighestEntityIndex(void) = 0;
    virtual void                  SetMaxEntities(int maxEnts) = 0;
    virtual int                   GetMaxEntities() = 0;
};
extern IClientEntityList*  g_pEntityList;
