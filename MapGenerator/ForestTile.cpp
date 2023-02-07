#include "ForestTile.h"

#include <JCEngine/ResourceManager.h>

ForestTile::ForestTile(int horizontalPosition, int verticalPosition) : TerrainTile(horizontalPosition, verticalPosition) {
	_terrainType = TerrainType::FOREST;
	_isPassable = false;
	_traversalDistance = -1;
	_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); //uv should just be standard for now

	_texture = JCEngine::ResourceManager::getTexture("Textures/ForestTile.png");
}

ForestTile::~ForestTile() {

}