#include "MountainTile.h"

#include <JCEngine/ResourceManager.h>

MountainTile::MountainTile(int horizontalPosition, int verticalPosition) : TerrainTile(horizontalPosition, verticalPosition) {
	_terrainType = TerrainType::MOUNTAIN;
	_isPassable = false;
	_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); //uv should just be standard for now

	_texture = JCEngine::ResourceManager::getTexture("Textures/MountainTile.png");
}

MountainTile::~MountainTile() {

}