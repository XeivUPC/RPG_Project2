#pragma once
#include <string>
#include <vector>
#include "Sprite.h"
#include "ITransformable.h"
using namespace std;
class AnimationClip
{
public:
	
	AnimationClip(string name, bool _loop, bool _stop, float _speed, vector<Sprite> sprites, Vector2* newPosition, float* newScale);
	AnimationClip();
	~AnimationClip();

	void Start();
	void CleanUp();

	void RenderClip();
	void RenderClip(Vector2 position, float scale);
	void UpdateClip();

	string Name();
	void Play();
	void Stop();
	void Loop(bool state);
	void Speed(float sp);
	void Flip(float flp);

	SDL_Rect& GetAnimationSpace();



public:
	SystemEvent<> onAnimationFinished;
private:
	
	void SetPosition(Vector2* newPosition);
	void SetScale(float* newScale);

private:
	string animationName;

	Vector2* position;
	float* scale;

	vector<Sprite> spriteList;
	SDL_Rect animation_space;

	bool loop = true;
	bool stop = false;
	bool flip = false;
	float speed = 1;

	int currentSprite = 0;
	float time = 0;
};

