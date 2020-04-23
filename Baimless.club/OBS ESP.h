#pragma once

#include "Singleton.h"
#include "SDK/CEntity.h"
#include "Render.h"





class Visuals : public Singleton<Visuals>
{
    friend class Singleton<Visuals>;

    CRITICAL_SECTION cs;

    Visuals();
    ~Visuals();
public:
    class Player
    {
    public:
        struct
        {
            C_BaseEntity* pl;
            bool          is_enemy;
            bool          is_visible;
            Color         clr;
            Vector        head_pos;
            Vector        feet_pos;
            RECT          bbox;
        } ctx;

        bool Begin(C_BaseEntity* pl);
        void RenderBox();
        void RenderName();
        void RenderWeaponName();
        void RenderHealth();
        void RenderArmour();
        void RenderSnapline();
    };
    void RenderCrosshair();
    void RenderWeapon(C_BaseCombatWeapon* ent);
    void RenderDefuseKit(C_BaseEntity* ent);
    void RenderPlantedC4(C_BaseEntity* ent);
    void RenderItemEsp(C_BaseEntity* ent);
    void ThirdPerson();
public:
    void AddToDrawList();
    void Render();
};
