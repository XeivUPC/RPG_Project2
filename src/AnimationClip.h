#pragma once
#include <string>
#include <vector>
#include "Sprite.h"
#include "ITransformable.h"
using namespace std;
class AnimationClip
{
private:
	string animationName;
	Vector2* position;
	vector<Sprite> spriteList;
	SDL_Rect animation_space;
	bool loop = true;
	bool stop = false;
	float speed = 1;
	float* scale;

	int currentSprite = 0;
	float time = 0;
	Vector2 GetPosition();
	float GetScale();

public:
	void RenderClip();
	AnimationClip(string name, bool _loop, bool _stop, float _speed, vector<Sprite> sprites, Vector2* newPosition, float* newScale);
	~AnimationClip();
	string Name();
	void Play();
	void Stop();
	void Loop(bool state);
	void Speed(float sp);

	SDL_Rect& GetAnimationSpace();

	void Start();
	void CleanUp();

	bool UpdateClip();
};

