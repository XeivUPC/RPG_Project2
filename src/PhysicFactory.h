#pragma once
#include "ModulePhysics.h"
#include <vector>


class PhysBody;
class PhysJoint;

class PhysicFactory
{
	public:
		PhysicFactory(b2World& _world);
		~PhysicFactory();
	
		PhysBody* CreateCircle(const Vector2& position,float radius, b2FixtureUserData userData = b2FixtureUserData()) const;
		PhysBody* CreateBox(const Vector2& position, float width, float height, b2FixtureUserData userData = b2FixtureUserData())const;
		PhysBody* CreateBevelBox(const Vector2& position, float width, float height, float bevelSize, b2FixtureUserData userData = b2FixtureUserData())const;
		PhysBody* CreateCapsule(const Vector2& position, float width, float height, float radius, b2FixtureUserData userData = b2FixtureUserData())const;
		PhysBody* CreateChain(const Vector2& position, const std::vector<Vector2>& vertices, b2FixtureUserData userData = b2FixtureUserData()) const;
		PhysBody* CreatePolygon(const Vector2& position, const std::vector<Vector2>& vertices, b2FixtureUserData userData = b2FixtureUserData()) const;

		int AddCircle(PhysBody* bodyToAddTo, const Vector2& offset, float radius, b2FixtureUserData userData = b2FixtureUserData())const;
		int AddBox(PhysBody* bodyToAddTo, const Vector2& offset, float width, float height, b2FixtureUserData userData = b2FixtureUserData())const;
		int AddPolygon(PhysBody* bodyToAddTo, const Vector2& offset, const std::vector<Vector2>& vertices, b2FixtureUserData userData = b2FixtureUserData())const;



		PhysJoint* CreateRevoluteJoint(PhysBody* bodyA, PhysBody* bodyB, const Vector2& localAnchorA, const Vector2& localAnchorB,
			bool enableLimit = false, float lowerAngle = 0.0f, float upperAngle = 0.0f,
			bool enableMotor = false, float motorSpeed = 0.0f, float maxMotorTorque = 0.0f)const;

		PhysJoint* CreatePrismaticJoint(PhysBody* bodyA, PhysBody* bodyB, const Vector2& anchor, const Vector2& axis,
			bool enableLimit = false, float lowerTranslation = 0.0f, float upperTranslation = 0.0f,
			bool enableMotor = false, float motorSpeed = 0.0f, float maxMotorForce = 0.0f)const;

		PhysJoint* CreateDistanceJoint(PhysBody* bodyA, PhysBody* bodyB, const Vector2& anchorA, const Vector2& anchorB,
			float length, float frequencyHz = 0.0f, float dampingRatio = 0.0f)const;

		PhysJoint* CreatePulleyJoint(PhysBody* bodyA, PhysBody* bodyB,
			const Vector2& groundAnchorA, const Vector2& groundAnchorB,
			const Vector2& anchorA, const Vector2& anchorB,
			float ratio, float lengthA = 0.0f, float lengthB = 0.0f)const;

		PhysJoint* CreateGearJoint(PhysJoint* joint1, PhysJoint* joint2, float ratio)const;

		PhysJoint* CreateWeldJoint(PhysBody* bodyA, PhysBody* bodyB, const Vector2& anchor, float frequencyHz = 0.0f, float dampingRatio = 0.0f)const;

		PhysJoint* CreateMouseJoint(PhysBody* bodyA, PhysBody* staticBodyAnchor, const Vector2& target, float maxForce, float frequencyHz, float dampingRatio)const;

		//// Triangulation
		std::vector<std::vector<Vector2>> Triangulate(const std::vector<Vector2>& polygon) const;

	private:
		b2CircleShape CreateCircleShape(float radius, b2Vec2 offset = {0,0})const;
		b2PolygonShape CreateBoxShape(float width, float height, b2Vec2 offset = { 0,0 })const;


		//// Triangulation
		float CrossProduct(const Vector2& v1, const Vector2& v2)const;
		bool IsPointInTriangle(const Vector2& p, const Vector2& p0, const Vector2& p1, const Vector2& p2)const;
		bool IsEar(const std::vector<Vector2>& polygon, int i, int j, int k)const;

	private:
		b2World* world;
};

