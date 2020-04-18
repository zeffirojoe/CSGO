#pragma once
#include "hack.h"
//https://guidedhacking.com/threads/ezp1z1s-csgo-direct3d9-hooking-esp-tutorial-series.14570/

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR col);

void DrawCircle(int X, int Y, int radius, int numSides, D3DCOLOR Color);

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);
void DrawLine(vec2 src, vec2 dst, int thickness, D3DCOLOR color);

void DrawEspBox2D(vec2 top, vec2 bot, int thickness, D3DCOLOR color);

void DrawEspBox3D(vec3 top, vec3 bot, float a, int width, int thickness, D3DCOLOR color);

void DrawText(const char* text, float x, float y, D3DCOLOR color);

void AlsoUseless();