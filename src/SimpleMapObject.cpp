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

void SimpleMapObject::SetData(string _atlasId, string _textureId, Vector2Int _position, float _scale)
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

void SimpleMapObject::SetData(Vector2Int _position, float _scale)
{
	SetPosition(move(_position));
	SetScale(move(_scale));
}

void SimpleMapObject::AddCollision(Vector2 _position, Vector2 size)
{
	PhysBody* body = Engine::Instance().m_physics->factory().CreateBevelBox(_position, size.x, size.y,0.1f);
	bodies.emplace_back(body);
	body->SetType(PhysBody::BodyType::Static);

	ModulePhysics::Layer category, mask;
	category.flags.ground_layer = 1;
	mask.flags.player_layer = 1;
	body->SetFilter(0, category.rawValue, mask.rawValue, 0);
	
}

bool SimpleMapObject::Update()
{
	return true;
}

void SimpleMapObject::Render()
{
	if (texture == nullptr)
		return;
	SDL_Rect positionRect = { (int)(position.x - rect.w * anchor.x),(int)(position.y - rect.h * anchor.y), rect.w,rect.h };
	if (Engine::Instance().m_render->IsRectCameraVisible(positionRect))
		Engine::Instance().m_render->painter().RenderTexture(*texture, position, &rect, { scale, scale }, 0, anchor);
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
	texture = nullptr;
	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_render->RemoveFomRenderQueue(*this);
	for (size_t i = 0; i < bodies.size(); i++)
	{
		delete bodies[i];
	}bodies.clear();
}
