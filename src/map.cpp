/*
 * File:	map.cpp
 *
 * Author:	James Letendre
 *
 * Representation of the map, 2D-array of cells
 */

#include "map.h"
#include "tileset.h"

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

			// determine the tileset type
			int cell_type = map[i][j].getBuildingType();

			if( cell_type < 0 )
				cell_type = find_neighbors(cell_type, i, j);

			if( cell_type >= 0 )
			{
				gc.set_translate((float)x_pos, (float)y_pos, 0);
				map[i][j].draw(gc, cell_width, cell_height, cell_type);
			}
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

/*
 * find_neighbors
 *
 * Find the types of the cells neighboring this cell
 * Used for cells that have different appearances based on neighbors
 */
int Map::find_neighbors( int type, int x, int y )
{
	int index = -type;

	// check N, S, E, W
	if( y > 0 				&& map[x][y-1].getBuildingType() == type ) index += MULTI_N;
	if( (size_t)y < height 	&& map[x][y+1].getBuildingType() == type ) index += MULTI_S;
	if( (size_t)x < width	&& map[x+1][y].getBuildingType() == type ) index += MULTI_E;
	if( x > 0				&& map[x-1][y].getBuildingType() == type ) index += MULTI_W;

	return index;
}
