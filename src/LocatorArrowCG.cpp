#include "LocatorArrowCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"

#include "UIImage.h"
#include "Globals.h"

LocatorArrowCG::LocatorArrowCG()
{
	SDL_Texture* compass_overlay = Engine::Instance().m_assetsDB->GetTexture("compass_overlay");
	Vector2Int compassTextureSize = Engine::Instance().m_assetsDB->GetTextureSize(*compass_overlay);
	overlay_image = new UIImage(*compass_overlay, { 640 - compassTextureSize.x - border.x, border.y}, compassTextureSize, {0,0});
	overlay_image->SetLocalScale(1);

	SDL_Texture* compass_arrow = Engine::Instance().m_assetsDB->GetTexture("compass_arrow");
	Vector2Int arrowTextureSize = Engine::Instance().m_assetsDB->GetTextureSize(*compass_arrow);
	arrow_image = new UIImage(*compass_arrow, { 640 - compassTextureSize.x - border.x + 13, 41 + border.y}, arrowTextureSize, { 0,0 });
	arrow_image->SetLocalScale(1);

	///// AddElements

	AddElementToCanvas(overlay_image);
	AddElementToCanvas(arrow_image);
}

void LocatorArrowCG::UpdateCanvas()
{
	UpdateOutsideScreen();

	UICanvas::UpdateCanvas();
}

void LocatorArrowCG::UpdateInsideScreen()
{

}

void LocatorArrowCG::UpdateOutsideScreen()
{
	Vector2 direction = Vector2::Direction(user->GetPosition(), targetLocation);
	direction.Angle();

	printf("Angle: %f\n", RADTODEG*(direction.Angle()));
	
	//arrow_image->SetRotation(direction.Angle());
}

void LocatorArrowCG::SetUser(Entity* _user)
{
	user = _user;
}

Entity* LocatorArrowCG::GetUser() const
{
	return user;
}

void LocatorArrowCG::SetLocation(Vector2 locationTarget)
{
	targetLocation = locationTarget;
}

Vector2 LocatorArrowCG::GetLocation() const
{
	return targetLocation;
}
