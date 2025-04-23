#include "GUIManager.hpp"
#include "SceneManager.hpp"
#include "WindowManager.hpp"
#include "imgui.h"

// Constructor
GUIManager::GUIManager()
{
	appearance_window_shown = 0;
	simulation_window_shown = 0;
	options_window_shown = 0;
	learn_window_shown = 0;
}

// creates an imgui context and initiates GLFW & OpenGL for imgui
void GUIManager::Init(GLFWwindow* window)
{
	ImGui::CreateContext();	
	ImGui_ImplGlfw_InitForOpenGL(window, 1);
	ImGui_ImplOpenGL3_Init();

	ImGui::GetIO().IniFilename = nullptr;

	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 0.85f;
	style.FrameRounding = 4.0f;
}

// Renders the UI
void GUIManager::Render()
{
	AddMenuBar();

	if (appearance_window_shown)
		AddAppearanceWindow();
	if (simulation_window_shown)
		AddSimulationWindow();
	if (options_window_shown)
		AddOptionsWindow();
	if (learn_window_shown)
		AddLearnWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Draws a menu bar at the top of the program, and calls functions to draw every other
// imgui window as well, (e.g. the appearance window).
void GUIManager::AddMenuBar()
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
				appearance_window_shown = !appearance_window_shown;
			if (ImGui::MenuItem("Simulation", "\tCtrl+S", simulation_window_shown))
				simulation_window_shown = !simulation_window_shown;
			if (ImGui::MenuItem("Options", "\tCtrl+O", options_window_shown))
				options_window_shown = !options_window_shown;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scenes"))
		{
			if (ImGui::MenuItem("Newtons Cradle Scene", NULL, (scene_manager.current_scene_number == 0)))
				scene_manager.SwitchScene(0);
			if (ImGui::MenuItem("Cloth Scene", NULL, (scene_manager.current_scene_number == 1)))
				scene_manager.SwitchScene(1);
			if (ImGui::MenuItem("Rope Scene", NULL, (scene_manager.current_scene_number == 2)))
				scene_manager.SwitchScene(2);
			if (ImGui::MenuItem("Pinball Scene", NULL, (scene_manager.current_scene_number == 3)))
				scene_manager.SwitchScene(3);
			if (ImGui::MenuItem("Hour Glass Scene", NULL, (scene_manager.current_scene_number == 4)))
				scene_manager.SwitchScene(4);
			if (ImGui::MenuItem("Wrecking Ball Scene", NULL, (scene_manager.current_scene_number == 5)))
				scene_manager.SwitchScene(5);
			if (ImGui::MenuItem("Weights Scene", NULL, (scene_manager.current_scene_number == 6)))
				scene_manager.SwitchScene(6);
			if (ImGui::MenuItem("Air Resistance Scene", NULL, (scene_manager.current_scene_number == 7)))
				scene_manager.SwitchScene(7);
			if (ImGui::MenuItem("Colour Pallete Test Scene", NULL, (scene_manager.current_scene_number == 8)))
				scene_manager.SwitchScene(8);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Learn How To Use Testing Environment", NULL, learn_window_shown))
				learn_window_shown = !learn_window_shown;
			ImGui::EndMenu();
		}

		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("https://github.com/Sonryle/JoePhys ").x);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
		ImGui::Text("https://github.com/Sonryle/JoePhys"); ImGui::PopStyleColor();

		ImGui::EndMainMenuBar();
	}
}

void GUIManager::AddAppearanceWindow()
{
	// Begin the window
	ImGui::Begin("Appearance", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// Set the correct position & scale
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 initial_size(550, 250);
	ImVec2 initial_pos(io.DisplaySize.x - ImGui::GetWindowWidth() - 10, 30);
	ImGui::SetWindowSize(initial_size, ImGuiCond_None);
	ImGui::SetWindowPos(initial_pos, ImGuiCond_None);

	// Create colour palette options
	if (ImGui::CollapsingHeader("Colour Palette"))
	{
		if(ImGui::Button("Set Autumn"))
		{
			palette.SetAutumn();
			colour bg = palette.colours[scene_manager.current_scene->colours.background];
			glClearColor(bg.r, bg.g, bg.b, bg.a);
		}
		if(ImGui::Button("Set Gruvbox"))
		{
			palette.SetGruvbox();
			colour bg = palette.colours[scene_manager.current_scene->colours.background];
			glClearColor(bg.r, bg.g, bg.b, bg.a);
		}
		if(ImGui::Button("Set Pastel"))
		{
			palette.SetPastel();
			colour bg = palette.colours[scene_manager.current_scene->colours.background];
			glClearColor(bg.r, bg.g, bg.b, bg.a);
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

// Draws a circle and allows user to edit the value of a vector inside it
void AddArrowDiagram(float* input, float max_val)
{
	float canvas_radius = 50.0f;  // Canvas circle radius
	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
	ImVec2 canvas_center = ImVec2(canvas_pos.x + canvas_radius + 15, canvas_pos.y + canvas_radius + 15);
	ImVec2 arrow_pos = ImVec2(((input[0] / max_val) * canvas_radius) + canvas_center.x, ((input[1] / max_val) * -canvas_radius) + canvas_center.y);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImGui::InvisibleButton("circular_canvas", ImVec2(canvas_radius * 2 + 15, canvas_radius * 2 + 30), ImGuiButtonFlags_MouseButtonLeft);
	
	// Draw the circular canvas
	colour g = palette.colours[palette.JP_DARK_GRAY];
	draw_list->AddCircle(canvas_center, canvas_radius, IM_COL32(g.r * 255, g.g * 255, g.b * 255, g.a * 255), 64, 2.0f);
	draw_list->AddCircleFilled(canvas_center, 3.0f, IM_COL32(g.r * 255, g.g * 255, g.b * 255, g.a * 255));
	
	if (settings.enable_gravity)
	{
		// Draw arrow
		colour p = palette.colours[scene_manager.current_scene->colours.particle];
		draw_list->AddLine(canvas_center, arrow_pos, IM_COL32(p.r * 255, p.g * 255, p.b * 255, p.a * 255), 2.0f);
		draw_list->AddCircleFilled(arrow_pos, 5.0f, IM_COL32(p.r * 255, p.g * 255, p.b * 255, p.a * 255));

		// If cursor is inside circle
		if (length(window_mgr.cursor_pos - vec2(canvas_center.x, canvas_center.y)) <= canvas_radius)
		{
			// Draw vector coordinates near the arrow tip
			char buffer[32];
			snprintf(buffer, sizeof(buffer), "(%.1f, %.1f)", input[0], input[1]);
			draw_list->AddText(ImVec2(arrow_pos.x + 5, arrow_pos.y - 10), IM_COL32(255, 255, 255, 255), buffer);
		}
	}

	// Update the input
	if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
	{
		settings.enable_gravity = 1;
		vec2 new_pos((window_mgr.cursor_pos - vec2(canvas_center.x, canvas_center.y)) / canvas_radius);

		if (length(new_pos) > 1.0f)
			new_pos = normalize(new_pos);

		// Apply new vector to our input
		input[0] = new_pos.x * max_val;
		input[1] = -new_pos.y * max_val;
	}

}

void GUIManager::AddSimulationWindow()
{
	// Begin the window
	ImGui::Begin("Simulation", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// Set the correct position & scale
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 initial_size(550, 360);
	ImVec2 initial_pos(io.DisplaySize.x - ImGui::GetWindowWidth() - 10, 295);
	ImGui::SetWindowSize(initial_size, ImGuiCond_None);
	ImGui::SetWindowPos(initial_pos, ImGuiCond_None);

	// If there is no world, then skip
	if (scene_manager.current_scene->world == nullptr)
	{
		ImGui::End();
		return;
	}

	// Create Contents of simulation window
	
	if (ImGui::Button("Set Default Scene Settings"))
		scene_manager.current_scene->SetUpSceneSettings();

	// Add a slider for the substeps and a description of what substeps do
	ImGui::SliderInt("Sub Steps", &settings.sub_steps, 1, 64);
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");
    	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		const char* desc = "If sub steps is set to 4 for example, then the physics simulation will\n"
				   "break up one physics step into 4 smaller sub steps. This increases the\n"
				   "accuracy of the simulation, as the computer has to aproximate smaller changes\n"
				   "in time, but it is more computationally expensive.";
       		ImGui::SetTooltip("%s", desc);
	}

	// Add a slider for the time divisor and a description of what it does
	ImGui::SliderInt("Time Divisor", &settings.time_divisor, 1, 50);
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");
    	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		const char* desc = "Time divisor slows down time. The simulation hertz (number of time steps per simulated second) is multiplied\n"
				   "by the the time divisor. This slows down time when the time divisor is greater than 1 because the simulation\n"
				   "hertz becomes out of sync with the framerate of the program. (both the framerate limit and the simulation\n"
				   "hertz are set to 120 by default, and we go forward by one time step every frame, so when the simulation hertz\n"
				   "is greater than the framerate of the program, time appears to have slowed down)\n";
       		ImGui::SetTooltip("%s", desc);
	}
	ImGui::SliderFloat("Chunk Scale", &settings.chunk_scale, 0.05f, 2.0f);
	ImGui::Checkbox("Gravity (m/s²)", &settings.enable_gravity);
	AddArrowDiagram(&settings.gravity.x, 50.0f);
	ImGui::Checkbox("Drag (Air Resistance)", &settings.enable_drag);
	ImGui::Checkbox("Springs", &settings.enable_springs);
	ImGui::Checkbox("Particle Movement", &settings.enable_particle_movement);
	ImGui::Checkbox("Particle Collision", &settings.enable_collision);
	ImGui::End();
}

void GUIManager::AddOptionsWindow()
{
	// Begin the window
	ImGui::Begin("Options", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// Set the correct position & scale
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 initial_size(550, 210);
	ImVec2 initial_pos(io.DisplaySize.x - ImGui::GetWindowWidth() - 10, 295 + 375);
	ImGui::SetWindowSize(initial_size, ImGuiCond_None);
	ImGui::SetWindowPos(initial_pos, ImGuiCond_None);

	// Create Contents of simulation window
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Attraction Tool"))
	{
		ImGui::SliderFloat("Strength ", &settings.attraction_tool_strength, 1, 50, "%g");
		ImGui::SliderFloat("Radius ", &settings.attraction_tool_radius, 1, 50, "%g");
	} 
	if (ImGui::CollapsingHeader("Repulsion Tool"))
	{
		ImGui::SliderFloat("Strength", &settings.repulsion_tool_strength, 1, 50, "%g");
		ImGui::SliderFloat("Radius", &settings.repulsion_tool_radius, 1, 50, "%g");
	}
	ImGui::Separator();
	ImGui::Checkbox("Pause", &settings.is_paused);
	ImGui::SameLine();
	ImGui::TextDisabled("\tCtrl+P");
	if (settings.is_paused)
	{
		if (ImGui::Button("Step Forward"))
			scene_manager.current_scene->Step();
		ImGui::SameLine();
		ImGui::TextDisabled("\tCtrl+F");
	}
	ImGui::Checkbox("Render Chunks", &settings.render_chunks);
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		const char* desc = "Chunks are only created/destroyed if particle collision is enabled.";
		ImGui::SetTooltip("%s", desc);
	}
	if (ImGui::Checkbox("Fullscreen", &settings.is_fullscreen))
		window_mgr.SetFullscreen(settings.is_fullscreen);
	ImGui::End();
}

void GUIManager::AddLearnWindow()
{
	// Begin the window
	ImGui::Begin("Learn How To Use The Testing Environment", NULL, ImGuiWindowFlags_None);

	// Set the correct position & scale
	ImGuiIO io = ImGui::GetIO();
	ImVec2 initial_size(io.DisplaySize.x * 0.6f, io.DisplaySize.y * 0.5f);
	ImVec2 initial_pos(io.DisplaySize.x / 2 - initial_size.x / 2, io.DisplaySize.y / 2 - initial_size.y / 2);
	ImGui::SetWindowSize(initial_size, ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(initial_pos, ImGuiCond_FirstUseEver);

	// Describe how to use program
	ImGui::SeparatorText("Scene Navigation:");
	ImGui::Spacing();

	ImGui::BulletText("Switch between scenes using the \"Scenes\" dropdown at the top of the program");
	ImGui::Indent();
	ImGui::BulletText("Alternatively, hold CTRL and press the left or right arrow keys to navigate through scenes");
	ImGui::Unindent();
	ImGui::Spacing();
	ImGui::BulletText("To restart any scene, hold CTRL and press the R key");
	ImGui::BulletText("To pan the camera, hold CTRL, then click and drag");
	ImGui::BulletText("To zoom the camera, hold CTRL and use the scroll wheel");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::SeparatorText("Particle Interaction:");
	ImGui::Spacing();

	ImGui::Text("You will notice that the closest particle to your cursor is always highlighted");
	ImGui::Spacing();
	ImGui::BulletText("You can click on the scene to teleport the selected particle to your cursor");
	ImGui::BulletText("You can scroll to increase the selected particle's radius");
	ImGui::Indent();
	ImGui::BulletText("Note that the mass of the particle will scale in direct proportion to its change in size");
	ImGui::Unindent();
	ImGui::Spacing();
	ImGui::BulletText("You can create an attraction force around your cursor by pressing A");
	ImGui::Indent();
	ImGui::BulletText("The radius and strength of this tool can be changed in the options window (settings->options->attraction tool");
	ImGui::Unindent();
	ImGui::Spacing();
	ImGui::BulletText("You can create a repulsion force around your cursor by pressing R");
	ImGui::Indent();
	ImGui::BulletText("The radius and strength of this tool can be changed in the options window (settings->options->repulsion tool");
	ImGui::Unindent();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::SeparatorText("World Interaction:");
	ImGui::Spacing();

	ImGui::BulletText("You can spawn new particles by pressing P");
	ImGui::Indent();
	ImGui::BulletText("Press and hold P to spawn particles at rapid speeds!");
	ImGui::Unindent();




	ImGui::End();
}

GUIManager gui_manager;
