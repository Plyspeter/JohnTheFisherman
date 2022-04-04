// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "GlobalSpawner.hpp"
#include "FishermanGame.hpp"
#include "SpriteComponent.hpp"
#include "SpriteAnimationComponent.hpp"
#include "SoundComponent.hpp"
#include "PlayerController.hpp"

std::shared_ptr<GameObject> GlobalSpawner::spawnPlayer()
{
	//GameObject
	auto player = FishermanGame::instance->createGlobalGameObject();
	player->setName("player");
	player->setPosition(PlayerController::initialPosition);

	//Sprite
	auto spriteComp = player->addComponent<SpriteComponent>();
	spriteComp->setSprite("Fisherman_idle-0.png");

	//Animation
	auto anim = player->addComponent<SpriteAnimationComponent>();

	std::vector<std::string> idleAnim = { "Fisherman_idle-0.png","Fisherman_idle-1.png","Fisherman_idle-2.png","Fisherman_idle-3.png"};
	anim->setSprites("Idle", idleAnim);
	anim->setAnimationTime("Idle", 0.33f);

	std::vector<std::string> walkAnim = { "Fisherman_walk-0.png","Fisherman_walk-1.png", "Fisherman_walk-2.png", "Fisherman_walk-3.png", "Fisherman_walk-4.png", "Fisherman_walk-5.png"};
	anim->setSprites("Walk", walkAnim);
	anim->setAnimationTime("Walk", 0.33f);

	std::vector<std::string> fishAnim = { "Fisherman_fish-0.png","Fisherman_fish-1.png", "Fisherman_fish-2.png", "Fisherman_fish-3.png"};
	anim->setSprites("Fish", fishAnim);
	anim->setAnimationTime("Fish", 0.33f);

	std::vector<std::string> attackAnim = { "Fisherman_attack-0.png","Fisherman_attack-1.png", "Fisherman_attack-2.png", "Fisherman_attack-3.png","Fisherman_attack-4.png", "Fisherman_attack-5.png"};
	anim->setSprites("Attack", attackAnim);
	anim->setAnimationTime("Attack", 0.1f);

	std::vector<std::string> hurtAnim = { "Fisherman_hurt-0.png","Fisherman_hurt-1.png" };
	anim->setSprites("Hurt", hurtAnim);
	anim->setAnimationTime("Hurt", 0.2f);

	anim->setAnimationFlipOffsetMap({ std::pair<std::string, glm::vec2>("Fisherman_fish-0.png", { -30, 0 }),
								  std::pair<std::string, glm::vec2>("Fisherman_fish-1.png", { -30, 0 }),
								  std::pair<std::string, glm::vec2>("Fisherman_fish-2.png", { -30, 0 }),
								  std::pair<std::string, glm::vec2>("Fisherman_fish-3.png", { -30, 0 }),
								  std::pair<std::string, glm::vec2>("Fisherman_attack-3.png", { -25, 0 }),
								  std::pair<std::string, glm::vec2>("Fisherman_attack-4.png", { -23, 0 }),
								  std::pair<std::string, glm::vec2>("Fisherman_attack-5.png", { -10, 0 }) });

	anim->setAnimation("Idle");

	//Sound
	auto sc = player->addComponent<SoundComponent>();
	sc->addSound("jump", "jump.wav");
	sc->addSound("swing", "whoosh.wav");
	sc->addSound("knife", "knife.wav");
	sc->addSound("hurt", "hurt.wav");

	//Physics
	auto phys = player->addComponent<PhysicsComponent>();
	phys->initBox(b2_dynamicBody, { 14, 26 }, 1);
	phys->fixRotation(true);

	//Player
	auto pc = player->addComponent<PlayerController>();
	return player;
}