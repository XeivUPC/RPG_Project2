#pragma once
#include <string>
#include <vector>
#include "Sprite.h"
#include "IUpdateable.h"
#include "IRendereable.h"
#include "ITransformable.h"
using namespace std;
class AnimationClip : public IUpdateable, public IRendereable
{
	string animationName;
	Vector2& position;
	vector<Sprite> spriteList;
	bool visible = false;
	bool loop = true;
	bool stop = false;
	float speed = 1;
	float& scale;

	int currentSprite = 0;
	float time = 0;
	Vector2 GetPosition();
	float GetScale();

	// Heredado vía IRendereable
	void Render() override;
public:
	AnimationClip(string name, bool _visible, bool _loop, bool _stop, float _speed, vector<Sprite> sprites, Vector2& newPosition, float& newScale);
	~AnimationClip();
	string Name();
	void Play();
	void Stop();
	void Visible(bool state);
	void Loop(bool state);
	void Speed(float sp);

	void Start();
	void CleanUp();

	bool Update() override;
};

