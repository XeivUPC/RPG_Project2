#include "AlertDisplayerCG.h"
#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleTime.h"

#include "UIImage.h"
#include "UITextBox.h"

AlertDisplayerCG::AlertDisplayerCG(float _hideTime, SDL_Texture* texture, Vector2Int _position, Vector2Int _size, Vector2 _pivot, float _scale)
{
	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");

	alert = new UIImage(*texture, _position, _size, _pivot, false, { 0,0,0,0 }, {0,0,0,100});
	text = new UITextBox("", *textFont, 16, { 184,132,78,255 }, _size/2, _size, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	
	hideTime = _hideTime;

	text->SetParent(alert);

	AddElementToCanvas(alert);

	CloseAlert();
}

void AlertDisplayerCG::UpdateCanvas()
{
	if (alert->localVisible) {
		timer.Step(ModuleTime::deltaTime);
		if (timer.ReadSec() > hideTime)
		{
			timer.Start();
			CloseAlert();
		}
	}

	UICanvas::UpdateCanvas();
}

void AlertDisplayerCG::SetAlertData(string alertInfo)
{
	text->SetText(alertInfo);
}

void AlertDisplayerCG::SetAlertData(float _hideTime)
{
	hideTime = _hideTime;
}

void AlertDisplayerCG::SetAlertData(float _hideTime, string alertInfo)
{
	SetAlertData(_hideTime);
	SetAlertData(alertInfo);
	
}

void AlertDisplayerCG::OpenAlert()
{
	timer.Start();
	alert->localVisible = true;
	onAlertOpened.Trigger();
}

void AlertDisplayerCG::CloseAlert()
{
	timer.Start();
	alert->localVisible = false;
	onAlertClosed.Trigger();
}
