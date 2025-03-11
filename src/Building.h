#pragma once
#include "Entity.h"


class Building : public Entity {
public:
	Building(string _atlasId, string _textureId, Vector2 _position, float _scale);
	~Building();

	bool Update() override;
	void Render() override;

	bool CleanUp() override;
public:

private:

private:
	string textureId = "";
	SDL_Texture* texture = nullptr;
	SDL_Rect rect;
};
