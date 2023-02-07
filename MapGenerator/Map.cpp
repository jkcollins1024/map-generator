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
const int MAX_CASTLE_COUNT = 1;
const int MAX_TOWN_COUNT = 4;

Map::Map(int height, int width) {
	/*_spriteBatch.init();
	_spriteBatch.begin();

	JCEngine::ColorRGBA8 tileColor{ 255, 255, 255, 255 };*/

	std::mt19937 randomEngine(time(nullptr));
	std::uniform_int_distribution<int> randomx(0,5);

	int maxImpassableCount = 200;

	int castleCount = 0;
	int townCount = 0;
	int forestCount = 0;
	int mountainCount = 0;
	int waterCount = 0;

	for (int y = 0; y < height; y++) {
		std::vector<TerrainTile> tiles = std::vector<TerrainTile>();

		for (int x = 0; x < width; x++) {
			if (x == 0 && y == 0) {
				tiles.push_back(GrassTile(x, y));
				continue;
			}

			int randomTerrainType = randomx(randomEngine);
			
			switch ((TerrainType)randomTerrainType) {
				case TerrainType::FOREST:
					if (forestCount < maxImpassableCount) {
						tiles.push_back(ForestTile(x, y));
						forestCount++;
					}
					else {
						tiles.push_back(GrassTile(x, y));
					}
					break;
				case TerrainType::MOUNTAIN:
					if (mountainCount < maxImpassableCount) {
						tiles.push_back(MountainTile(x, y));
						mountainCount++;
					}
					else {
						tiles.push_back(GrassTile(x, y));
					}
					break;
				case TerrainType::WATER:
					if (waterCount < maxImpassableCount) {
						tiles.push_back(WaterTile(x, y));
						waterCount++;
					}
					else {
						tiles.push_back(GrassTile(x, y));
					}
					break;
				case TerrainType::CASTLE:
					if (castleCount < MAX_CASTLE_COUNT) {
						tiles.push_back(CastleTile(x, y));
						castleCount++;
					}
					else {
						tiles.push_back(GrassTile(x, y));
					}
					break;
				case TerrainType::TOWN:
					if (townCount < MAX_TOWN_COUNT) {
						tiles.push_back(TownTile(x, y));
						townCount++;
					}
					else {
						tiles.push_back(GrassTile(x, y));
					}
					break;
				case TerrainType::GRASS:
				default:
					tiles.push_back(GrassTile(x, y));
					break;
			}

			/*glm::vec4 positionRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			_spriteBatch.draw(positionRect, newTile->getUV(), newTile->getTexture().id, 0.0f, tileColor);*/
		}

		_mapData.push_back(tiles);
	}

	/*_spriteBatch.end();*/
}

Map::~Map() {

}

void Map::makeMapTraversible() {
	TerrainTile curr = _mapData[0][0];

	curr.setTraversalDistance(0);

	traverseFromCurrentLocation(0, 0, 1);

	//move traversible tiles that can't be accessed
	std::vector<glm::vec4> inaccessibleTraversibleTiles = std::vector<glm::vec4>();
	std::vector<glm::vec4> accessibleNonTraversibleTiles = std::vector<glm::vec4>();

	//register traversible tiles that are inaccessible
	for (int y = 0; y < _mapData.size(); y++) {
		for (int x = 0; x < _mapData[y].size(); x++) {
			TerrainTile curr = _mapData[y][x];

			if (curr.getTraversalDistance() >= 0)
				continue;

			if (!curr.getIsPassable()) {
				//check up
				if (y < _mapData.size() - 1 && _mapData[y + 1][x].getIsPassable()) {
					accessibleNonTraversibleTiles.push_back(glm::vec4(x, y, (int)curr.getTerrainType(), _mapData[y + 1][x].getTraversalDistance() + 1));
					continue;
				}

				//check right
				if (x < _mapData[y].size() - 1 && _mapData[y][x + 1].getIsPassable()) {
					accessibleNonTraversibleTiles.push_back(glm::vec4(x, y, (int)curr.getTerrainType(), _mapData[y][x + 1].getTraversalDistance() + 1));
					continue;
				}

				//check down
				if (y > 0 && _mapData[y - 1][x].getIsPassable()) {
					accessibleNonTraversibleTiles.push_back(glm::vec4(x, y, (int)curr.getTerrainType(), _mapData[y - 1][x].getTraversalDistance() + 1));
					continue;
				}

				//check left
				if (x > 0 && _mapData[y][x - 1].getIsPassable()) {
					accessibleNonTraversibleTiles.push_back(glm::vec4(x, y, (int)curr.getTerrainType(), _mapData[y][x - 1].getTraversalDistance() + 1));
					continue;
				}
			}

			//grab the non-traversible borders, and add to inaccessible tile list
			if (x > 0) {
				inaccessibleTraversibleTiles.push_back(glm::vec4(x, y, (int)_mapData[x - 1][y].getTerrainType(), -1));
			}
			else {
				inaccessibleTraversibleTiles.push_back(glm::vec4(x, y, (int)_mapData[x + 1][y].getTerrainType(), -1));
			}
		}
	}

	//switch tiles that need to be switched
	for (int x = 0; x < inaccessibleTraversibleTiles.size(); x++) {
		glm::vec4 curr = inaccessibleTraversibleTiles[x];

		int accessibleTileToErase = -1;

		for (int y = 0; y < accessibleNonTraversibleTiles.size(); y++) {
			if (accessibleNonTraversibleTiles[y].z == curr.z) {
				glm::vec4 swap = accessibleNonTraversibleTiles[y];

				TerrainTile temp = _mapData[curr.y][curr.x];

				_mapData[curr.y][curr.x] = _mapData[swap.y][swap.x];
				temp.setTraversalDistance(swap.w);
				_mapData[swap.y][swap.x] = temp;

				accessibleTileToErase = y;
				break;
			}
		}

		if (accessibleTileToErase > 0) {
			accessibleNonTraversibleTiles.erase(accessibleNonTraversibleTiles.begin() + accessibleTileToErase);
		}		

		/*auto iterator = std::find_if(accessibleNonTraversibleTiles.begin(), accessibleNonTraversibleTiles.end(), [curr](glm::vec4 vec) { return vec.z == curr.z;});*/

		//if (*iterator != *accessibleNonTraversibleTiles.end()) {
		//	TerrainTile temp = _mapData[curr.y][curr.x];

		//	_mapData[curr.y][curr.x] = _mapData[iterator->y][iterator->x];
		//	temp.setTraversalDistance(iterator->w);
		//	_mapData[iterator->y][iterator->x] = temp;

		//	//std::swap(_mapData[curr.y][curr.x], _mapData[iterator->y][iterator->x]);
		//	//_mapData[iterator->y][iterator->x].setTraversalDistance(iterator->w);
		//}
	}

	//add castle and town tiles after, maybe?

	//add entry points to impassable sections
}

void Map::traverseFromCurrentLocation(int x, int y, int distance) {
	//check to see if we're still on map
	if (x < 0 || y < 0 || y >= _mapData.size() || x >= _mapData[y].size())
		return;

	TerrainTile curr = _mapData[y][x];

	//just return if we're on an impassable tile or new distance is greater than the current distance
	if (!curr.getIsPassable() || (curr.getTraversalDistance() > 0 && distance > curr.getTraversalDistance()))
		return;

	//set new traversal distance if we can
	if (curr.getIsPassable() && curr.getTraversalDistance() > distance)
		curr.setTraversalDistance(distance);

	//check up
	traverseFromCurrentLocation(x, y + 1, distance + 1);

	//check right
	traverseFromCurrentLocation(x + 1, y, distance + 1);

	//check down
	traverseFromCurrentLocation(x, y - 1, distance + 1);

	//check left
	traverseFromCurrentLocation(x - 1, y, distance + 1);
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