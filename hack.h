#pragma once
#include "csgo.h"
#include <d3d9.h>
#include <d3dx9.h>

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a,b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__) [size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#define ABS(x) ((x < 0) ? (-x) : (x))

#define TORAD(x) ((x) * 0.01745329252)

#define W2S(x,y) tool->World2Screen(x, y)

namespace argb {
	const D3DCOLOR white = D3DCOLOR_ARGB(255, 255, 255, 255);
	const D3DCOLOR red = D3DCOLOR_ARGB(255, 255, 0, 0);
	const D3DCOLOR green = D3DCOLOR_ARGB(255, 0, 255, 0);
	const D3DCOLOR blue = D3DCOLOR_ARGB(255, 0, 0, 255);
	const D3DCOLOR purple = D3DCOLOR_ARGB(255, 255, 0, 255);
}

class Vector3
{
public:

	float x, y, z;

	Vector3() {};
	Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator * (const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
	Vector3 operator / (const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }
	Vector3& operator += (const Vector3& rhs) { return *this = *this + rhs; }
	Vector3& operator -= (const Vector3& rhs) { return *this = *this - rhs; }
	Vector3& operator *= (const float& rhs) { return *this = *this * rhs; }
	Vector3& operator /= (const float& rhs) { return *this = *this / rhs; }
	float Length() const { return sqrtf(x * x + y * y + z * z); }
	Vector3 Normalize() const { return *this * (1 / Length()); }
	float Distance(const Vector3& rhs) const { return (*this - rhs).Length(); }
};

struct vec4
{
	float x, y, z, w;
};

using vec3 = Vector3;
using Vec3 = Vector3;
using vec2 = Vector3;
using Vec2 = Vector3;
using vec = Vector3;
using Vec = Vector3;


class Ent {
public:
	union {
		// isDormant
		DEFINE_MEMBER_N(bool, isDormant, offsets::m_bDormant);
		//iHealth
		DEFINE_MEMBER_N(int, iHealth, offsets::m_iHealth);
		//VecOrigin
		DEFINE_MEMBER_N(Vec3, vecOrigin, offsets::m_vecOrigin);
		//iTeamNum
		DEFINE_MEMBER_N(int, iTeamNum, offsets::m_iTeamNum);
		//BoneMatrix
		DEFINE_MEMBER_N(int, boneMatrix, offsets::m_dwBoneMatrix);
		//armorvalue
		DEFINE_MEMBER_N(int, ArmorValue, offsets::m_ArmorValue);
		//aimpunchAngle
		DEFINE_MEMBER_N(vec3, aimPunchAngle, offsets::m_aimPunchAngle);
		//angEyeAnglesX
		DEFINE_MEMBER_N(float, angEyeAnglesX, offsets::m_angEyeAnglesX);
		//angEyeAnglesY
		DEFINE_MEMBER_N(float, angEyeAnglesY, offsets::m_angEyeAnglesY);
		//vecVelocity
		DEFINE_MEMBER_N(vec3, vecVelocity, offsets::m_vecVelocity);
		//bHasHelmet
		DEFINE_MEMBER_N(bool, bHasHelmet, offsets::m_bHasHelmet);
		//Origin
		DEFINE_MEMBER_N(vec3, m_vecOrigin, offsets::m_vecOrigin);
		//ViewOffeset
		DEFINE_MEMBER_N(vec3, m_vecViewOffset, offsets::m_vecViewOffset);
		//state
		DEFINE_MEMBER_N(int, m_lifeState, offsets::m_lifeState);
		//clientid
		DEFINE_MEMBER_N(int, clientId, 0x64);
		//shots fird
		DEFINE_MEMBER_N(int, m_iShotsFired, offsets::m_iShotsFired);
		//ViewAngle
		DEFINE_MEMBER_N(int, m_iShotsFired, offsets::m_viewPunchAngle);

	};
};

class EntListObj {
public:
	struct Ent* ent;
	char padding[12];
};

class EntList {
public:
	EntListObj ents[32];
};

class MainTool
{
public:
	uintptr_t dwEntityList = offsets::dwEntityList;
	uintptr_t dwViewMatrix = offsets::dwViewMatrix;

	uintptr_t engine;
	uintptr_t client;
	Ent* localEnt;
	EntList* entList;
	float viewMatrix[16];

	ID3DXLine* LineL;
	ID3DXFont* FontF;

	vec2 crosshar2D;
	int crosshairSize = 4;

	~MainTool();

	void Init();
	void Update();

	//for Menu
	void CheckButtons();

	//ESP and 3D
	bool checkValidEnt(Ent* ent);
	bool World2Screen(Vec3 pos, vec2& screen);
	vec3 GetBonePos(Ent* ent, int bone);
	vec3 TransformVec(vec3 src, vec3 ang, float d);

	//Aim and RCS
	Ent* GetBestTarget(Ent* localPlayer, vec3* viewAngles, EntList* entList);
	bool IsValidTarget(Ent* localPlayer, Ent* ent);
	vec3 oPunch{ 0, 0, 0, };

	//Junk Code
	void VeryUseless();

	//Config
	struct settings {
		bool showMenu = false;
		bool showTeammates = true;
		bool snaplines = false;
		bool box2d = false;
		bool status2D = false;
		bool statusText = false;
		bool box3D = false;
		bool velEsp = false;
		bool headlineESP = false;
		bool rcsCrosshair = false;
		bool aimbot = false;
		bool rcs = false;
	}config;

	struct Colors {
		D3DCOLOR health = D3DCOLOR_ARGB(255, 46, 139, 87);
		D3DCOLOR armor = D3DCOLOR_ARGB(255, 30, 144, 255);
		D3DCOLOR crosshair = argb::white;
		struct team {
			D3DCOLOR esp = argb::green;
			D3DCOLOR snapline = argb::green;
			D3DCOLOR velocity = argb::blue;
			D3DCOLOR headline = argb::blue;
		}team;
		struct Enemy {
			D3DCOLOR esp = argb::red;
			D3DCOLOR snapline = argb::red;
			D3DCOLOR velocity = argb::purple;
			D3DCOLOR headline = argb::purple;
		}enemy;

	}color;

	struct Buttons {
		DWORD showMenuBtn = VK_INSERT;
		DWORD showTeammatesBtn = VK_NUMPAD1;
		DWORD snaplinesBtn = VK_NUMPAD2;
		DWORD box2dBtn = VK_NUMPAD3;
		DWORD status2DBtn = VK_NUMPAD4;
		DWORD statusTextBtn = VK_NUMPAD5;
		DWORD box3DBtn = VK_NUMPAD6;
		DWORD velEspBtn = VK_NUMPAD7;
		DWORD headlineESPBtn = VK_NUMPAD8;
		DWORD rcsCrosshairBtn = VK_NUMPAD9;
		DWORD aimbotBtn = VK_F1;
		DWORD rcsBtn = VK_NUMPAD0;
	}button;
};