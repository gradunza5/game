/*
 * File:	map.cpp
 *
 * Author:	James Letendre
 *
 * Representation of the map, 2D-array of cells
 */

#include "map/map.h"
#include "map/tileset.h"
#include "entity/mover.h"

#include <algorithm>

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
 * process cell changes
 */
void Map::update()
{
	for( auto iter = modified_list.begin(); iter != modified_list.end(); iter++ )
	{
		if( getCell(iter->x, iter->y)->isBuilt() )
		{
			iter = modified_list.erase(iter) - 1;
		}
	}
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

/**
 * Get the specified cell out of the map
 */
const Cell* Map::getCell( size_t x, size_t y )
{
	if( x < width && y < height )
		return &map[x][y];
	else
		return NULL;
}

/**
 * Sets the base type of this cell
 */
void Map::setCellBase( size_t x, size_t y, int id )
{
	if( x < width && y < height )
	{
		map[x][y].setBaseId(id);

		modified_list.push_back( CL_Point(x,y) );
	}
}

/**
 * Sets the building type of this cell
 */
void Map::setCellBuilding( size_t x, size_t y, int id )
{
	if( x < width && y < height )
	{
		map[x][y].setBuildingId(id);

		modified_list.push_back( CL_Point(x,y) );
	}
}

/*
 * Build the cell
 */
void Map::buildCell( size_t x, size_t y, double speed )
{
	if( x < width && y < height )
	{
		map[x][y].build( speed );

		if( map[x][y].isBuilt() )
		{
			CL_Point p(x,y);
			modified_list.erase( std::find(modified_list.begin(), modified_list.end(), p ));
		}
	}
}

/*
 * command robots to perform changes to cells
 */
void Map::processChanges( std::vector<Mover*> &robots )
{
	if( !hasChanges() ) return;

	auto iter = modified_list.begin();

	for( Mover *m : robots )
	{
		if( m->isIdle() )
		{
			m->setDestination( iter->x, iter->y );
			iter++;

			if( iter == modified_list.end() ) 
				return;
		}

	}
}

