#include "Engine.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleUpdater.h"

#include "CollisionsDispatcher.h"
#include "PhysicFactory.h"
#include "Globals.h"
#include "ModuleTime.h"
#include "Log.h"
#include "DrawingTools.h"

#include <math.h>


ModulePhysics::ModulePhysics(bool start_active) : Module(start_active)
{
	debug = true;
	collisionsManager = new CollisionsDispatcher();
	renderLayer = 10;

	
}

ModulePhysics::~ModulePhysics()
{
	
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
	
	b2Vec2 gravity = { 0,0 };
	world = new b2World(gravity);
	physicFactory = new PhysicFactory(*world);

	ground = physicFactory->CreateBox({ 0,0 }, 1, 1);
	ground->SetType(PhysBody::BodyType::Static);
	ground->SetSensor(0,true);

	world->SetContactListener(collisionsManager);

	Engine::Instance().m_render->AddToRenderQueue(*this);
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);

	return true;
}

bool ModulePhysics::PreUpdate()
{
	double dt = ModuleTime::deltaTime;

	if(simulationOn)
		world->Step(1/60.f, 6, 2);
	return true;
}

bool ModulePhysics::Update()
{
	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		debug = !debug;
	}
	return true;
}

// 
bool ModulePhysics::PostUpdate()
{

	return true;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	delete collisionsManager;
	delete physicFactory;

	delete ground;
	delete world;

	Engine::Instance().m_render->RemoveFomRenderQueue(*this);
	Engine::Instance().m_updater->RemoveFomUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
	Engine::Instance().m_updater->RemoveFomUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);

	return true;
}

void ModulePhysics::Render()
{
	if (!debug)
	{
		return;
	} 

	ModuleRender& renderer = *Engine::Instance().m_render;
	const DrawingTools& painter = renderer.painter();

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			b2Shape* shape = f->GetShape();
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* circleShape = (b2CircleShape*)shape;
				b2Vec2 pos = b->GetWorldPoint(circleShape->m_p);

				if (renderer.IsCircleCameraVisible({ (float)METERS_TO_PIXELS(pos.x), (float)METERS_TO_PIXELS(pos.y) },(float)METERS_TO_PIXELS(circleShape->m_radius)))
				{
					painter.RenderCircle({ METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y) },(float)METERS_TO_PIXELS(circleShape->m_radius), {255,255,255,255});
				}
			}
			break;

			case b2Shape::e_chain:
			{
				b2ChainShape* chainShape = (b2ChainShape*)shape;
				b2Vec2 prev, v;

				for (int32 i = 0; i < chainShape->m_count; ++i)
				{
					v = b->GetWorldPoint(chainShape->m_vertices[i]);
					if (i > 0)
					{
						if (renderer.IsLineCameraVisible({ (float)METERS_TO_PIXELS(prev.x), (float)METERS_TO_PIXELS(prev.y) }, {(float)METERS_TO_PIXELS(v.x), (float)METERS_TO_PIXELS(v.y) }))
						{
							painter.RenderLine({ METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y) }, {METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y) },{0,255,0,255});
						}
					}
					prev = v;
				}
			}
			break;

			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)shape;
				int32 count = polygonShape->m_count;
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->m_vertices[i]);
					if (i > 0)
					{
						if (renderer.IsLineCameraVisible({ (float)METERS_TO_PIXELS(prev.x), (float)METERS_TO_PIXELS(prev.y) }, { (float)METERS_TO_PIXELS(v.x), (float)METERS_TO_PIXELS(v.y) }))
						{
							painter.RenderLine({ METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y) }, {METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y) },{ 255,0,0,255 });
						}
					}
					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->m_vertices[0]);

				if (renderer.IsLineCameraVisible({ (float)METERS_TO_PIXELS(prev.x), (float)METERS_TO_PIXELS(prev.y) }, { (float)METERS_TO_PIXELS(v.x), (float)METERS_TO_PIXELS(v.y) }))
				{
					painter.RenderLine({ METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y) }, {METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y) },{ 255,0,0,255 });
				}
			}
			break;

			case b2Shape::e_edge:
			{
				b2EdgeShape* edgeShape = (b2EdgeShape*)shape;
				b2Vec2 v1 = b->GetWorldPoint(edgeShape->m_vertex0);
				b2Vec2 v2 = b->GetWorldPoint(edgeShape->m_vertex1);
				if(renderer.IsLineCameraVisible({ (float)METERS_TO_PIXELS(v1.x), (float)METERS_TO_PIXELS(v1.y) }, { (float)METERS_TO_PIXELS(v2.x), (float)METERS_TO_PIXELS(v2.y) }))
				{
					painter.RenderLine({ METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y) }, {METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y) },{ 0,0,255,255 });
				}
			}
			break;
			}
		}
	}

	for (b2Joint* j = world->GetJointList(); j; j = j->GetNext())
	{
		b2Vec2 anchorA = j->GetAnchorA();
		b2Vec2 anchorB = j->GetAnchorB();

		painter.RenderCircle({ METERS_TO_PIXELS(anchorA.x), METERS_TO_PIXELS(anchorA.y) }, 4, { 0,255,255,255 });
		painter.RenderCircle({ METERS_TO_PIXELS(anchorB.x), METERS_TO_PIXELS(anchorB.y) }, 4, { 0,255,255,255 });
		painter.RenderLine({ METERS_TO_PIXELS(anchorA.x), METERS_TO_PIXELS(anchorA.y) }, { METERS_TO_PIXELS(anchorB.x), METERS_TO_PIXELS(anchorB.y) }, { 0,0,255,255 });
	}
}

bool ModulePhysics::IsDebugActive()
{
	return debug;
}

void ModulePhysics::PauseSimulation()
{
	simulationOn = false;
}

void ModulePhysics::StartSimulation()
{
	simulationOn = true;
}

bool ModulePhysics::IsSimulationPaused()
{
	return simulationOn;
}

const PhysicFactory& ModulePhysics::factory()
{
	return *physicFactory;
}

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////

#pragma region PhysBody

PhysBody::PhysBody()
{

}

PhysBody::~PhysBody()
{
	DestroyBody();
}

Vector2 PhysBody::GetPhysicPosition() const
{
	if (body) {
		b2Vec2 pos = body->GetPosition();
		int x = METERS_TO_PIXELS(pos.x);
		int y = METERS_TO_PIXELS(pos.y);
		return { (float)x,(float)y };
	}
	return { 0,0 };
}

Vector2 PhysBody::GetPosition() const
{
	if (body) {
		b2Vec2 pos = body->GetPosition();
		return { pos.x,pos.y };
	}
	return { 0,0 };
}

Vector2 PhysBody::GetWorldCenter() const
{
	if (body) {
		b2Vec2 worldCenter = body->GetWorldCenter();
		return { worldCenter.x, worldCenter.y };
	}
	return { 0,0 };
}

Vector2 PhysBody::GetWorldVector(Vector2 axis) const
{
	if (body) {
		b2Vec2 worldVector = body->GetWorldVector({ axis.x, axis.y });
		return { worldVector.x, worldVector.y };
	}
	return { 0,0 };
}

void PhysBody::SetPosition(float x, float y) {
	if (body) {
		body->SetTransform({ x, y }, body->GetAngle());
	}
}

void PhysBody::SetPositionAndRotation(float x, float y, float rotation)
{
	if (body) {
		body->SetTransform({ x, y }, rotation);
	}
}

void PhysBody::SetPhysicPosition(float x, float y) {
	if (body) {

		float posX = PIXEL_TO_METERS(x);
		float posY = PIXEL_TO_METERS(y);
		body->SetTransform({ posX, posY }, body->GetAngle());
	}
}

void PhysBody::SetPhysicPositionAndRotation(float x, float y, float rotation)
{
	if (body) {

		float posX = PIXEL_TO_METERS(x);
		float posY = PIXEL_TO_METERS(y);
		body->SetTransform({ posX, posY }, rotation);
	}
}

void PhysBody::SetRotation(float rotation)
{
	if (body) {
		body->SetTransform(body->GetWorldCenter(),rotation);
	}
}

void PhysBody::ApplyForce(const Vector2& force, const Vector2& point) {
	if (body) {
		body->ApplyForce({ force.x, force.y }, { point.x, point.y }, true);
	}
}

void PhysBody::ApplyLinearImpulse(const Vector2& impulse, const Vector2& point) {
	if (body) {
		
		body->ApplyLinearImpulse({ impulse.x, impulse.y }, {point.x, point.y}, true);
	}
}

void PhysBody::ApplyAngularImpulse(float impulse)
{
	if (body) {

		body->ApplyAngularImpulse(impulse, true);
	}
}

void PhysBody::ApplyTorque(float torque)
{
	if (body) {
		body->ApplyTorque(torque, true);
	}
}

void PhysBody::SetVelocity(const Vector2& velocity) {
	if (body) {
		body->SetLinearVelocity({ velocity.x,velocity.y});
	}
}

void PhysBody::SetLinearDamping(float damping)
{
	if (body) {
		body->SetLinearDamping(damping);
	}
}

void PhysBody::SetAngularDamping(float angularDamping)
{
	if (body) {
		body->SetAngularDamping(angularDamping);
	}
}

Vector2 PhysBody::GetLinearVelocity() const {
	if (body) {
		b2Vec2 vel = body->GetLinearVelocity();
		return {vel.x,vel.y};
	}
	return {0,0};
}

float PhysBody::GetAngularVelocity() const
{
	if (body) {
		float vel = body->GetAngularVelocity();
		return vel;
	}
	return 0;
}

float PhysBody::GetAngularDamping() const
{
	if (body) {
		float val = body->GetAngularDamping();
		return val;
	}
	return 0;
}

float PhysBody::GetInertia() const
{
	if (body) {
		float vel = body->GetInertia();
		return vel;
	}
	return 0;
}

float PhysBody::GetAngle() const {
	if (body) {
		return body->GetAngle();
	}
	return 0.0f;
}

float PhysBody::GetMass() const
{
	if (body) {
		return body->GetMass();
	}
	return 0.0f;
}

b2Fixture* PhysBody::GetFixtureByIndex(size_t fixtureIndex) const {
	if (!body) return nullptr;
	b2Fixture* fixture = body->GetFixtureList();
	size_t i = 0;
	while (fixture && i < fixtureIndex) {
		fixture = fixture->GetNext();
		i++;
	}
	return fixture;
}

void PhysBody::SetMass(float mass, const Vector2& center, float inertia)
{
	b2MassData massData;
	massData.mass = mass;
	massData.center = {center.x, center.y};
	massData.I = inertia;
	body->SetMassData(&massData);
}

void PhysBody::SetType(BodyType type)
{
	if (body) {
		switch (type)
		{
		case PhysBody::BodyType::Static:
			body->SetType(b2_staticBody);
			break;
		case PhysBody::BodyType::Dynamic:
			body->SetType(b2_dynamicBody);
			break;
		case PhysBody::BodyType::Kinematic:
			body->SetType(b2_kinematicBody);
			break;
		default:
			break;
		}
		
	}
}

void PhysBody::SetBullet(bool status)
{
	if (body) {
		body->SetBullet(status);
	}
}

void PhysBody::SetFriction(size_t fixtureIndex, float friction) {
	if (b2Fixture* fixture = GetFixtureByIndex(fixtureIndex)) {
		fixture->SetFriction(friction);
	}
}

void PhysBody::SetDensity(size_t fixtureIndex, float density) {
	if (b2Fixture* fixture = GetFixtureByIndex(fixtureIndex)) {
		fixture->SetDensity(density);
		body->ResetMassData();
	}
}

void PhysBody::SetRestitution(size_t fixtureIndex, float restitution) {
	if (b2Fixture* fixture = GetFixtureByIndex(fixtureIndex)) {
		fixture->SetRestitution(restitution);
	}
}

void PhysBody::SetRestitutionThreshold(size_t fixtureIndex, float restitutionThreshold)
{
	if (b2Fixture* fixture = GetFixtureByIndex(fixtureIndex)) {
		fixture->SetRestitutionThreshold(restitutionThreshold);
	}
}

void PhysBody::SetSensor(size_t fixtureIndex, bool isSensor) {
	if (b2Fixture* fixture = GetFixtureByIndex(fixtureIndex)) {
		fixture->SetSensor(isSensor);
	}
}

b2Filter PhysBody::GetFilter(size_t fixtureIndex) const {
	if (b2Fixture* fixture = GetFixtureByIndex(fixtureIndex)) {
		return fixture->GetFilterData();
	}
	return {};
}

float PhysBody::GetFriction(size_t fixtureIndex) const {
	if (b2Fixture* fixture = GetFixtureByIndex(fixtureIndex)) {
		return fixture->GetFriction();
	}
	return 0.0f;
}

float PhysBody::GetDensity(size_t fixtureIndex) const {
	if (b2Fixture* fixture = GetFixtureByIndex(fixtureIndex)) {
		return fixture->GetDensity();
	}
	return 0.0f;
}

float PhysBody::GetRestitution(size_t fixtureIndex) const {
	if (b2Fixture* fixture = GetFixtureByIndex(fixtureIndex)) {
		return fixture->GetRestitution();
	}
	return 0.0f;
}

float PhysBody::GetRestitutionThreshold(size_t fixtureIndex) const
{
	if (b2Fixture* fixture = GetFixtureByIndex(fixtureIndex)) {
		return fixture->GetRestitutionThreshold();
	}
	return 0.0f;
}

b2FixtureUserData PhysBody::GetFixtureUserData(size_t fixtureIndex) const
{
	if (b2Fixture* fixture = GetFixtureByIndex(fixtureIndex)) {
		return fixture->GetUserData();
	}
	return b2FixtureUserData();
}

bool PhysBody::IsSensor(size_t fixtureIndex) const {
	if (b2Fixture* fixture = GetFixtureByIndex(fixtureIndex)) {
		return fixture->IsSensor();
	}
	return false;
}

void PhysBody::SetFilter(size_t fixtureIndex, uint16 categoryBits, uint16 maskBits, int16 groupIndex) {
	if (b2Fixture* fixture = GetFixtureByIndex(fixtureIndex)) {
		b2Filter filter;
		filter.categoryBits = categoryBits;
		filter.maskBits = maskBits;
		filter.groupIndex = groupIndex;
		fixture->SetFilterData(filter);
	}
}

size_t PhysBody::GetFixtureCount() const {
	size_t count = 0;
	for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		++count;
	}
	return count;
}


void PhysBody::DestroyBody() {
	if (body && body->GetWorld()) {
		body->GetWorld()->DestroyBody(body);
		body = nullptr;
	}
}

#pragma endregion

#pragma region PhysJoint

PhysJoint::PhysJoint()
{
}

PhysJoint::~PhysJoint()
{
	DestroyJoint();
}

void PhysJoint::SetMotorSpeed(float speed)
{
	if (auto* revoluteJoint = dynamic_cast<b2RevoluteJoint*>(joint))
	{
		revoluteJoint->SetMotorSpeed(speed);
	}
	else if (auto* prismaticJoint = dynamic_cast<b2PrismaticJoint*>(joint))
	{
		prismaticJoint->SetMotorSpeed(speed);
	}
}

void PhysJoint::SetMaxMotorForceOrTorque(float force)
{
	if (auto* prismaticJoint = dynamic_cast<b2PrismaticJoint*>(joint))
	{
		prismaticJoint->SetMaxMotorForce(force);
	}
	else if (auto* revoluteJoint = dynamic_cast<b2RevoluteJoint*>(joint))
	{
		revoluteJoint->SetMaxMotorTorque(force);
	}
}

void PhysJoint::SetLimits(float lower, float upper)
{
	if (auto* revoluteJoint = dynamic_cast<b2RevoluteJoint*>(joint))
	{
		revoluteJoint->SetLimits(lower, upper);
		revoluteJoint->EnableLimit(true);
	}
	else if (auto* prismaticJoint = dynamic_cast<b2PrismaticJoint*>(joint))
	{
		prismaticJoint->SetLimits(lower, upper);
		prismaticJoint->EnableLimit(true);
	}
}

void PhysJoint::EnableMotor(bool enable)
{
	if (auto* revoluteJoint = dynamic_cast<b2RevoluteJoint*>(joint))
	{
		revoluteJoint->EnableMotor(enable);
	}
	else if (auto* prismaticJoint = dynamic_cast<b2PrismaticJoint*>(joint))
	{
		prismaticJoint->EnableMotor(enable);
	}
}

void PhysJoint::EnableLimit(bool enable)
{
	if (auto* revoluteJoint = dynamic_cast<b2RevoluteJoint*>(joint))
	{
		revoluteJoint->EnableLimit(enable);
	}
	else if (auto* prismaticJoint = dynamic_cast<b2PrismaticJoint*>(joint))
	{
		prismaticJoint->EnableLimit(enable);
	}
}

Vector2 PhysJoint::GetReactionForce(float inv_dt) const
{
	b2Vec2 force = joint->GetReactionForce(inv_dt);
	return { force.x, force.y };
}

float PhysJoint::GetReactionTorque(float inv_dt) const
{
	return joint->GetReactionTorque(inv_dt);
}

float PhysJoint::GetJointAngle() const
{
	if (auto* revoluteJoint = dynamic_cast<b2RevoluteJoint*>(joint))
	{
		return revoluteJoint->GetJointAngle();
	}
	return 0.0f;
}

float PhysJoint::GetJointSpeed() const
{
	if (auto* revoluteJoint = dynamic_cast<b2RevoluteJoint*>(joint))
	{
		return revoluteJoint->GetJointSpeed();
	}else if (auto* prismaticJoint = dynamic_cast<b2PrismaticJoint*>(joint))
	{
		return prismaticJoint->GetJointSpeed();
	}
	return 0.0f;
}

float PhysJoint::GetJointTranslation() const
{
	if (auto* prismaticJoint = dynamic_cast<b2PrismaticJoint*>(joint))
	{
		return prismaticJoint->GetJointTranslation();
	}
	return 0.0f;
}

void PhysJoint::SetLength(float length)
{
	if (auto* distanceJoint = dynamic_cast<b2DistanceJoint*>(joint))
	{
		distanceJoint->SetLength(length);
	}
}

float PhysJoint::GetLength() const
{
	if (auto* distanceJoint = dynamic_cast<b2DistanceJoint*>(joint))
	{
		return distanceJoint->GetLength();
	}
	return 0.0f;
}

float PhysJoint::GetRatio() const
{
	if (auto* pulleyJoint = dynamic_cast<b2PulleyJoint*>(joint))
	{
		return pulleyJoint->GetRatio();
	}else if (auto* gearJoint = dynamic_cast<b2GearJoint*>(joint))
	{
		return gearJoint->GetRatio();
	}
	return 0.0f;
}

float PhysJoint::GetLengthA() const
{
	if (auto* pulleyJoint = dynamic_cast<b2PulleyJoint*>(joint))
	{
		return pulleyJoint->GetLengthA();
	}
	return 0.0f;
}

float PhysJoint::GetLengthB() const
{
	if (auto* pulleyJoint = dynamic_cast<b2PulleyJoint*>(joint))
	{
		return pulleyJoint->GetLengthB();
	}
	return 0.0f;
}

void PhysJoint::SetRatio(float ratio)
{
	if (auto* gearJoint = dynamic_cast<b2GearJoint*>(joint))
	{
		gearJoint->SetRatio(ratio);
	}
}

void PhysJoint::SetTarget(const Vector2& target)
{
	if (auto* mouseJoint = dynamic_cast<b2MouseJoint*>(joint))
	{
		mouseJoint->SetTarget(b2Vec2(target.x, target.y));
	}
}

Vector2 PhysJoint::GetTarget() const
{
	if (auto* mouseJoint = dynamic_cast<b2MouseJoint*>(joint))
	{
		b2Vec2 target = mouseJoint->GetTarget();
		return { target.x, target.y };
	}
	return { 0,0 };
}

void PhysJoint::SetMaxForce(float maxForce)
{
	if (auto* mouseJoint = dynamic_cast<b2MouseJoint*>(joint))
	{
		mouseJoint->SetMaxForce(maxForce);
	}else if (auto* motorJoint = dynamic_cast<b2MotorJoint*>(joint))
	{
		motorJoint->SetMaxForce(maxForce);
	}
}

float PhysJoint::GetMaxForce() const
{
	if (auto* mouseJoint = dynamic_cast<b2MouseJoint*>(joint))
	{
		return mouseJoint->GetMaxForce();
	}else if (auto* motorJoint = dynamic_cast<b2MotorJoint*>(joint))
	{
		motorJoint->GetMaxForce();
	}
	return 0.0f;
}

void PhysJoint::SetLinearOffset(const Vector2& offset) {
	if (auto* motorJoint = dynamic_cast<b2MotorJoint*>(joint))
	{
		motorJoint->SetLinearOffset(b2Vec2(offset.x, offset.y));
	}
}

Vector2  PhysJoint::GetLinearOffset() const {
	if (auto* motorJoint = dynamic_cast<b2MotorJoint*>(joint))
	{
		b2Vec2 offset = motorJoint->GetLinearOffset();
		return { offset.x, offset.y };
	}
	return {0,0};
}

void PhysJoint::SetAngularOffset(float offset) {
	if (auto* motorJoint = dynamic_cast<b2MotorJoint*>(joint))
	{
		motorJoint->SetAngularOffset(offset);
	}
}

float PhysJoint::GetAngularOffset() const
{
	if (auto* motorJoint = dynamic_cast<b2MotorJoint*>(joint))
	{
		return motorJoint->GetAngularOffset();
	}
	return 0.0f;
}

Vector2 PhysJoint::GetPositionBodyA() const
{
	if (joint) {
		b2Vec2 pos = joint->GetBodyA()->GetPosition();
		return {pos.x,pos.y};
	}
	return { 0,0 };
}

Vector2 PhysJoint::GetPositionBodyB() const
{
	if (joint) {
		b2Vec2 pos = joint->GetBodyB()->GetPosition();
		return { pos.x,pos.y };
	}
	return { 0,0 };
}

Vector2 PhysJoint::GetPhysicPositionBodyA() const
{
	if (joint) {
		Vector2 pos = GetPositionBodyB();
		int x = METERS_TO_PIXELS(pos.x);
		int y = METERS_TO_PIXELS(pos.y);
		return { (float)x,(float)y };
	}
	return { 0,0 };
}

Vector2 PhysJoint::GetPhysicPositionBodyB() const
{
	if (joint) {
		Vector2 pos = GetPositionBodyB();
		int x = METERS_TO_PIXELS(pos.x);
		int y = METERS_TO_PIXELS(pos.y);
		return { (float)x,(float)y };
	}
	return { 0,0 };
}

void PhysJoint::DestroyJoint()
{
	if (joint)
	{
		joint->GetBodyA()->GetWorld()->DestroyJoint(joint);
		joint = nullptr;
	}
}

#pragma endregion