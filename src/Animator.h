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
	Animator();
	AnimationClip* operator()();
	void Animate(string animation);
	~Animator();
};

