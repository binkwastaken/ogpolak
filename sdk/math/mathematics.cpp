#include "mathematics.h"

#include "../../engine/utils/utils.h"
#include "../../extensions/imgui/imgui.h"


bool CMath::WorldToScreen(Vector3D& Position, Vector2D& ScreenPosition)
{
    static float(*ViewMatrix)[4][4] = reinterpret_cast<float(*)[4][4]>(g_pUtils->m_Memory.ResolveRip(g_pUtils->m_Memory.PatternScan("client.dll", "48 8D ?? ?? ?? ?? ?? 48 C1 E0 06 48 03 C1 C3 CC CC"), 3, 7));

    if (!ViewMatrix)
        return false;

    const float w = (*ViewMatrix)[3][0] * Position.x + (*ViewMatrix)[3][1] * Position.y + (*ViewMatrix)[3][2] * Position.z + (*ViewMatrix)[3][3];
    if (w < 0.001f)
        return false;

    ScreenPosition.x = ImGui::GetIO().DisplaySize.x * 0.5f;
    ScreenPosition.y = ImGui::GetIO().DisplaySize.y * 0.5f;

    ScreenPosition.x *= 1.0f + (((*ViewMatrix)[0][0] * Position.x + (*ViewMatrix)[0][1] * Position.y + (*ViewMatrix)[0][2] * Position.z + (*ViewMatrix)[0][3])) / w;
    ScreenPosition.y *= 1.0f - (((*ViewMatrix)[1][0] * Position.x + (*ViewMatrix)[1][1] * Position.y + (*ViewMatrix)[1][2] * Position.z + (*ViewMatrix)[1][3])) / w;

    return true;
}

bool CMath::GetPlayerBoundingBox(C_PlayerPawn* ent, BoundingBox& in)
{
	Vector3D worldPoints[8];

	C_BaseEntity* BaseEntity = (C_BaseEntity*)ent; // Pointer to CBaseEntity*
	if (!BaseEntity)
		return false;

	auto origin = BaseEntity->GetGameSceneNode()->GetVecOrigin();
	if (origin.IsZero())
		return false;

	const auto min = BaseEntity->GetCollision()->VecMins() + origin;
	const auto max = BaseEntity->GetCollision()->VecMaxs() + origin;

	worldPoints[0] = Vector3D(min.x, min.y, min.z);
	worldPoints[1] = Vector3D(min.x, max.y, min.z);
	worldPoints[2] = Vector3D(max.x, max.y, min.z);
	worldPoints[3] = Vector3D(max.x, min.y, min.z);
	worldPoints[4] = Vector3D(max.x, max.y, max.z);
	worldPoints[5] = Vector3D(min.x, max.y, max.z);
	worldPoints[6] = Vector3D(min.x, min.y, max.z);
	worldPoints[7] = Vector3D(max.x, min.y, max.z);

	Vector2D screenPoints[8];

	for (int i = 0; i < 8; ++i)
	{
		if (!WorldToScreen(worldPoints[i], screenPoints[i]))
			return false;
	}

	// Calculate bounding box dimensions
	float left = screenPoints[0].x;
	float top = screenPoints[0].y;
	float right = screenPoints[0].x;
	float bottom = screenPoints[0].y;

	for (int i = 1; i < 8; ++i)
	{
		left = min(left, screenPoints[i].x);
		top = min(top, screenPoints[i].y);
		right = max(right, screenPoints[i].x);
		bottom = max(bottom, screenPoints[i].y);
	}

	// Set bounding box dimensions
	in.x = static_cast<int>(left);
	in.y = static_cast<int>(top);
	in.w = static_cast<int>(right - left);
	in.h = static_cast<int>(bottom - top);

	return true;
}
