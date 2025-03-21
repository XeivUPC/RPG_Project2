#pragma once
#include "Animator.h"
#include "AnimationClip.h"
#include <vector>
using namespace std;
class Animator
{
	vector<AnimationClip> AnimationList;
	AnimationClip* currentAnimation;

public:
	Animator(vector<AnimationClip> Animations, int current);
	Animator();
	~Animator();
	void AddAnimationClip(AnimationClip clip);
	AnimationClip* clip();
	void Animate(const string& animation, bool keepTime= false);
	void CleanUp();
};

