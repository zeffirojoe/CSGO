#include "includes.h"
//https://guidedhacking.com/threads/ezp1z1s-csgo-direct3d9-hooking-esp-tutorial-series.14570/

extern Hack* hack;

// filled rectangle
void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
	D3DRECT rect = { x,y,x + w,y + h };
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color) {
	ID3DXLine* LineL;
	D3DXCreateLine(pDevice, &LineL);

	if (!hack->LineL)
		D3DXCreateLine(pDevice, &hack->LineL);

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);

	hack->LineL->SetWidth(thickness);
	hack->LineL->Draw(Line, 2, color);
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
