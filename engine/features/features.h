#pragma once
#include "../gui/gui.h"
#include "../renderer/renderer.h"
#include "../interfaces/manager.h"


class CFeatures {
public:

	class ESP {
	public:
		void Players();
		void World();
	private:
	};
	ESP m_ESP;
};
inline CFeatures* g_pFeatures = new CFeatures();