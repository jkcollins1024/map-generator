#include "Map.h"
#include "TerrainTile.h"
#include "GrassTile.h"
#include "ForestTile.h"
#include "MountainTile.h"
#include "WaterTile.h"
#include "CastleTile.h"
#include "TownTile.h"

#include <random>
#include <ctime>

const float TILE_WIDTH = 64.0f;

struct ImpassableRegion {
	int xBegin;
	int xEnd;
	int yBegin;
	int yEnd;
	int xEntrance;
	int yEntrance;
	TerrainType type;

	ImpassableRegion(int xBegin, int xEnd, int yBegin, int yEnd, int xEntrance, int yEntrance, TerrainType terrainType) : xBegin(xBegin), xEnd(xEnd), yBegin(yBegin), yEnd(yEnd), xEntrance(xEntrance), yEntrance(yEntrance), type(terrainType) {}
};

Map::Map(int height, int width) {

	std::mt19937 randomEngine(time(nullptr));

	std::uniform_int_distribution<int> randomx(0, width - 1);
	std::uniform_int_distribution<int> randomy(0, height - 1);
	std::uniform_int_distribution<int> xSize(1, 6);
	std::uniform_int_distribution<int> ySize(1, 6);
	std::uniform_int_distribution<int> sideSelection(1, 4);

	std::vector<ImpassableRegion> regions = std::vector<ImpassableRegion>();

	int regionCount = width / 5;

	for (int i = 1; i < 4; i++) {
		for (int j = 0; j < regionCount; j++) {
			int xBegin = randomx(randomEngine);
			int yBegin = randomy(randomEngine);

			int xEnd = xSize(randomEngine) + xBegin;
			int yEnd = ySize(randomEngine) + yBegin;

			if (xEnd >= width) {
				xEnd = width - 1;
			}

			if (yEnd >= height) {
				yEnd = height - 1;
			}

			int xEntrance = xBegin;
			int yEntrance = yBegin;
			int side = sideSelection(randomEngine);

			std::uniform_int_distribution<int> xEntranceCoord(xBegin, xEnd);
			std::uniform_int_distribution<int> yEntranceCoord(yBegin, yEnd);
			switch (side) {
				case 1:
					yEntrance = yEntranceCoord(randomEngine);
					break;
				case 2:
					yEntrance = yEnd;
					xEntrance = xEntranceCoord(randomEngine);
					break;
				case 3:
					xEntrance = xEnd;
					yEntrance = yEntranceCoord(randomEngine);
					break;
				case 4:
					xEntrance = xEntranceCoord(randomEngine);
					break;
			}

			regions.push_back(ImpassableRegion(xBegin, xEnd, yBegin, yEnd, xEntrance, yEntrance, (TerrainType)i));
		}
	}

	for (int y = 0; y < height; y++) {
		std::vector<TerrainTile> tiles = std::vector<TerrainTile>();

		for (int x = 0; x < width; x++) {
			if (x == 0 && y == 0) {
				tiles.push_back(GrassTile(x, y));
				continue;
			}
			
			TerrainType newTileType = TerrainType::GRASS;

			for (int i = 0; i < regions.size(); i++) {
				ImpassableRegion curr = regions[i];
				if (curr.xBegin <= x && curr.xEnd >= x && curr.yBegin <= y && curr.yEnd >= y) {
					newTileType = curr.type;
					break;
				}
			}

			switch (newTileType) {
			case TerrainType::FOREST:
				tiles.push_back(ForestTile(x, y));
				break;
			case TerrainType::MOUNTAIN:
				tiles.push_back(MountainTile(x, y));
				break;
			case TerrainType::WATER:
				tiles.push_back(WaterTile(x, y));
				break;
			case TerrainType::GRASS:
			default:
				tiles.push_back(GrassTile(x, y));
				break;
			}
		}

		_mapData.push_back(tiles);
	}

	//add towns and castle
	for (int i = 0; i < regionCount; i++) {
		int xCoord = randomx(randomEngine);
		int yCoord = randomy(randomEngine);

		while (_mapData[yCoord][xCoord].getTerrainType() != TerrainType::GRASS) {
			xCoord = randomx(randomEngine);
			yCoord = randomy(randomEngine);
		}

		_mapData[yCoord][xCoord] = TownTile(xCoord, yCoord);
	}

	int xCoord = randomx(randomEngine);
	int yCoord = randomy(randomEngine);

	//single castle for now
	while (_mapData[yCoord][xCoord].getTerrainType() != TerrainType::GRASS) {
		xCoord = randomx(randomEngine);
		yCoord = randomy(randomEngine);
	}

	_mapData[yCoord][xCoord] = CastleTile(xCoord, yCoord);

	//add impassable region entrances
	std::uniform_int_distribution<int> side(1, 4);
	for (int i = 0; i < regions.size(); i++) {
		ImpassableRegion curr = regions[i];
		
		_mapData[curr.yEntrance][curr.xEntrance].addEntrance();
	}
}

Map::~Map() {

}

void Map::draw() {
	_spriteBatch.init();
	_spriteBatch.begin();

	JCEngine::ColorRGBA8 tileColor{ 255, 255, 255, 255 };

	for (int y = 0; y < _mapData.size(); y++) {
		for (int x = 0; x < _mapData[y].size(); x++) {
			_mapData[y][x].draw(_spriteBatch);
		}
	}

	_spriteBatch.end();
	_spriteBatch.renderBatch();
}