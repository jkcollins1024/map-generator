#include "CastleTile.h"

#include <JCEngine/ResourceManager.h>

CastleTile::CastleTile(int horizontalPosition, int verticalPosition) : TerrainTile(horizontalPosition, verticalPosition) {
	_terrainType = TerrainType::CASTLE;
	_isPassable = true;
	_traversalDistance = -1;
	_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); //uv should just be standard for now

	_texture = JCEngine::ResourceManager::getTexture("Textures/CastleTile.png");
}

CastleTile::~CastleTile() {

}