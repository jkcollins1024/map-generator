#include "TownTile.h"

#include <JCEngine/ResourceManager.h>

TownTile::TownTile(int horizontalPosition, int verticalPosition) : TerrainTile(horizontalPosition, verticalPosition) {
	_terrainType = TerrainType::TOWN;
	_isPassable = true;
	_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); //uv should just be standard for now

	_texture = JCEngine::ResourceManager::getTexture("Textures/TownTile.png");
}

TownTile::~TownTile() {

}