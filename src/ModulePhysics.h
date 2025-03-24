#pragma once

#include "Module.h"
#include "IRendereable.h"
#include "Globals.h"
#include "Vector2.h"
#include "box2d/box2d.h"


#define PIXELS_PER_METER 16.0f
#define METER_PER_PIXEL 1/PIXELS_PER_METER // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * (m)))
#define METERS_TO_PIXELS_RAW(m) (floor(PIXELS_PER_METER * (m)))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * (p))


class PhysJoint
{
	public:
		enum class JointType
		{
			Revolute,
			Prismatic,
			Distance,
			Pulley,
			Gear,
			Weld,
			Mouse
		};

		PhysJoint();
		~PhysJoint();

		// Manipulación
		void SetMotorSpeed(float speed);
		void SetMaxMotorForceOrTorque(float force);
		void SetLimits(float lower, float upper);
		void EnableMotor(bool enable);
		void EnableLimit(bool enable);
		void SetLength(float length);
		void SetRatio(float ratio);
		void SetTarget(const Vector2& target);
		void SetMaxForce(float maxForce);
		void SetLinearOffset(const Vector2& offset);
		void SetAngularOffset(float offset);

		// Consultas
		Vector2 GetReactionForce(float inv_dt) const;
		float GetReactionTorque(float inv_dt) const;
		float GetJointAngle() const;
		float GetJointSpeed() const;
		float GetJointTranslation() const;
		float GetLength() const;
		float GetRatio() const;
		float GetLengthA() const;
		float GetLengthB() const;
		Vector2 GetTarget() const;
		float GetMaxForce() const;
		Vector2 GetLinearOffset() const;
		float GetAngularOffset() const;

		Vector2 GetPositionBodyA() const;
		Vector2 GetPositionBodyB() const;
		Vector2 GetPhysicPositionBodyA() const;
		Vector2 GetPhysicPositionBodyB() const;

		// Destruir la articulación
		void DestroyJoint();

	public:
		b2Joint* joint = nullptr; // Puntero a la articulación subyacente de Box2D
		JointType type = JointType::Weld;           // Tipo de la articulación

	private:
};


// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
	public:

		enum class BodyType
		{
			Static,
			Dynamic,
			Kinematic
		};
		PhysBody();
		~PhysBody();

		Vector2 GetPhysicPosition() const;
		Vector2 GetPosition() const;
		Vector2 GetWorldCenter() const;
		Vector2 GetWorldVector(Vector2 axis) const;

		void SetPosition(float x, float y);
		void SetPositionAndRotation(float x, float y, float rotation);
		void SetPhysicPosition(float x, float y);
		void SetPhysicPositionAndRotation(float x, float y, float rotation);
		void SetRotation(float rotation);
		void ApplyForce(const Vector2& force, const Vector2& point);
		void ApplyLinearImpulse(const Vector2& impulse, const Vector2& point);
		void ApplyAngularImpulse(float impulse);
		void ApplyTorque(float torque);
		void SetVelocity(const Vector2& velocity);
		void SetLinearDamping(float damping);
		void SetAngularDamping(float angularDamping);
		Vector2 GetLinearVelocity() const;
		float GetAngularVelocity() const;
		float GetAngularDamping() const;
		float GetInertia() const;
		float GetAngle() const;
		float GetMass() const;

		void SetMass(float mass, const Vector2& center, float inertia);
		void SetType(BodyType type);
		void SetBullet(bool status);
		void SetFixedRotation(bool flag);
		void SetFriction(size_t fixtureIndex, float friction);
		void SetDensity(size_t fixtureIndex, float density);
		void SetRestitution(size_t fixtureIndex, float restitution);
		void SetRestitutionThreshold(size_t fixtureIndex, float restitutionThreshold);
		void SetSensor(size_t fixtureIndex, bool isSensor);
		void SetFilter(size_t fixtureIndex, uint16 categoryBits, uint16 maskBits, int16 groupIndex);

		float GetFriction(size_t fixtureIndex) const;
		float GetDensity(size_t fixtureIndex) const;
		float GetRestitution(size_t fixtureIndex) const;
		float GetRestitutionThreshold(size_t fixtureIndex) const;
		b2FixtureUserData GetFixtureUserData(size_t fixtureIndex) const;
		bool IsSensor(size_t fixtureIndex) const;
		b2Filter GetFilter(size_t fixtureIndex) const;

		size_t GetFixtureCount() const;

		void DestroyBody();
	
	public:
		int width, height = 0;
		uintptr_t data;

		/// <summary>
		/// DO NOT MODIFY!!
		/// </summary>
		b2Body* body = nullptr;
	private:
		b2Fixture* GetFixtureByIndex(size_t fixtureIndex) const;


};


class PhysicFactory;
class CollisionsDispatcher;

// Module --------------------------------------
class ModulePhysics : public Module, public IRendereable
{
	friend class Engine;

	

	public:

		union Layer {
			Uint16 rawValue = 0;
			struct internalFlags {
				char default_layer : 1;
				char ground_layer : 1;
				char player_layer : 1;
				char interactable_layer : 1;
				char trigger_layer : 1;
				char npc_layer : 1;
			}flags;
		};

		ModulePhysics(bool start_active = true);
		~ModulePhysics();

		bool IsDebugActive();
		void PauseSimulation();
		void StartSimulation();
		bool IsSimulationPaused();

		const PhysicFactory& factory();

	public:
		PhysBody* ground = nullptr;

	private:
		// Inherited via IInitializable
		bool Start() override;
		// Inherited via IUpdateable
		bool PreUpdate() override;
		// Inherited via IUpdateable
		bool Update() override;
		// Inherited via IUpdateable
		bool PostUpdate() override;
		// Inherited via IRendereable
		void Render() override;
		// Inherited via ICleanable
		bool CleanUp() override;

	private:
		bool debug = false;
		b2World* world = nullptr;
		bool simulationOn = true;

		CollisionsDispatcher* collisionsManager = nullptr;
		PhysicFactory* physicFactory = nullptr;

		
};