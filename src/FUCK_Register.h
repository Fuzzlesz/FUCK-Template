#pragma once

#include "TEMPLATE-HUD.h"
#include "TEMPLATE-Menu.h"

namespace FUCK_Register
{
	inline void Install()
	{
		// Load the widgets' images
		HudWidgetWindow::GetSingleton()->Initialize();
		HudWidgetRightAnchor::GetSingleton()->Initialize();

		// Load the settings and hotkey now, so they work before the tool is ever opened
		MenuToolTemplate::GetSingleton()->LoadSettings();

		// Register the window into the framework so FUCK draws it
		FUCK::RegisterWindow(HudWidgetWindow::GetSingleton());
		FUCK::RegisterWindow(HudWidgetRightAnchor::GetSingleton());

		// Register our new sidebar menu tool
		FUCK::RegisterTool(MenuToolTemplate::GetSingleton());
	}
}
