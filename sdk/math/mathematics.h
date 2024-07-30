#pragma once
#include <math.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "../classes/entities.h"
#include "vectors.h"

class C_PlayerPawn;

class CMath {
public:

	struct BoundingBox {

		int x, y, w, h;

		BoundingBox() = default;
		BoundingBox(const int x, const int y, const int w, const int h) : x(x), y(y), w(w), h(h) {}
	};

	bool WorldToScreen(Vector3D& Position, Vector2D& ScreenPosition);
	bool GetPlayerBoundingBox(C_PlayerPawn* ent, BoundingBox& in);
};
inline CMath* g_pMath = new CMath();