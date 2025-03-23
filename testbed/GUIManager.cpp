#include "GUIManager.hpp"
#include "SceneManager.hpp"

// Constructor
GUIManager::GUIManager() : appearance_window_shown(0)
{
	return;
}

// creates an imgui context and initiates GLFW & OpenGL for imgui
void GUIManager::Init(GLFWwindow* window)
{
	ImGui::CreateContext();	
	ImGui_ImplGlfw_InitForOpenGL(window, 1);
	ImGui_ImplOpenGL3_Init();
}

// Toggles the "appearance" window on and off
void GUIManager::ToggleAppearanceWindow()
{
	appearance_window_shown = !appearance_window_shown;
}

// Draws a menu bar at the top of the program, and calls functions to draw every other
// imgui window as well, (e.g. the appearance window).
void GUIManager::DrawGui()
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
			if (ImGui::MenuItem("Appearance", "\tCtrl+A", appearance_window_shown))
			{ 
				appearance_window_shown = (appearance_window_shown == 1)? 0 : 1;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scenes"))
		{
			if (ImGui::MenuItem("Collision Scene", NULL, (scene_manager.current_scene_number == 0)))
				scene_manager.SwitchScene(0);
			if (ImGui::MenuItem("Pinball Scene", NULL, (scene_manager.current_scene_number == 1)))
				scene_manager.SwitchScene(1);
			if (ImGui::MenuItem("Squishy Square Scene", NULL, (scene_manager.current_scene_number == 2)))
				scene_manager.SwitchScene(2);
			if (ImGui::MenuItem("Squishy Rectangle Scene", NULL, (scene_manager.current_scene_number == 3)))
				scene_manager.SwitchScene(3);
			if (ImGui::MenuItem("Colour Test Scene", NULL, (scene_manager.current_scene_number == 4)))
				scene_manager.SwitchScene(4);

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
void GUIManager::DrawAppearanceWindow()
{
	if (!appearance_window_shown)
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

GUIManager gui_manager;
