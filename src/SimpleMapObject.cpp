#include "SimpleMapObject.h"
#include "Pooling.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"
#include "TextureAtlas.h"
#include "ModuleRender.h"
#include "ModuleUpdater.h"
#include "DrawingTools.h"

SimpleMapObject::SimpleMapObject()
{
}

SimpleMapObject::~SimpleMapObject()
{
}

void SimpleMapObject::SetData(string _atlasId, string _textureId, Vector2 _position, float _scale)
{
	SetPosition(move(_position));
	SetScale(move(_scale));

	TextureAtlas* atlas = Engine::Instance().m_assetsDB->GetAtlas(_atlasId);
	texture = atlas->texture;
	rect = atlas->sprites[_textureId].rect;
	renderLayer = 3;

	textureId = _textureId;

	anchor = { 0.5f,1 };
}

void SimpleMapObject::AddCollision(Vector2 position, Vector2 size)
{
	body = Engine::Instance().m_physics->factory().CreateBox(position, size.x, size.y);
	body->SetType(PhysBody::BodyType::Static);
}

bool SimpleMapObject::Update()
{
	return true;
}

void SimpleMapObject::Render()
{
	Engine::Instance().m_render->SetCameraMode(true);

	SDL_Rect positionRect = { (int)(position.x - rect.w * anchor.x),(int)(position.y - rect.h * anchor.y), rect.w,rect.h };
	if (Engine::Instance().m_render->IsRectCameraVisible(positionRect))
		Engine::Instance().m_render->painter().RenderTexture(*texture, position, &rect, { scale, scale }, 0, anchor);

	Engine::Instance().m_render->SetCameraMode(false);
}

bool SimpleMapObject::CleanUp()
{
	Pooling::Instance().ReturnObject(this);
	return true;
}

void SimpleMapObject::InitPoolObject()
{
	Engine::Instance().m_render->AddToRenderQueue(*this, *this);
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_updater->AddToUpdateGroup(*this, "Entity");
}

void SimpleMapObject::ResetPoolObject()
{
	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_render->RemoveFomRenderQueue(*this);
}
