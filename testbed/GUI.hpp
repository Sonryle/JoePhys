#ifndef JP_GUI
#define JP_GUI

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "GLFW/glfw3.h"
#include "Settings.hpp"


static void initImGui(GLFWwindow* window)
{
	ImGui::CreateContext();	
	ImGui_ImplGlfw_InitForOpenGL(window, 1);
	ImGui_ImplOpenGL3_Init();
}

static bool appearanceWindowShown = 0;
static void AppearanceWindow();

static void updateImGui()
{
	// Create New Frame
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	// Add Our Top Main Menu Bar
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
			if (ImGui::MenuItem("Colour Test Scene", NULL, (settings.scene_number == 2)))
			{
				settings.scene_has_changed = 1;
				settings.scene_number = 2;
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
		if (ImGui::TreeNode("Scene"))
		{
			// list of items MUST be in the same order as the colour palette enum (from colours.hpp)
			const char* items[] = { "Gray", "Red", "Green", "Yellow", "Blue", "Purple", "Aqua", "White",
						"Dark Gray", "Dark Red", "Dark Green", "Dark Yellow", "Dark Blue",
						"Dark Purple", "Dark Aqua", "Dark White" };

			// Selection box for background colour
			const char* background_preview_value = items[settings.scene_colours.background];
			if (ImGui::BeginCombo("Background", background_preview_value, 0))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (settings.scene_colours.background == n);

					if (ImGui::Selectable(items[n], is_selected))
						settings.scene_colours.background = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			// Selection box for circles colour
			const char* circle_preview_value = items[settings.scene_colours.circles];
			if (ImGui::BeginCombo("Circles", circle_preview_value, 0))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (settings.scene_colours.circles == n);

					if (ImGui::Selectable(items[n], is_selected))
						settings.scene_colours.circles = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			// Selection box for circle outlines colour
			const char* circle_outline_preview_value = items[settings.scene_colours.circle_outlines];
			if (ImGui::BeginCombo("Circle Outlines", circle_outline_preview_value, 0))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (settings.scene_colours.circle_outlines == n);

					if (ImGui::Selectable(items[n], is_selected))
						settings.scene_colours.circle_outlines = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Palette"))
		{
			if(ImGui::Button("Set Autumn"))
			{
				palette.SetAutumn();
			}
			if(ImGui::Button("Set Gruvbox"))
			{
				palette.SetGruvbox();
			}
			if(ImGui::Button("Set Pastel"))
			{
				palette.SetPastel();
			}
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
