#include "globals.h"
C_PlayerPawn* Globals::LocalPlayerPawn = nullptr;
C_PlayerController* Globals::LocalPlayerController = nullptr;
std::vector<Vector3D> Globals::LocalPlayerOrigin = {};
std::shared_mutex Globals::mtx = std::shared_mutex{};