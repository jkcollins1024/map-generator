#pragma once

#include "Actor.h"

class Player : public Actor {
public:
	Player() {}
	Player(glm::vec2 position, JCEngine::Camera2D* camera);
	~Player();

	virtual void move(const std::vector<std::vector<TerrainTile>>& levelData, std::vector<Actor*>& actors, Player* player, float deltaTime) override;

	void setDirectionFacing(JCEngine::InputManager* inputManager);

private:
	JCEngine::Camera2D* _camera;

};