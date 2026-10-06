#include "TEMPLATE-Menu.h"

void MenuToolTemplate::ApplyWidgetState()
{
	HudWidgetWindow::GetSingleton()->SetOpen(_showWidgets);
	HudWidgetRightAnchor::GetSingleton()->SetOpen(_showWidgets);
}

void MenuToolTemplate::LoadSettings()
{
	// Called once from FUCK_Register::Install(), so the hotkey and widget visibility apply from the start.
	// NOTE: To use FUCK::PluginSettings and FUCK::INI, you must have 
	// #define FUCK_API_ENABLE_SIMPLEINI placed before including "FUCK_API.h" in your PCH.
	FUCK::PluginSettings settings;
	
	settings.Load([this](CSimpleIniA& ini) {
		_showWidgets  = FUCK::INI::LoadBool(ini,  "Settings", "ShowWidgets", true);
		_featureValue = FUCK::INI::LoadFloat(ini, "Settings", "FeatureValue", 50.0f);
		_selectedType = FUCK::INI::LoadInt(ini,   "Settings", "FeatureType", 0);
	});

	// FUCK::ManagedHotkey stores Mouse/KB and Gamepad bindings separately.
	// We load both, including up to 2 chorded modifiers (Shift, Ctrl, Alt) for each.
	settings.LoadKeybinds([this](CSimpleIniA& ini) {
		_toggleHotkey.kKey  = FUCK::INI::LoadInt(ini, "Toggle", "Key", 0);
		_toggleHotkey.kMod1 = FUCK::INI::LoadInt(ini, "Toggle", "Mod1", -1);
		_toggleHotkey.kMod2 = FUCK::INI::LoadInt(ini, "Toggle", "Mod2", -1);
		_toggleHotkey.gKey  = FUCK::INI::LoadInt(ini, "Toggle", "GPKey", 0);
		_toggleHotkey.gMod1 = FUCK::INI::LoadInt(ini, "Toggle", "GPMod1", -1);
		_toggleHotkey.gMod2 = FUCK::INI::LoadInt(ini, "Toggle", "GPMod2", -1);
	});

	// Ensure widgets immediately reflect the loaded INI state
	ApplyWidgetState();
}

void MenuToolTemplate::OnClose()
{
	// Triggered automatically when the user clicks away from this tool or closes the menu.
	FUCK::PluginSettings settings;
	
	settings.Save([this](CSimpleIniA& ini) {
		FUCK::INI::SaveBool(ini,   "Settings", "ShowWidgets",  _showWidgets, true);
		FUCK::INI::SaveDouble(ini, "Settings", "FeatureValue", _featureValue, 50.0f);
		FUCK::INI::SaveInt(ini,    "Settings", "FeatureType",  _selectedType, 0);
	});

	// Delta saving handles removing unchanged keys to keep the INI clean.
	settings.SaveKeybinds([this](CSimpleIniA& ini) {
		FUCK::INI::SaveInt(ini, "Toggle", "Key",    _toggleHotkey.kKey, 0);
		FUCK::INI::SaveInt(ini, "Toggle", "Mod1",   _toggleHotkey.kMod1, -1);
		FUCK::INI::SaveInt(ini, "Toggle", "Mod2",   _toggleHotkey.kMod2, -1);
		FUCK::INI::SaveInt(ini, "Toggle", "GPKey",  _toggleHotkey.gKey, 0);
		FUCK::INI::SaveInt(ini, "Toggle", "GPMod1", _toggleHotkey.gMod1, -1);
		FUCK::INI::SaveInt(ini, "Toggle", "GPMod2", _toggleHotkey.gMod2, -1);
	});
}

bool MenuToolTemplate::OnAsyncInput(const void* e)
{
	// 1. If we are actively assigning a hotkey in the FUCK menu, intercept the input for binding.
	if (_toggleHotkey.isBinding) {
		if (FUCK::UpdateManagedHotkey(e, _toggleHotkey)) {
			return true;  // Input absorbed by the binder
		}
	}
	// 2. Otherwise, if normal gameplay is happening, check if the user pressed the hotkey.
	else if (!FUCK::IsMenuOpen()) {
		if (FUCK::ProcessManagedHotkey(e, _toggleHotkey)) {
			if (_toggleHotkey.wasTriggered) {
				// Toggle visibility
				_showWidgets = !_showWidgets;
				ApplyWidgetState();

				// Save instantly so the change persists even if the user never opens the FUCK menu
				FUCK::PluginSettings settings;
				settings.Save([this](CSimpleIniA& ini) {
					FUCK::INI::SaveBool(ini, "Settings", "ShowWidgets", _showWidgets, true);
				});

				return true;
			}
		}
	}
	return false;
}

void MenuToolTemplate::Draw()
{
	// NOTE: The "_T" operator on string literals automatically swaps the $key with
	// the translated value from your localisation text file (e.g. "$TEMPL_MenuTitle"_T).
	//
	// IMPORTANT: Your localisation text files (e.g. FUCK-Template_ENGLISH.txt) must be
	// saved with UTF-16LE encoding, otherwise the translation will fail.
	FUCK::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "$TEMPL_MenuTitle"_T);
	FUCK::Separator();
	FUCK::Spacing();

	// Toggles the widgets instantly when clicked
	if (FUCK::Checkbox("$TEMPL_ShowWidgets"_T, &_showWidgets)) {
		ApplyWidgetState();
	}

	FUCK::Spacing(2);
	FUCK::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "$TEMPL_Hotkeys"_T);
	FUCK::Separator();
	FUCK::Spacing();

	FUCK::TextDisabled("$TEMPL_HotkeyNote"_T);
	FUCK::Spacing();

	FUCK::DrawManagedHotkey("$TEMPL_ToggleHotkey"_T, _toggleHotkey);

	FUCK::Spacing(2);
	FUCK::Separator();
	FUCK::Spacing();

	// Gray out the rest of the demo if widgets are disabled
	if (!_showWidgets) {
		FUCK::BeginDisabled();
	}

	FUCK::TextDisabled("$TEMPL_DemoNote"_T);
	FUCK::Spacing();

	FUCK::SliderFloat("$TEMPL_FeatureIntensity"_T, &_featureValue, 0.0f, 100.0f, "%.1f");

	// Combo boxes require an array of const char* for the items
	const char* types[] = { "$TEMPL_OptionA"_T, "$TEMPL_OptionB"_T, "$TEMPL_OptionC"_T };
	FUCK::Combo("$TEMPL_FeatureType"_T, &_selectedType, types, 3);

	if (!_showWidgets) {
		FUCK::EndDisabled();
	}

	FUCK::Spacing(2);
	FUCK::SeparatorText("$TEMPL_Actions"_T);

	if (FUCK::Button("$TEMPL_Reset"_T)) {
		_showWidgets  = true;
		_featureValue = 50.0f;
		_selectedType = 0;
		_toggleHotkey.Clear();  // Clears all mapped keys/modifiers for this hotkey
		ApplyWidgetState();
	}
}
