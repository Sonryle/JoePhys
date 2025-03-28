#include "SceneManager.hpp"
#include "imgui.h"
#include "scenes/NewtonsCradleScene.hpp"
#include "scenes/PendulumClockScene.hpp"

void SceneManager::SwitchScene(int scene_number)
{
	delete current_scene;
	current_scene_number = scene_number;
	switch (scene_number)
	{
	case 0:
		current_scene = new CollisionScene();
		break;
	case 1:
		current_scene = new PinballScene();
		break;
	case 2:
		current_scene = new SquishySquareScene();
		break;
	case 3:
		current_scene = new SquishyRectangleScene();
		break;
	case 4:
		current_scene = new NewtonsCradleScene();
		break;
	case 5:
		current_scene = new ClothScene();
		break;
	case 6:
		current_scene = new RopeScene();
		break;
	case 7:
		current_scene = new PendulumClockScene();
		break;
	case 8:
		current_scene = new ColourDebugScene();
		break;
	default:
		SwitchScene(0);
		break;
	}

	ImGuiStyle& style = ImGui::GetStyle();

	colour bg = palette.colours[current_scene->colours.background];
	real w = 0.45f;	// Window darken
	real m = 0.6f;	// Menu darken
	real t = 0.6f;	// Title darken
	style.Colors[ImGuiCol_WindowBg]		= ImVec4(bg.r*w, bg.g*w, bg.b*w, bg.a);
        style.Colors[ImGuiCol_MenuBarBg]	= ImVec4(bg.r*m, bg.g*m, bg.b*m, bg.a);
        style.Colors[ImGuiCol_TitleBgActive]	= ImVec4(bg.r*t, bg.g*t, bg.b*t, bg.a);

}

SceneManager scene_manager;
