#pragma once

#include "TEMPLATE-HUD.h"  // Needed to toggle the HUD widgets

// A template for a sidebar configuration menu.
class MenuToolTemplate :
	public FUCK::ITool,
	public REX::Singleton<MenuToolTemplate>
{
public:
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

	// State variables
	bool  _showWidgets  = true;
	float _featureValue = 50.0f;
	int   _selectedType = 0;

	// Hotkey struct for toggling widget visibility
	FUCK::ManagedHotkey _toggleHotkey;
};
