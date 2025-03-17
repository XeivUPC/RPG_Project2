#include "AnimationClip.h"
#include "ModuleTime.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleUpdater.h"
#include "DrawingTools.h"

AnimationClip::AnimationClip(string name, bool _visible, bool _loop, bool _stop, float _speed, vector<Sprite> sprites) :
	animationName(name),
	visible(_visible),
	loop(_loop),
	stop(_stop),
	speed(_speed),
	spriteList(sprites)
{}

void AnimationClip::Play()
{
	stop = false;
}

void AnimationClip::Stop()
{
	stop = true;
}

void AnimationClip::Visible(bool state)
{
	visible = state;
}

void AnimationClip::Loop(bool state)
{
	loop = state;
}

void AnimationClip::Speed(float sp)
{
	speed = sp;
}

void AnimationClip::Start()
{
	Engine::Instance().m_render->AddToRenderQueue(*this);
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	currentSprite = 0;
	time = 0;
}

void AnimationClip::CleanUp()
{
	Engine::Instance().m_render->RemoveFomRenderQueue(*this);
	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
}


bool AnimationClip::Update()
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

void AnimationClip::Render()
{
	//Engine::Instance().m_render->painter().RenderTexture();
}

AnimationClip::~AnimationClip()
{
}
