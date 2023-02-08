#include "TerrainTile.h"

#include <JCEngine/ResourceManager.h>

const float TILE_WIDTH = 64.0f;

TerrainTile::TerrainTile(int horizontalPosition, int verticalPosition) {
	_position = glm::vec2(horizontalPosition, verticalPosition);
	_traversalDistance = -1;
}

TerrainTile::~TerrainTile() {

}

void TerrainTile::setTraversalDistance(int traversalDistance) {
	_traversalDistance = traversalDistance;
}

void TerrainTile::draw(JCEngine::SpriteBatch& spriteBatch) {
	JCEngine::ColorRGBA8 tileColor{ 255, 255, 255, 255 };
	glm::vec4 positionRect(_position.x * TILE_WIDTH, _position.y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

	spriteBatch.draw(positionRect, getUV(), getTexture().id, 0.0f, tileColor);
}

void TerrainTile::addEntrance() {
	if (_terrainType == TerrainType::FOREST) {
		_texture = JCEngine::ResourceManager::getTexture("Textures/ForestTileWithEntrance.png");
	}
	else if (_terrainType == TerrainType::MOUNTAIN) {
		_texture = JCEngine::ResourceManager::getTexture("Textures/MountainTileWithEntrance.png");
	}

	_isPassable = true;
}