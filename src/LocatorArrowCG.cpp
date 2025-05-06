#include "LocatorArrowCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"

#include "UIImage.h"
#include "Globals.h"

LocatorArrowCG::LocatorArrowCG()
{
	SDL_Texture* compass_overlay = Engine::Instance().m_assetsDB->GetTexture("compass_overlay");
	Vector2Int compassTextureSize = Engine::Instance().m_assetsDB->GetTextureSize(*compass_overlay);
	overlay_image = new UIImage(*compass_overlay, { 640 - border.x - compassTextureSize.x/2 , border.y + compassTextureSize.y/2}, compassTextureSize, { 0.5f,0.5f });
	overlay_image->SetLocalScale(1);

	SDL_Texture* compass_arrow = Engine::Instance().m_assetsDB->GetTexture("compass_arrow");
	Vector2Int arrowTextureSize = Engine::Instance().m_assetsDB->GetTextureSize(*compass_arrow);
	arrow_image = new UIImage(*compass_arrow, { 0,5}, arrowTextureSize, { 0.5f,0.5f });
	arrow_image->SetLocalScale(1);
	arrow_image->SetParent(overlay_image);

	///// AddElements

	AddElementToCanvas(overlay_image);
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
	
	arrow_image->SetLocalAngle(RADTODEG*direction.Angle() + 180);
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
