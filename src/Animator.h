#pragma once
#include "Animator.h"
#include "AnimationClip.h"
#include <vector>
using namespace std;
class Animator
{

public:
	Animator(vector<AnimationClip> Animations, int current);
	~Animator();
	void AddAnimationClip(AnimationClip clip);
	AnimationClip* clip();
	void Animate(const string& animation, bool keepTime= false);
	void CleanUp();

	AnimationClip* GetAnimationClip(const string& animationName);

public:
	SystemEvent<string> onAnimationChanged;

private:

private:
	vector<AnimationClip> AnimationList;
	AnimationClip* currentAnimation;

};

