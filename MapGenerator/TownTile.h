#pragma once
#include "TerrainTile.h"

class TownTile : public TerrainTile {
public:
	TownTile(int horizontalPosition, int verticalPosition);
	~TownTile();
};