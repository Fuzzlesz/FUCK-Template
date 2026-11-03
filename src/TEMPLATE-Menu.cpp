#include "TEMPLATE-Menu.h"

// NOTE: To use FUCK::PluginSettings and FUCK::INI, you must have
// #define FUCK_API_ENABLE_SIMPLEINI placed before including "FUCK_API.h" in your PCH.
namespace
{
	// Each INI key named once, with the member it fills. Defaults come from the struct, and a value left at its default is
	// taken out of the user's INI rather than written, keeping it clean
	const FUCK::INI::Fields<MenuToolTemplate::Values> SETTINGS{ "Settings", {
		{ "ShowWidgets",  &MenuToolTemplate::Values::showWidgets  },
		{ "FeatureValue", &MenuToolTemplate::Values::featureValue },
		{ "FeatureType",  &MenuToolTemplate::Values::selectedType },
	} };

	// Works for any struct, so the hotkey's own. FUCK::ManagedHotkey stores Mouse/KB and Gamepad bindings separately,
	// each with up to 2 chorded modifiers (Shift, Ctrl, Alt)
	const FUCK::INI::Fields<FUCK::ManagedHotkey> TOGGLE_KEYS{ "Toggle", {
		{ "Key",    &FUCK::ManagedHotkey::kKey  },
		{ "Mod1",   &FUCK::ManagedHotkey::kMod1 },
		{ "Mod2",   &FUCK::ManagedHotkey::kMod2 },
		{ "GPKey",  &FUCK::ManagedHotkey::gKey  },
		{ "GPMod1", &FUCK::ManagedHotkey::gMod1 },
		{ "GPMod2", &FUCK::ManagedHotkey::gMod2 },
	} };
}

void MenuToolTemplate::ApplyWidgetState()
{
	HudWidgetWindow::GetSingleton()->SetOpen(_values.showWidgets);
	HudWidgetRightAnchor::GetSingleton()->SetOpen(_values.showWidgets);
}

void MenuToolTemplate::LoadSettings()
{
	// Called once from FUCK_Register::Install(), so the hotkey and widget visibility apply from the start
	FUCK::PluginSettings settings;
	settings.Load(SETTINGS, _values);
	settings.LoadKeybinds([this](CSimpleIniA& ini) { TOGGLE_KEYS.Load(ini, _toggleHotkey); });

	// Ensure widgets immediately reflect the loaded INI state
	ApplyWidgetState();
}

void MenuToolTemplate::SaveSettings() const
{
	FUCK::PluginSettings().Save(SETTINGS, _values);
}

void MenuToolTemplate::OnClose()
{
	// Triggered automatically when the user clicks away from this tool or closes the menu
	SaveSettings();
	FUCK::PluginSettings().SaveKeybinds([this](CSimpleIniA& ini) { TOGGLE_KEYS.Save(ini, _toggleHotkey); });
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
				_values.showWidgets = !_values.showWidgets;
				ApplyWidgetState();

				// Save instantly so the change persists even if the user never opens the FUCK menu
				SaveSettings();

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
	if (FUCK::Checkbox("$TEMPL_ShowWidgets"_T, &_values.showWidgets)) {
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
	if (!_values.showWidgets) {
		FUCK::BeginDisabled();
	}

	FUCK::TextDisabled("$TEMPL_DemoNote"_T);
	FUCK::Spacing();

	FUCK::SliderFloat("$TEMPL_FeatureIntensity"_T, &_values.featureValue, 0.0f, 100.0f, "%.1f");

	// Combo boxes require an array of const char* for the items
	const char* types[] = { "$TEMPL_OptionA"_T, "$TEMPL_OptionB"_T, "$TEMPL_OptionC"_T };
	FUCK::Combo("$TEMPL_FeatureType"_T, &_values.selectedType, types, 3);

	if (!_values.showWidgets) {
		FUCK::EndDisabled();
	}

	FUCK::Spacing(2);
	FUCK::SeparatorText("$TEMPL_Actions"_T);

	if (FUCK::Button("$TEMPL_Reset"_T)) {
		_values = {};           // Every setting back to its default
		_toggleHotkey.Clear();  // Clears all mapped keys/modifiers for this hotkey
		ApplyWidgetState();
	}
}
