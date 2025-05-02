#include "Animator.h"

Animator::Animator()
{
}

Animator::Animator(vector<AnimationClip> Animations, int current):
AnimationList(Animations)
{
	currentAnimationIndex = current;
	clip()->Start();
}
Animator::~Animator()
{}
void Animator::AddAnimationClip(AnimationClip clip)
{
	AnimationList.emplace_back(clip);
	if (currentAnimationIndex == -1)
		currentAnimationIndex = AnimationList.size()-1;
}
AnimationClip* Animator::clip()
{
	return &AnimationList[currentAnimationIndex];
}

void Animator::Animate(const string& animation, bool keepTime)
{
	AnimationClip* currentAnimation = clip();

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

vector<AnimationClip>& Animator::GetAnimationClips()
{
	return AnimationList;
}
