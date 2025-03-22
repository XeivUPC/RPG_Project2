#pragma once
#include "Animator.h"
#include "AnimationClip.h"
#include <vector>
using namespace std;
class Animator
{

public:
	Animator(vector<AnimationClip> Animations, int current);
	Animator();
	~Animator();
	void AddAnimationClip(AnimationClip clip);
	AnimationClip* clip();
	void Animate(const string& animation, bool keepTime= false);
	void CleanUp();

public:
	SystemEvent<string> onAnimationChanged;

private:

private:
	vector<AnimationClip> AnimationList;
	AnimationClip* currentAnimation;

};

