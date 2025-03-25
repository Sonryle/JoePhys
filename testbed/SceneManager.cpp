#include "SceneManager.hpp"
#include "imgui.h"
#include "scenes/NewtonsCradleScene.hpp"

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
		current_scene = new ColourDebugScene();
		break;
	default:
		SwitchScene(0);
		break;
	}

	ImGuiStyle& style = ImGui::GetStyle();

	real w = 0.45f;
	real m = 0.6f;
	colour bg = palette.colours[current_scene->colours.background];
	style.Colors[ImGuiCol_WindowBg] = ImVec4(bg.r*w, bg.g*w, bg.b*w, bg.a);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(bg.r*m, bg.g*m, bg.b*m, bg.a);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(bg.r*m, bg.g*m, bg.b*m, bg.a);

}

SceneManager scene_manager;
