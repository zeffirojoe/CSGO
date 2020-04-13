#pragma once
#include "csgo.h"
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a,b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__) [size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#define ABS(x) ((x < 0) ? (-x) : (x))

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

class Hack
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

	vec2 crosshar2D;
	int crosshairSize = 4;

	void Init();
	void Update();
	bool checkValidEnt(Ent* ent);
	bool W2S(Vec3 pos, vec2& screen);
	vec3 GetBonePos(Ent* ent, int bone);
};
