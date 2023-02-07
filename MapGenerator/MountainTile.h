#pragma once
#include "TerrainTile.h"

class MountainTile : public TerrainTile {
public:
	MountainTile(int horizontalPosition, int verticalPosition);
	~MountainTile();
};