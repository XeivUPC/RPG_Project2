#include "AnimationClip.h"
#include "ModuleTime.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleUpdater.h"
#include "DrawingTools.h"

AnimationClip::AnimationClip(string name, bool _loop, bool _stop, float _speed, vector<Sprite> sprites, Vector2* newPosition, float* newScale) :
	animationName(name),
	loop(_loop),
	stop(_stop),
	speed(_speed),
	spriteList(sprites),
	position(newPosition),
	scale(newScale)
{}

void AnimationClip::Play()
{
	stop = false;
}

void AnimationClip::Stop()
{
	stop = true;
}

void AnimationClip::Loop(bool state)
{
	loop = state;
}

void AnimationClip::Speed(float sp)
{
	speed = sp;
}

SDL_Rect& AnimationClip::GetAnimationSpace()
{
	Sprite& sprite = spriteList[currentSprite];
	animation_space = sprite.Section();
	animation_space.x = GetPosition().x - animation_space.w * sprite.Pivot().x + sprite.Offset().x;
	animation_space.y = GetPosition().y - animation_space.h * sprite.Pivot().y + sprite.Offset().x;
	return animation_space;
}

void AnimationClip::Start()
{	
	currentSprite = 0;
	time = 0;
}

void AnimationClip::CleanUp()
{
	
	for (size_t i = 0; i < spriteList.size(); i++)
	{
		spriteList[i].CleanUp();
	}
	spriteList.clear();
}


bool AnimationClip::UpdateClip()
{
	if(!stop || (time >= speed && currentSprite == spriteList.size() - 1 && !loop))
		time += ModuleTime::deltaTime;
	if (time >= speed)
	{
		time = 0;
		if (currentSprite == spriteList.size() - 1 && loop)
			currentSprite = 0;
		else if (currentSprite <= spriteList.size() - 1)
			currentSprite++;
	}
	return true;
}

void AnimationClip::RenderClip()
{
	Engine::Instance().m_render->painter().RenderTexture(*spriteList[currentSprite].Texture(), GetPosition() + spriteList[currentSprite].Offset(), &spriteList[currentSprite].Section(), {GetScale(), GetScale()}, 0, spriteList[currentSprite].Pivot());
}

Vector2 AnimationClip::GetPosition()
{
	return *position;
}

float AnimationClip::GetScale()
{
	return *scale;
}

AnimationClip::~AnimationClip()
{
}

string AnimationClip::Name()
{
	return animationName;
}
