#include "Animator.h"

Animator::Animator(vector<AnimationClip*> Animations, int current):
AnimationList(Animations)
{
	currentAnimation = AnimationList[current];
}
Animator::~Animator()
{}
AnimationClip* Animator::operator()()
{
	return currentAnimation;
}

void Animator::Animate(string animation)
{
	for (size_t i = 0; i < AnimationList.size(); i++)
	{
		if (AnimationList[i]->Name() == animation)
		{
			currentAnimation = AnimationList[i];
			return;
		}
	}
}

void Animator::CleanUp()
{
	AnimationList.clear();
}
