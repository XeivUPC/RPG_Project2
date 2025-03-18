#include "Animator.h"

Animator::Animator(vector<AnimationClip*> Animations, AnimationClip* current)
{
	
}
AnimationClip* Animator::operator()()
{
	return currentAnimation;
}