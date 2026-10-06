#pragma once

#include "HUD-UI.h"

namespace FUCK_Register
{
	inline void Install()
	{
		// Initialize our listeners (hooks up HUDMenu detection)
		HudWidgetWindow::GetSingleton()->Initialize();

		// Register the window into the framework so FUCK draws it
		FUCK::RegisterWindow(HudWidgetWindow::GetSingleton());
	}
}
