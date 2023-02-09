#include "MainGame.h"

#include <JCEngine\JCEngine.h>
#include <JCEngine\ResourceManager.h>
#include <math.h>
#include <iostream>
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
#include <JCEngine\GameErrors.h>
#include <Windows.h>

const int MAP_WIDTH = 30;
const int MAP_HEIGHT = 30;


MainGame::MainGame(int screenWidth, int screenHeight) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_state = GameState::PLAY;
	_currentLevel = 0;
	_cameraState = CameraState::PLAYER;

	//init game engine
	JCEngine::init();

	//create window, set color to gray
	_window.create("MapGenerator", screenWidth, screenHeight, 0);
	_window.setColor(0.8f, 0.8f, 0.8f, 1.0f);

	//construct shader program and init
	initShaders();

	//init audio
	_audioEngine.init();

	//init camera
	_camera.Init(screenWidth, screenHeight);
	_hudCamera.Init(screenWidth, screenHeight);
	_hudCamera.SetPosition(glm::vec2(screenWidth / 2, screenHeight / 2));

	//init sprite batchers
	_spriteBatch.init();
	_hudSpriteBatch.init();

	//init sprite font
	_spriteFont = new JCEngine::SpriteFont("Fonts/chintzy.ttf", 32);

	//init fps limiter
	_fpsLimiter.init(60.0f);
}

MainGame::~MainGame() {

}

void MainGame::run() {
	initMap();

	gameLoop();
}

void MainGame::initSystems() {
	//initializing most things in the constructor for now
}

void MainGame::initMap() {
	_map = Map(MAP_WIDTH, MAP_HEIGHT);

	_player = Player(glm::vec2(0.0f, 0.0f), &_camera);

	_camera.SetPosition(_player.getPosition());

	//create humans with random positions
	std::mt19937 randomEngine(time(nullptr));
	std::uniform_int_distribution<int> randomx(5, MAP_WIDTH - 5);
	std::uniform_int_distribution<int> randomy(5, MAP_HEIGHT - 5);

	int minSize = MAP_HEIGHT < MAP_WIDTH ? MAP_HEIGHT : MAP_WIDTH;
	int numHumans = minSize / 5;
	glm::vec2 spriteSize(64.0f, 64.0f);

	for (int i = 0; i < numHumans; i++) {
		int xCoord = randomx(randomEngine);
		int yCoord = randomy(randomEngine);

		while (_map.getMapData()[yCoord][xCoord].getTerrainType() != TerrainType::GRASS) {
			xCoord = randomx(randomEngine);
			yCoord = randomy(randomEngine);
		}

		_actors.push_back(new Human(glm::vec2(xCoord * 64.0f, yCoord * 64.0f), spriteSize));
	}
}

void MainGame::initShaders() {
	_shaderProgram.compileShaders("Shaders/ColorShading.vert.txt", "Shaders/ColorShading.frag.txt");
	_shaderProgram.addAttribute("vertexPosition");
	_shaderProgram.addAttribute("vertexColor");
	_shaderProgram.addAttribute("vertexUV");
	_shaderProgram.linkShaders();
}

void MainGame::gameLoop() {
	const int MAX_STEPS = 6;
	const float DESIRED_FPS = 60.0f;
	const float MILLISECONDS_PER_SECOND = 1000;
	const float DESIRED_FRAME_TIME = MILLISECONDS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	float currentTicks = SDL_GetTicks();

	while (_state == GameState::PLAY) {
		_fpsLimiter.start();

		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - currentTicks;
		currentTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAME_TIME;

		_inputManager.update();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_STEPS) {
			float deltaTime = MAX_DELTA_TIME < totalDeltaTime ? MAX_DELTA_TIME : totalDeltaTime;

			processInput(deltaTime);

			//_camera.SetPosition(_player.getPosition());

			updateAgents(deltaTime);

			_particleEngine.update(deltaTime);
			totalDeltaTime -= deltaTime;
			i++;
		}

		draw();

		_fpsLimiter.stop();
	}
}

void MainGame::updateAgents(float deltaTime) {
	if (_cameraState == CameraState::PLAYER) {
		glm::vec2 spriteSize(64.0f, 64.0f);

		for (int i = 0; i < _actors.size(); i++) {
			_actors[i]->move(_map.getMapData(), _actors, &_player, deltaTime);
		}
	}
}

void MainGame::processInput(float deltaTime) {
	SDL_Event ev;

	const float CAMERA_SPEED = 5.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&ev)) {
		switch (ev.type)
		{
		case SDL_QUIT:
			_state = GameState::EXIT;
			return;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(ev.motion.x, ev.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.keyPress(ev.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.keyRelease(ev.button.button);
			break;
		case SDL_KEYDOWN:
			_inputManager.keyPress(ev.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.keyRelease(ev.key.keysym.sym);
			break;
		default:
			break;
		}
	}

	if (_cameraState == CameraState::PLAYER) {
		glm::vec2 movementDirection = glm::vec2(0.0f, 0.0f);

		if (_inputManager.isKeyDown(SDLK_w))
			movementDirection.y += 1;
		if (_inputManager.isKeyDown(SDLK_s))
			movementDirection.y -= 1;
		if (_inputManager.isKeyDown(SDLK_a))
			movementDirection.x -= 1;
		if (_inputManager.isKeyDown(SDLK_d))
			movementDirection.x += 1;
		if (_inputManager.isKeyDown(SDLK_q))
			_camera.SetScale(_camera.GetScale() - SCALE_SPEED);
		if (_inputManager.isKeyDown(SDLK_e))
			_camera.SetScale(_camera.GetScale() + SCALE_SPEED);

		//only move camera and player if it is necessary
		glm::normalize(movementDirection);
		if (movementDirection.x != 0 || movementDirection.y != 0) {
			_player.setDirection(movementDirection);
			_player.move(_map.getMapData(), _actors, &_player, deltaTime);
			//_player.collideWithLevel(_levels[_currentLevel]->getLevelData());
		}

		_player.setDirectionFacing(&_inputManager);

		_camera.SetPosition(_player.getPosition());

		_camera.Update();

		if (_inputManager.isKeyPressed(SDLK_m))
			toggleMapView();
	}
	else if (_cameraState == CameraState::MAP) {
		if (_inputManager.isKeyPressed(SDLK_m))
			toggleMapView();
	}
	
}

void MainGame::toggleMapView() {
	if (_cameraState == CameraState::MAP) {
		_cameraState = CameraState::PLAYER;

		_camera.SetPosition(_player.getPosition());
		_camera.SetScale(0.8f);

		_camera.Update();
	}
	else if (_cameraState == CameraState::PLAYER) {
		_cameraState = CameraState::MAP;

		int maxSize = MAP_HEIGHT > MAP_WIDTH ? MAP_HEIGHT : MAP_WIDTH;

		_camera.SetScale(12.0f / (float)maxSize);

		_camera.SetPosition(glm::vec2((MAP_WIDTH * 64.0f) / 2.0f, (MAP_HEIGHT * 64.0f) / 2.0f));

		_camera.Update();
	}
}

void MainGame::draw() {
	glClearDepth(1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shaderProgram.use();
	glActiveTexture(GL_TEXTURE0);

	//make sure shader uses texture 0
	GLint textureLocation = _shaderProgram.getUniformLocation("spriteTexture");
	glUniform1i(textureLocation, 0);

	//set camera matrix
	GLuint pLocation = _shaderProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.GetCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_map.draw();

	_spriteBatch.begin();

	const glm::vec2 actorDimensions = glm::vec2(64.0f, 64.0f);

	_player.draw(_spriteBatch);

	for (int i = 0; i < _actors.size(); i++) {
		if (_camera.isBoxInView(_actors[i]->getPosition(), actorDimensions)) {
			_actors[i]->draw(_spriteBatch);
		}
	}

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_shaderProgram.unuse();

	_window.swapBuffer();
}