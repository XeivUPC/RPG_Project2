#include "Animator.h"
AnimationClip* Animator::operator()()
{
	return currentAnimation;
}