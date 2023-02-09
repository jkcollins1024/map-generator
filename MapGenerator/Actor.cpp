#include "Actor.h"

#include "TerrainTile.h"

#include <iostream>
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
#include <SDL/SDL.h>
#include <JCEngine\ResourceManager.h>

Actor::Actor() {
}

Actor::~Actor() {

}

void Actor::draw(JCEngine::SpriteBatch& spriteBatch) {
	glm::vec4 positionRect(_position.x, _position.y, _size.x, _size.y);
	spriteBatch.draw(positionRect, _uv, _texture.id, 0, _color, _directionFacing);
}

bool Actor::applyDamage(int damage) {
	_health -= damage;

	return _health <= 0;
}

bool Actor::collideWithLevel(const std::vector<std::vector<TerrainTile>>& levelData) {
	std::vector<glm::vec2> collideTilePositions;
	bool didCollide = false;

	//check four corners
	glm::vec2 cornerPosition1 = glm::vec2(floor(_position.x / 64.0f), floor(_position.y / 64.0f));
	glm::vec2 cornerPosition2 = glm::vec2(floor((_position.x + _size.x) / 64.0f), floor(_position.y / 64.0f));
	glm::vec2 cornerPosition3 = glm::vec2(floor(_position.x / 64.0f), floor((_position.y + _size.y) / 64.0f));
	glm::vec2 cornerPosition4 = glm::vec2(floor((_position.x + _size.x) / 64.0f), floor((_position.y + _size.y) / 64.0f));

	//if (cornerPosition1.x < 0 || cornerPosition4.x >= levelData[0].size() ||
	//	cornerPosition1.y < 0 || cornerPosition4.y >= levelData.size()) {
	//	//return false;
	//}

	if (cornerPosition1.x < 0) {
		glm::vec2 cornerPosition = (cornerPosition1 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
		didCollide = true;
	}

	if (cornerPosition1.y < 0) {
		glm::vec2 cornerPosition = (cornerPosition1 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
		didCollide = true;
	}

	if (cornerPosition4.x >= levelData[0].size()) {
		glm::vec2 cornerPosition = (cornerPosition4 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
		didCollide = true;
	}

	if (cornerPosition4.y >= levelData.size()) {
		glm::vec2 cornerPosition = (cornerPosition4 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
		didCollide = true;
	}

	if (collideTilePositions.size() > 0) {
		for (int i = 0; i < collideTilePositions.size(); i++) {
			collideWithTile(collideTilePositions[i]);
		}

		return didCollide;
	}

	if (!levelData[cornerPosition1.y][cornerPosition1.x].getIsPassable()) {
		//glm::vec2 cornerPosition = glm::vec2(cornerPosition1.x * 64.0f + 32.0f, cornerPosition1.y * 64.0f + 32.0f);
		glm::vec2 cornerPosition = (cornerPosition1 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
		didCollide = true;
	}

	if (!levelData[cornerPosition2.y][cornerPosition2.x].getIsPassable()) {
		//glm::vec2 cornerPosition = glm::vec2(cornerPosition2.x * 64.0f + 32.0f, cornerPosition2.y * 64.0f + 32.0f);
		glm::vec2 cornerPosition = (cornerPosition2 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
		didCollide = true;
	}

	if (!levelData[cornerPosition3.y][cornerPosition3.x].getIsPassable()) {
		//glm::vec2 cornerPosition = glm::vec2(cornerPosition3.x * 64.0f + 32.0f, cornerPosition3.y * 64.0f + 32.0f);
		glm::vec2 cornerPosition = (cornerPosition3 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
		didCollide = true;
	}

	if (!levelData[cornerPosition4.y][cornerPosition4.x].getIsPassable()) {
		//glm::vec2 cornerPosition = glm::vec2(cornerPosition4.x * 64.0f + 32.0f, cornerPosition4.y * 64.0f + 32.0f);
		glm::vec2 cornerPosition = (cornerPosition4 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
		didCollide = true;
	}

	for (int i = 0; i < collideTilePositions.size(); i++) {
		collideWithTile(collideTilePositions[i]);
	}

	return didCollide;
}

void Actor::collideWithTile(glm::vec2 tilePosition) {
	const float MIN_DISTANCE = 64.0f;

	glm::vec2 centerPlayerPosition = _position + glm::vec2(32.0f);
	glm::vec2 distVec = centerPlayerPosition - tilePosition;

	float xdepth = MIN_DISTANCE - abs(distVec.x);
	float ydepth = MIN_DISTANCE - abs(distVec.y);

	if (xdepth > 0 || ydepth > 0) {
		if (xdepth < ydepth) {
			if (distVec.x > 0)
				_position.x += xdepth;
			else
				_position.x -= xdepth;
		}
		else {
			if (distVec.y > 0)
				_position.y += ydepth;
			else
				_position.y -= ydepth;
		}
	}
}

bool Actor::collideWithActor(Actor* actor) {
	const float MIN_DISTANCE = 64.0f;

	glm::vec2 currentCenter = _position + glm::vec2(32.0f);
	glm::vec2 otherCenter = actor->getPosition() + glm::vec2(32.0f);

	glm::vec2 distVec = currentCenter - otherCenter;

	float depth = glm::length(distVec);

	glm::vec2 collisionDepthVec = glm::normalize(distVec);

	if (depth == 0) {
		collisionDepthVec = glm::normalize(glm::vec2(1.0f, 1.0f));
	}

	if (depth < MIN_DISTANCE) {
		collisionDepthVec = collisionDepthVec * (MIN_DISTANCE - depth);
		actor->setPosition(actor->getPosition() - (collisionDepthVec / 2.0f));
		setPosition(_position + (collisionDepthVec / 2.0f));
		return true;
	}
	return false;
}