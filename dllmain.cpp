#include "includes.h"
#include <sstream>
#include <string.h>

// credits to:
// Broihon and 0xDEC0DE for the dummy device template
//https://guidedhacking.com/threads/ezp1z1s-csgo-direct3d9-hooking-esp-tutorial-series.14570/

// data
void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;

//MainHack
Hack* hack;

// hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice)
		pDevice = o_pDevice;

	// drawing stuff
	DrawText("IF YOU'RE USING THIS, YOU MUST BE PRETTY WHITE", windowWidth / 2, windowHeight - 20, argb::white);

	int menuOffX = windowWidth / 2;
	int menuOffy = 50;

	if (!hack->settings.showMenu) {
		DrawText("Show Menu (INS)", menuOffX, menuOffy, argb::white);
	}
	else {
		DrawText("Show Teammates (F1)", menuOffX, menuOffy + (0 * 12), hack->settings.showTeammates ? argb::green : argb::red);
		DrawText("Snap Lines (F2)", menuOffX, menuOffy + (1 * 12), hack->settings.snaplines ? argb::green : argb::red);
		DrawText("2D Box (F3)", menuOffX, menuOffy + (2 * 12), hack->settings.box2d ? argb::green : argb::red);
		DrawText("2D Status (F4)", menuOffX, menuOffy + (3 * 12), hack->settings.status2D ? argb::green : argb::red);
		DrawText("Status Text (F5)", menuOffX, menuOffy + (4 * 12), hack->settings.statusText ? argb::green : argb::red);
		DrawText("3D Box (F6)", menuOffX, menuOffy + (5 * 12), hack->settings.box3D ? argb::green : argb::red);
		DrawText("Velocity ESP (F7)", menuOffX, menuOffy + (6 * 12), hack->settings.velEsp ? argb::green : argb::red);
		DrawText("HeadLine ESP (F8)", menuOffX, menuOffy + (7 * 12), hack->settings.headlineESP ? argb::green : argb::red);
		DrawText("Recoil Crosshair (F9)", menuOffX, menuOffy + (8 * 12), hack->settings.rcsCrosshair ? argb::green : argb::red);
		DrawText("Aimbot RMC (F10)", menuOffX, menuOffy + (9 * 12), hack->settings.aimbot ? argb::green : argb::red);
		DrawText("Hide Menu (INS)", menuOffX, menuOffy + (10 * 12), argb::white);
	}

	for (int i = 1; i < 32; i++) {
		Ent* curEnt = hack->entList->ents[i].ent;
		if (!hack->checkValidEnt(curEnt))
			continue;

		D3DCOLOR espColor, snaplineColor, velocityColor, headlineColor;
		if (curEnt->iTeamNum == hack->localEnt->iTeamNum) {
			espColor = hack->color.team.esp;
			snaplineColor = hack->color.team.snapline;
			velocityColor = hack->color.team.velocity;
			headlineColor = hack->color.team.headline;
		}
		else {
			espColor = hack->color.enemy.esp;
			snaplineColor = hack->color.enemy.snapline;
			velocityColor = hack->color.enemy.velocity;
			headlineColor = hack->color.enemy.headline;
		}

		if (!hack->settings.showTeammates && (curEnt->iTeamNum == hack->localEnt->iTeamNum))
			continue;

		vec3 entHead3D = hack->GetBonePos(curEnt, 8);
		entHead3D.z += 8;
		vec2 entPos2D, entHead2D;

		if (hack->World2Screen(curEnt->vecOrigin, entPos2D)) {
			//velESP
			if (hack->settings.velEsp) {
				vec3 velOff = curEnt->vecOrigin + (curEnt->vecVelocity * .25);
				vec2 velOff2D;

				if (hack->World2Screen(velOff, velOff2D)) {
					DrawLine(entPos2D, velOff2D, 2, velocityColor);
					DrawFilledRect(velOff2D.x - 2, velOff2D.y - 2, 4, 4, espColor);
				}
			}

			//snaplines
			if (hack->settings.snaplines) {
				DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 2, snaplineColor); //snapLine
			}

			if (hack->World2Screen(entHead3D, entHead2D)) {
				//2D Boxes
				if (hack->settings.box2d) {
					DrawEspBox2D(entPos2D, entHead2D, 2, espColor);
				}
				//3D Boxes
				if (hack->settings.box3D) {
					DrawEspBox3D(entHead3D, curEnt->vecOrigin, curEnt->angEyeAnglesY, 37, 3, espColor);
				}
				//2D status
				if (hack->settings.status2D) {
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

					DrawLine(botHealth, topHealth, 2, hack->color.health);
					DrawLine(botArmor, topArmor, 2, hack->color.armor);
				}
				//Headline ESP
				if (hack->settings.headlineESP) {
					vec3 head3D = hack->GetBonePos(curEnt, 8);
					vec3 entAngles;
					entAngles.x = curEnt->angEyeAnglesX;
					entAngles.y = curEnt->angEyeAnglesY;
					entAngles.z = 0;
					vec3 endPoint = hack->TransformVec(head3D, entAngles, 60);
					vec2 endPoint2D, head2D;
					hack->World2Screen(head3D, head2D);
					if (hack->World2Screen(endPoint, endPoint2D)) {
						DrawLine(head2D, endPoint2D, 2, headlineColor);
					}
				}

				if (hack->settings.statusText) {
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
	if (hack->settings.aimbot) {
		//while (GetAsyncKeyState(VK_RBUTTON))
		//{
		Ent* localPlayer = hack->localEnt;
		uintptr_t playerStatePtr = hack->engine + offsets::dwClientState;
		vec3* viewAngles = (vec3*)(*(uintptr_t*)(playerStatePtr)+offsets::dwClientState_ViewAngles);
		EntList* entList = (EntList*)(hack->client + offsets::dwEntityList);

		Ent* target = hack->GetBestTarget(localPlayer, viewAngles, entList);

		if (target)
		{
			vec3 body = target->m_vecOrigin;
			//body.z -= 10;
			*viewAngles = angles::CalcAngle(localPlayer->m_vecOrigin, body);
		}

		//Sleep(5);
	//}
	}

	// crosshair
	if (hack->settings.rcsCrosshair) {
		vec2 l, r, t, b;
		l = r = t = b = hack->crosshar2D;
		l.x -= hack->crosshairSize;
		r.x += hack->crosshairSize;
		b.y += hack->crosshairSize;
		t.y -= hack->crosshairSize;

		DrawLine(l, r, 2, hack->color.crosshair);
		DrawLine(t, b, 2, hack->color.crosshair);
		// call og function
	}
	oEndScene(pDevice);
}

DWORD WINAPI HackThread(HMODULE hModule) {

	// hook
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

		oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
	}

	hack = new Hack();
	hack->Init();

	// hack loop
	while (!GetAsyncKeyState(VK_END)) {
		hack->Update();

		vec3 pAng = hack->localEnt->aimPunchAngle;

		hack->crosshar2D.x = windowWidth / 2 - (windowWidth / 90 * pAng.y);
		hack->crosshar2D.y = windowHeight / 2 + (windowHeight / 90 * pAng.x);
	}


	// unhook
	Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);


	Sleep(1000);
	// uninject
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr) {
	if (reason == DLL_PROCESS_ATTACH)
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
	return TRUE;
}