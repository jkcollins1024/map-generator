#pragma once

#include "Actor.h"

class Human : public Actor {
public:
	Human(glm::vec2 position, glm::vec2 size);
	~Human();

	virtual void move(const std::vector<std::vector<TerrainTile>>& levelData, std::vector<Actor*>& actors, Player* player, float deltaTime) override;
};