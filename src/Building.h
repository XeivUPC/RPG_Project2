#pragma once
#include "Entity.h"
#include "IPooleable.h"


class Building : public Entity, public IPooleable {
public:
	Building();
	~Building();
	void SetData(string _atlasId, string _textureId, Vector2 _position, float _scale);

	bool Update() override;
	void Render() override;

	bool CleanUp() override;

	// Inherited via IPooleable
	void InitPoolObject() override;
	// Inherited via IPooleable
	void ResetPoolObject() override;

public:

private:

private:
	string textureId = "";
	SDL_Texture* texture = nullptr;
	SDL_Rect rect = {0,0,0,0};


};
