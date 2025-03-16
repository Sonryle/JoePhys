// Contains functions which set up the GUI for JoePhys (using imgui)
#ifndef JP_GUI
#define JP_GUI

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "GLFW/glfw3.h"
#include "Settings.hpp"

// function to draw the GUI for the appearance window. Must be called every frame which
// you want the appearance window to be visible in.
static void DrawAppearanceWindow();
static bool appearanceWindowHidden = 1;

// creates an imgui context and initiates GLFW & OpenGL for imgui
static void initImGui(GLFWwindow* window)
{
	ImGui::CreateContext();	
	ImGui_ImplGlfw_InitForOpenGL(window, 1);
	ImGui_ImplOpenGL3_Init();
}

// Draws a menu bar at the top of the program, and calls functions to draw every other
// imgui window as well, (e.g. the appearance window).
static void DrawGui()
{
	// Begin New Frame
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	// Add Our Top Main Menu Bar
        if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::MenuItem("Appearance", "\tCtrl+A", !appearanceWindowHidden))
			{ 
				appearanceWindowHidden = (appearanceWindowHidden == 1)? 0 : 1;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scenes"))
		{
			if (ImGui::MenuItem("Collision Scene", NULL, (settings.scene_number == 0)))
			{
				settings.scene_has_changed = 1;
				settings.scene_number = 0;
			}
			if (ImGui::MenuItem("Pinball Scene", NULL, (settings.scene_number == 1)))
			{
				settings.scene_has_changed = 1;
				settings.scene_number = 1;
			}
			if (ImGui::MenuItem("Squishy Square Scene", NULL, (settings.scene_number == 2)))
			{
				settings.scene_has_changed = 1;
				settings.scene_number = 2;
			}
			if (ImGui::MenuItem("Squishy Rectangle Scene", NULL, (settings.scene_number == 3)))
			{
				settings.scene_has_changed = 1;
				settings.scene_number = 3;
			}
			if (ImGui::MenuItem("Colour Test Scene", NULL, (settings.scene_number == 4)))
			{
				settings.scene_has_changed = 1;
				settings.scene_number = 4;
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

	// Call the functions which draw all other windows
	DrawAppearanceWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Draws the "appearance" imgui window (only if appearanceWindowHidden is false)
static void DrawAppearanceWindow()
{
	if (appearanceWindowHidden)
		return;
	
	// Begin the window
	ImGui::Begin("Appearance", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// Set the correct position & scale
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 initial_size(500, 500);
	ImVec2 initial_pos(io.DisplaySize.x - ImGui::GetWindowWidth() - 10, 30);
	ImGui::SetWindowSize(initial_size, ImGuiCond_None);
	ImGui::SetWindowPos(initial_pos, ImGuiCond_None);

	// Create Contents of appearance window
	if (ImGui::CollapsingHeader("Colour"))
	{
		// "Scene" will hold a number of drop down boxes (ImGui "Combos") which will allow you
		// to choose the colour of certain objects in the scene
		if (ImGui::TreeNode("Scene"))
		{
			// Here we create a list of colours which the user will be able to select from.
			// this list of colours MUST be in the same order as the colour palette enum (from colours.hpp)
			// This is so that we can set the scene colour to be "n" of "items[]" (because it would
			// correspond to the same colour in the enum)
			const char* items[] = { "Gray", "Red", "Green", "Yellow", "Blue", "Purple", "Aqua", "White",
						"Dark Gray", "Dark Red", "Dark Green", "Dark Yellow", "Dark Blue",
						"Dark Purple", "Dark Aqua", "Dark White" };

			// Combo (or "Dropdown menu") for background colour
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
			const char* circle_preview_value = items[settings.scene_colours.particle];
			if (ImGui::BeginCombo("Circles", circle_preview_value, 0))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (settings.scene_colours.particle == n);

					if (ImGui::Selectable(items[n], is_selected))
						settings.scene_colours.particle = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			// Selection box for circle outlines colour
			const char* circle_outline_preview_value = items[settings.scene_colours.particle_outline];
			if (ImGui::BeginCombo("Circle Outlines", circle_outline_preview_value, 0))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (settings.scene_colours.particle_outline == n);

					if (ImGui::Selectable(items[n], is_selected))
						settings.scene_colours.particle_outline = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::TreePop();
		}
		// "Palette" will hold buttons for changing the colour palette
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
	// Create Renderer options
	if (ImGui::CollapsingHeader("Renderer"))
	{
		// Add a slider which will control the resolution of circles
		ImGui::SliderInt("Circle Resolution", &settings.circle_res, 3, 100);
		// Add a description to that slider describing what "Circle Resolution" means
		ImGui::SameLine();
		ImGui::TextDisabled("(?)");
    		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
		{
			const char* desc = "Controls how many vertices make up the circles which are rendered.\n"
					   "The higher the resolution, the smoother the circles appear, and the\n"
					   "lower the resolution, the more sharp and polygonal the circles appear.\n";
        		ImGui::SetTooltip("%s", desc);
		}

		// TEMPORARY SLIDER FOR SIMULATION HERTZ
		ImGui::SliderInt("SIMULATION HERTZ", &settings.simulation_hertz, 1, 500);
		// TEMPORARY SLIDER FOR SUB STEPS
		ImGui::SliderInt("SUB STEPS", &settings.sub_steps, 1, 500);
		// TEMPORARY SLIDER FOR FPS LIMIT
		ImGui::SliderInt("APPLICATION FRAME LIMIT", &settings.frame_limit, 1, 500);
	}
	ImGui::End();
}


#endif
