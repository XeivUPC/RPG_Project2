#include "PhysicFactory.h"


PhysicFactory::PhysicFactory(b2World& _world)
{
	world = &_world;
}

PhysicFactory::~PhysicFactory()
{
}

PhysBody* PhysicFactory::CreateCircle(const Vector2& position, float radius, b2FixtureUserData userData)const
{
	PhysBody* pbody = new PhysBody();
	
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);
	b2Body* body = world->CreateBody(&bodyDef);

	b2CircleShape circleShape = CreateCircleShape(radius);

	b2FixtureDef circleFixtureDef;
	circleFixtureDef.shape = &circleShape;
	circleFixtureDef.userData = userData;
	circleFixtureDef.density = 1.0f;
	circleFixtureDef.friction = 1.0f;
	body->CreateFixture(&circleFixtureDef);

	pbody->body = body;


	return pbody;
}


PhysBody* PhysicFactory::CreateBox(const Vector2& position, float width, float height, b2FixtureUserData userData)const
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);
	b2Body* body = world->CreateBody(&bodyDef);

	b2PolygonShape boxShape = CreateBoxShape(width, height);

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.userData = userData;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 1.0f;
	boxFixtureDef.friction = 1.0f;
	body->CreateFixture(&boxFixtureDef);

	pbody->body = body;

	return pbody;
}

PhysBody* PhysicFactory::CreateBevelBox(const Vector2& position, float width, float height, float bevelSize, b2FixtureUserData userData)const
{
	PhysBody* pbody = new PhysBody();

	std::vector<b2Vec2> vertices;

	// Calculate half dimensions
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);
	b2Body* body = world->CreateBody(&bodyDef);

	// Define vertices for the beveled box
	vertices.push_back(b2Vec2(-halfWidth + bevelSize, -halfHeight));
	vertices.push_back(b2Vec2(halfWidth - bevelSize, -halfHeight));
	vertices.push_back(b2Vec2(halfWidth, -halfHeight + bevelSize));
	vertices.push_back(b2Vec2(halfWidth, halfHeight - bevelSize));
	vertices.push_back(b2Vec2(halfWidth - bevelSize, halfHeight));
	vertices.push_back(b2Vec2(-halfWidth + bevelSize, halfHeight));
	vertices.push_back(b2Vec2(-halfWidth, halfHeight - bevelSize));
	vertices.push_back(b2Vec2(-halfWidth, -halfHeight + bevelSize));

	// Create the polygon shape
	b2PolygonShape beveledBox;
	beveledBox.Set(&vertices[0], (int)vertices.size());

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.userData = userData;
	boxFixtureDef.shape = &beveledBox;
	boxFixtureDef.density = 1.0f;
	boxFixtureDef.friction = 1.0f;
	body->CreateFixture(&boxFixtureDef);

	pbody->body = body;

	return pbody;
}

PhysBody* PhysicFactory::CreateCapsule(const Vector2& position, float width, float height, float radius, b2FixtureUserData userData)const
{
	PhysBody* pbody = new PhysBody();

	float rectangleHeight = height - 2 * radius; // Adjust height to accommodate the circle ends

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);
	b2Body* body = world->CreateBody(&bodyDef);

	b2CircleShape circleShape = CreateCircleShape(radius);
	b2PolygonShape boxShape = CreateBoxShape(width, rectangleHeight);

	// Create the rectangle fixture
	b2FixtureDef rectangleFixtureDef;
	rectangleFixtureDef.shape = &boxShape;
	rectangleFixtureDef.userData = userData;
	rectangleFixtureDef.density = 1.0f;
	rectangleFixtureDef.friction = 1.0f;

	body->CreateFixture(&rectangleFixtureDef);

	// Top circle
	b2FixtureDef topCircleFixtureDef;
	circleShape.m_p.Set(0, rectangleHeight / 2.0f); // Move the circle to the top
	topCircleFixtureDef.shape = &circleShape;
	topCircleFixtureDef.density = 1.0f;
	topCircleFixtureDef.friction = 1.0f;
	body->CreateFixture(&topCircleFixtureDef);

	// Bottom circle
	b2FixtureDef bottomCircleFixtureDef;
	circleShape.m_p.Set(0, -rectangleHeight / 2.0f); // Move the circle to the bottom
	bottomCircleFixtureDef.shape = &circleShape;
	bottomCircleFixtureDef.density = 1.0f;
	bottomCircleFixtureDef.friction = 1.0f;
	body->CreateFixture(&bottomCircleFixtureDef);

	pbody->body = body;

	return pbody;

}

PhysBody* PhysicFactory::CreateChain(const Vector2& position, const std::vector<Vector2>& vertices, b2FixtureUserData userData) const
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody; // Typically chains are static
	bodyDef.position.Set(position.x, position.y);
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);
	b2Body* body = world->CreateBody(&bodyDef);

	b2ChainShape chainShape;
	std::vector<b2Vec2> b2Vertices(vertices.size());
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		b2Vertices[i].Set(vertices[i].x, vertices[i].y);
	}
	chainShape.CreateLoop(&b2Vertices[0], (int)b2Vertices.size());

	b2FixtureDef chainFixtureDef;
	chainFixtureDef.shape = &chainShape;
	chainFixtureDef.userData = userData;
	chainFixtureDef.density = 1.0f;
	chainFixtureDef.friction = 1.0f;
	body->CreateFixture(&chainFixtureDef);

	pbody->body = body;

	return pbody;
}

PhysBody* PhysicFactory::CreatePolygon(const Vector2& position, const std::vector<Vector2>& vertices, b2FixtureUserData userData) const
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody; // Typically chains are static
	bodyDef.position.Set(position.x, position.y);
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);
	b2Body* body = world->CreateBody(&bodyDef);

	b2PolygonShape chainShape;
	std::vector<b2Vec2> b2Vertices(vertices.size());
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		b2Vertices[i].Set(vertices[i].x, vertices[i].y);
	}
	chainShape.Set(&b2Vertices[0], (int)b2Vertices.size());

	b2FixtureDef chainFixtureDef;
	chainFixtureDef.shape = &chainShape;
	chainFixtureDef.userData = userData;
	chainFixtureDef.density = 1.0f;
	chainFixtureDef.friction = 1.0f;
	body->CreateFixture(&chainFixtureDef);

	pbody->body = body;

	return pbody;
}

int PhysicFactory::AddCircle(PhysBody* bodyToAddTo, const Vector2& offset, float radius, b2FixtureUserData userData)const
{
	b2CircleShape circleShape = CreateCircleShape(radius, { offset.x, offset.y });

	b2FixtureDef circleFixtureDef;
	circleFixtureDef.shape = &circleShape;
	circleFixtureDef.density = 0.0f;
	circleFixtureDef.userData = userData;
	circleFixtureDef.friction = 1.0f;


	b2Fixture* fixtureCreated = bodyToAddTo->body->CreateFixture(&circleFixtureDef);

	int fixtureIndex = 0;

	const b2Fixture* fixture = bodyToAddTo->body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture == fixtureCreated)
			return fixtureIndex;
		fixture = fixture->GetNext();
		fixtureIndex++;
	}
	return -1;
}

int PhysicFactory::AddBox(PhysBody* bodyToAddTo, const Vector2& offset, float width, float height, b2FixtureUserData userData)const
{

	b2PolygonShape boxShape = CreateBoxShape(width, height, { offset.x, offset.y });

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.userData = userData;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 0.0f;
	boxFixtureDef.friction = 1.0f;

	b2Fixture* fixtureCreated = bodyToAddTo->body->CreateFixture(&boxFixtureDef);

	int fixtureIndex = 0;

	const b2Fixture* fixture = bodyToAddTo->body->GetFixtureList();
	while (fixture != NULL)
	{
		if (fixture == fixtureCreated)
			return fixtureIndex;
		fixture = fixture->GetNext();
		fixtureIndex++;
	}
	return -1;
}

int PhysicFactory::AddPolygon(PhysBody* bodyToAddTo, const Vector2& offset, const std::vector<Vector2>& vertices, b2FixtureUserData userData) const
{
	b2PolygonShape chainShape;
	std::vector<b2Vec2> b2Vertices(vertices.size());
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		b2Vertices[i].Set(vertices[i].x + offset.x, vertices[i].y + offset.y);
	}
	chainShape.Set(&b2Vertices[0], (int)b2Vertices.size());

	b2FixtureDef chainFixtureDef;
	chainFixtureDef.shape = &chainShape;
	chainFixtureDef.userData = userData;
	chainFixtureDef.density = 0.0f;
	chainFixtureDef.friction = 1.0f;
	b2Fixture* fixtureCreated = bodyToAddTo->body->CreateFixture(&chainFixtureDef);

	int fixtureIndex = 0;

	const b2Fixture* fixture = bodyToAddTo->body->GetFixtureList();
	while (fixture != NULL)
	{
		if (fixture == fixtureCreated)
			return fixtureIndex;
		fixture = fixture->GetNext();
		fixtureIndex++;
	}
	return -1;
}


//////////////////////////////////

//////////////////////////////////


b2CircleShape PhysicFactory::CreateCircleShape(float radius, b2Vec2 offset)const
{
	b2CircleShape circle;
	circle.m_radius = radius;
	circle.m_p.Set(offset.x, offset.y);
	return circle;
}

b2PolygonShape PhysicFactory::CreateBoxShape(float width, float height, b2Vec2 offset)const
{
	b2PolygonShape rectangle;
	rectangle.SetAsBox(width / 2.0f, height / 2.0f, offset, 0);
	return rectangle;
}

PhysJoint* PhysicFactory::CreateRevoluteJoint(PhysBody* bodyA, PhysBody* bodyB, const Vector2& localAnchorA, const Vector2& localAnchorB,
	bool enableLimit, float lowerAngle, float upperAngle,
	bool enableMotor, float motorSpeed, float maxMotorTorque)const
{
	PhysJoint* pjoint = new PhysJoint();

	b2RevoluteJointDef jointDef;
	jointDef.bodyA = bodyA->body;
	jointDef.bodyB = bodyB->body;
	jointDef.localAnchorA.Set(localAnchorA.x, localAnchorA.y);
	jointDef.localAnchorB.Set(localAnchorB.x, localAnchorB.y);

	jointDef.enableLimit = enableLimit;
	jointDef.lowerAngle = lowerAngle;
	jointDef.upperAngle = upperAngle;
	jointDef.enableMotor = enableMotor;
	jointDef.motorSpeed = motorSpeed;
	jointDef.maxMotorTorque = maxMotorTorque;

	pjoint->joint = bodyA->body->GetWorld()->CreateJoint(&jointDef);
	pjoint->type = PhysJoint::JointType::Revolute;

	return pjoint;
}

PhysJoint* PhysicFactory::CreatePrismaticJoint(PhysBody* bodyA, PhysBody* bodyB, const Vector2& anchor, const Vector2& axis,
	bool enableLimit, float lowerTranslation, float upperTranslation,
	bool enableMotor, float motorSpeed, float maxMotorForce)const
{
	PhysJoint* pjoint = new PhysJoint();

	b2PrismaticJointDef jointDef;
	jointDef.Initialize(bodyA->body, bodyB->body, b2Vec2(anchor.x, anchor.y), b2Vec2(axis.x, axis.y));
	jointDef.enableLimit = enableLimit;
	jointDef.lowerTranslation = lowerTranslation;
	jointDef.upperTranslation = upperTranslation;
	jointDef.enableMotor = enableMotor;
	jointDef.motorSpeed = motorSpeed;
	jointDef.maxMotorForce = maxMotorForce;

	pjoint->joint = bodyA->body->GetWorld()->CreateJoint(&jointDef);
	pjoint->type = PhysJoint::JointType::Prismatic;

	return pjoint;
}

PhysJoint* PhysicFactory::CreateDistanceJoint(PhysBody* bodyA, PhysBody* bodyB, const Vector2& anchorA, const Vector2& anchorB,
	float length, float frequencyHz, float dampingRatio)const
{
	PhysJoint* pjoint = new PhysJoint();

	b2DistanceJointDef jointDef;
	jointDef.Initialize(bodyA->body, bodyB->body, b2Vec2(anchorA.x, anchorA.y), b2Vec2(anchorB.x, anchorB.y));
	jointDef.length = length;
	jointDef.stiffness = frequencyHz;
	jointDef.damping = dampingRatio;

	pjoint->joint = bodyA->body->GetWorld()->CreateJoint(&jointDef);
	pjoint->type = PhysJoint::JointType::Distance;

	return pjoint;
}

PhysJoint* PhysicFactory::CreatePulleyJoint(PhysBody* bodyA, PhysBody* bodyB, const Vector2& groundAnchorA, const Vector2& groundAnchorB, const Vector2& anchorA, const Vector2& anchorB, float ratio, float lengthA, float lengthB) const
{
	PhysJoint* pjoint = new PhysJoint();

	b2PulleyJointDef jointDef;
	jointDef.Initialize(bodyA->body, bodyB->body,
		b2Vec2(groundAnchorA.x, groundAnchorA.y),
		b2Vec2(groundAnchorB.x, groundAnchorB.y),
		b2Vec2(anchorA.x, anchorA.y),
		b2Vec2(anchorB.x, anchorB.y),
		ratio);
	if (lengthA > 0.0f) jointDef.lengthA = lengthA;
	if (lengthB > 0.0f) jointDef.lengthB = lengthB;

	pjoint->joint = bodyA->body->GetWorld()->CreateJoint(&jointDef);
	pjoint->type = PhysJoint::JointType::Pulley;

	return pjoint;
}

PhysJoint* PhysicFactory::CreateGearJoint(PhysJoint* joint1, PhysJoint* joint2, float ratio) const
{
	if (!joint1 || !joint2) return nullptr;

	PhysJoint* pjoint = new PhysJoint();

	b2GearJointDef jointDef;
	jointDef.bodyA = joint1->joint->GetBodyA();
	jointDef.bodyB = joint2->joint->GetBodyB();
	jointDef.joint1 = joint1->joint;
	jointDef.joint2 = joint2->joint;
	jointDef.ratio = ratio;

	pjoint->joint = jointDef.bodyA->GetWorld()->CreateJoint(&jointDef);
	pjoint->type = PhysJoint::JointType::Gear;

	return pjoint;
}

PhysJoint* PhysicFactory::CreateWeldJoint(PhysBody* bodyA, PhysBody* bodyB, const Vector2& anchor, float frequencyHz, float dampingRatio) const
{
	PhysJoint* pjoint = new PhysJoint();

	b2WeldJointDef jointDef;
	jointDef.Initialize(bodyA->body, bodyB->body, b2Vec2(anchor.x, anchor.y));
	jointDef.stiffness = frequencyHz;
	jointDef.damping = dampingRatio;

	pjoint->joint = bodyA->body->GetWorld()->CreateJoint(&jointDef);
	pjoint->type = PhysJoint::JointType::Weld;

	return pjoint;
}

PhysJoint* PhysicFactory::CreateMouseJoint(PhysBody* bodyA, PhysBody* staticBodyAnchor, const Vector2& target, float maxForce, float frequencyHz, float dampingRatio) const
{
	PhysJoint* pjoint = new PhysJoint();

	b2MouseJointDef jointDef;
	jointDef.bodyA = staticBodyAnchor->body;
	jointDef.bodyB = bodyA->body;
	jointDef.target = b2Vec2(target.x, target.y);
	jointDef.maxForce = maxForce;
	jointDef.stiffness = frequencyHz;
	jointDef.damping = dampingRatio;

	pjoint->joint = bodyA->body->GetWorld()->CreateJoint(&jointDef);
	pjoint->type = PhysJoint::JointType::Mouse;

	return pjoint;
}


float PhysicFactory::CrossProduct(const Vector2& v1, const Vector2& v2)const
{
	return v1.x * v2.y - v1.y * v2.x;
}

bool PhysicFactory::IsPointInTriangle(const Vector2& p, const Vector2& p0, const Vector2& p1, const Vector2& p2)const
{
	float c1 = (p1-p0).dot((p-p0));
	float c2 = (p2-p1).dot((p-p1));
	float c3 = (p0-p2).dot((p-p2));
	return (c1 >= 0 && c2 >= 0 && c3 >= 0) || (c1 <= 0 && c2 <= 0 && c3 <= 0);
}

bool PhysicFactory::IsEar(const std::vector<Vector2>& polygon, int i, int j, int k)const
{
	Vector2 p0 = polygon[i];
	Vector2 p1 = polygon[j];
	Vector2 p2 = polygon[k];

	// Comprobar si el triángulo es convexo
	if ((p1 - p0).dot((p2 - p0)) < 0) {
		return false; // Si el triángulo es cóncavo, no es un "ear"
	}

	// Comprobar que no haya otros puntos dentro del triángulo
	for (size_t m = 0; m < polygon.size(); m++) {
		if (m != i && m != j && m != k) {
			if (IsPointInTriangle(polygon[m], p0, p1, p2)) {
				return false;
			}
		}
	}

	return true;
}


std::vector<std::vector<Vector2>> PhysicFactory::Triangulate(const std::vector<Vector2>& polygon)const
{
	std::vector<std::vector<Vector2>> triangles;
	std::vector<Vector2> tempPolygon = polygon;

	while (tempPolygon.size() > 3) {
		bool earFound = false;

		for (size_t i = 0; i < tempPolygon.size(); i++) {
			size_t prev = (i == 0) ? tempPolygon.size() - 1 : i - 1;
			size_t next = (i == tempPolygon.size() - 1) ? 0 : i + 1;

			if (IsEar(tempPolygon, (int)prev, (int)i, (int)next)) {
				triangles.push_back({ tempPolygon[prev], tempPolygon[i], tempPolygon[next] });
				tempPolygon.erase(tempPolygon.begin() + i);
				earFound = true;
				break;
			}
		}

		if (!earFound) {
			printf("Error: No se pudo dividir el polígono en triángulos");
			break;
		}
	}

	// Agregar el último triángulo
	if (tempPolygon.size() == 3) {
		triangles.push_back({ tempPolygon[0], tempPolygon[1], tempPolygon[2] });
	}

	return triangles;
}