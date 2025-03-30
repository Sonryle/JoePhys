#include "GUIManager.hpp"
#include "SceneManager.hpp"
#include "WindowManager.hpp"
#include "imgui.h"

// Constructor
GUIManager::GUIManager() : appearance_window_shown(0), simulation_window_shown(0), options_window_shown(0)
{
	return;
}

// creates an imgui context and initiates GLFW & OpenGL for imgui
void GUIManager::Init(GLFWwindow* window)
{
	ImGui::CreateContext();	
	ImGui_ImplGlfw_InitForOpenGL(window, 1);
	ImGui_ImplOpenGL3_Init();


	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 0.85f;
	style.FrameRounding = 4.0f;
}
// Toggles the "appearance" window on and off
void GUIManager::ToggleAppearanceWindow()
{
	appearance_window_shown = !appearance_window_shown;
}

// Toggles the "appearance" window on and off
void GUIManager::ToggleSimulationWindow()
{
	simulation_window_shown = !simulation_window_shown;
}

void GUIManager::ToggleOptionsWindow()
{
	options_window_shown = !options_window_shown;
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
				appearance_window_shown = !appearance_window_shown;
			}
			if (ImGui::MenuItem("Simulation", "\tCtrl+S", simulation_window_shown))
			{ 
				simulation_window_shown = !simulation_window_shown;
			}
			if (ImGui::MenuItem("Options", "\tCtrl+W", options_window_shown))
			{ 
				options_window_shown = !options_window_shown;
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
			if (ImGui::MenuItem("Newtons Cradle Scene", NULL, (scene_manager.current_scene_number == 4)))
				scene_manager.SwitchScene(4);
			if (ImGui::MenuItem("Cloth Scene", NULL, (scene_manager.current_scene_number == 5)))
				scene_manager.SwitchScene(5);
			if (ImGui::MenuItem("Rope Scene", NULL, (scene_manager.current_scene_number == 6)))
				scene_manager.SwitchScene(6);
			if (ImGui::MenuItem("Pendulum Clock Scene", NULL, (scene_manager.current_scene_number == 7)))
				scene_manager.SwitchScene(7);
			if (ImGui::MenuItem("Colour Pallete Test Scene", NULL, (scene_manager.current_scene_number == 8)))
				scene_manager.SwitchScene(8);

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
	DrawSimulationWindow();
	DrawOptionsWindow();

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
	ImVec2 initial_size(500, 250);
	ImVec2 initial_pos(io.DisplaySize.x - ImGui::GetWindowWidth() - 10, 30);
	ImGui::SetWindowSize(initial_size, ImGuiCond_None);
	ImGui::SetWindowPos(initial_pos, ImGuiCond_None);

	// Create colour palette options
	if (ImGui::CollapsingHeader("Colour Palette"))
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
	}
	ImGui::End();
}


// Draws the "simulation" imgui window (only if simulationWindowHidden is false)
void GUIManager::DrawSimulationWindow()
{
	if (!simulation_window_shown)
		return;
	
	// Begin the window
	ImGui::Begin("Simulation", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// Set the correct position & scale
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 initial_size(500, 250);
	ImVec2 initial_pos(io.DisplaySize.x - ImGui::GetWindowWidth() - 10, 295);
	ImGui::SetWindowSize(initial_size, ImGuiCond_None);
	ImGui::SetWindowPos(initial_pos, ImGuiCond_None);

	// Create Contents of simulation window
	ImGui::Separator();
	ImGui::SliderInt("Sub Steps", &settings.sub_steps, 4, 64);
	ImGui::SliderInt("Time Divisor", &settings.time_divisor, 1, 50);
	ImGui::Separator();
	if (ImGui::CollapsingHeader("World"))
	{
		ImGui::Checkbox("Gravity", &settings.enable_gravity);
		ImGui::Checkbox("Drag", &settings.enable_drag);
		ImGui::Checkbox("Springs", &settings.enable_springs);
		ImGui::Checkbox("Particle Movement", &settings.enable_particle_movement);
		ImGui::Checkbox("Particle Collision", &settings.enable_collision);
	}
	ImGui::End();
}

// Draws the "simulation" imgui window (only if simulationWindowHidden is false)
void GUIManager::DrawOptionsWindow()
{
	if (!options_window_shown)
		return;
	
	// Begin the window
	ImGui::Begin("Options", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// Set the correct position & scale
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 initial_size(500, 250);
	ImVec2 initial_pos(io.DisplaySize.x - ImGui::GetWindowWidth() - 10, 295 + 250 + 15);
	ImGui::SetWindowSize(initial_size, ImGuiCond_None);
	ImGui::SetWindowPos(initial_pos, ImGuiCond_None);

	// Create Contents of simulation window
	ImGui::Separator();
	ImGui::SliderFloat("TEMPORARY A", &settings.attraction_tool_strength, 1, 50);
	ImGui::SliderFloat("TEMPORARY R", &settings.repulsion_tool_strength, 1, 50);
	ImGui::Separator();
	if (ImGui::Checkbox("Fullscreen", &settings.is_fullscreen))
		window_manager.SetFullscreen(settings.is_fullscreen);
	ImGui::End();
}

GUIManager gui_manager;
