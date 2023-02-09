#pragma once

#include "TerrainTile.h"

#include <glm/glm.hpp>
#include <JCEngine/SpriteBatch.h>
#include <JCEngine/GLTexture.h>
#include <JCEngine/InputManager.h>
#include <JCEngine\Camera2D.h>

enum class ActorType { ZOMBIE, HUMAN, PLAYER };

class Human;
class Player;

class Actor
{
public:
	Actor();
	~Actor();

	void draw(JCEngine::SpriteBatch& spriteBatch);

	//depends on actors around it - will need some processing in main game
	virtual void move(const std::vector<std::vector<TerrainTile>>& levelData, std::vector<Actor*>& actors, Player* player, float deltaTime) {};

	//returns true if dead
	bool applyDamage(int damage);

	bool collideWithLevel(const std::vector<std::vector<TerrainTile>>& levelData);
	bool collideWithActor(Actor* actor);
	bool isPlayer() { return _type == ActorType::PLAYER; };
	bool isHuman() { return _type == ActorType::HUMAN; };

	//getters
	glm::vec2 getPosition() { return _position; };

	//setters
	void setPosition(glm::vec2 position) { _position = position; };
	void setDirection(glm::vec2 direction) { _direction = direction; _directionFacing = direction; };

protected:
	float _speed = 20;
	glm::vec2 _position;
	glm::vec2 _size;
	glm::vec4 _uv;
	JCEngine::ColorRGBA8 _color;
	JCEngine::GLTexture _texture;
	ActorType _type;
	glm::vec2 _direction;
	glm::vec2 _directionFacing;
	int _frames;
	int _health;

	void collideWithTile(glm::vec2 tilePosition);
};

