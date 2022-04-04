// Game Programming 2020
// Exam Project: John The Fisherman

#pragma once
#include "FishermanGame.hpp"
#include "Room.hpp"
#include "PlayerController.hpp"
#include "SoundEngine.hpp"
#include "GlobalSpawner.hpp"

const glm::vec2 FishermanGame::windowSize(960, 700);
FishermanGame* FishermanGame::instance = nullptr;

FishermanGame::FishermanGame()
{
	instance = this;

	r.setWindowSize(windowSize);
	r.init()
		.withSdlInitFlags(SDL_INIT_EVERYTHING)
		.withSdlWindowFlags(SDL_WINDOW_OPENGL);

	init();

	r.frameUpdate = [&](float deltaTime) { update(deltaTime); };
	r.frameRender = [&]() { render(); };
	r.keyEvent = [&](SDL_Event& event) { onKey(event); };
	r.startEventLoop();
}

void FishermanGame::init()
{
	camera.reset();
	globalGameObjects.clear();
	roomGameObjects.clear();
	soundEngine.reset();
	roomController.reset();
	spriteAtlas.reset();
	player = nullptr;
	physicsEngine.reset();

	currentState = GameState::Running;

	spriteAtlas = sre::SpriteAtlas::create("./resources/fisherman.json", "./resources/fisherman.png");

	soundEngine = std::make_shared<SoundEngine>();
	soundEngine->setMusic("placeholder.mp3");
	soundEngine->playMusic();

	physicsEngine = std::make_shared<PhysicsEngine>();
	
	player = GlobalSpawner::spawnPlayer();

	camera = std::make_shared<Camera>();

	roomController = std::make_shared<RoomController>(camera, player);
}

void FishermanGame::update(float deltaTime)
{
	if (restart) {
		init();
		restart = false;
		return;
	}

	std::function<void()> cleanRoom;

	if (currentState == GameState::Running)
	{
		physicsEngine->update();
		auto cleanRoom = roomController->update();
		camera->update(deltaTime);
		updateObjects(deltaTime, &roomGameObjects);
		updateObjects(deltaTime, &globalGameObjects);
		cleanRoom();
	}
	else  if (currentState == GameState::CameraShift)
	{
		cleanRoom = roomController->update();
		camera->update(deltaTime);
		cleanRoom();
	}
}

void FishermanGame::updateObjects(float deltaTime, std::vector<std::shared_ptr<GameObject>>* gos)
{
	for (int i = 0; i < gos->size(); i++)
	{
		(*gos)[i]->update(deltaTime);

		// Removes gameobjects if they are marked for removal in the game
		if ((*gos)[i]->shouldRemoveObject())
		{
			gos->erase(gos->begin() + i);
			i--; // Need to do this, to not skip the next object.
		}
	}
}

void FishermanGame::render()
{
	auto gos = getAllGameObjects();
	auto renderPass = sre::RenderPass::create()
		.withCamera(camera->getCamera())
		.build();

	auto spriteBatchBuilder = sre::SpriteBatch::create();
	for (auto& obj : gos)
		obj->render(spriteBatchBuilder);

	auto spriteBuilder = spriteBatchBuilder.build();
	renderPass.draw(spriteBuilder);

	for (auto& obj : gos)
		obj->renderDrawing(&renderPass);

	physicsEngine->renderDebug(&renderPass);
}

void FishermanGame::onKey(SDL_Event& event)
{
	if (currentState == GameState::Running)
	{
		for each (auto go in getAllGameObjects())
		{
			for each (auto comp in go->getComponents())
			{
				comp->onKey(event);
			}
		}
	}
	else player->getComponent<PlayerController>()->onKey(event);

	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_z:
			physicsEngine->swapDebugDraw();
			break;
		case SDLK_r:
			restart = true;
			break;
		case SDLK_m:
			if (soundEngine->isPaused()) soundEngine->resumeMusic();
			else if (soundEngine->isPlaying()) soundEngine->pauseMusic();
			break;
		case SDLK_p:
			if (currentState == GameState::Paused)
			{
				if (soundEngine->isPaused()) soundEngine->resumeMusic();
				currentState = GameState::Running;
				break;
			}

			if (currentState == GameState::Running)
			{
				if (soundEngine->isPlaying()) soundEngine->pauseMusic();
				currentState = GameState::Paused;
				break;
			}
			break;
		default:
			break;
		}
	}
}

std::shared_ptr<GameObject> FishermanGame::createGlobalGameObject()
{
	auto obj = std::shared_ptr<GameObject>(new GameObject());
	globalGameObjects.push_back(obj);
	return obj;
}

void FishermanGame::clearGlobalGameObjectsExceptPlayer()
{
	for each (auto go in globalGameObjects)
	{
		if (go->getName() == "player") continue;
		go->removeObject();
	}
}

void FishermanGame::addRoomGameObject(std::shared_ptr<GameObject> go) { roomGameObjects.push_back(go); }
void FishermanGame::addRoomGameObjects(std::vector<std::shared_ptr<GameObject>> gos) { roomGameObjects.insert(roomGameObjects.begin(), gos.begin(), gos.end()); }
std::vector<std::shared_ptr<GameObject>> FishermanGame::getRoomGameObjects() {return roomGameObjects;}

void FishermanGame::clearRoomGameObjects()
{
	for each (auto go in roomGameObjects)
	{
		go->removeObject();
	}
}

std::vector<std::shared_ptr<GameObject>> FishermanGame::getAllGameObjects()
{
	std::vector<std::shared_ptr<GameObject>> gos;
	//gos.reserve(roomGameObjects.size() + globalGameObjects.size()); // preallocate memory
	gos.insert(gos.end(), roomGameObjects.begin(), roomGameObjects.end());
	gos.insert(gos.end(), globalGameObjects.begin(), globalGameObjects.end());
	return gos;
}

std::shared_ptr<GameObject> FishermanGame::getPlayer() { return player; }
std::shared_ptr<sre::SpriteAtlas> FishermanGame::getSpriteAtlas() { return spriteAtlas; }

float FishermanGame::getTopScreenCoord() {return windowSize.y + camera->getCurrentScreenCenter().y;}
float FishermanGame::getBottomScreenCoord() {return camera->getCurrentScreenCenter().y;}
float FishermanGame::getLeftScreenCoord() {return (-windowSize.x / 2) + camera->getCurrentScreenCenter().x;}
float FishermanGame::getRightScreenCoord() {return (windowSize.x / 2) + camera->getCurrentScreenCenter().x;}
GameState FishermanGame::getCurrentState() {return currentState;}
void FishermanGame::setCurrentState(GameState newState) {currentState = newState;}