#pragma once

#ifndef GAME_MAP_H
#define GAME_MAP_H
#include "CommonFunction.h"
#include "BaseObject.h"

#define MAX_TILES 20
class TileMat : public BaseObject
{
public:
	TileMat(){};
	~TileMat() {};
};

class GameMap
{
public:
	GameMap() { ; }
	~GameMap() { ; }

	void LoadMap(const char* name);// load digital data file
	void LoadTiles(SDL_Renderer* screen);//load image file
	void DrawMap(SDL_Renderer* screen);
private:
	Map game_map_;
	TileMat tile_mat[MAX_TILES];

};


#endif