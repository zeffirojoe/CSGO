#include "includes.h"

MainTool::~MainTool() {
	this->FontF->Release();
	this->LineL->Release();
}

void MainTool::Init()
{
	client = (uintptr_t)GetModuleHandle("client_panorama.dll");
	engine = (uintptr_t)GetModuleHandle("engine.dll");
	entList = (EntList*)(client + dwEntityList);
	localEnt = entList->ents[0].ent;
}

void MainTool::Update()
{
	memcpy(&viewMatrix, (PBYTE*)(client + dwViewMatrix), sizeof(viewMatrix));
	this->CheckButtons();
}

void MainTool::CheckButtons() {
	if (GetAsyncKeyState(button.showMenuBtn) & 1) {
		config.showMenu = !config.showMenu;
	}
	if (GetAsyncKeyState(button.showTeammatesBtn) & 1) {
		config.showTeammates = !config.showTeammates;
	}
	if (GetAsyncKeyState(button.snaplinesBtn) & 1) {
		config.snaplines = !config.snaplines;
	}
	if (GetAsyncKeyState(button.box2dBtn) & 1) {
		config.box2d = !config.box2d;
	}
	if (GetAsyncKeyState(button.status2DBtn) & 1) {
		config.status2D = !config.status2D;
	}
	if (GetAsyncKeyState(button.statusTextBtn) & 1) {
		config.statusText = !config.statusText;
	}
	if (GetAsyncKeyState(button.box3DBtn) & 1) {
		config.box3D = !config.box3D;
	}
	if (GetAsyncKeyState(button.velEspBtn) & 1) {
		config.velEsp = !config.velEsp;
	}
	if (GetAsyncKeyState(button.headlineESPBtn) & 1) {
		config.headlineESP = !config.headlineESP;
	}
	if (GetAsyncKeyState(button.rcsCrosshairBtn) & 1) {
		config.rcsCrosshair = !config.rcsCrosshair;
	}
	if (GetAsyncKeyState(button.aimbotBtn) & 1) {
		config.aimbot = !config.aimbot;
	}
	if (GetAsyncKeyState(button.rcsBtn) & 1) {
		config.rcs = !config.rcs;
	}
}

bool MainTool::checkValidEnt(Ent* ent)
{
	if (ent == nullptr)
		return false;
	if (ent == localEnt)
		return false;
	if (ent->iHealth <= 0)
		return false;
	if (ent->isDormant)
		return false;
	return true;
}

bool MainTool::World2Screen(Vec3 pos, vec2& screen)
{
	vec4 clipCoords;
	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
	clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
	clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
	clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

vec3 MainTool::GetBonePos(Ent* ent, int bone) {
	uintptr_t bonePtr = ent->boneMatrix;
	vec3 bonePos;
	bonePos.x = *(float*)(bonePtr + 0x30 * bone + 0x0C);
	bonePos.y = *(float*)(bonePtr + 0x30 * bone + 0x1C);
	bonePos.z = *(float*)(bonePtr + 0x30 * bone + 0x2C);
	return bonePos;
}

vec3 MainTool::TransformVec(vec3 src, vec3 ang, float d) {
	vec3 newPos;
	newPos.x = src.x + (cosf(TORAD(ang.y)) * d);
	newPos.y = src.y + (sinf(TORAD(ang.y)) * d);
	newPos.z = src.z + (tanf(TORAD(ang.x)) * d);
	return newPos;
}

bool MainTool::IsValidTarget(Ent* ent)
{
	if (ent && ent->iTeamNum != localEnt->iTeamNum &&
		ent->clientId != localEnt->clientId && ent->m_lifeState == 0
		&& !ent->isDormant && ent->clientId > 0 && ent->clientId < 64)
	{
		return true;
	}
	else return false;
}

Ent* MainTool::GetBestTarget()
{
	VeryUseless();

	float oldDistance = FLT_MAX;
	float newDistance = 0;
	Ent* target = nullptr;

	for (auto ent : entList->ents)
	{
		if (IsValidTarget(ent.ent))
		{
			//vec3 angleTo = angles::CalcAngle(localEnt->m_vecOrigin, ent.ent->vecOrigin);
			//newDistance = angles::Distance(localEnt->aimPunchAngle, angleTo);
			float newDistance = localEnt->vecOrigin.Distance(ent.ent->vecOrigin);
			if (newDistance < oldDistance)
			{
				oldDistance = newDistance;
				target = ent.ent;
			}
		}
	}
	return target;
}

void MainTool::VeryUseless() {
	float zSkOfUlYEz = 95418461744045; zSkOfUlYEz = 2274233199310; if (zSkOfUlYEz = 28087541776112) zSkOfUlYEz = 60550931521548; zSkOfUlYEz = 68442988863279; zSkOfUlYEz = 88632796844298;
	if (zSkOfUlYEz = 43084956055093)zSkOfUlYEz = 17761126875005; zSkOfUlYEz = 97035384553138;
	if (zSkOfUlYEz = 16492359801252)zSkOfUlYEz = 17761126875005; zSkOfUlYEz = 97035384553138;
	if (zSkOfUlYEz = 16492359801252)zSkOfUlYEz = 17761126875005; zSkOfUlYEz = 97035384553138;
	if (zSkOfUlYEz = 16492359801252)zSkOfUlYEz = 17761126875005; zSkOfUlYEz = 97035384553138;
	if (zSkOfUlYEz = 16492359801252)zSkOfUlYEz = 17761126875005; zSkOfUlYEz = 97035384553138; zSkOfUlYEz = 61106176757072;
}

void MainTool::AimAt(Ent* ent) {
	static vec3* viewAngles = (vec3*)(*(uintptr_t*)(engine + offsets::dwClientState) + offsets::dwClientState_ViewAngles);

	vec3 origin = localEnt->vecOrigin;
	vec3 viewOffset = localEnt->m_vecViewOffset;
	vec3* myPos = &(origin + viewOffset);

	vec3 newAngle =	angles::CalcAngle(*myPos, GetBonePos(ent, 8));

	newAngle = angles::Norm(newAngle);

	viewAngles->x = newAngle.x;
	viewAngles->y = newAngle.y;
}