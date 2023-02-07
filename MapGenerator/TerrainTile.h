#pragma once

#include <glm/glm.hpp>
#include <JCEngine/SpriteBatch.h>
#include <JCEngine/GLTexture.h>
#include <JCEngine/InputManager.h>
#include <Zombies\Gun.h>
#include <JCEngine\Camera2D.h>

enum class TerrainType { GRASS, WATER, MOUNTAIN, FOREST, TOWN, CASTLE };

class TerrainTile {
	public:
		TerrainTile(int horizontalPosition, int verticalPosition);
		~TerrainTile();

		void setTraversalDistance(int traversalDistance) { _traversalDistance = traversalDistance; };

		void draw(JCEngine::SpriteBatch& spriteBatch);

		TerrainType getTerrainType() const { return _terrainType; };
		bool getIsPassable() const { return _isPassable; };
		glm::vec4 getUV() const { return _uv; };
		JCEngine::GLTexture getTexture() const { return _texture; };
		int getTraversalDistance() const { return _traversalDistance; };

	protected:
		glm::vec2 _position;
		TerrainType _terrainType;
		bool _isPassable;
		JCEngine::GLTexture _texture;
		glm::vec4 _uv;
		int _traversalDistance;
};