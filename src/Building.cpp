#include "Building.h"
#include "Pooling.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "TextureAtlas.h"
#include "ModuleRender.h"
#include "ModuleUpdater.h"
#include "DrawingTools.h"

Building::Building()
{

}

Building::~Building()
{

}

void Building::SetData(string _atlasId, string _textureId, Vector2 _position, float _scale)
{
	SetPosition(move(_position));
	SetScale(move(_scale));

	TextureAtlas* atlas = Engine::Instance().m_assetsDB->GetAtlas(_atlasId);
	texture = atlas->texture;
	rect = atlas->sprites[_textureId].rect;
	renderLayer = 3;

	anchor = { 0.5f,1 };
}

bool Building::Update()
{
	return true;
}

void Building::Render()
{
	Engine::Instance().m_render->SetCameraMode(true);

	SDL_Rect positionRect = { (int)(position.x- rect.w*anchor.x),(int)(position.y- rect.h * anchor.y), rect.w,rect.h };
	if (Engine::Instance().m_render->IsRectCameraVisible(positionRect))
		Engine::Instance().m_render->painter().RenderTexture(*texture, position, &rect, { scale, scale }, 0, anchor);

	Engine::Instance().m_render->SetCameraMode(false);
}

bool Building::CleanUp()
{
	Pooling::Instance().ReturnObject(this);
	return true;
}

void Building::InitPoolObject()
{
	Engine::Instance().m_render->AddToRenderQueue(*this);
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
}

void Building::ResetPoolObject()
{
	Engine::Instance().m_updater->RemoveFomUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_render->RemoveFomRenderQueue(*this);
}


