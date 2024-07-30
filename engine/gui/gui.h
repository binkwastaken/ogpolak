#pragma once

#include "../renderer/renderer.h"

#include "framework.h"

class CGui {
public:
	bool IsOpen = false;

	void DrawGui();

	class Variables {
	public:
		class ESP {
		public:
			bool players, name, team, localplayer,boxes,health,weapon,distance,ammo,flags,money,glow,snaplines,skeletons;
			bool world, droppedweapons, plantedc4, projectiles, droppedbomb,defusekit,healthshot,chicken,droppednades,hostages;
			ImVec4 NameColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 BoxesColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 SnapLinesColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 WeaponColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 SkeletonsColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 GlowColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec4 AmmoColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			int SnaplinesRadius = 1;
		};
		ESP m_ESP;
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
	ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;

};

inline CGui* g_pGui = new CGui();