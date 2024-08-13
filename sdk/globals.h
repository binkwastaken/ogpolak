#pragma once

#include <mutex>
#include <shared_mutex>

#include "classes/entities.h"
#include "math/vectors.h"

class Globals {
public:
	static C_PlayerPawn* LocalPlayerPawn;
	static C_PlayerController* LocalPlayerController;
	static Vector3D LocalPlayerOrigin;
	static std::shared_mutex mtx;
};