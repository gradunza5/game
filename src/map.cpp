/*
 * File:	map.cpp
 *
 * Author:	James Letendre
 *
 * Representation of the map, 2D-array of cells
 */

#include "map.h"

/*
 * Map(w, h)
 *
 * Create a new map with width, w, and height, h.
 */
Map::Map(size_t w, size_t h) 
	: width(w), height(h)
{
	map = new Cell*[width];
	for( size_t i = 0; i < width; i++ )
	{
		map[i] = new Cell[height];
	}
}

/*
 * ~Map()
 *
 * Cleanup memory
 */
Map::~Map()
{
	for( size_t i = 0; i < width; i++ )
	{
		delete[] map[i];
	}
	delete[] map;
}

/*
 * draw(gc)
 *
 * Draw the map using the specified GraphicContext
 */
void Map::draw( CL_GraphicContext &gc, double origin_x, double origin_y, double cell_width, double cell_height )
{
	gc.push_modelview();

	// draw each cell
	for( size_t i = 0; i < width; i++ )
	{
		for( size_t j = 0; j < height; j++ )
		{
			gc.set_translate(i*cell_width + origin_x, j*cell_height + origin_y, 0);
			map[i][j].draw(gc, cell_width, cell_height);
		}
	}

	gc.pop_modelview();
}

/*
 * operator[]
 *
 * Get the row out of the map
 */
Cell* Map::operator[](size_t i)
{
	if( i < width )
		return map[i];
	return NULL;
}

