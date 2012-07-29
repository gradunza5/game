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
class Mover;

class Map 
{
	public:
		/**
		 * Map(w, h)
		 *
		 * Create a new map with width, w, and height, h.
		 */
		Map(size_t w, size_t h);

		/**
		 * ~Map()
		 *
		 * Cleanup memory
		 */
		~Map();

		/**
		 * Process changes to map
		 */
		void update();

		/**
		 * draw(gc)
		 *
		 * Draw the map using the specified GraphicContext
		 */
		void draw( CL_GraphicContext &gc, double origin_x, double origin_y, double cell_width, double cell_height, double window_width, double window_height );

		/**
		 * getWidth()
		 * getHeight()
		 *
		 * Get the size of the map
		 */
		size_t getWidth() { return width; }
		size_t getHeight() { return height; }

		/**
		 * Get the specified cell out of the map
		 */
		const Cell* getCell( size_t x, size_t y );

		/**
		 * Sets the base type of this cell
		 */
		void setCellBase( size_t x, size_t y, int id );

		/**
		 * Sets the building type of this cell
		 */
		void setCellBuilding( size_t x, size_t y, int id );

		/**
		 * Build the specified cell
		 */
		void buildCell( size_t x, size_t y, double speed );

		/**
		 * Check if there are cells that need attention in the map
		 */
		bool hasChanges() { return modified_list.size() > 0; }

		/**
		 * Command idle robots from the list to move to cells needing work
		 */
		void processChanges( std::vector<Mover*> &robots );

		/*
		 * TODO: More functionality
		 */

	private:
		int find_neighbors( int type, int x, int y );

		/// The underlying map
		Cell **map;

		/// The list of modified cells
		std::vector<CL_Point> modified_list;

		/// Size of map
		size_t width, height;
};

#endif

