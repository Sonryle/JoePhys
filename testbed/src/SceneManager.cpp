#include "SceneManager.hpp"
#include "imgui.h"

void SceneManager::SwitchScene(int scene_number)
{
	// Delete old scene and replace with new scene
	delete current_scene;
	switch (scene_number)
	{
	case 0:
		current_scene = new NewtonsCradleScene();
		break;
	case 1:
		current_scene = new ClothScene();
		break;
	case 2:
		current_scene = new PinballScene();
		break;
	case 3:
		current_scene = new RopeScene();
		break;
	case 4:
		current_scene = new HourGlassScene();
		break;
	case 5:
		current_scene = new ColourDebugScene();
		break;
	default:
		current_scene = new ColourDebugScene();
		break;
	}

	// if its not the same scene as before, set up scene settings
	if (current_scene_number != scene_number)
	{
		settings.Reset();
		current_scene->SetUpSceneSettings();
	}
	current_scene_number = scene_number;

	// Set Up Scene
	current_scene->SetUpScene();

	// Change the GUI to be fit the colour palette of the new scene

	ImGuiStyle& style = ImGui::GetStyle();

	colour bg = palette.colours[current_scene->colours.background];
	glClearColor(bg.r, bg.g, bg.b, bg.a);

	real w = 0.45f;	// Window darken
	real m = 0.6f;	// Menu darken
	real t = 0.6f;	// Title darken

	style.Colors[ImGuiCol_WindowBg]		= ImVec4(bg.r*w, bg.g*w, bg.b*w, bg.a);
        style.Colors[ImGuiCol_MenuBarBg]	= ImVec4(bg.r*m, bg.g*m, bg.b*m, bg.a);
        style.Colors[ImGuiCol_TitleBgActive]	= ImVec4(bg.r*t, bg.g*t, bg.b*t, bg.a);

}

SceneManager scene_manager;
