// Contains functions which set up the GUI for JoePhys (using imgui)
#ifndef JP_GUI
#define JP_GUI

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "Settings.hpp"
#include "SceneManager.hpp"

struct GUIManager
{
	// constructor & destructor
	GUIManager();
	~GUIManager() {}

	// functions
	void Init(GLFWwindow* window);
	void AddMenuBar();
	void AddAppearanceWindow();
	void AddSimulationWindow();
	void AddOptionsWindow();
	void AddLearnUIWindow();
	void AddLearnJoePhysWindow();
	void AddProgrammerGuideWindow();
	void Render();
	
	// variables

	bool appearance_window_shown;
	bool simulation_window_shown;
	bool options_window_shown;

	bool learn_ui_window_shown;
	bool learn_joephys_window_shown;
	bool programmer_guide_window_shown;
};

extern GUIManager gui_manager;

#endif
