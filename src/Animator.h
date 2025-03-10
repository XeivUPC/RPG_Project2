#pragma once
#include "Animator.h"
#include "AnimationClip.h"
#include <vector>
using namespace std;
class Animator
{
	vector<AnimationClip*> AnimationList;
	AnimationClip* currentAnimation;

public:
	AnimationClip* operator()();
};

