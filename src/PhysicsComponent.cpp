#include <Box2D/Box2D.h>
#include "PhysicsComponent.hpp"
#include "FishermanGame.hpp"
#include "Room.hpp"
#include "GlobalMacros.hpp"

PhysicsComponent::PhysicsComponent(GameObject* gameObject) 
	: Component(gameObject)
{ 
	ASSERT(gameObject->positionIsSet(), "Physics component added before position was set in gameobject: " + gameObject->getName());
	world = PhysicsEngine::instance->world;
}

PhysicsComponent::~PhysicsComponent()
{
	PhysicsEngine::instance->deregisterPhysicsComponent(this);

	delete polygon;
	delete circle;

	if (body != nullptr && fixture != NULL)
	{
		auto lars = gameObject;
		body->DestroyFixture(fixture);
		fixture = nullptr;
	}

	if (world != nullptr && body != nullptr)
	{
		world->DestroyBody(body);
		body = nullptr;
	}
}

void PhysicsComponent::addForce(glm::vec2 force) {body->ApplyForce({ force.x, force.y }, body->GetWorldCenter(), true);}
void PhysicsComponent::addImpulse(glm::vec2 force) {body->ApplyLinearImpulse({ force.x, force.y }, body->GetWorldCenter(), true);}

void PhysicsComponent::addRevoluteJoint(PhysicsComponent* other, b2Vec2 localAnchorA, b2Vec2 localAnchorB)
{
	b2RevoluteJointDef* jointDef = new b2RevoluteJointDef;
	jointDef->bodyA = this->body;
	jointDef->bodyB = other->body;
	jointDef->collideConnected = false;
	jointDef->localAnchorA = localAnchorA;
	jointDef->localAnchorB = localAnchorB;
	jointDefs.push_back(jointDef);
}

void PhysicsComponent::addRopeJoint(PhysicsComponent* other, b2Vec2 localAnchorA, b2Vec2 localAnchorB, float maxLength)
{
	b2RopeJointDef* jointDef = new b2RopeJointDef;
	jointDef->bodyA = this->body;
	jointDef->bodyB = other->body;
	jointDef->collideConnected = true;
	jointDef->localAnchorA = localAnchorA;
	jointDef->localAnchorB = localAnchorB;
	jointDef->maxLength = maxLength / PhysicsEngine::physicsScale;
	jointDefs.push_back(jointDef);
}

void PhysicsComponent::setLinearVelocity(glm::vec2 velocity)
{
	if (velocity != glm::vec2(0, 0))
		body->SetAwake(true);
	body->SetLinearVelocity({velocity.x, velocity.y});
}

glm::vec2 PhysicsComponent::getLinearVelocity()
{
	b2Vec2 velocity = body->GetLinearVelocity();
	return { velocity.x, velocity.y };
}

void PhysicsComponent::moveTo(glm::vec2 position)
{
	glm::vec2 delta = glm::vec2(position.x / PhysicsEngine::physicsScale, position.y / PhysicsEngine::physicsScale) - glm::vec2(body->GetPosition().x, body->GetPosition().y);
	setLinearVelocity(delta * (1 / PhysicsEngine::timeStep));
}

void PhysicsComponent::fixRotation(bool flag)
{
	fixture->GetBody()->SetFixedRotation(flag);
	rotationIsFixed = flag;
}

void PhysicsComponent::setSensor() {fixture->SetSensor(true);}

void PhysicsComponent::setGravityScale(float32 scale) {body->SetGravityScale(scale);}

void PhysicsComponent::initBox(b2BodyType type, glm::vec2 size, float density, glm::vec2 relPos)
{
	assert(body == nullptr);
	this->type = PcType::pcBox;
	this->size = size;
	shapeType = b2Shape::Type::e_polygon;
	bodyType = type;
	
	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position = b2Vec2((gameObject->getPosition().x + relPos.x) / PhysicsEngine::physicsScale, (gameObject->getPosition().y + relPos.y) / PhysicsEngine::physicsScale);
	body = world->CreateBody(&bodyDef);

	polygon = new b2PolygonShape();
	polygon->SetAsBox(size.x / PhysicsEngine::physicsScale, size.y / PhysicsEngine::physicsScale, { 0, 0 }, 0);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = polygon;
	fixtureDef.density = density;
	fixture = body->CreateFixture(&fixtureDef);

	PhysicsEngine::instance->registerPhysicsComponent(this);
}

void PhysicsComponent::initCircle(b2BodyType type, float radius, float density, glm::vec2 relPos)
{
	assert(body == nullptr);
	this->type = PcType::pcCircle;
	this->radius = radius;
	shapeType = b2Shape::Type::e_circle;
	bodyType = type;

	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position = b2Vec2((gameObject->getPosition().x + relPos.x) / PhysicsEngine::physicsScale, (gameObject->getPosition().y + relPos.y) / PhysicsEngine::physicsScale);
	body = world->CreateBody(&bodyDef);

	circle = new b2CircleShape();
	circle->m_radius = radius / PhysicsEngine::physicsScale;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = circle;
	fixtureDef.density = density;
	fixture = body->CreateFixture(&fixtureDef);

	PhysicsEngine::instance->registerPhysicsComponent(this);
}

glm::vec2 PhysicsComponent::getCollisionNormal()
{
	auto normal = body->GetContactList()->contact->GetManifold()->localNormal;
	return { normal.x, normal.y };
}

b2Body* PhysicsComponent::getBody() { return body; }
float PhysicsComponent::getRadius() { return radius; }
glm::vec2 PhysicsComponent::getSize() { return size; }

void PhysicsComponent::update(float deltaTime)
{
	for each (b2JointDef * jointDef in jointDefs)
	{
		joints.push_back(world->CreateJoint(jointDef));
		delete jointDef;
	}
	jointDefs.clear();
}

void PhysicsComponent::read(rapidjson::Value& component)
{
	int bt = component["bodyType"].GetInt();
	b2BodyType bodyType;
	switch (bt)
	{
	case 0:
		bodyType = b2_staticBody;
		break;
	case 1:
		bodyType = b2_kinematicBody;
		break;
	case 2:
		bodyType = b2_dynamicBody;
		break;
	default:
		throw "Unknown b2 body type in physicsComponent";
		break;
	}
	float density = component["density"].GetFloat();
	
	if(component.HasMember("relativePosition")) 
		relativePosition = { component["relativePosition"]["x"].GetFloat(), component["relativePosition"]["y"].GetFloat() };

	PcType type = (PcType) component["pcType"].GetInt();
	switch (type)
	{
	case PcType::pcBox:
		glm::vec2 size = glm::vec2(component["size"]["x"].GetFloat(), component["size"]["y"].GetFloat());
		initBox(bodyType, size, density, relativePosition);
		break;
	case PcType::pcCircle:
	{
		float radius = component["radius"].GetFloat();
		initCircle(bodyType, radius, density, relativePosition);
		break;
	}
	default:
		throw "PhysicsComponent shape not recognized";
		break;
	}
	if(component.HasMember("fixRotation"))
		fixRotation(component["fixRotation"].GetBool());
	if (component.HasMember("gravityScale"))
		setGravityScale(component["gravityScale"].GetFloat());
}

void PhysicsComponent::write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a)
{
	rapidjson::Value componenType((int)Comp::PhysicsC);
	c.AddMember("componentType", componenType, a);

	rapidjson::Value bt;
	switch (bodyType)
	{
	case b2_staticBody:
		bt = 0;
		break;
	case b2_kinematicBody:
		bt = 1;
		break;
	case b2_dynamicBody:
		bt = 2;
		break;
	default:
		throw "bodyType unkown";
		break;
	}
	c.AddMember("bodyType", bt, a);

	rapidjson::Value density(fixture->GetDensity());
	c.AddMember("density", density, a);

	rapidjson::Value relPos(rapidjson::kObjectType);
	{
		rapidjson::Value x(relativePosition.x);
		relPos.AddMember("x", x, a);
		rapidjson::Value y(relativePosition.y);
		relPos.AddMember("y", y, a);
	}
	c.AddMember("relativePosition", relPos, a);

	rapidjson::Value gravityScale(body->GetGravityScale());
	c.AddMember("gravityScale", gravityScale, a);

	rapidjson::Value fixRot(rotationIsFixed);
	c.AddMember("fixRotation", fixRot, a);


	rapidjson::Value pcType((int)type);
	c.AddMember("pcType", pcType, a);
	switch (type)
	{
	case PcType::pcBox:
		{
			rapidjson::Value jSize(rapidjson::kObjectType);
			{
				rapidjson::Value x(size.x);
				jSize.AddMember("x", x, a);
				rapidjson::Value y(size.y);
				jSize.AddMember("y", y, a);
			}
			c.AddMember("size", jSize, a);
		}
		break;
	case PcType::pcCircle:
		{
			rapidjson::Value radius(radius);
			c.AddMember("radius", radius, a);
		}
		break;
	default:
		break;
	}
}
