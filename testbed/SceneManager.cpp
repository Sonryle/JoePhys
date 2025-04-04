#include "SceneManager.hpp"
#include "imgui.h"

void SceneManager::SwitchScene(int input_scene_number)
{
	// Delete old scene and replace with new scene
	delete scene;
	switch (scene_number)
	{
	case 0:
		scene = new NewtonsCradleScene();
		break;
	case 1:
		scene = new ClothScene();
		break;
	case 2:
		scene = new PinballScene();
		break;
	case 3:
		scene = new RopeScene();
		break;
	case 4:
		scene = new ColourDebugScene();
		break;
	default:
		scene = new ColourDebugScene();
		break;
	}

	// if its not the same scene as before, set up scene settings
	if (scene_number != input_scene_number)
	{
		settings.Reset();
		scene->SetUpSceneSettings();
	}
	scene_number = input_scene_number;

	// Set Up Scene
	scene->SetUpScene();

	// Change the GUI to be fit the colour palette of the new scene

	ImGuiStyle& style = ImGui::GetStyle();

	colour bg = palette.colours[scene->colours.background];
	glClearColor(bg.r, bg.g, bg.b, bg.a);

	real w = 0.45f;	// Window darken
	real m = 0.6f;	// Menu darken
	real t = 0.6f;	// Title darken

	style.Colors[ImGuiCol_WindowBg]		= ImVec4(bg.r*w, bg.g*w, bg.b*w, bg.a);
        style.Colors[ImGuiCol_MenuBarBg]	= ImVec4(bg.r*m, bg.g*m, bg.b*m, bg.a);
        style.Colors[ImGuiCol_TitleBgActive]	= ImVec4(bg.r*t, bg.g*t, bg.b*t, bg.a);

}

SceneManager sm;
