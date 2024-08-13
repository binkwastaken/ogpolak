#pragma once

#include "../renderer/renderer.h"

#include "framework.h"
#include "configs.h"

class CGui {
public:
	bool IsOpen = false;

	void DrawGui();

	class Variables {
	public:
		class ESP {
		public:
			bool players = false, name = false, team = false, localplayer = false, boxes = false, health = false, weapon = false, distance = false, ammo = false, flags = false, money = false, glow = false, snaplines = false, skeletons = false;
			bool world = false, droppedweapons = false, plantedc4 = false, projectiles = false, droppedbomb = false, defusekit = false, healthshot = false, chicken = false, droppednades = false, hostages = false;
			ImVec4 NameColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 BoxesColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 SnapLinesColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 WeaponColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 SkeletonsColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 GlowColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 AmmoColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			int SnaplinesRadius = 0;
			int SnaplinesPosition = 0;
		};
		ESP m_ESP;
		class Models {
		public:
			bool enable,ignorez;
			int SelectedTextureLocal = 0;
			int SelectedTextureEnemy = 0;
			int SelectedTextureTeam = 0;

			bool localPlayer;
			bool team,teaminvisible;
			bool enemy,enemyinvisible;
			ImVec4 LocalPlayerColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 EnemyColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 EnemyColorInvisible = ImVec4(1.f, 1.f, 1.f, 1.f);

			ImVec4 TeamColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 TeamColorInvisible = ImVec4(1.f, 1.f, 1.f, 1.f);
		};
		Models m_Models;
		class WorldModulation
		{
		public:
			bool lighting,worldcolor;
			ImVec4 LightingColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			int LightingIntensity = 1;
			ImVec4 WorldColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			int	WorldIntensity = 1;
		};
		WorldModulation m_WorldModulation;

		class OtherVisuals {
		public:
			bool Watermark = false;
			bool InfoPanel = false;
			bool CustomMenuTheme = false;
			ImVec4 MenuThemeColor = ImVec4(1.f, 1.f, 1.f, 1.f);
		};
		OtherVisuals m_OtherVisuals;

		class Configuration {
		public:
			int SelectedIndexConfig = 0;
			std::vector<std::string> ConfigNames = { "Default" };
			char CustomNameConfig[256] = "";  // Adjust the size as needed
		};
		Configuration m_Configuration;

	};
	Variables m_Vars;

private:
	void SetWindowPosition();
	void SetupStyles();
	int w = 500, h = 500;
	ImVec2 WindowPostion, WindowSize;
	bool IsDragging = false;
	int clickedItem = 0;
	ImVec2 DragDelta;
	ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

};

inline CGui* g_pGui = new CGui();