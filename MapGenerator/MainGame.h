#pragma once

#include <JCEngine/SpriteBatch.h>
#include <JCEngine/Window.h>
#include <JCEngine/GLSLProgram.h>
#include <JCEngine/InputManager.h>
#include <JCEngine/Camera2D.h>
#include <JCEngine/SpriteFont.h>
#include <JCEngine\AudioEngine.h>
#include <JCEngine\ParticleEngine2D.h>
#include <JCEngine/ParticleBatch2D.h>
#include <JCEngine\Timing.h>
#include <glm/glm.hpp>

#include "Actor.h"
#include "Player.h"
#include "Human.h"
#include "Map.h"


enum class GameState {
	PLAY,
	EXIT
};

enum class CameraState {
	PLAYER,
	MAP
};

class MainGame
{
public:
	MainGame(int screenWidth, int screenHeight);
	~MainGame();

	void run();

private:
	int _screenWidth;
	int _screenHeight;
	GameState _state;
	int _currentLevel;
	float _fullMapCameraScale;
	glm::vec2 _fullMapCameraPosition;
	float _playerCameraScale;
	CameraState _cameraState;

	JCEngine::Window _window;
	JCEngine::GLSLProgram _shaderProgram;
	JCEngine::Camera2D _camera;
	JCEngine::SpriteBatch _spriteBatch;
	JCEngine::InputManager _inputManager;
	JCEngine::FPSLimiter _fpsLimiter;
	JCEngine::AudioEngine _audioEngine;
	JCEngine::ParticleEngine2D _particleEngine;
	JCEngine::ParticleBatch2D* _bloodParticleBatch;

	JCEngine::SpriteFont* _spriteFont;
	JCEngine::SpriteBatch _hudSpriteBatch;
	JCEngine::Camera2D _hudCamera;

	std::vector<Actor*> _actors;
	//std::vector<Bullet*> _bullets;
	Map _map;
	Player _player;

	//initializes game systems
	void initSystems();

	//sets up the map
	void initMap();

	//initializes the shaders we need
	void initShaders();

	//sets input from users
	void processInput(float deltaTime);

	//loop to execute game logic
	void gameLoop();

	void updateAgents(float deltaTime);

	void toggleMapView();

	//draws the game
	void draw();
};