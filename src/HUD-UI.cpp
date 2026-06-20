#include "HUD-UI.h"

void HudWidgetWindow::Initialize()
{
	// Load the image once during initialisation
	_hudImage = FUCK::Image("Data\\SKSE\\Plugins\\test.png", false);

	// Use the RAII wrapper
	_menuListener = FUCK::MenuEventListener([this](const char* menuName, bool opening) {
		if (menuName == RE::HUDMenu::MENU_NAME) {
			_hudMenuOpen = opening;
		}
	});
}

void HudWidgetWindow::Draw()
{
	if (!_hudMenuOpen) {
		return;
	}

	if (_hudImage.IsLoaded()) {
		ImVec2 imageSize = FUCK::Scale(_hudImage.GetWidth(), _hudImage.GetHeight());
		FUCK::DrawImage(_hudImage.GetID(), imageSize);
	} else {
		FUCK::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Image Missing");
	}
}
