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
		class Aimbot {
		public:
			bool enable = false;
			int fov = 0;
			bool ignoreteam = false;
			bool silentaim = false;
			bool autowall = false;
			bool autostop = false;
			bool removerecoil = false;

			bool smoothness = false;

			

			int smoothnessvalue = 0;

			int selectedautostop = 0;
			int selectedhitbox = 0;
			int multipoint = 0;
			int selectedweaponslot = 0;

			int hitchancePistol = 0, hitchanceRifle = 0, hitchanceSniper = 0, hitchanceSmg = 0, hitchanceAuto = 0, hitchanceHeavy = 0, hitchanceZeus = 0;
			int minimaldamagePistol = 0, minimaldamageRifle = 0, minimaldamageSniper = 0, minimaldamageSmg = 0, minimaldamageAuto = 0, minimaldamageHeavy = 0, minimaldamageZeus = 0;



		};
		Aimbot m_Aimbot;

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
			bool enable = false,ignorez = false;
			int SelectedTextureLocal = 0;
			int SelectedTextureEnemy = 0;
			int SelectedTextureTeam = 0;

			bool localPlayer = false;
			bool team = false,teaminvisible = false;
			bool enemy = false,enemyinvisible = false;
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
			bool lighting = false , worldcolor = false;
			ImVec4 LightingColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			int LightingIntensity = 0;
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
			bool ForceCrosshair = false;
			bool SniperZoom = false;
			int SniperZoomType = 0;
			ImVec4 SniperZoomColor = ImVec4(0.f, 0.f, 0.f, 1.f);
			ImVec4 SniperZoomColorGradient = ImVec4(0.f, 0.f, 0.f, 1.f);
		};
		OtherVisuals m_OtherVisuals;

		class Configuration {
		public:
			int SelectedIndexConfig = 0;
			std::vector<std::string> ConfigNames = { "default" };
			char CustomNameConfig[256] = "";
		};
		Configuration m_Configuration;

		class Removals {
		public:
			bool norecoil = false;
			bool noflash = false;
			bool nosmoke = false;
			bool nolegspreview = false;
			bool nozoom = false;
		};
		Removals m_Removals;
		
		class View {
		public:
			bool fovchanger = false;
			bool fovwhilescoped = false;
			int fov = 0;
			bool viewmodelchanger = false;
			int viewmodelx = 0;
			int viewmodely = 0;
			int viewmodelz = 0;
			int viewmodelfov = 0;
		};
		View m_View;

		class Movement {
		public:
			bool bunnyhop = false;
			bool autostrafe = false;
			bool duckinair = false;
			bool crouchjump = false;
		};
		Movement m_Movement;
	};
	Variables m_Vars;

private:
	void SetWindowPosition();
	void SetupStyles();
	int w = 500, h = 520;
	ImVec2 WindowPostion, WindowSize;
	bool IsDragging = false;
	int clickedItem = 0;
	ImVec2 DragDelta;
	ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

};

inline CGui* g_pGui = new CGui();