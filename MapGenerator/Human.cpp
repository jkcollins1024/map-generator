#include "Human.h"

#include <JCEngine/ResourceManager.h>
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Human::Human(glm::vec2 position, glm::vec2 size) {
	_position = position;
	_size = size;
	_type = ActorType::HUMAN;
	_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); //uv should just be standard for now

	_texture = JCEngine::ResourceManager::getTexture("Textures/human.png");

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randomDirection(-1.0f, 1.0f);

	//get a random direction for movement
	_direction = glm::vec2(randomDirection(randomEngine), randomDirection(randomEngine));
	_direction = glm::normalize(_direction);
	_directionFacing = _direction;

	//humans are blue
	_color = JCEngine::ColorRGBA8();
	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;

	//humans slower than zombies
	_speed = 0.4f;
	_frames = 0;
	_health = 20;
}

Human::~Human() {

}

void Human::move(const std::vector<std::vector<TerrainTile>>& levelData, std::vector<Actor*>& actors, Player* player, float deltaTime) {
	//normalize just in case it isn't already
	_direction = glm::normalize(_direction);

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randomRotation(-0.5f, 0.5f);

	if (_frames == 20) {
		_direction = glm::rotate(_direction, randomRotation(randomEngine));
		_directionFacing = _direction;
		_frames = 0;
	}
	else {
		_frames++;
	}

	_position += _direction * _speed * deltaTime;

	if (collideWithLevel(levelData)) {
		_direction = glm::rotate(_direction, randomRotation(randomEngine));
		_directionFacing = _direction;
	}

}