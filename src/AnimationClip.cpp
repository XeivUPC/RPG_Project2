#include "AnimationClip.h"

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

AnimationClip::~AnimationClip()
{
}
