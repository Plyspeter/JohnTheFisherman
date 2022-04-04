// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "Box2D/Dynamics/b2Body.h"
#include <Box2D/Dynamics/Joints/b2Joint.h>
#include "Component.hpp"

enum class PcType {
	pcBox = 0,
	pcCircle = 1
};

class PhysicsComponent : public Component
{
public:
	explicit PhysicsComponent(GameObject* gameObject);
	virtual ~PhysicsComponent();

	void addForce(glm::vec2 force);
	void addImpulse(glm::vec2 force);
	void addRevoluteJoint(PhysicsComponent* other, b2Vec2 localAnchorA, b2Vec2 localAnchorB);
	void addRopeJoint(PhysicsComponent* other, b2Vec2 localAnchorA, b2Vec2 localAnchorB, float maxLength);
	void setLinearVelocity(glm::vec2 velocity);
	glm::vec2 getLinearVelocity();

	void moveTo(glm::vec2 position);
	void fixRotation(bool flag);
	void setSensor();

	void setGravityScale(float32 scale);

	void initBox(b2BodyType type, glm::vec2 size, float density, glm::vec2 relPos = { 0,0 });
	void initCircle(b2BodyType type, float radius, float density, glm::vec2 relPos = { 0,0 });

	glm::vec2 getCollisionNormal();

	void read(rapidjson::Value& component) override;
	void write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a) override;
	void update(float deltaTime) override;
	glm::vec2 getSize();
	b2Body* getBody();

	float getRadius();

private:
	PcType type;
	glm::vec2 size;
	float radius;
	b2PolygonShape* polygon = nullptr;
	b2CircleShape* circle = nullptr;
	
	b2Body* body = nullptr;
	b2Shape::Type shapeType;
	b2Fixture* fixture = nullptr;
	b2BodyType bodyType;
	std::vector<b2Joint*> joints;
	std::vector<b2JointDef*> jointDefs;
	glm::vec2 relativePosition = { 0,0 };
	bool rotationIsFixed = false;
	bool isHookable = false;

	b2World* world = nullptr;
	
	friend class PhysicsEngine;
};