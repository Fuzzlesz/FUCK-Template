#include "HUD-UI.h"

// ==================================================
// Static Image
// ==================================================

void HudWidgetWindow::Initialize()
{
	std::string path = FUCK::GetConfigDirectory() + "test.png";
	_hudImage        = FUCK::Image(path.c_str(), false);
}

void HudWidgetWindow::Draw()
{
	if (_hudImage.IsLoaded()) {
		ImVec2 imageSize = FUCK::Scale(_hudImage.GetWidth(), _hudImage.GetHeight());
		FUCK::DrawImage(_hudImage.GetID(), imageSize);
	} else {
		FUCK::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "test.png Missing");
	}
}

// ==================================================
// Editable Image
// ==================================================

void HudWidgetRightAnchor::Initialize()
{
	std::string path = FUCK::GetConfigDirectory() + "test2.png";
	_hudImage        = FUCK::Image(path.c_str(), false);
}

void HudWidgetRightAnchor::Draw()
{
	if (!_hudImage.IsLoaded()) {
		FUCK::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "test2.png Missing");
		return;
	}

	// FUCK sets scale and rotation from its handles, applying them is up to the widget
	float  scale = FUCK::GetWindowScale();
	ImVec2 size  = FUCK::Scale(_hudImage.GetWidth() * scale, _hudImage.GetHeight() * scale);

	FUCK::DrawImageRotated(_hudImage.GetID(), FUCK::GetCursorScreenPos(), size, FUCK::GetWindowRotation());

	// DrawImageRotated doesn't take up layout space, so the auto-sized window needs this to fit the image
	FUCK::Dummy(size);
}
