#include "MainGame.h"
#include <JCEngine\JCEngine.h>

//#include "Actor.h"
//#include "Gun.h"

#include <JCEngine\ResourceManager.h>
#include <math.h>
#include <iostream>
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
#include <JCEngine\GameErrors.h>
#include <Windows.h>


MainGame::MainGame(int screenWidth, int screenHeight) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_state = GameState::PLAY;
	_currentLevel = 0;

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
	_map = Map(30, 30);

	_camera.SetScale(.4);

	_camera.SetPosition(glm::vec2(960.0f, 960.0f));

	_camera.Update();

	draw();
}

void MainGame::initShaders() {
	_shaderProgram.compileShaders("Shaders/ColorShading.vert.txt", "Shaders/ColorShading.frag.txt");
	_shaderProgram.addAttribute("vertexPosition");
	_shaderProgram.addAttribute("vertexColor");
	_shaderProgram.addAttribute("vertexUV");
	_shaderProgram.linkShaders();
}

void MainGame::gameLoop() {
	while (_state == GameState::PLAY) {

		_camera.SetScale(.4);

		_camera.SetPosition(glm::vec2(960.0f, 960.0f));

		_camera.Update();

		draw();
	}
}

void MainGame::checkVictory() {
}

void MainGame::updateAgents(float deltaTime) {
}

void MainGame::processInput(float deltaTime) {
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

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	drawHUD();

	glBindTexture(GL_TEXTURE_2D, 0);
	_shaderProgram.unuse();

	_window.swapBuffer();
}

void MainGame::drawHUD() {
}