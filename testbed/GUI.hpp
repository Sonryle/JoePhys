#ifndef JP_GUI
#define JP_GUI

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

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
			if (ImGui::MenuItem("Appearance", NULL, appearanceWindowShown))
			{ 
				appearanceWindowShown = (appearanceWindowShown == 1)? 0 : 1;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scenes"))
		{
			if (ImGui::MenuItem("Newton's Cradle", NULL, appearanceWindowShown))
			{
				appearanceWindowShown = (appearanceWindowShown == 1)? 0 : 1;
			}
			if (ImGui::MenuItem("Colour Testing", NULL, appearanceWindowShown))
			{
				appearanceWindowShown = (appearanceWindowShown == 1)? 0 : 1;
			}
			if (ImGui::MenuItem("Angry Birds", NULL, appearanceWindowShown))
			{
				appearanceWindowShown = (appearanceWindowShown == 1)? 0 : 1;
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

	ImGui::Begin("Appearance", NULL, ImGuiWindowFlags_None);

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 initial_size(300, 500);
	ImVec2 initial_pos(io.DisplaySize.x - ImGui::GetWindowWidth(), 30);
	ImGui::SetWindowSize(initial_size, ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(initial_pos, ImGuiCond_FirstUseEver);

	if (ImGui::CollapsingHeader("Colour"))
	{
		if (ImGui::CollapsingHeader("Scene Colours"))
		{
			const char* items[] = { "Gray", "Red", "Green", "Yellow", "Blue", "Purple", "Aqua", "White",
						"Dark Gray", "Dark Red", "Dark Green", "Dark Yellow", "Dark Blue",
						"Dark Purple", "Dark Aqua", "Dark White" };
			static int item_selected = 0;
			const char* combo_preview_value = items[item_selected];
			if (ImGui::BeginCombo("Background", combo_preview_value, 0))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_selected == n);
					if (ImGui::Selectable(items[n], is_selected))
					{
						item_selected = n;
						switch (n)
						{
						case 0:
							settings.scene_colours.background = &palette.gray;
							break;
						case 1:
							settings.scene_colours.background = &palette.red;
							break;
						case 2:
							settings.scene_colours.background = &palette.green;
							break;
						case 3:
							settings.scene_colours.background = &palette.yellow;
							break;
						case 4:
							settings.scene_colours.background = &palette.blue;
							break;
						case 5:
							settings.scene_colours.background = &palette.purple;
							break;
						case 6:
							settings.scene_colours.background = &palette.aqua;
							break;
						case 7:
							settings.scene_colours.background = &palette.white;
							break;
						case 8:
							settings.scene_colours.background = &palette.dark_gray;
							break;
						case 9:
							settings.scene_colours.background = &palette.dark_red;
							break;
						case 10:
							settings.scene_colours.background = &palette.dark_green;
							break;
						case 11:
							settings.scene_colours.background = &palette.dark_yellow;
							break;
						case 12:
							settings.scene_colours.background = &palette.dark_blue;
							break;
						case 13:
							settings.scene_colours.background = &palette.dark_purple;
							break;
						case 14:
							settings.scene_colours.background = &palette.dark_aqua;
							break;
						case 15:
							settings.scene_colours.background = &palette.dark_white;
							break;
						}
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}
		if (ImGui::CollapsingHeader("Palette"))
		{
			ImGui::Separator();
			if (ImGui::CollapsingHeader("Colours"))
			{
				ImGui::ColorEdit4("Gray", palette.gray);
				ImGui::ColorEdit4("Red", palette.red);
				ImGui::ColorEdit4("Green", palette.green);
				ImGui::ColorEdit4("Yellow", palette.yellow);
				ImGui::ColorEdit4("Blue", palette.blue);
				ImGui::ColorEdit4("Purple", palette.purple);
				ImGui::ColorEdit4("Aqua", palette.aqua);
				ImGui::ColorEdit4("White", palette.white);
			}
			if (ImGui::CollapsingHeader("Dark Variants"))
			{
				ImGui::ColorEdit4("Dark Gray", palette.dark_gray);
				ImGui::ColorEdit4("Dark Red", palette.dark_red);
				ImGui::ColorEdit4("Dark Green", palette.dark_green);
				ImGui::ColorEdit4("Dark Yellow", palette.dark_yellow);
				ImGui::ColorEdit4("Dark Blue", palette.dark_blue);
				ImGui::ColorEdit4("Dark Purple", palette.dark_purple);
				ImGui::ColorEdit4("Dark Aqua", palette.dark_aqua);
				ImGui::ColorEdit4("Dark White", palette.dark_white);
			}
			if(ImGui::Button("Set Gruvbox"))
			{
				palette.SetGruvbox();
			}
			if(ImGui::Button("Set Purpbox"))
			{
				palette.SetPurpbox();
			}
			ImGui::Separator();
		}
	}
	ImGui::End();
}


#endif
