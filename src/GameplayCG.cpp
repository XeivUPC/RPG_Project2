#include "GameplayCG.h"

#include "Engine.h"
#include "ModuleTime.h"
#include "ModuleAssetDatabase.h"
#include "GameScene.h"

#include "UIImage.h"
#include "UITextBox.h"
#include "Globals.h"

#include "MissionsCG.h"

#include <iomanip>
#include <sstream>

GameplayCG::GameplayCG(int _renderLayer)
{

	renderLayer = _renderLayer;

	missionCanvas = new MissionsCG(renderLayer);

	SDL_Texture* compass_overlay = Engine::Instance().m_assetsDB->GetTexture("compass_overlay");
	Vector2Int compassTextureSize = Engine::Instance().m_assetsDB->GetTextureSize(*compass_overlay);
	compassTextureSize.x /= 2;
	overlay_image = new UIImage(*compass_overlay, { 640 - border.x - compassTextureSize.x/2 , border.y + compassTextureSize.y/2}, compassTextureSize, { 0.5f,0.5f }, true, overlay_rect1);
	overlay_image->SetLocalScale(1);

	SDL_Texture* compass_arrow = Engine::Instance().m_assetsDB->GetTexture("compass_arrow");
	Vector2Int arrowTextureSize = Engine::Instance().m_assetsDB->GetTextureSize(*compass_arrow);
	arrow_image = new UIImage(*compass_arrow, { 0,-16}, arrowTextureSize, { 0.5f,0.5f });
	arrow_image->SetLocalScale(1);
	arrow_image->SetParent(overlay_image);


	_TTF_Font* font = Engine::Instance().m_assetsDB->GetFont("alagard");
	clock_text = new UITextBox("", *font, 16, { 184,132,78,255 }, clock_text_pos1, {41*2,13*2}, {0.5f,0.5f},UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	clock_text->SetLocalScale(0.5f);	
	clock_text->SetParent(overlay_image);
	///// AddElements

	AddElementToCanvas(overlay_image);

	RemoveLocation();
}

GameplayCG::~GameplayCG()
{
	delete missionCanvas;
}

void GameplayCG::UpdateCanvas()
{
	missionCanvas->isVisible = isVisible;
	missionCanvas->UpdateCanvas();

	if (compassHasLocation) {
		UpdateCompass();
		UpdateClock();
	}
	else {
		UpdateClock();
	}

	UICanvas::UpdateCanvas();
}

void GameplayCG::UpdateCompass()
{
	if (user->GetPosition() == targetLocation)
		return;
	Vector2 direction = Vector2::Direction(user->GetPosition(), targetLocation);
	direction.Angle();
	arrow_image->SetLocalAngle(RADTODEG * direction.Angle() + 180);
}

void GameplayCG::UpdateClock()
{
	
	int timePassed = Engine::Instance().s_game->GetTime();

	int hours = (timePassed / 3600) % 24;
	int minutes = (timePassed / 60) % 60;

	std::ostringstream oss;

	oss << setw(2) << setfill('0') << hours
		<< ":" << setw(2) << setfill('0') << minutes;
	clock_text->SetText(oss.str());

}

void GameplayCG::SetUser(Entity* _user)
{
	user = _user;
}

Entity* GameplayCG::GetUser() const
{
	return user;
}

void GameplayCG::SetLocation(Vector2 locationTarget)
{
	targetLocation = locationTarget;
	compassHasLocation = true;
	clock_text->SetLocalPosition(clock_text_pos1);
	arrow_image->localVisible = true;
	overlay_image->SetRect(overlay_rect1);
}

void GameplayCG::RemoveLocation()
{
	compassHasLocation = false;
	clock_text->SetLocalPosition(clock_text_pos2);
	arrow_image->localVisible = false;
	overlay_image->SetRect(overlay_rect2);
}

Vector2 GameplayCG::GetLocation() const
{
	return targetLocation;
}