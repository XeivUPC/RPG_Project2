#include "Animator.h"

Animator::Animator(vector<AnimationClip> Animations, int current):
AnimationList(Animations)
{
	currentAnimation = &AnimationList[current];
	currentAnimation->Start();
}
Animator::~Animator()
{}
AnimationClip* Animator::clip()
{
	return currentAnimation;
}

void Animator::Animate(string animation)
{
	for (size_t i = 0; i < AnimationList.size(); i++)
	{
		if (AnimationList[i].Name() == animation)
		{
			currentAnimation = &AnimationList[i];
			return;
		}
	}
	currentAnimation->Start();
}

void Animator::CleanUp()
{
	for (size_t i = 0; i < AnimationList.size(); i++)
	{
		AnimationList[i].CleanUp();
	}
	AnimationList.clear();
}
