#include "LocatorArrowCG.h"

LocatorArrowCG::LocatorArrowCG()
{
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

	

	Vector2 line;
	line.Direction(user->GetPosition(), targetLocation);
	

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
