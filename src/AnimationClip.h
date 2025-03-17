#pragma once
#include <string>
#include <vector>
#include "Sprite.h"
#include "IUpdateable.h"
#include "IRendereable.h"
using namespace std;
class AnimationClip : public IUpdateable, public IRendereable
{
	string animationName;
	vector<Sprite> spriteList;
	bool visible = false;
	bool loop = true;
	bool stop = false;
	float speed = 1;

	int currentSprite = 0;
	float time = 0;
public:
	AnimationClip(string name, bool _visible, bool _loop, bool _stop, float _speed, vector<Sprite> sprites);
	~AnimationClip();
	void Play();
	void Stop();
	void Visible(bool state);
	void Loop(bool state);
	void Speed(float sp);

	void Start();
	void CleanUp();

	bool Update() override;

	// Heredado vía IRendereable
	void Render() override;
};

