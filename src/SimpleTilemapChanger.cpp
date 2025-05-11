#include "SimpleTilemapChanger.h"
#include "Pooling.h"
#include "Tilemap.h"

#include "Engine.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"
#include "ModuleAssetDatabase.h"
#include "TextureAtlas.h"
#include "ModuleRender.h"
#include "ModuleUpdater.h"
#include "DrawingTools.h"
#include "GameScene.h"

SimpleTilemapChanger::SimpleTilemapChanger()
{
	priority_updating = -1;
}

SimpleTilemapChanger::~SimpleTilemapChanger()
{
}

bool SimpleTilemapChanger::PostUpdate()
{
	if (entrySensor.OnTriggerEnter()) {
		if (targetPath != "") {
			if (entryPoint!=-1) {
				Engine::Instance().s_game->ChangeTilemap(targetPath, entryPoint);
			}
			else
			{
				Engine::Instance().s_game->GetLastTilemap()->SetSpawnPoint(exitPosition);
				Engine::Instance().s_game->AddTilemap(targetPath);
			}	
		}
		else {
			Engine::Instance().s_game->RemoveLastTilemap();
		}
	}

	return true;
}

void SimpleTilemapChanger::SetExitPosition(Vector2 _exitPosition)
{
	exitPosition = _exitPosition;
}

void SimpleTilemapChanger::SetTargetTilemapPath(string _targetPath)
{
	targetPath = _targetPath;
}

void SimpleTilemapChanger::SetEntryPoint(int _entryPoint)
{
	entryPoint = _entryPoint;
}

void SimpleTilemapChanger::SetEntryTrigger(Vector2 _position, Vector2 size)
{
	b2FixtureUserData sensorData;
	sensorData.pointer = (uintptr_t)(&entrySensor);
	/// set Sensor;
	entryTrigger = Engine::Instance().m_physics->factory().CreateBox(_position, size.x, size.y, sensorData);
	entryTrigger->SetType(PhysBody::BodyType::Static);
	entryTrigger->SetSensor(0, true);
	entrySensor.SetFixtureToTrack(entryTrigger, 0);

	ModulePhysics::Layer category, mask;
	category.flags.trigger_layer = 1;
	mask.flags.player_layer = 1;
	entryTrigger->SetFilter(0, category.rawValue, mask.rawValue, 0);
}

void SimpleTilemapChanger::InitPoolObject()
{
	SimpleMapObject::InitPoolObject();
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);
}

void SimpleTilemapChanger::ResetPoolObject()
{
	SimpleMapObject::ResetPoolObject();
	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);
	targetPath = "";
	exitPosition = { 0,0 };
	entryPoint = -1;
	entrySensor.Reset();

	if (entryTrigger != nullptr) {
		delete entryTrigger;
		entryTrigger = nullptr;
	}
}
