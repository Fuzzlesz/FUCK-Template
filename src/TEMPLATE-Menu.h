#pragma once

#include "TEMPLATE-HUD.h"  // Needed to toggle the HUD widgets

// A template for a sidebar configuration menu, a page in the FUCK menu.
// It derives from FUCK::ITool to use OnClose (saving when the page is closed) and OnAsyncInput (watching for its hotkey in
// gameplay). A page that only draws doesn't need a class like this: see FUCK::SimpleTool in FUCK_API.h.
class MenuToolTemplate :
	public FUCK::ITool,
	public REX::Singleton<MenuToolTemplate>
{
public:
	// The settings, each with its default. Public so the INI fields can name its members (see TEMPLATE-Menu.cpp)
	struct Values
	{
		bool  showWidgets  = true;
		float featureValue = 50.0f;
		int   selectedType = 0;
	};

	// --- ITool Overrides ---

	// Name(): The name of this specific tool page in the sidebar list (e.g. "General Settings")
	const char* Name() const override { return "$TEMPL_ToolName"_T; }

	// Group(): The collapsible header this tool sits under in the FUCK sidebar. Tools naming the same group share it.
	// Here "$TEMPL_Group_General"_T puts it under "DEMO - Template". Use "::" to nest, e.g. "Parent::Child",
	// or return nullptr for no group.
	const char* Group() const override { return "$TEMPL_Group_General"_T; }

	void Draw() override;
	void OnClose() override;
	bool OnAsyncInput(const void* e) override;

	// Loads the settings and hotkey. Called once at startup, so both work before the tool is ever opened
	void LoadSettings();

private:
	// Helper to push the boolean state to the actual widgets
	void ApplyWidgetState();

	// Saves the settings (not the hotkey). Called by OnClose, and by the hotkey, which toggles the widgets in gameplay
	void SaveSettings() const;

	Values _values;

	// Hotkey struct for toggling widget visibility
	FUCK::ManagedHotkey _toggleHotkey;
};
