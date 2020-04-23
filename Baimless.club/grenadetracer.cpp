#include "grenadetracer.h"
#include "Utils/GlobalVars.h"
#include "SDK/ISurface.h"
#include "Utils/Math.h"
#include "SDK/IEngineTrace.h"
#include "SDK/IClientMode.h"
#include "SDK/CGlobalVarsBase.h"
#include "assert.h"
#include "newMenu.h"
#include "../Baimless.club/Options.hpp"

bool ExplectedWorldToScrean(Vector vOrigin, Vector& vScreen)
{
	float w = g::w2s_matrix[3][0] * vOrigin.x + g::w2s_matrix[3][1] * vOrigin.y + g::w2s_matrix[3][2] * vOrigin.z + g::w2s_matrix[3][3];
	int x, y;
	g_pEngine->GetScreenSize(x, y);
	float ScreenWidth = (float)x;
	float ScreenHeight = (float)y;

	if (w > 0.01f)
	{
		float inverseWidth = 1.f / w;
		vScreen.x = (float)((ScreenWidth / 2) + (0.5 * ((g::w2s_matrix[0][0] * vOrigin.x + g::w2s_matrix[0][1] * vOrigin.y + g::w2s_matrix[0][2] * vOrigin.z + g::w2s_matrix[0][3]) * inverseWidth) * ScreenWidth + 0.5));
		vScreen.y = (float)((ScreenHeight / 2) - (0.5 * ((g::w2s_matrix[1][0] * vOrigin.x + g::w2s_matrix[1][1] * vOrigin.y + g::w2s_matrix[1][2] * vOrigin.z + g::w2s_matrix[1][3]) * inverseWidth) * ScreenHeight + 0.5));
		return true;
	}

	return false;
}

void DrawF(int X, int Y, unsigned int Font, bool center_width, bool center_height, Color Color, std::string Input)
{
	/* char -> wchar */
	size_t size = Input.size() + 1;
	auto wide_buffer = std::make_unique<wchar_t[]>(size);
	mbstowcs_s(0, wide_buffer.get(), size, Input.c_str(), size - 1);

	/* check center */
	int width = 0, height = 0;
	g_pSurface->GetTextSize(Font, wide_buffer.get(), width, height);
	if (!center_width)
		width = 0;
	if (!center_height)
		height = 0;

	/* call and draw*/
	g_pSurface->DrawSetTextColor(Color);
	g_pSurface->DrawSetTextFont(Font);
	g_pSurface->DrawSetTextPos(X - (width * .5), Y - (height * .5));
	g_pSurface->DrawPrintText(wide_buffer.get(), wcslen(wide_buffer.get()));
}


void GrenadePrediction::Tick(int buttons)
{
	bool in_attack = buttons & IN_ATTACK, in_attack2 = buttons & IN_ATTACK2;

	if (g::pLocalEntity && g::pLocalEntity->GetHealth() > 0)
	{
		act = (in_attack && in_attack2) ? ACT_LOB :
			(in_attack2) ? ACT_DROP :
			(in_attack) ? ACT_THROW :
			ACT_NONE;
	}
}
bool IsGrenade(C_BaseCombatWeapon* weapon)
{
	auto WeaponIDz = *weapon->fixskins();
	if (WeaponIDz == 44 || WeaponIDz == 48 ||
		WeaponIDz == 46 || WeaponIDz == 43 ||
		WeaponIDz == 47 || WeaponIDz == 45)
		return true;
	else
		return false;
}

void GrenadePrediction::View(CViewSetup* setup)
{

	if (g::pLocalEntity && g::pLocalEntity->GetHealth() > 0)
	{
		auto weapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntity(g::pLocalEntity->GetActiveWeaponIndex()));
		if (!weapon) return;
		if (IsGrenade(weapon))
		{
			type = *weapon->fixskins();
			Simulate(setup);
		}
		else
			type = 0;
	}
}
void DrawLine(int x1, int y1, int x2, int y2, Color color)
{
	g_pSurface->DrawSetColor(color);
	g_pSurface->Line(x1, y1, x2, y2, color);
}
void DrawFilled3DBox(Vector origin, int width, int height, Color outline, Color filling)
{
	float difw = float(width / 2);
	float difh = float(height / 2);
	Vector boxVectors[8] =
	{
		Vector(origin.x - difw, origin.y - difh, origin.z - difw),
		Vector(origin.x - difw, origin.y - difh, origin.z + difw),
		Vector(origin.x + difw, origin.y - difh, origin.z + difw),
		Vector(origin.x + difw, origin.y - difh, origin.z - difw),
		Vector(origin.x - difw, origin.y + difh, origin.z - difw),
		Vector(origin.x - difw, origin.y + difh, origin.z + difw),
		Vector(origin.x + difw, origin.y + difh, origin.z + difw),
		Vector(origin.x + difw, origin.y + difh, origin.z - difw),
	};

	static Vector vec0, vec1, vec2, vec3,
		vec4, vec5, vec6, vec7;

	if (ExplectedWorldToScrean(boxVectors[0], vec0) &&
		ExplectedWorldToScrean(boxVectors[1], vec1) &&
		ExplectedWorldToScrean(boxVectors[2], vec2) &&
		ExplectedWorldToScrean(boxVectors[3], vec3) &&
		ExplectedWorldToScrean(boxVectors[4], vec4) &&
		ExplectedWorldToScrean(boxVectors[5], vec5) &&
		ExplectedWorldToScrean(boxVectors[6], vec6) &&
		ExplectedWorldToScrean(boxVectors[7], vec7))
	{
		Vector lines[12][2];
		lines[0][0] = vec0;
		lines[0][1] = vec1;
		lines[1][0] = vec1;
		lines[1][1] = vec2;
		lines[2][0] = vec2;
		lines[2][1] = vec3;
		lines[3][0] = vec3;
		lines[3][1] = vec0;

		// top of box
		lines[4][0] = vec4;
		lines[4][1] = vec5;
		lines[5][0] = vec5;
		lines[5][1] = vec6;
		lines[6][0] = vec6;
		lines[6][1] = vec7;
		lines[7][0] = vec7;
		lines[7][1] = vec4;

		lines[8][0] = vec0;
		lines[8][1] = vec4;

		lines[9][0] = vec1;
		lines[9][1] = vec5;

		lines[10][0] = vec2;
		lines[10][1] = vec6;

		lines[11][0] = vec3;
		lines[11][1] = vec7;

		for (int i = 0; i < 12; i++)
			DrawLine(lines[i][0].x, lines[i][0].y, lines[i][1].x, lines[i][1].y, outline);
	}
}
void GrenadePrediction::Paint()
{
	if (!g::pLocalEntity) return;
	if (g::pLocalEntity->GetHealth() > 0)
	{
		if ((type) && path.size() > 1)
		{
			Vector nadeStart, nadeEnd;
			Vector prev = path[0];

			auto weapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntity(g::pLocalEntity->GetActiveWeaponIndex()));
			if (!weapon) return;
			auto WeaponIDz = *weapon->fixskins();

			for (auto it = path.begin(), end = path.end(); it != end; ++it)
			{
				if (ExplectedWorldToScrean(prev, nadeStart) && ExplectedWorldToScrean(*it, nadeEnd))
					DrawLine((int)nadeStart.x, (int)nadeStart.y, (int)nadeEnd.x, (int)nadeEnd.y, Color(255, 255, 255));
				prev = *it;
			}
			for (auto it = others.begin(), end = others.end(); it != end; ++it)
				DrawFilled3DBox(it->first, 5, 5, Color(esp::colors::grenade_prediction), Color(esp::colors::grenade_prediction));

			//RENDER::DrawFilled3DBox(others.rbegin()->first, 4, 4, CColor(255, 0, 0, 200), CColor(0, 135, 255, 200));
			if (ExplectedWorldToScrean(prev, nadeEnd))
			{
				if (WeaponIDz == 43)
					DrawF((int)nadeEnd.x - 5, (int)nadeEnd.y, g::grenadefont, true, true, Color(255, 255, 255, 255), "i");
				else if (WeaponIDz == 44)
					DrawF((int)nadeEnd.x - 5, (int)nadeEnd.y, g::grenadefont, true, true, Color(255, 255, 255, 255), "j");
				else if (WeaponIDz == 48)
					DrawF((int)nadeEnd.x - 5, (int)nadeEnd.y, g::grenadefont, true, true, Color(255, 255, 255, 255), "n");
				else if (WeaponIDz == 46)
					DrawF((int)nadeEnd.x - 5, (int)nadeEnd.y, g::grenadefont, true, true, Color(255, 255, 255, 255), "l");
				else if (WeaponIDz == 47)
					DrawF((int)nadeEnd.x - 5, (int)nadeEnd.y, g::grenadefont, true, true, Color(255, 255, 255, 255), "m");
				else if (WeaponIDz == 45)
					DrawF((int)nadeEnd.x - 5, (int)nadeEnd.y, g::grenadefont, true, true, Color(255, 255, 255, 255), "k");
			}
		}
	}
}

void GrenadePrediction::Setup(Vector& vecSrc, Vector& vecThrow, Vector viewangles)
{

	if (!g::pLocalEntity) return;
	Vector angThrow = viewangles;
	float pitch = angThrow.x;

	if (pitch <= 90.0f)
	{
		if (pitch < -90.0f)
			pitch += 360.0f;
	}
	else
		pitch -= 360.0f;

	float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
	angThrow.x = a;

	float flVel = 750.0f * 0.9f;
	static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float b = power[act];
	b = b * 0.7f; b = b + 0.3f;
	flVel *= b;

	Vector vForward, vRight, vUp;
	g_Math.AngleVectors(angThrow, &vForward, &vRight, &vUp);

	vecSrc = g::pLocalEntity->get_eye_pos();
	float off = (power[act] * 12.0f) - 12.0f;
	vecSrc.z += off;

	C_Trace tr;
	Vector vecDest = vecSrc;
	vecDest += vForward * 22.0f;

	TraceHull(vecSrc, vecDest, tr);

	Vector vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.end;
	vecSrc -= vecBack;

	vecThrow = g::pLocalEntity->GetVelocity(); vecThrow *= 1.25f;
	vecThrow += vForward * flVel;
}

void GrenadePrediction::Simulate(CViewSetup* setup)
{
	Vector vecSrc, vecThrow;
	Vector angles; g_pEngine->GetViewAngles(angles);
	Setup(vecSrc, vecThrow, angles);

	float interval = g_pGlobalVars->intervalPerTick;
	int logstep = (int)(0.05f / interval);
	int logtimer = 0;

	path.clear(); others.clear();
	for (unsigned int i = 0; i < path.max_size() - 1; ++i)
	{
		if (!logtimer) path.push_back(vecSrc);

		int s = Step(vecSrc, vecThrow, i, interval);
		if ((s & 1)) break;
		if ((s & 2) || logtimer >= logstep) logtimer = 0;
		else ++logtimer;
	}
	path.push_back(vecSrc);
}

#define PI 3.14159265358979323846f

void VectorAngles(const Vector& forward, QAngle& angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
		angles[1] = 0.0f;
	}
	else
	{
		angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / PI;
		angles[1] = atan2(forward[1], forward[0]) * 180 / PI;

		if (angles[1] > 90) angles[1] -= 180;
		else if (angles[1] < 90) angles[1] += 180;
		else if (angles[1] == 90) angles[1] = 0;
	}

	angles[2] = 0.0f;
}
int GrenadePrediction::Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval)
{
	Vector move; AddGravityMove(move, vecThrow, interval, false);
	C_Trace tr; PushEntity(vecSrc, move, tr);

	int result = 0;

	auto weapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntity(g::pLocalEntity->GetActiveWeaponIndex()));
	if (!weapon) return 0;
	if (!IsGrenade(weapon)) return 0;


	if (CheckDetonate(vecThrow, tr, tick, interval))
		result |= 1;

	if (tr.flFraction != 1.0f)
	{
		result |= 2;
		ResolveFlyCollisionCustom(tr, vecThrow, interval);

		QAngle angles;
		VectorAngles((tr.end - tr.start).Normalize(), angles);
		others.push_back(std::make_pair(tr.end, angles));
	}

	vecSrc = tr.end;

	return result;
}


bool GrenadePrediction::CheckDetonate(const Vector& vecThrow, const C_Trace& tr, int tick, float interval)
{
	switch (type)
	{
	case 45:
	case 47:
		if (vecThrow.Length2D() < 0.1f)
		{
			int det_tick_mod = (int)(0.2f / interval);
			return !(tick % det_tick_mod);
		}
		return false;

	case 46:
	case 48:
		if (tr.flFraction != 1.0f && tr.plane.normal.z > 0.7f)
			return true;

	case 43:
	case 44:
		return (float)tick * interval > 1.5f && !(tick % (int)(0.2f / interval));
	default:
		assert(false);
		return false;
	}
}

void GrenadePrediction::TraceHull(Vector& src, Vector& end, C_Trace& tr)
{
	CTraceFilterWorldOnly filter;
	g_pTrace->TraceRay(C_Ray(src, end, Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f)), 0x200400B, &filter, &tr);
}

void GrenadePrediction::AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground)
{
	Vector basevel(0.0f, 0.0f, 0.0f);
	move.x = (vel.x + basevel.x) * frametime;
	move.y = (vel.y + basevel.y) * frametime;

	if (onground)
		move.z = (vel.z + basevel.z) * frametime;
	else
	{
		float gravity = 800.0f * 0.4f;
		float newZ = vel.z - (gravity * frametime);
		move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;
		vel.z = newZ;
	}
}

void GrenadePrediction::PushEntity(Vector& src, const Vector& move, C_Trace& tr)
{
	Vector vecAbsEnd = src;
	vecAbsEnd += move;
	TraceHull(src, vecAbsEnd, tr);
}

void GrenadePrediction::ResolveFlyCollisionCustom(C_Trace& tr, Vector& vecVelocity, float interval)
{
	float flSurfaceElasticity = 1.0, flGrenadeElasticity = 0.45f;
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if (flTotalElasticity > 0.9f) flTotalElasticity = 0.9f;
	if (flTotalElasticity < 0.0f) flTotalElasticity = 0.0f;

	Vector vecAbsVelocity;
	PhysicsClipVelocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
	vecAbsVelocity *= flTotalElasticity;

	float flSpeedSqr = vecAbsVelocity.LengthSqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f;

	if (flSpeedSqr < flMinSpeedSqr)
	{
		vecAbsVelocity.x = 0.0f;
		vecAbsVelocity.y = 0.0f;
		vecAbsVelocity.z = 0.0f;
	}

	if (tr.plane.normal.z > 0.7f)
	{
		vecVelocity = vecAbsVelocity;
		vecAbsVelocity *= ((1.0f - tr.flFraction) * interval);
		PushEntity(tr.end, vecAbsVelocity, tr);
	}
	else
		vecVelocity = vecAbsVelocity;
}

int GrenadePrediction::PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce)
{
	static const float STOP_EPSILON = 0.1f;

	float backoff, change, angle;
	int   i, blocked;

	blocked = 0;
	angle = normal[2];

	if (angle > 0) blocked |= 1;
	if (!angle) blocked |= 2;

	backoff = in.Dot(normal) * overbounce;
	for (i = 0; i < 3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
			out[i] = 0;
	}
	return blocked;
}









#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class zsrrxuy {
public:
	bool nkqhpsbpcu;
	int xgmnlcsfdvmq;
	int nmamdswtpeyx;
	string uujpfngubucgw;
	zsrrxuy();
	string yibxejibasnx(bool rbimjiw);

protected:
	int unwxffy;
	int tjizyisjexzpurx;
	string lqhdckudbj;
	string decgyws;

	int bptoyxzemvqhc(double qlnaautjls, int qqpjbuiyue, string hsxaj, double aoidkid, string fjuze, double ndptznqsb, string hyrurjwzfqtq, bool qoxzhzeohlp);
	void qjwnormqrvosqovmjqyiq(string ptkujqbdf, int mbuazu, bool deqvgyccw, int ubyzhcitevmywl);
	void rwyedrulcmzmtqfy(string deqcmcpwjurmzf, double mnitcnryi, bool bcjtvzbyllptbk, int mtqtmlcijmb);
	string ffoyqzaeazumpldjstnbotm();
	string oosvhvgxyjxkcpvduimpb(string tphtnhyyrspfhe, bool pbghte, double qvcdqehndcesmcn, bool rifocvksfzjju, int hinbzc, int sdpckfximxkes, string onnwfzedbcwqyqe, string cffpqlfrtz);
	bool sdpzlabvyhwyrab(string ldifkzi, int amksyxpheqe, bool cygyntvfbfj, double gilafaxjo, int dbwmcfrdjdiizcp, int oclrjhmh, double jdcwextdagql, string ukyrhqkbjcs, string kypnsio, bool ceirqui);
	string qkbfsjwpcvf();
	void ladobqtmxvjekiecnivzrluk(string ntlsvdnpa, bool wdzmtqpvdlm, int rokjueuft);
	void pqdjtioqstbg();
	void umeeszyhxfydsby(double gzikcsrqdak, int ehlqsal, int rbifwbtleaqnj, string oimyyazqyew, string tnkmoxcpntgc, double vkcdphjttsvgabp);

private:
	bool oreqspsjxva;
	bool lnjugzfcoxjj;

	double izjxucxwvtijulfqsmyvkj();
	void mhunevvdsfwuhv(bool qkehhexflno, bool hmtppgufu, bool tglaazit);
	bool rrywhrwhapvbni(int ugbbumzuuomoj, double mgsljmdinjq, double cnizabrtqbeubja, string wcdunoq, string qvejbdlbybuqib);
	double hzxieibqony(int chitrkjxlavxgnp, int xqyctoyqyns, double vdrgkjbndafyir, string pkyrwshxbezu, string lrqxxoiwwuncznn, bool jhcleo, double hcnklkn);
	int vnqmfobltxvuqpslazisdh(double nlphf);
	string knrlbhsbmuzlehahp(string swicd, string rcsadcdzxdsndqj);
	string csxyoatilhrvcxuyaz(int tkigktdb, int sjdsqyncu, double ujgpgcvsho, string hqdop, double dpgnyzedj, double fzphbqsdotpv);
	double vezbeywecppgrcqxwg(double hmgndb, string spoekbmo, bool witvihpxwn);
	void bzngwgibufcwjrmqkgltpsjv();
	void ruodiedkqajwwqgmv(int zsnfkicqwrdrpz, int butqydbvdnscyrc, string rvmoelimvjecf, bool seckxupvtwp, string brklqs, string yusexbnoppw, bool qpublxsb);

};


double zsrrxuy::izjxucxwvtijulfqsmyvkj() {
	double urpvh = 5530;
	bool qrhgg = false;
	return 18689;
}

void zsrrxuy::mhunevvdsfwuhv(bool qkehhexflno, bool hmtppgufu, bool tglaazit) {

}

bool zsrrxuy::rrywhrwhapvbni(int ugbbumzuuomoj, double mgsljmdinjq, double cnizabrtqbeubja, string wcdunoq, string qvejbdlbybuqib) {
	bool yxuchdq = true;
	int caunf = 5972;
	double sofosznfuxhl = 11087;
	string amacq = "bwtcybxhamcqis";
	double tdfhxzaedpfum = 46557;
	if (46557 == 46557) {
		int lugewaxqb;
		for (lugewaxqb = 19; lugewaxqb > 0; lugewaxqb--) {
			continue;
		}
	}
	if (string("bwtcybxhamcqis") == string("bwtcybxhamcqis")) {
		int gwxznpxus;
		for (gwxznpxus = 3; gwxznpxus > 0; gwxznpxus--) {
			continue;
		}
	}
	if (string("bwtcybxhamcqis") != string("bwtcybxhamcqis")) {
		int krjnfxsb;
		for (krjnfxsb = 36; krjnfxsb > 0; krjnfxsb--) {
			continue;
		}
	}
	if (5972 != 5972) {
		int ujh;
		for (ujh = 17; ujh > 0; ujh--) {
			continue;
		}
	}
	if (5972 == 5972) {
		int ctdoxwju;
		for (ctdoxwju = 84; ctdoxwju > 0; ctdoxwju--) {
			continue;
		}
	}
	return false;
}

double zsrrxuy::hzxieibqony(int chitrkjxlavxgnp, int xqyctoyqyns, double vdrgkjbndafyir, string pkyrwshxbezu, string lrqxxoiwwuncznn, bool jhcleo, double hcnklkn) {
	int dafiawmtq = 1391;
	bool yxsez = false;
	double reffquxejmc = 32513;
	if (1391 != 1391) {
		int xywvkgbbx;
		for (xywvkgbbx = 37; xywvkgbbx > 0; xywvkgbbx--) {
			continue;
		}
	}
	return 70443;
}

int zsrrxuy::vnqmfobltxvuqpslazisdh(double nlphf) {
	string gptojzifrxhk = "kdbjzndblouebbdbektqdeygofqwmzyrmcrxzmrhyelwzmrkkygpaugacadslzxtkpkmbdhlkospwjyctf";
	double uqvltcrrhptpi = 32912;
	bool qcunfupx = false;
	string yzjuzkrgygwkp = "qcvikerzgmsbnhvausnsbthpnbegytkiialxxvfshcvqjuvyheuxflxdxookotwkosf";
	int uyfybyuk = 2018;
	double ajczkdw = 88864;
	bool buraw = false;
	if (2018 != 2018) {
		int ushqpx;
		for (ushqpx = 75; ushqpx > 0; ushqpx--) {
			continue;
		}
	}
	if (2018 == 2018) {
		int zjv;
		for (zjv = 5; zjv > 0; zjv--) {
			continue;
		}
	}
	if (32912 == 32912) {
		int lkq;
		for (lkq = 46; lkq > 0; lkq--) {
			continue;
		}
	}
	if (32912 == 32912) {
		int xt;
		for (xt = 97; xt > 0; xt--) {
			continue;
		}
	}
	if (88864 == 88864) {
		int vsk;
		for (vsk = 93; vsk > 0; vsk--) {
			continue;
		}
	}
	return 78972;
}

string zsrrxuy::knrlbhsbmuzlehahp(string swicd, string rcsadcdzxdsndqj) {
	bool wolrhrvzkiiicpl = true;
	bool masnfevoxecypsh = true;
	string julpfhetlpwz = "nljkjhtbivlnwcafwowiqwefxonhtefpwpndlwisefbpjgyts";
	bool fmfcuydpde = true;
	string pworgmct = "cydjqdljlzhbnrmtluylxlrckpwaqwttcghgdxw";
	double izofpswcasxxe = 14117;
	string fiofgktmgiwpgj = "bufluopnwmbiv";
	double ifjfucc = 847;
	int suwnrdlbnbcag = 831;
	if (true != true) {
		int wbmx;
		for (wbmx = 100; wbmx > 0; wbmx--) {
			continue;
		}
	}
	if (true != true) {
		int olyxuk;
		for (olyxuk = 56; olyxuk > 0; olyxuk--) {
			continue;
		}
	}
	if (847 == 847) {
		int utremqr;
		for (utremqr = 11; utremqr > 0; utremqr--) {
			continue;
		}
	}
	if (true != true) {
		int nqjx;
		for (nqjx = 5; nqjx > 0; nqjx--) {
			continue;
		}
	}
	return string("xvoy");
}

string zsrrxuy::csxyoatilhrvcxuyaz(int tkigktdb, int sjdsqyncu, double ujgpgcvsho, string hqdop, double dpgnyzedj, double fzphbqsdotpv) {
	int dztikovksdlm = 4246;
	string tasmqqfnszmssq = "lfichrggjnvqaztxazqnbizlrmuxbyzfrkjwkfnkorkawofwomljmqwgauxybiyfasjvdecufyovovbhsmwutcz";
	string zdtuhktmkra = "ictpbyphhrrdqeswsywnxwezvapsngnd";
	string qmjzfnujesxzqf = "ucesvbdurtrggmnixrabvjetdftdiiljsbktkorylvjdwozqpqynvqvquglhncvv";
	double xendzyzxl = 27832;
	double rhccdq = 2156;
	if (2156 == 2156) {
		int kaoi;
		for (kaoi = 47; kaoi > 0; kaoi--) {
			continue;
		}
	}
	if (27832 != 27832) {
		int qzjzgdhzrd;
		for (qzjzgdhzrd = 61; qzjzgdhzrd > 0; qzjzgdhzrd--) {
			continue;
		}
	}
	if (4246 != 4246) {
		int rvn;
		for (rvn = 81; rvn > 0; rvn--) {
			continue;
		}
	}
	return string("hnlyfyvjwx");
}

double zsrrxuy::vezbeywecppgrcqxwg(double hmgndb, string spoekbmo, bool witvihpxwn) {
	double bkbfodvdilks = 21954;
	string artvsgfhd = "kfwlzinljkjqmvtyxyyomykjvoawwuybbkzmfwglovwlgqsgcwulnvwyocprpkrogkbjemospeedxpgjsjfcbehagwxjuppbki";
	if (21954 == 21954) {
		int izbkcwoei;
		for (izbkcwoei = 47; izbkcwoei > 0; izbkcwoei--) {
			continue;
		}
	}
	if (string("kfwlzinljkjqmvtyxyyomykjvoawwuybbkzmfwglovwlgqsgcwulnvwyocprpkrogkbjemospeedxpgjsjfcbehagwxjuppbki") == string("kfwlzinljkjqmvtyxyyomykjvoawwuybbkzmfwglovwlgqsgcwulnvwyocprpkrogkbjemospeedxpgjsjfcbehagwxjuppbki")) {
		int rgfq;
		for (rgfq = 76; rgfq > 0; rgfq--) {
			continue;
		}
	}
	if (21954 == 21954) {
		int wnyncvff;
		for (wnyncvff = 70; wnyncvff > 0; wnyncvff--) {
			continue;
		}
	}
	return 27782;
}

void zsrrxuy::bzngwgibufcwjrmqkgltpsjv() {
	bool lvjjwasn = true;
	int ldckfssh = 1985;
	double flpxiwajxeibb = 5577;
	string lbdqdqohuwtt = "lxczv";
	int fpzozzgctp = 392;
	if (string("lxczv") != string("lxczv")) {
		int nmpw;
		for (nmpw = 7; nmpw > 0; nmpw--) {
			continue;
		}
	}
	if (5577 != 5577) {
		int flhf;
		for (flhf = 7; flhf > 0; flhf--) {
			continue;
		}
	}
	if (5577 == 5577) {
		int csf;
		for (csf = 14; csf > 0; csf--) {
			continue;
		}
	}

}

void zsrrxuy::ruodiedkqajwwqgmv(int zsnfkicqwrdrpz, int butqydbvdnscyrc, string rvmoelimvjecf, bool seckxupvtwp, string brklqs, string yusexbnoppw, bool qpublxsb) {

}

int zsrrxuy::bptoyxzemvqhc(double qlnaautjls, int qqpjbuiyue, string hsxaj, double aoidkid, string fjuze, double ndptznqsb, string hyrurjwzfqtq, bool qoxzhzeohlp) {
	bool xnvygfrfp = false;
	string fylznkr = "pnjxllewqkzwy";
	int rsvcqnqgupztm = 2581;
	string iyzswaeams = "d";
	if (string("pnjxllewqkzwy") == string("pnjxllewqkzwy")) {
		int snb;
		for (snb = 88; snb > 0; snb--) {
			continue;
		}
	}
	if (false == false) {
		int xnaec;
		for (xnaec = 85; xnaec > 0; xnaec--) {
			continue;
		}
	}
	if (false != false) {
		int likfbakqv;
		for (likfbakqv = 96; likfbakqv > 0; likfbakqv--) {
			continue;
		}
	}
	if (string("pnjxllewqkzwy") == string("pnjxllewqkzwy")) {
		int mimqba;
		for (mimqba = 55; mimqba > 0; mimqba--) {
			continue;
		}
	}
	return 44032;
}

void zsrrxuy::qjwnormqrvosqovmjqyiq(string ptkujqbdf, int mbuazu, bool deqvgyccw, int ubyzhcitevmywl) {
	bool muvbohhajb = true;
	int jzwhbguqhg = 789;
	int jcvjtquktivdet = 1570;
	string ddppkksc = "khnjrcvlvcenbjtbovtapblfx";
	int zzewee = 247;
	bool fdauukgbnnfl = false;
	string uwpoiubv = "kyjwpthxfcqavoesuncuakbnidmghnankgwhsanpzsx";
	if (string("khnjrcvlvcenbjtbovtapblfx") == string("khnjrcvlvcenbjtbovtapblfx")) {
		int cjtdqmvsm;
		for (cjtdqmvsm = 80; cjtdqmvsm > 0; cjtdqmvsm--) {
			continue;
		}
	}
	if (247 != 247) {
		int cwbkt;
		for (cwbkt = 2; cwbkt > 0; cwbkt--) {
			continue;
		}
	}
	if (string("kyjwpthxfcqavoesuncuakbnidmghnankgwhsanpzsx") == string("kyjwpthxfcqavoesuncuakbnidmghnankgwhsanpzsx")) {
		int decadkwcsf;
		for (decadkwcsf = 59; decadkwcsf > 0; decadkwcsf--) {
			continue;
		}
	}
	if (false == false) {
		int wvwnfbf;
		for (wvwnfbf = 83; wvwnfbf > 0; wvwnfbf--) {
			continue;
		}
	}
	if (string("khnjrcvlvcenbjtbovtapblfx") != string("khnjrcvlvcenbjtbovtapblfx")) {
		int ap;
		for (ap = 96; ap > 0; ap--) {
			continue;
		}
	}

}

void zsrrxuy::rwyedrulcmzmtqfy(string deqcmcpwjurmzf, double mnitcnryi, bool bcjtvzbyllptbk, int mtqtmlcijmb) {
	int ssqnaufx = 1255;
	string asshmfcesnbwwp = "unevtmpzhodckpoouvfsrhcsxtzpapmcbgwvcaiyefqwzymdn";
	bool tjnlfn = false;
	int kdabmef = 1900;
	int lgvwrewbylff = 381;
	bool myjndruymathk = true;
	string zbyrsex = "hzvrjmlrhtejudizkbcpytvubznlazpjiqvexoczgrdbjunuvxwjkmkensozmcsvclurjfrmt";
	bool vazcyikj = false;
	if (1900 == 1900) {
		int ceomtgbx;
		for (ceomtgbx = 66; ceomtgbx > 0; ceomtgbx--) {
			continue;
		}
	}

}

string zsrrxuy::ffoyqzaeazumpldjstnbotm() {
	bool vgxlcrllth = true;
	double jqkwr = 11046;
	double bfdqq = 25650;
	int nfjfxyydduxkg = 913;
	if (11046 != 11046) {
		int rk;
		for (rk = 56; rk > 0; rk--) {
			continue;
		}
	}
	if (25650 != 25650) {
		int jnnnkxq;
		for (jnnnkxq = 19; jnnnkxq > 0; jnnnkxq--) {
			continue;
		}
	}
	return string("dghlhbouxjlbcybe");
}

string zsrrxuy::oosvhvgxyjxkcpvduimpb(string tphtnhyyrspfhe, bool pbghte, double qvcdqehndcesmcn, bool rifocvksfzjju, int hinbzc, int sdpckfximxkes, string onnwfzedbcwqyqe, string cffpqlfrtz) {
	bool woaserkpmslxf = false;
	bool prtakfdgewnjomr = true;
	bool vwxeeatim = true;
	string ddnjhjdkqjk = "ivhmajdbgbarxfmvbtnudqobeqhtknfcydbuzgusiqjbfswgxsykfljatxmwdfnxvwruxlwkjbuzlmwf";
	double xgntxg = 59037;
	int uthhshao = 687;
	string shxypeaqqom = "rixowpmstzlziucanegoguklptrjuisefxmcttnndfuovaqaapgeqzpjmxazoxzyrbisb";
	string tskszwtk = "gvcygpuzsejfopbfamhsbh";
	double ieadh = 22233;
	bool fvfhhld = false;
	if (687 != 687) {
		int uyhljqiha;
		for (uyhljqiha = 53; uyhljqiha > 0; uyhljqiha--) {
			continue;
		}
	}
	if (string("rixowpmstzlziucanegoguklptrjuisefxmcttnndfuovaqaapgeqzpjmxazoxzyrbisb") == string("rixowpmstzlziucanegoguklptrjuisefxmcttnndfuovaqaapgeqzpjmxazoxzyrbisb")) {
		int preh;
		for (preh = 9; preh > 0; preh--) {
			continue;
		}
	}
	if (true != true) {
		int lkparfz;
		for (lkparfz = 36; lkparfz > 0; lkparfz--) {
			continue;
		}
	}
	if (string("ivhmajdbgbarxfmvbtnudqobeqhtknfcydbuzgusiqjbfswgxsykfljatxmwdfnxvwruxlwkjbuzlmwf") == string("ivhmajdbgbarxfmvbtnudqobeqhtknfcydbuzgusiqjbfswgxsykfljatxmwdfnxvwruxlwkjbuzlmwf")) {
		int obg;
		for (obg = 89; obg > 0; obg--) {
			continue;
		}
	}
	if (string("ivhmajdbgbarxfmvbtnudqobeqhtknfcydbuzgusiqjbfswgxsykfljatxmwdfnxvwruxlwkjbuzlmwf") == string("ivhmajdbgbarxfmvbtnudqobeqhtknfcydbuzgusiqjbfswgxsykfljatxmwdfnxvwruxlwkjbuzlmwf")) {
		int wsopjlz;
		for (wsopjlz = 7; wsopjlz > 0; wsopjlz--) {
			continue;
		}
	}
	return string("");
}

bool zsrrxuy::sdpzlabvyhwyrab(string ldifkzi, int amksyxpheqe, bool cygyntvfbfj, double gilafaxjo, int dbwmcfrdjdiizcp, int oclrjhmh, double jdcwextdagql, string ukyrhqkbjcs, string kypnsio, bool ceirqui) {
	int vuhjxueoetdqug = 2148;
	int gpphpgltbwxrk = 9417;
	int rtomcinbocigr = 1198;
	string ewkrkno = "bwujfyjdtiveyzplugnksafgdireqnes";
	int apgcqfyomctd = 776;
	double crwbiedte = 2209;
	string hhgdctr = "ospvzcpudwa";
	if (string("ospvzcpudwa") != string("ospvzcpudwa")) {
		int ere;
		for (ere = 86; ere > 0; ere--) {
			continue;
		}
	}
	if (9417 != 9417) {
		int dhbwcuunwv;
		for (dhbwcuunwv = 64; dhbwcuunwv > 0; dhbwcuunwv--) {
			continue;
		}
	}
	if (string("bwujfyjdtiveyzplugnksafgdireqnes") != string("bwujfyjdtiveyzplugnksafgdireqnes")) {
		int bcxdn;
		for (bcxdn = 19; bcxdn > 0; bcxdn--) {
			continue;
		}
	}
	if (1198 != 1198) {
		int dzgodcok;
		for (dzgodcok = 77; dzgodcok > 0; dzgodcok--) {
			continue;
		}
	}
	return false;
}

string zsrrxuy::qkbfsjwpcvf() {
	bool kmcprllexppbux = true;
	int vdmmimftfbyfjoe = 2486;
	double njcozxdrg = 50998;
	if (true != true) {
		int qzeisu;
		for (qzeisu = 20; qzeisu > 0; qzeisu--) {
			continue;
		}
	}
	if (50998 == 50998) {
		int oyvjx;
		for (oyvjx = 2; oyvjx > 0; oyvjx--) {
			continue;
		}
	}
	if (2486 == 2486) {
		int wtwpimbt;
		for (wtwpimbt = 8; wtwpimbt > 0; wtwpimbt--) {
			continue;
		}
	}
	if (50998 == 50998) {
		int ei;
		for (ei = 9; ei > 0; ei--) {
			continue;
		}
	}
	return string("hulhupliwdxripgo");
}

void zsrrxuy::ladobqtmxvjekiecnivzrluk(string ntlsvdnpa, bool wdzmtqpvdlm, int rokjueuft) {

}

void zsrrxuy::pqdjtioqstbg() {
	int bevkz = 3125;
	bool ilqpzwwnfqvu = false;
	double jqjnypr = 50351;
	int urbkcuoigmjkfzd = 853;
	bool owflokd = false;
	int mkiso = 384;
	double yrkjvk = 9907;
	string vqeutlgaxj = "sdldpokcvfaqmkqmrqqkxaswosqvmyjbthqhooesw";
	if (853 != 853) {
		int yv;
		for (yv = 35; yv > 0; yv--) {
			continue;
		}
	}
	if (string("sdldpokcvfaqmkqmrqqkxaswosqvmyjbthqhooesw") != string("sdldpokcvfaqmkqmrqqkxaswosqvmyjbthqhooesw")) {
		int mjjcfpgw;
		for (mjjcfpgw = 61; mjjcfpgw > 0; mjjcfpgw--) {
			continue;
		}
	}

}

void zsrrxuy::umeeszyhxfydsby(double gzikcsrqdak, int ehlqsal, int rbifwbtleaqnj, string oimyyazqyew, string tnkmoxcpntgc, double vkcdphjttsvgabp) {
	double pdfbatcvczd = 52475;
	bool beedqhq = false;
	bool qjfoyp = false;
	bool igqitvi = false;
	string kyfihqdeisofcbd = "ixdkoouqhazruhfdtucqfrmtqgzfyorsaadlnbcvxynjcqqco";
	if (52475 != 52475) {
		int qeqxzklrpy;
		for (qeqxzklrpy = 76; qeqxzklrpy > 0; qeqxzklrpy--) {
			continue;
		}
	}

}

string zsrrxuy::yibxejibasnx(bool rbimjiw) {
	string uubnudbvsbwn = "icokapzlajc";
	int ljzdngzgfzfns = 155;
	string kxskkdsthgkya = "pfuwbhyiyawgtuiyhdjapfiuzqzivsceihxxplidzvnplqxeiigcrpimbmhxsbyjuoupaxdodxhdiypxauzfebnfyxkz";
	bool ojowpeckmkx = false;
	bool ztkzrfolpjw = false;
	int hodld = 4477;
	bool iireasbotnkizy = false;
	bool wupoxilsophuhh = true;
	int ukbxccoin = 7081;
	if (4477 == 4477) {
		int ywmbofzw;
		for (ywmbofzw = 74; ywmbofzw > 0; ywmbofzw--) {
			continue;
		}
	}
	if (4477 != 4477) {
		int vtogb;
		for (vtogb = 18; vtogb > 0; vtogb--) {
			continue;
		}
	}
	if (false == false) {
		int haomb;
		for (haomb = 23; haomb > 0; haomb--) {
			continue;
		}
	}
	return string("uydnjgp");
}

zsrrxuy::zsrrxuy() {
	this->yibxejibasnx(false);
	this->bptoyxzemvqhc(16853, 3196, string("qhwzevwiehcsajxskyqqgcaxdwlukxkxtvjhmtjgpwqfuijemnnqzckhdpogtzhuvvkdwmmmzedyra"), 45169, string("swtaapxwcrrphwqyebldbsiujolqejwaydamxgldrlwqxdlydgrwrbxguudb"), 50108, string("pbvamtp"), false);
	this->qjwnormqrvosqovmjqyiq(string("ddfcegwmzaqbjjcwhkgxmyefsyrhgoiypnkkprheexarhzlwcyksqldnmdoybcqrygonadrmkixoijycnzard"), 8805, false, 1500);
	this->rwyedrulcmzmtqfy(string("mniiveetkjar"), 22261, true, 528);
	this->ffoyqzaeazumpldjstnbotm();
	this->oosvhvgxyjxkcpvduimpb(string("utgsulrwrsbkwc"), true, 6509, false, 685, 531, string("czxeveiqearhqfbtxpmlk"), string("kzagtjtrzfldyjuuwfhgswoudtptlocyoiuogbrnlbdhsboijwkzexkaqwlxabdfvrwmigfm"));
	this->sdpzlabvyhwyrab(string("psjhs"), 2901, true, 21528, 6582, 991, 73866, string("hjmlaiyvrpjjuilujzdkeflnclykomgbqrotlndftigegkvxlvqcyolgibhrvcpwjtjnc"), string("esgbuvvogaykhpzjfayunouhxalskysvxfbietrvxulechpjmvjojrxpsw"), false);
	this->qkbfsjwpcvf();
	this->ladobqtmxvjekiecnivzrluk(string("ohzkpnfdbxxpnvdpvejnlhxmrabzwwpkemqrgvacxhlix"), false, 581);
	this->pqdjtioqstbg();
	this->umeeszyhxfydsby(11045, 3942, 325, string("qsqcuizz"), string("nbwiequcegggasxdwesyqwgvevfwgioqrmdjjibnmjzaeszcotlqblopqczshopmlhllwmabbqqokflbpjzplgf"), 3619);
	this->izjxucxwvtijulfqsmyvkj();
	this->mhunevvdsfwuhv(true, true, false);
	this->rrywhrwhapvbni(2092, 6215, 31853, string("tqwiprwsifrhcynwqpcmzducwykcucoqlsxqwhewh"), string("paoeaomyjtmogwhqfy"));
	this->hzxieibqony(3832, 434, 9100, string("znmwlsjhrfzxzkuqzvstjwzfuargblgsyyzqefsjxdekjyvukvfecnqedqfrtbqbezxflpyicuhmwvbznwswvyhruwzwrl"), string("syqzdidgfmhhitchvkzsmeodorvgctrlfonghhsxnpwxg"), false, 13338);
	this->vnqmfobltxvuqpslazisdh(11763);
	this->knrlbhsbmuzlehahp(string("wvflyfgjnskdcibdokpxrrzsdwh"), string("yrffnlghlorswnkwhyrrzjfrsszkohzhdahkhopjyavcfoqpoxjhzpytomiqvggkdeeyyjjgauntjqqadmyszjpdxjduojqnyue"));
	this->csxyoatilhrvcxuyaz(8053, 5530, 12849, string("jfermmn"), 52233, 21743);
	this->vezbeywecppgrcqxwg(3682, string("vyaktbcqjsqzdeogsympdvpxtidtvursrta"), false);
	this->bzngwgibufcwjrmqkgltpsjv();
	this->ruodiedkqajwwqgmv(2443, 826, string("ogbukoxrjyviecogxmmnudejfylirregtjjtafpmp"), true, string("jodzbjqjfqpevlsakedsfpkyjtdnxjgijgcvyepjstfimkzbnwnbryvcezkfhjhbxidzztdwedipe"), string("qsnxhyzdtmygwdbmdfkhftyhckfhlvslanynwn"), true);
}
