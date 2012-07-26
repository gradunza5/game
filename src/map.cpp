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
void Map::draw( CL_GraphicContext &gc, double origin_x, double origin_y, double cell_width, double cell_height, double window_width, double window_height )
{
	gc.push_modelview();

	// draw each cell
	for( size_t i = 0; i < width; i++ )
	{
		double x_pos = i*cell_width + origin_x;

		if( x_pos < -cell_width ) continue;		// not in window yet, not visible
		if( x_pos > window_width ) break;		// beyond end of window, not visible

		for( size_t j = 0; j < height; j++ )
		{
			double y_pos = j*cell_height + origin_y;

			if( y_pos < -cell_height ) continue;	// not in window yet, not visible
			if( y_pos > window_height ) break;		// beyond end of window, not visible

			gc.set_translate(x_pos, y_pos, 0);
			map[i][j].draw(gc, cell_width, std::min(cell_height, window_height - y_pos));
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

