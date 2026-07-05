#pragma once

#include "FUCK_API.h"

class HudWidgetWindow :
	public FUCK::IWindow,
	public REX::Singleton<HudWidgetWindow>
{
public:
	void Initialize();

	const char* Id() const override { return "HUD_Widget"; }
	const char* Title() const override { return "HUD Widget"; }

	void Draw() override;
	bool IsOpen() const override { return _isOpen; }
	void SetOpen(bool a_open) override { _isOpen = a_open; }

	FUCK::WindowFlags GetFlags() const override
	{
		return FUCK::WindowFlags::kNoDecoration    |
		       FUCK::WindowFlags::kNoBackground    |
		       FUCK::WindowFlags::kAutoResize      |
		       FUCK::WindowFlags::kPassInputToGame |
		       FUCK::WindowFlags::kNoMove          |
		       FUCK::WindowFlags::kRenderDuringTM  |
		       FUCK::WindowFlags::kCloseOnGameMenu ;
	}

	ImVec2 GetDefaultPos() const override
	{
		return FUCK::Scale(500.0f, 100.0f);
	}

private:
	bool _isOpen      = true;  // Starts true, Host will auto-suspend it during load screen/game menus with flag
	bool _hudMenuOpen = false;

	FUCK::Image             _hudImage;
	FUCK::MenuEventListener _menuListener;
};
