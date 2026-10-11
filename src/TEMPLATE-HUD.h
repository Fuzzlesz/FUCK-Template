#pragma once

// A static image. The bare minimum: load a file, draw it, fixed position
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

	// kHideWithHUD hides it with the game's HUD, during the intro, cutscenes and dialogue
	FUCK::WindowFlags GetFlags() const override
	{
		return FUCK::WindowFlags::kNoDecoration    |
		       FUCK::WindowFlags::kNoBackground    |
		       FUCK::WindowFlags::kAutoResize      |
		       FUCK::WindowFlags::kPassInputToGame |
		       FUCK::WindowFlags::kNoMove          |
		       FUCK::WindowFlags::kRenderDuringTM  |
		       FUCK::WindowFlags::kCloseOnGameMenu |
		       FUCK::WindowFlags::kHideWithHUD     ;
	}

	ImVec2 GetDefaultPos() const override
	{
		return FUCK::Scale(500.0f, 100.0f);
	}

private:
	bool        _isOpen = true;
	FUCK::Image _hudImage;
};

// An editable image. While the FUCK menu is open, FUCK lets the user move, scale and rotate it, and saves all three
class HudWidgetRightAnchor :
	public FUCK::IWindow,
	public REX::Singleton<HudWidgetRightAnchor>
{
public:
	void Initialize();

	const char* Id() const override { return "HUD_Widget_Right"; }
	const char* Title() const override { return "HUD Right Anchored"; }

	void Draw() override;
	bool IsOpen() const override { return _isOpen; }
	void SetOpen(bool a_open) override { _isOpen = a_open; }

	FUCK::WindowFlags GetFlags() const override
	{
		return FUCK::WindowFlags::kNoDecoration    |
		       FUCK::WindowFlags::kNoBackground    |
		       FUCK::WindowFlags::kAutoResize      |
		       FUCK::WindowFlags::kPassInputToGame |
		       FUCK::WindowFlags::kCloseOnGameMenu |
		       FUCK::WindowFlags::kHideWithHUD     |
		       FUCK::WindowFlags::kEditable        |
		       FUCK::WindowFlags::kScalable        |
		       FUCK::WindowFlags::kRotatable       |
		       FUCK::WindowFlags::kAnchorRight     ;
	}

	// With kAnchorRight this is the top-right corner
	ImVec2 GetDefaultPos() const override
	{
		return { FUCK::GetDisplaySize().x - FUCK::Scale(20.0f), FUCK::Scale(100.0f) };
	}

private:
	bool        _isOpen = true;
	FUCK::Image _hudImage;
};
