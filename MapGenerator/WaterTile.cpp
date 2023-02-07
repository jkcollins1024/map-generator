#include "WaterTile.h"

#include <JCEngine/ResourceManager.h>

WaterTile::WaterTile(int horizontalPosition, int verticalPosition) : TerrainTile(horizontalPosition, verticalPosition) {
	_terrainType = TerrainType::WATER;
	_isPassable = false;
	_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); //uv should just be standard for now

	_texture = JCEngine::ResourceManager::getTexture("Textures/WaterTile.png");
}

WaterTile::~WaterTile() {

}