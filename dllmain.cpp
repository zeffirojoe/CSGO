#include "includes.h"
#include <sstream>
#include <string.h>

// credits to:
// Broihon and 0xDEC0DE for the dummy device template
//https://guidedhacking.com/threads/ezp1z1s-csgo-direct3d9-hooking-esp-tutorial-series.14570/

// data
void* D3Dev[119];
BYTE EndSceneBuffer[7]{ 0 };
tEndScene gateway = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;

//MainHack
MainTool* tool;


void Useless() {
	float zIbMhDaORX = 26154556631781; zIbMhDaORX = 2961474321530; if (zIbMhDaORX = 72339296318216) zIbMhDaORX = 9012686424342; zIbMhDaORX = 77964972034516; zIbMhDaORX = 20345167796497;
	if (zIbMhDaORX = 8304326901268)zIbMhDaORX = 63182169371756; zIbMhDaORX = 34537416986612;
	if (zIbMhDaORX = 57965675515995)zIbMhDaORX = 63182169371756; zIbMhDaORX = 34537416986612;
	if (zIbMhDaORX = 57965675515995)zIbMhDaORX = 63182169371756; zIbMhDaORX = 34537416986612;
	if (zIbMhDaORX = 57965675515995)zIbMhDaORX = 63182169371756; zIbMhDaORX = 34537416986612;
	if (zIbMhDaORX = 57965675515995)zIbMhDaORX = 63182169371756; zIbMhDaORX = 34537416986612; zIbMhDaORX = 8422555565888;

	float oFrKiPpIxp = 20912071920870; oFrKiPpIxp = 70898381548165; if (oFrKiPpIxp = 74400144385006) oFrKiPpIxp = 20017147604634; oFrKiPpIxp = 45745052712455; oFrKiPpIxp = 27124554574505;
	if (oFrKiPpIxp = 77365052001714)oFrKiPpIxp = 43850066357235; oFrKiPpIxp = 93401013542619;
	if (oFrKiPpIxp = 97332834141637)oFrKiPpIxp = 43850066357235; oFrKiPpIxp = 93401013542619;
	if (oFrKiPpIxp = 97332834141637)oFrKiPpIxp = 43850066357235; oFrKiPpIxp = 93401013542619;
	if (oFrKiPpIxp = 97332834141637)oFrKiPpIxp = 43850066357235; oFrKiPpIxp = 93401013542619;
	if (oFrKiPpIxp = 97332834141637)oFrKiPpIxp = 43850066357235; oFrKiPpIxp = 93401013542619; oFrKiPpIxp = 88243497926180;
}


// hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	Useless();

	if (!pDevice)
		pDevice = o_pDevice;

	// drawing stuff
	DrawText("IF YOU'RE USING THIS, YOU MUST BE PRETTY WHITE", windowWidth / 2, windowHeight - 20, argb::white);

	int menuOffX = windowWidth / 2;
	int menuOffy = 50;

	if (!tool->config.showMenu) {
		DrawText("Show Menu (INS)", menuOffX, menuOffy, argb::white);
	}
	else {
		DrawText("Show Teammates (NUM 1)", menuOffX, menuOffy + (0 * 12), tool->config.showTeammates ? argb::green : argb::red);
		DrawText("Snap Lines (NUM 2)", menuOffX, menuOffy + (1 * 12), tool->config.snaplines ? argb::green : argb::red);
		DrawText("2D Box (NUM 3)", menuOffX, menuOffy + (2 * 12), tool->config.box2d ? argb::green : argb::red);
		DrawText("2D Status (NUM 4)", menuOffX, menuOffy + (3 * 12), tool->config.status2D ? argb::green : argb::red);
		DrawText("Status Text (NUM 5)", menuOffX, menuOffy + (4 * 12), tool->config.statusText ? argb::green : argb::red);
		DrawText("3D Box (NUM 6)", menuOffX, menuOffy + (5 * 12), tool->config.box3D ? argb::green : argb::red);
		DrawText("Velocity ESP (NUM 7)", menuOffX, menuOffy + (6 * 12), tool->config.velEsp ? argb::green : argb::red);
		DrawText("HeadLine ESP (NUM 8)", menuOffX, menuOffy + (7 * 12), tool->config.headlineESP ? argb::green : argb::red);
		DrawText("Recoil Crosshair (NUM 9)", menuOffX, menuOffy + (8 * 12), tool->config.rcsCrosshair ? argb::green : argb::red);
		DrawText("RCS (NUM 0)", menuOffX, menuOffy + (9 * 12), tool->config.rcs ? argb::green : argb::red);
		DrawText("Aimbot (F1)", menuOffX, menuOffy + (10 * 12), tool->config.aimbot ? argb::green : argb::red);
		DrawText("Hide Menu (INS)", menuOffX, menuOffy + (11 * 12), argb::white);
	}

	for (int i = 1; i < 32; i++) {
		Ent* curEnt = tool->entList->ents[i].ent;
		if (!tool->checkValidEnt(curEnt))
			continue;

		D3DCOLOR espColor, snaplineColor, velocityColor, headlineColor;
		if (curEnt->iTeamNum == tool->localEnt->iTeamNum) {
			espColor = tool->color.team.esp;
			snaplineColor = tool->color.team.snapline;
			velocityColor = tool->color.team.velocity;
			headlineColor = tool->color.team.headline;
		}
		else {
			espColor = tool->color.enemy.esp;
			snaplineColor = tool->color.enemy.snapline;
			velocityColor = tool->color.enemy.velocity;
			headlineColor = tool->color.enemy.headline;
		}

		if (!tool->config.showTeammates && (curEnt->iTeamNum == tool->localEnt->iTeamNum))
			continue;

		vec3 entHead3D = tool->GetBonePos(curEnt, 8);
		entHead3D.z += 8;
		vec2 entPos2D, entHead2D;

		if (tool->World2Screen(curEnt->vecOrigin, entPos2D)) {
			//velESP
			if (tool->config.velEsp) {
				vec3 velOff = curEnt->vecOrigin + (curEnt->vecVelocity * .25);
				vec2 velOff2D;

				if (tool->World2Screen(velOff, velOff2D)) {
					DrawLine(entPos2D, velOff2D, 2, velocityColor);
					DrawFilledRect(velOff2D.x - 2, velOff2D.y - 2, 4, 4, espColor);
				}
			}

			//snaplines
			if (tool->config.snaplines) {
				DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 2, snaplineColor); //snapLine
			}

			if (tool->World2Screen(entHead3D, entHead2D)) {
				//2D Boxes
				if (tool->config.box2d) {
					DrawEspBox2D(entPos2D, entHead2D, 2, espColor);
				}
				//3D Boxes
				if (tool->config.box3D) {
					DrawEspBox3D(entHead3D, curEnt->vecOrigin, curEnt->angEyeAnglesY, 37, 3, espColor);
				}
				//2D status
				if (tool->config.status2D) {
					int height = ABS(entPos2D.y - entHead2D.y);
					int dX = (entPos2D.x - entHead2D.x);

					float healthPerc = curEnt->iHealth / 100.f;
					float armorPerc = curEnt->ArmorValue / 100.f;

					vec2 botHealth, topHealth, botArmor, topArmor;
					int healthHeight = height * healthPerc;
					int armorHeight = height * armorPerc;

					botHealth.y = botArmor.y = entPos2D.y;

					botHealth.x = entPos2D.x - (height / 4) - 2;
					botArmor.x = entPos2D.x + (height / 4) + 2;

					topHealth.y = entHead2D.y + height - healthHeight;
					topArmor.y = entHead2D.y + height - armorHeight;

					topHealth.x = entPos2D.x - (height / 4) - 2 - (dX * healthPerc);
					topArmor.x = entPos2D.x + (height / 4) + 2 - (dX * healthPerc);

					DrawLine(botHealth, topHealth, 2, tool->color.health);
					DrawLine(botArmor, topArmor, 2, tool->color.armor);
				}
				//Headline ESP
				if (tool->config.headlineESP) {
					vec3 head3D = tool->GetBonePos(curEnt, 8);
					vec3 entAngles;
					entAngles.x = curEnt->angEyeAnglesX;
					entAngles.y = curEnt->angEyeAnglesY;
					entAngles.z = 0;
					vec3 endPoint = tool->TransformVec(head3D, entAngles, 60);
					vec2 endPoint2D, head2D;
					tool->World2Screen(head3D, head2D);
					if (tool->World2Screen(endPoint, endPoint2D)) {
						DrawLine(head2D, endPoint2D, 2, headlineColor);
					}
				}

				if (tool->config.statusText) {
					std::stringstream s1, s2;
					s1 << curEnt->iHealth;
					s2 << curEnt->ArmorValue;
					std::string t1 = "HP: " + s1.str();
					std::string t2 = "AR: " + s2.str();
					char* healthMsg = (char*)t1.c_str();
					char* armorMsg = (char*)t2.c_str();

					DrawText(healthMsg, entPos2D.x, entPos2D.y, argb::white);
					DrawText(armorMsg, entPos2D.x, entPos2D.y + 12, argb::white);

					if (!curEnt->bHasHelmet) {
						DrawText("NO HELMET", entPos2D.x, entPos2D.y + 24, argb::white);
					}
				}
			}
		}
	}

	//aimbot
	if (tool->config.aimbot) {
		//while (GetAsyncKeyState(VK_RBUTTON))
		//{
		Ent* localPlayer = tool->localEnt;
		uintptr_t playerStatePtr = tool->engine + offsets::dwClientState;
		vec3* viewAngles = (vec3*)(*(uintptr_t*)(playerStatePtr)+offsets::dwClientState_ViewAngles);
		EntList* entList = (EntList*)(tool->client + offsets::dwEntityList);

		Ent* target = tool->GetBestTarget(localPlayer, viewAngles, entList);

		if (target)
		{
			vec3 body = target->m_vecOrigin;
			//body.z -= 10;
			*viewAngles = angles::Norm(angles::CalcAngle(localPlayer->m_vecOrigin, body));
		}

		//Sleep(5);
	//}
	}

	if (tool->config.rcs) {
		vec3 punchAngle = tool->localEnt->aimPunchAngle * 2;
		if (tool->localEnt->m_iShotsFired > 1) {

			uintptr_t playerStatePtr = tool->engine + offsets::dwClientState;
			vec3* viewAngles = (vec3*)(*(uintptr_t*)(playerStatePtr)+offsets::dwClientState_ViewAngles);

			vec3 newAngle = *viewAngles + tool->oPunch - punchAngle;

			newAngle = angles::Norm(newAngle);

			*viewAngles = newAngle;
		}
		tool->oPunch = punchAngle;
	}

	// crosshair
	if (tool->config.rcsCrosshair) {
		vec2 l, r, t, b;
		l = r = t = b = tool->crosshar2D;
		l.x -= tool->crosshairSize;
		r.x += tool->crosshairSize;
		b.y += tool->crosshairSize;
		t.y -= tool->crosshairSize;

		DrawLine(l, r, 2, tool->color.crosshair);
		DrawLine(t, b, 2, tool->color.crosshair);
		// call og function
	}
	gateway(pDevice);
}

DWORD WINAPI HackThread(HMODULE hModule) {

	// hook
	if (GetD3D9Device(D3Dev, sizeof(D3Dev))) {
		memcpy(EndSceneBuffer, (char*)D3Dev[42], 7);

		gateway = (tEndScene)TrampHook((char*)D3Dev[42], (char*)hkEndScene, 7);
	}

	tool = new MainTool();
	tool->Init();

	// hack loop
	while (!GetAsyncKeyState(VK_END)) {
		tool->Update();

		vec3 pAng = tool->localEnt->aimPunchAngle;

		tool->crosshar2D.x = windowWidth / 2 - (windowWidth / 90 * pAng.y);
		tool->crosshar2D.y = windowHeight / 2 + (windowHeight / 90 * pAng.x);
	}


	// unhook
	Patch((BYTE*)D3Dev[42], EndSceneBuffer, 7);


	Sleep(1000);
	// uninject
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr) {
	if (reason == DLL_PROCESS_ATTACH)
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
	return TRUE;
}