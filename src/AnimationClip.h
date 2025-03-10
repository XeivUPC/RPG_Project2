#pragma once
#include <string>
#include <vector>
#include "Sprite.h"
using namespace std;
class AnimationClip
{
	string animationName;
	vector<Sprite> spriteList;
	bool visible;
	bool loop;
	bool stop;
	float speed;
public:
	AnimationClip();
	void Play();
	void Stop();
	void Visible(bool state);
	void Loop(bool state);
	void Speed(float sp);
	~AnimationClip();
};

