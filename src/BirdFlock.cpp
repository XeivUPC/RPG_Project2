#include "BirdFlock.h"
#include "Bird.h"
#include "Pooling.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleUpdater.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"
#include <random>

BirdFlock::BirdFlock()
{
	renderLayer = 4;
}

BirdFlock::~BirdFlock()
{
}

bool BirdFlock::Update()
{
	if (hasBirds && interactionSensor.OnTriggerEnter()) {
		PhysBody* body = interactionSensor.GetClosestBodyColliding();
		if (body == nullptr) {
			hasBirds = true;	
		}
		else {
			for (auto bird : birds) {
				bird->Scare(body->GetPhysicPosition());
			}
			hasBirds = false;
		}
	}

	for (auto bird : birds) {
		bird->UpdateBird();
	}
    return true;
}

void BirdFlock::Render()
{
	for (auto bird : birds) {
		bird->RenderBird();
	}
}

void BirdFlock::SpawnBirds()
{
	if (hasBirds) return;
	int amount = rand() % (maxAmount - minAmount + 1) + minAmount;
	int spawnChance = rand() % 100;

	if (spawnChance < spawnPercentage) {
		for (int i = 0; i < amount; i++) {
			auto bird = Pooling::Instance().AcquireObject<Bird>();
			bird->Initialize(this, GetRandomPointInside());
			birds.push_back(bird.get());
		}
		if(amount>0)
			hasBirds = true;
	}
}

void BirdFlock::RemoveBird(Bird* birdToRemove)
{
	auto it = std::find(birds.begin(), birds.end(), birdToRemove);
	if (it != birds.end()) {
		Pooling::Instance().ReturnObject(*it);
		birds.erase(it);
	}
}

void BirdFlock::Initialize(Vector2 _position, float _radius)
{
	radius = _radius;
	position = _position;


	/// Collider
	b2FixtureUserData sensorData;
	sensorData.pointer = (uintptr_t)(&interactionSensor);

	ModulePhysics::Layer category, mask;

	body = Engine::Instance().m_physics->factory().CreateCircle({ 0,0 }, PIXEL_TO_METERS(_radius), sensorData);
	body->SetSensor(0, true);
	category.flags.trigger_layer = 1;
	mask.flags.player_layer = 1;
	body->SetFilter(0, category.rawValue, mask.rawValue, 0);
	interactionSensor.SetFixtureToTrack(body, 0);

	body->SetPhysicPosition(position.x, position.y);

}

void BirdFlock::InitPoolObject()
{
	Engine::Instance().m_render->AddToRenderQueue(*this, *this);
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_updater->AddToUpdateGroup(*this, "Entity");
}

void BirdFlock::ResetPoolObject()
{
	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_render->RemoveFromRenderQueue(*this);

	
	birds.clear();
	hasBirds = false;

	delete body;
}

Vector2 BirdFlock::GetRandomPointInside()
{
	std::mt19937 gen(std::random_device{}());
	std::uniform_real_distribution<> dis_r01(0.0, 1.0);
	std::uniform_real_distribution<> dis_theta(0.0, 2 * M_PI);

	double r = radius * sqrt(dis_r01(gen));
	double theta = dis_theta(gen);

	Vector2 p;
	p.x = position.x + r * cos(theta);
	p.y = position.y + r * sin(theta);

	return p;
}