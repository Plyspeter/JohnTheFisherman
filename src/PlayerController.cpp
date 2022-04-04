// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "PlayerController.hpp"
#include "PhysicsComponent.hpp"
#include "SpriteComponent.hpp"
#include "SpriteAnimationComponent.hpp"
#include "FishermanGame.hpp"
#include "Enemy.hpp"
#include "Room.hpp"
#include "Knife.hpp"
#include <SDL_events.h>
#include "SoundComponent.hpp"
#include "GlobalMacros.hpp"
#include "sre/Texture.hpp"
#include <imgui.h>

std::shared_ptr<sre::Texture> PlayerController::lifeTex = nullptr;
const glm::vec2 PlayerController::initialPosition = { -400, 400 };

PlayerController::PlayerController(GameObject* gameObject)
	: Component(gameObject)
{
	ASSERT(gameObject->getComponent<PhysicsComponent>().get() != nullptr, "PlayerController component missing PhysicsComponent in gameobject: " + gameObject->getName());
	ASSERT(gameObject->getComponent<SpriteAnimationComponent>().get() != nullptr, "PlayerController component missing SpriteAnimationComponent in gameobject: " + gameObject->getName());
	ASSERT(gameObject->getComponent<SpriteComponent>().get() != nullptr, "PlayerController component missing SpriteComponent in gameobject: " + gameObject->getName());
	ASSERT(gameObject->getComponent<SoundComponent>().get() != nullptr, "PlayerController component missing SoundComponent in gameobject: " + gameObject->getName());
}

void PlayerController::update(float deltaTime)
{
	auto phys = gameObject->getComponent<PhysicsComponent>();
	auto anim = gameObject->getComponent<SpriteAnimationComponent>();
	auto sprite = gameObject->getComponent<SpriteComponent>();
	
	checkIfGrounded();
	
	glm::vec2 movement{ 0, 0 };

	if (right)
	{
		sprite->setFlip(false);
		movement.x++;
	}
	else if (left)
	{
		sprite->setFlip(true);
		movement.x--;
	}

	if (attacking) {
		attackTime -= deltaTime;
		if (attackTime <= 0) {
			attacking = false;
			attackTime = 0.5f;
			if (hooking) {
				anim->setAnimation("Fish");
			}
		}
	}

	if (hurting) {
		hurtTime -= deltaTime;
		if (hurtTime <= 0) {
			hurting = false;
			b2Filter filter;
			filter.maskBits = 0xFFFF;
			gameObject->getComponent<PhysicsComponent>()->getBody()->GetFixtureList()->SetFilterData(filter);
			hurtTime = 1.5f;
		}
	}
	
	
	if (!hooking && !attacking && !hurting) {
		if (movement.x == 0 && movement.y == 0 && anim->getCurrentAnimation() != "Idle") {
			anim->setAnimation("Idle");
		}
		else if (!(movement.x == 0 && movement.y == 0) && anim->getCurrentAnimation() != "Walk") {
			anim->setAnimation("Walk");
		}
	}

	if (hooking && hook->shouldRemoveObject()) {
		hook = nullptr;
		hooking = false;
	}
	
	//physics
	phys->addImpulse(movement * acc);

	auto linearVelocity = phys->getLinearVelocity();
	float currentVelocity = linearVelocity.x;

	if (abs(currentVelocity) > maxVelocity)
	{
		linearVelocity.x = glm::sign(linearVelocity.x) * maxVelocity;
		phys->setLinearVelocity(linearVelocity);
	}
	
	//If game object falls below room he should take damage and be put back into room
	if (gameObject->getPosition().y < -50)
	{
		hurt();
		resetPosition();
	}
}

void PlayerController::resetPosition()
{
	gameObject->setPosition(initialPosition);
	auto body = gameObject->getComponent<PhysicsComponent>()->getBody();
	body->SetLinearVelocity({ 0,0 });
	body->SetTransform({ initialPosition.x / PhysicsEngine::physicsScale, initialPosition.y / PhysicsEngine::physicsScale } , 0);
}

void PlayerController::onKey(SDL_Event& event)
{
	bool running = FishermanGame::instance->getCurrentState() == GameState::Running;
	switch (event.key.keysym.sym)
	{
	case SDLK_RIGHT:
	case SDLK_d:
		right = event.type == SDL_KEYDOWN;
		movingRight = true;
		break;

	case SDLK_LEFT:
	case SDLK_a:
		left = event.type == SDL_KEYDOWN;
		movingRight = false;
		break;

	case SDLK_SPACE:
	case SDLK_w:
	case SDLK_UP:
		if (event.type == SDL_KEYDOWN && jumpup) jump();
		jumpup = event.type == SDL_KEYUP;
		break;

	case SDLK_e:
		if (!running) return;
		if (event.type == SDL_KEYDOWN && hookup) throwHook();
		hookup = event.type == SDL_KEYUP;
		break;

	case SDLK_f:
		if (!running) return;
		if (event.type == SDL_KEYDOWN && !attacking) attack();
		break;

	default:
		break;
	}
}

void PlayerController::jump()
{
	// Only allow the player to jump, if they are standing on an object, like a platform.
	if (isGrounded)
	{
		// Gives the player an upwords force
		gameObject->getComponent<PhysicsComponent>()->addImpulse({ 0, jumpHeight });
		gameObject->getComponent<SoundComponent>()->play("jump", 200);
	}
}

void PlayerController::checkIfGrounded()
{
	auto phys = gameObject->getComponent<PhysicsComponent>();
	auto center = phys->getBody()->GetWorldCenter();
	auto scale = PhysicsEngine::physicsScale;
	// Find the center of the players physics box
	b2Vec2 from0 = b2Vec2(center.x - (phys->getSize().x) / scale , center.y);
	b2Vec2 from1 = center;
	b2Vec2 from2 = b2Vec2(center.x + (phys->getSize().x) / scale, center.y);

	// Calculation of the distance from players center to just under the player.
	auto toY = center.y - (10 / PhysicsEngine::physicsScale) * 3.6f;
	b2Vec2 to0 = { from0.x, toY};
	b2Vec2 to1 = { from1.x, toY };
	b2Vec2 to2 = { from2.x, toY };

	// By setting isGrounded to false, only the response from the raycast can flip this value.
	isGrounded = false;

	// Sends out a raycast to check if there is ground under the player.
	// If ground is found ReportFixture() is called.
	PhysicsEngine::instance->world->RayCast(this, from0, to0);
	PhysicsEngine::instance->world->RayCast(this, from1, to1);
	PhysicsEngine::instance->world->RayCast(this, from2, to2);
}

void PlayerController::throwHook()
{
	// If there is no hook, create one:	
	if (hook == nullptr)
	{
		gameObject->getComponent<SoundComponent>()->play("swing", 30);
		hooking = true;
		gameObject->getComponent<SpriteAnimationComponent>()->setAnimation("Fish");
		hook = FishermanGame::instance->createGlobalGameObject();
		hook->addComponent<Hook>();
		auto spriteComp = hook->addComponent<SpriteComponent>();
		auto sprite = FishermanGame::instance->getSpriteAtlas()->get("ball.png");
		glm::vec2 scale = { 0.02, 0.02 };
		spriteComp->setSprite("ball.png", scale);
	}
	// Else return the hook to the player
	else
	{
		if (!hook->getComponent<Hook>()->isReturning())
		{
			gameObject->getComponent<SoundComponent>()->play("swing", 30);
			hook->getComponent<Hook>()->returnHook();
		}
	}
}

void PlayerController::attack()
{
	gameObject->getComponent<SoundComponent>()->play("knife", 128);
	auto anim = gameObject->getComponent<SpriteAnimationComponent>();
	anim->setAnimation("Attack");
	attacking = true;
	FishermanGame::instance->createGlobalGameObject()->addComponent<Knife>();
}

float32 PlayerController::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
	// If a reponse is send to the method, then the player must be standing on an object.
	isGrounded = true;
	return 0;
}

void PlayerController::onCollisionStart(PhysicsComponent* component)
{
	if (component->getGameObject()->getName() == "health")
	{
		numOfLifes++;
		numOfLifes = numOfLifes > 3 ? 3 : numOfLifes;
		component->getGameObject()->removeObject();
		//Play life animation?
	}
}

bool PlayerController::hurt()
{
	if (hurting) return false;
	gameObject->getComponent<SoundComponent>()->play("hurt", 40);
	b2Filter filter;
	filter.maskBits = 0xFFFD;
	gameObject->getComponent<PhysicsComponent>()->getBody()->GetFixtureList()->SetFilterData(filter);
	if (numOfLifes == 0)
	{
		FishermanGame::instance->setCurrentState(GameState::GameOver);
	}
	numOfLifes--;
	auto anim = gameObject->getComponent<SpriteAnimationComponent>();
	anim->setAnimation("Hurt");
	hurting = true;
	return true;
}

bool PlayerController::isMovingRight()
{
	return this->movingRight;
}

void PlayerController::onGui()
{
	ImGui::SetNextWindowPos({ -5,-10 }, ImGuiCond_Always, { 0,0 });
	ImGui::SetNextWindowSize({ 200, 50 }, ImGuiCond_Always);
	auto flags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoMove;
	ImGui::PushStyleColor(ImGuiCol_WindowBg,
		ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_Border,
		ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	ImGui::Begin("", nullptr, flags);

	if (PlayerController::lifeTex.get() == nullptr)
	{
		PlayerController::lifeTex = sre::Texture::create()
			.withFile("resources/ui/life.png")
			.withFilterSampling(false)
			.build();
	}
	auto playId = lifeTex->getNativeTexturePtr();
	ImVec2 size(40, 40);
	ImVec2 uv0(0, 1);
	ImVec2 uv1(1, 0);
	for (int i = 0; i < numOfLifes; i++)
	{
		ImGui::SameLine(0, 3);
		ImGui::Image(playId, size, uv0, uv1);
	}
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

std::shared_ptr<GameObject> PlayerController::getHook()
{
	return hook;
}

void PlayerController::renderDrawing(sre::RenderPass* renderPass) {
	if (hook != nullptr) {
		std::vector<glm::vec3> fishingLinePoints;
		bool isFlipped = gameObject->getComponent<SpriteComponent>()->getFlip();
		auto x = isFlipped ? gameObject->getPosition().x - 40 : gameObject->getPosition().x + 40;
		fishingLinePoints.push_back({ x, gameObject->getPosition().y + 5, 0 });
		fishingLinePoints.push_back({ hook->getPosition(), 0 });
		renderPass->drawLines(fishingLinePoints, sre::Color(0,0,0,1));
	}
}