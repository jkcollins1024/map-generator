#include "Player.h"

#include <JCEngine/ResourceManager.h>

Player::Player(glm::vec2 position, JCEngine::Camera2D* camera) {
	_type = ActorType::PLAYER;
	_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); //uv should just be standard for now

	_texture = JCEngine::ResourceManager::getTexture("Textures/player.png");

	_camera = camera;

	//player fastest
	_speed = 5.0f;

	//player is green
	_color = JCEngine::ColorRGBA8();
	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;

	//always initialize player to middle of the screen, since that's where we want player to stay
	_position = position;
	_size = glm::vec2(64.0f, 64.0f);
}

Player::~Player() {

}

void Player::move(const std::vector<std::vector<TerrainTile>>& levelData, std::vector<Actor*>& actors, Player* player, float deltaTime) {
	//normalize just in case it isn't already
	_direction = glm::normalize(_direction);

	setPosition(_position + (_direction * _speed * deltaTime));

	collideWithLevel(levelData);

	//return _position + (direction * _speed);
}

void Player::setDirectionFacing(JCEngine::InputManager* inputManager) {
	glm::vec2 mouseCoords = inputManager->getMouseCoords();
	mouseCoords = _camera->ConvertScreenToWorld(mouseCoords);

	glm::vec2 playerPosition = getPosition() + glm::vec2(16.0f);
	_directionFacing = glm::normalize(mouseCoords - playerPosition);
}