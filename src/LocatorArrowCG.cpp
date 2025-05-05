#include "LocatorArrowCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"

#include "UIImage.h"

LocatorArrowCG::LocatorArrowCG()
{
	SDL_Texture* compass_overlay = Engine::Instance().m_assetsDB->GetTexture("compass_overlay");

	overlay_image = new UIImage({ 152,0 }, { 488,360 }, { 0,0 }, true, { 0,0,488,360 }, { 0,0,0,199 });
	overlay_image->SetLocalScale(1);

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*compass_overlay);
	UIImage* bgButtons_image = new UIImage(*compass_overlay, { 0,0 }, textureSize, { 0,0 });
	overlay_image->SetLocalScale(1);
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
