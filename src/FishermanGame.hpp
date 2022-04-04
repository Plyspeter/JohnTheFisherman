// Game Programming 2020
// Exam Project: John the Fisherman

#pragma once
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "PhysicsEngine.hpp"
#include "SoundEngine.hpp"
#include "RoomController.hpp"

enum class GameState {
	Running,
	GameOver,
	Paused,
	CameraShift
};

class FishermanGame {
public:
	FishermanGame();

	std::shared_ptr<GameObject> createGlobalGameObject();
	void addRoomGameObject(std::shared_ptr<GameObject> go);
	void addRoomGameObjects(std::vector<std::shared_ptr<GameObject>> gos);
	void clearGlobalGameObjectsExceptPlayer();
	void clearRoomGameObjects();
	std::vector<std::shared_ptr<GameObject>> getRoomGameObjects();
	std::vector<std::shared_ptr<GameObject>> getAllGameObjects();
	std::shared_ptr<GameObject> getPlayer();
	std::shared_ptr<sre::SpriteAtlas> getSpriteAtlas();

	float getTopScreenCoord();
	float getBottomScreenCoord();
	float getLeftScreenCoord();
	float getRightScreenCoord();

	GameState getCurrentState();
	void setCurrentState(GameState newState);

	static FishermanGame* instance;
	static const glm::vec2 windowSize;

private:
	sre::SDLRenderer r;
	std::shared_ptr<sre::SpriteAtlas> spriteAtlas;
	GameState currentState = GameState::Running;
	bool restart = false;

	std::shared_ptr<Camera> camera;
	std::shared_ptr<RoomController> roomController;
	std::shared_ptr<GameObject> player;
	std::shared_ptr<PhysicsEngine> physicsEngine;
	std::shared_ptr<SoundEngine> soundEngine;

	std::vector<std::shared_ptr<GameObject>> roomGameObjects;
	std::vector<std::shared_ptr<GameObject>> globalGameObjects;


	void init();
	void update(float time);
	void updateObjects(float deltaTime, std::vector<std::shared_ptr<GameObject>>* gos);
	void render();
	void onKey(SDL_Event& event);
};