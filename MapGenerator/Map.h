#pragma once

#include<vector>
#include<JCEngine/SpriteBatch.h>

#include "TerrainTile.h"

class Map {
	public:
		Map() {}
		Map(int height, int width);
		~Map();

		void draw();

		const std::vector<std::vector<TerrainTile>>& getMapData() const { return _mapData; };

		int getWidth() const { return _mapData[0].size(); };
		int getHeight() const { return _mapData.size(); };

	private:
		std::vector<std::vector<TerrainTile>> _mapData;
		JCEngine::SpriteBatch _spriteBatch;
};