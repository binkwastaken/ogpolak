#pragma once

#include <mutex>
#include <shared_mutex>
#include <vector>

#include "classes/entities.h"
#include "math/vectors.h"

class Globals {
public:
	static C_PlayerPawn* LocalPlayerPawn;
	static C_PlayerController* LocalPlayerController;
	static std::vector<Vector3D> LocalPlayerOrigin;
	static std::shared_mutex mtx;
};