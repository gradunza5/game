/*
 * File:	map.h
 *
 * Author:	James Letendre
 *
 * Representation of the map, 2D-array of cells
 */
#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

#include "cell.h"
#include <ClanLib/display.h>

class Map 
{
	public:
		/*
		 * Map(w, h)
		 *
		 * Create a new map with width, w, and height, h.
		 */
		Map(size_t w, size_t h);

		/*
		 * ~Map()
		 *
		 * Cleanup memory
		 */
		~Map();

		/*
		 * draw(gc)
		 *
		 * Draw the map using the specified GraphicContext
		 */
		void draw( CL_GraphicContext &gc, double origin_x, double origin_y, double cell_width, double cell_height, double window_width, double window_height );

		/*
		 * getWidth()
		 * getHeight()
		 *
		 * Get the size of the map
		 */
		size_t getWidth() { return width; }
		size_t getHeight() { return height; }

		/*
		 * operator[]
		 *
		 * Get the row out of the map
		 */
		Cell* operator[](size_t i);

		/*
		 * TODO: More functionality
		 */

	private:
		int find_neighbors( int type, int x, int y );

		// The underlying map
		Cell **map;

		// Size of map
		size_t width, height;
};

#endif

