#ifndef JP_GUI
#define JP_GUI

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "GLFW/glfw3.h"
#include "Settings.hpp"

static bool appearanceWindowShown = 0;

static void initImGui(GLFWwindow* window)
{
	ImGui::CreateContext();	
	ImGui_ImplGlfw_InitForOpenGL(window, 1);
	ImGui_ImplOpenGL3_Init();
}

static void AppearanceWindow();

static void updateImGui()
{
	// Create New Frame
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	// Add Our Knobs & Dails
        if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::MenuItem("Appearance", "\tCtrl+A", appearanceWindowShown))
			{ 
				appearanceWindowShown = (appearanceWindowShown == 1)? 0 : 1;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scenes"))
		{
			if (ImGui::MenuItem("Test Scene", NULL, (settings.scene_number == 0)))
			{
				settings.scene_has_changed = 1;
				settings.scene_number = 0;
			}
			if (ImGui::MenuItem("Test Scene Two", NULL, (settings.scene_number == 1)))
			{
				settings.scene_has_changed = 1;
				settings.scene_number = 1;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Help Is Coming Soon (But Not Yet!!!)"))
			{

			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	AppearanceWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void AppearanceWindow()
{
	if (!appearanceWindowShown)
		return;

	ImGui::Begin("Appearance", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 initial_size(500, 500);
	ImVec2 initial_pos(io.DisplaySize.x - ImGui::GetWindowWidth() - 10, 30);
	ImGui::SetWindowSize(initial_size, ImGuiCond_None);
	ImGui::SetWindowPos(initial_pos, ImGuiCond_None);

	if (ImGui::CollapsingHeader("Colour"))
	{
		if (ImGui::TreeNode("Palette"))
		{
			ImGui::Separator();
			if(ImGui::Button("Set Purpbox"))
			{
				palette.SetPurpbox();
			}
			if(ImGui::Button("Set Gruvbox"))
			{
				palette.SetGruvbox();
			}
			if(ImGui::Button("Set Pastel"))
			{
				palette.SetPastel();
			}
			ImGui::Separator();
			ImGui::TreePop();
		}
	}
	if (ImGui::CollapsingHeader("Renderer"))
	{
		ImGui::SliderInt("Circle Resolution", &settings.circle_res, 3, 32);
		// Add a (?) button with a tooltip
		ImGui::SameLine();
		ImGui::TextDisabled("(?)");
    		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
		{
			const char* desc = "Controls how many vertices make up the circles which are rendered.\n"
					   "The higher the resolution, the smoother the circles appear, and the\n"
					   "lower the resolution, the more sharp and polygonal the circles appear.\n";
        		ImGui::SetTooltip("%s", desc);
		}
	}
	ImGui::End();
}


#endif
