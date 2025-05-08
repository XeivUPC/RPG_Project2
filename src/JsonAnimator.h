#pragma once
#include "Animator.h"
#include <string>

using namespace std;

class JsonAnimator : public Animator
{

public:
	JsonAnimator();
	JsonAnimator(string jsonPath);
	JsonAnimator(vector<AnimationClip> Animations, int current);
	~JsonAnimator();

	/// It will reset everything
	void AddJsonAnimationClip(string jsonPath);

public:
private:
private:
};
