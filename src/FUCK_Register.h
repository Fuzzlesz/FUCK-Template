#pragma once

#include "HUD-UI.h"

namespace FUCK_Register
{
	inline void Install()
	{
		HudWidgetWindow::GetSingleton()->Initialize();
		HudWidgetRightAnchor::GetSingleton()->Initialize();

		FUCK::RegisterWindow(HudWidgetWindow::GetSingleton());
		FUCK::RegisterWindow(HudWidgetRightAnchor::GetSingleton());
	}
}
