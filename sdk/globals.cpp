#include "globals.h"
C_PlayerPawn* Globals::LocalPlayerPawn = nullptr;
C_PlayerController* Globals::LocalPlayerController = nullptr;
Vector3D Globals::LocalPlayerOrigin = Vector3D{};
std::shared_mutex Globals::mtx = std::shared_mutex{};