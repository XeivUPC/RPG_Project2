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
	spriteList(move(sprites)),
	position(newPosition),
	scale(newScale)
{}

AnimationClip::AnimationClip()
{
}

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

void AnimationClip::Flip(float flp)
{
	flip = flp;
}

SDL_Rect& AnimationClip::GetAnimationSpace()
{
	Sprite& sprite = spriteList[currentSprite];
	animation_space = sprite.Section();
	animation_space.x = (int)(position->x - animation_space.w * sprite.Pivot().x + sprite.Offset().x);
	animation_space.y = (int)(position->y - animation_space.h * sprite.Pivot().y + sprite.Offset().x);
	return animation_space;
}

Sprite& AnimationClip::GetSprite(int index)
{
	return spriteList[index];
}

vector<Sprite>& AnimationClip::GetSprites()
{
	return spriteList;
}

void AnimationClip::SetPosition(Vector2* newPosition)
{
	position = newPosition;
}

void AnimationClip::SetScale(float* newScale)
{
	scale = newScale;
}

void AnimationClip::Start()
{	
	currentSprite = 0;
	spriteList[currentSprite].onSpriteSelected.Trigger();
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


void AnimationClip::UpdateClip()
{
	if(!stop || (time >= speed && currentSprite == spriteList.size() - 1 && !loop))
		time += (float)ModuleTime::deltaTime;
	if (time >= speed)
	{
		time = 0;
		if (currentSprite == spriteList.size() - 1 && loop) {
			currentSprite = 0;
			onAnimationFinished.Trigger();
			spriteList[currentSprite].onSpriteSelected.Trigger();
			

		}
		else if (currentSprite < spriteList.size() - 1) {
			currentSprite++;
			spriteList[currentSprite].onSpriteSelected.Trigger();
			if (currentSprite == spriteList.size() - 1)
			{
				onAnimationFinished.Trigger();
			}
		}
	}
}

void AnimationClip::RenderClip(SDL_Color color)
{
	RenderClip(*position, *scale, color);
}

void AnimationClip::RenderClip(Vector2 _position, float _scale, SDL_Color color)
{
	Engine::Instance().m_render->painter().RenderTexture(*spriteList[currentSprite].Texture(), _position + spriteList[currentSprite].Offset(), &spriteList[currentSprite].Section(), { _scale, _scale }, 0, spriteList[currentSprite].Pivot(), flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE, color);
}

AnimationClip::~AnimationClip()
{
}

string AnimationClip::Name()
{
	return animationName;
}
