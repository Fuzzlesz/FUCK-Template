#include "HUD-UI.h"

void HudWidgetWindow::Initialize()
{
	_hudImage = FUCK::Image("Data\\SKSE\\Plugins\\test.png", false);
}

void HudWidgetWindow::Draw()
{
	if (_hudImage.IsLoaded()) {
		ImVec2 imageSize = FUCK::Scale(_hudImage.GetWidth(), _hudImage.GetHeight());
		FUCK::DrawImage(_hudImage.GetID(), imageSize);
	} else {
		FUCK::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Image Missing");
	}
}
