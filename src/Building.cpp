#include "Building.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "TextureAtlas.h"
#include "ModuleRender.h"
#include "DrawingTools.h"

Building::Building(string _atlasId, string _textureId, Vector2 _position, float _scale)
{
	SetPosition(move(_position));
	SetScale(move(_scale));

	TextureAtlas* atlas = Engine::Instance().m_assetsDB->GetAtlas(_atlasId);
	texture = atlas->texture;
	rect = atlas->sprites[_textureId].rect;

	Engine::Instance().m_render->AddToRenderQueue(*this);
	renderLayer = 3;

	anchor = { 0.5f,1 };
}

Building::~Building()
{

}

bool Building::Update()
{
	return true;
}

void Building::Render()
{
	Engine::Instance().m_render->painter().RenderTexture(*texture, position, &rect, { scale, scale }, 0, anchor);
}

bool Building::CleanUp()
{
	Engine::Instance().m_render->RemoveFomRenderQueue(*this);
	return true;
}
