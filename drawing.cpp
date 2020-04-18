#include "includes.h"
//https://guidedhacking.com/threads/ezp1z1s-csgo-direct3d9-hooking-esp-tutorial-series.14570/

extern MainTool* tool;

void DrawCircle(int X, int Y, int radius, int numSides, D3DCOLOR Color)
{
	D3DXVECTOR2 Line[128];
	float Step = PI * 2.0 / numSides;
	int Count = 0;
	for (float a = 0; a < PI * 2.0; a += Step)
	{
		float X1 = radius * cos(a) + X;
		float Y1 = radius * sin(a) + Y;
		float X2 = radius * cos(a + Step) + X;
		float Y2 = radius * sin(a + Step) + Y;
		Line[Count].x = X1;
		Line[Count].y = Y1;
		Line[Count + 1].x = X2;
		Line[Count + 1].y = Y2;
		Count += 2;
	}
	tool->LineL->Begin();
	tool->LineL->Draw(Line, Count, Color);
	tool->LineL->End();
}

// filled rectangle
void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
	D3DRECT rect = { x,y,x + w,y + h };
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color) {
	ID3DXLine* LineL;
	D3DXCreateLine(pDevice, &LineL);

	if (!tool->LineL)
		D3DXCreateLine(pDevice, &tool->LineL);

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);

	tool->LineL->SetWidth(thickness);
	tool->LineL->Draw(Line, 2, color);
}

void DrawLine(vec2 src, vec2 dst, int thickness, D3DCOLOR color) {
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawEspBox2D(vec2 top, vec2 bot, int thickness, D3DCOLOR color) {
	int height = ABS(top.y - bot.y);
	vec2 tl, tr;
	tl.x = top.x - height / 4;
	tr.x = top.x + height / 4;
	tl.y = tr.y = top.y;

	vec2 bl, br;

	bl.x = bot.x - height / 4;
	br.x = bot.x + height / 4;
	bl.y = br.y = bot.y;

	DrawLine(tl, tr, thickness, color);
	DrawLine(bl, br, thickness, color);
	DrawLine(tl, bl, thickness, color);
	DrawLine(tr, br, thickness, color);
}

void DrawEspBox3D(vec3 top, vec3 bot, float a, int width, int thickness, D3DCOLOR color) {
	int height3D = ABS(top.z - bot.z);
	vec3 b1, b2, b3, b4, t1, t2, t3, t4;
	b1.z = b2.z = b3.z = b4.z = bot.z;

	b1.x = bot.x + (cos(TORAD(a + 45)) * width);
	b1.y = bot.y + (sin(TORAD(a + 45)) * width);

	b2.y = bot.y + (sin(TORAD(a + 135)) * width);
	b2.x = bot.x + (cos(TORAD(a + 135)) * width);

	b3.x = bot.x + (cos(TORAD(a + 225)) * width);
	b3.y = bot.y + (sin(TORAD(a + 225)) * width);

	b4.x = bot.x + (cos(TORAD(a + 315)) * width);
	b4.y = bot.y + (sin(TORAD(a + 315)) * width);

	t1.x = b1.x;
	t1.y = b1.y;
	t1.z = b1.z + height3D;

	t2.x = b2.x;
	t2.y = b2.y;
	t2.z = b2.z + height3D;

	t3.x = b3.x;
	t3.y = b3.y;
	t3.z = b3.z + height3D;

	t4.x = b4.x;
	t4.y = b4.y;
	t4.z = b4.z + height3D;

	vec2 b1_2, b2_2, b3_2, b4_2, t1_2, t2_2, t3_2, t4_2;
	if (W2S(b1, b1_2) && W2S(b2, b2_2) && W2S(b3, b3_2) && W2S(b4, b4_2) && W2S(t1, t1_2) && W2S(t2, t2_2) && W2S(t3, t3_2) && W2S(t4, t4_2)) {
		//columns
		DrawLine(t1_2, b1_2, thickness, color);
		DrawLine(t2_2, b2_2, thickness, color);
		DrawLine(t3_2, b3_2, thickness, color);
		DrawLine(t4_2, b4_2, thickness, color);
		//topbase
		DrawLine(t1_2, t2_2, thickness, color);
		DrawLine(t2_2, t3_2, thickness, color);
		DrawLine(t3_2, t4_2, thickness, color);
		DrawLine(t4_2, t1_2, thickness, color);
		//bottom base
		DrawLine(b1_2, b2_2, thickness, color);
		DrawLine(b2_2, b3_2, thickness, color);
		DrawLine(b3_2, b4_2, thickness, color);
		DrawLine(b4_2, b1_2, thickness, color);
	}
}

void DrawText(const char* text, float x, float y, D3DCOLOR color) {
	RECT rect;

	if (!tool->FontF)
		D3DXCreateFont(pDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &tool->FontF);

	SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
	tool->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

	SetRect(&rect, x, y, x, y);
	tool->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}

void AlsoUseless() {
	float zSkOfUlYEz = 95418461744045; zSkOfUlYEz = 2274233199310; if (zSkOfUlYEz = 28087541776112) zSkOfUlYEz = 60550931521548; zSkOfUlYEz = 68442988863279; zSkOfUlYEz = 88632796844298;
	if (zSkOfUlYEz = 43084956055093)zSkOfUlYEz = 17761126875005; zSkOfUlYEz = 97035384553138;
	if (zSkOfUlYEz = 16492359801252)zSkOfUlYEz = 17761126875005; zSkOfUlYEz = 97035384553138;
	if (zSkOfUlYEz = 16492359801252)zSkOfUlYEz = 17761126875005; zSkOfUlYEz = 97035384553138;
	if (zSkOfUlYEz = 16492359801252)zSkOfUlYEz = 17761126875005; zSkOfUlYEz = 97035384553138;
	if (zSkOfUlYEz = 16492359801252)zSkOfUlYEz = 17761126875005; zSkOfUlYEz = 97035384553138; zSkOfUlYEz = 61106176757072;
}

