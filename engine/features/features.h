#pragma once
#include <string>
#include <unordered_map>
#include <algorithm> 
#include <cctype> 

#include "../gui/gui.h"
#include "../renderer/renderer.h"
#include "../interfaces/manager.h"

#include "../../sdk/globals.h"

class CFeatures {
public:

	class ESP {
	public:
		class Players {
		public:
			void Draw();
		private:
			void Box(CMath::BoundingBox bbox);
			void Name(CMath::BoundingBox& pos, const EntityListInfo& player);
			void HealthBar(CMath::BoundingBox& bbox, const EntityListInfo& player);
			void Weapon(CMath::BoundingBox& boundingBox, const EntityListInfo& playerInfo);
			void Ammo(CMath::BoundingBox& bbox, const EntityListInfo& pPawn);

			void Distance(const EntityListInfo& playerInfo, CMath::BoundingBox& pos);

			void Flags(const EntityListInfo& playerInfo, CMath::BoundingBox& pos);

			void Money(const EntityListInfo& playerInfo, CMath::BoundingBox& pos);

			std::unordered_map<int, float> previousHealth;
		};
		Players m_Players;

		class World {
		public:
		};
		World m_World;
	};
	ESP m_ESP;
};
inline CFeatures* g_pFeatures = new CFeatures();