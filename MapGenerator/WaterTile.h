#pragma once
#include "TerrainTile.h"

class WaterTile : public TerrainTile {
public:
	WaterTile(int horizontalPosition, int verticalPosition);
	~WaterTile();
};