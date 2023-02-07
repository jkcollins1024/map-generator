#pragma once
#include "TerrainTile.h"

class GrassTile: public TerrainTile {
public:
	GrassTile(int horizontalPosition, int verticalPosition);
	~GrassTile();
};