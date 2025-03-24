#include "Animator.h"

Animator::Animator(vector<AnimationClip> Animations, int current):
AnimationList(Animations)
{
	currentAnimation = &AnimationList[current];
	currentAnimation->Start();
}
Animator::~Animator()
{}
void Animator::AddAnimationClip(AnimationClip clip)
{
	AnimationList.emplace_back(clip);
	if (currentAnimation == nullptr)
		currentAnimation = &AnimationList[0];
}
AnimationClip* Animator::clip()
{
	return currentAnimation;
}

void Animator::Animate(const string& animation, bool keepTime)
{
	if (currentAnimation && currentAnimation->Name() == animation)
		return;
	for (size_t i = 0; i < AnimationList.size(); i++)
	{
		if (AnimationList[i].Name() == animation)
		{
			currentAnimation = &AnimationList[i];
			onAnimationChanged.Trigger(currentAnimation->Name());
			return;
		}
	}
	if(!keepTime)
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

AnimationClip* Animator::GetAnimationClip(const string& animationName)
{
	for (size_t i = 0; i < AnimationList.size(); i++)
	{
		if (AnimationList[i].Name() == animationName)
		{
			return &AnimationList[i];
		}
	}
	return nullptr;
}
