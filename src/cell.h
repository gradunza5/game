/*
 * File:	cell.h
 * 
 * Author:	James Letendre
 *
 * A cell from the map
 */
#ifndef CELL_H
#define CELL_H

#include <ClanLib/display.h>

class Cell
{
	public:
		enum Type
		{
			Empty,
			Wall
		};

		/*
		 * Cell(type=Empty)
		 *
		 * Create a Cell of the specified type
		 */
		Cell(Type type = Empty);

		/*
		 * getId()
		 *
		 * Get the Type of this Cell
		 */
		Type getId() { return type; }

		/*
		 * setType(type)
		 * 
		 * Change the type of this cell
		 */
		void setType( Type type );

		/*
		 * draw(gc)
		 *
		 * Draw this cell
		 */
		void draw( CL_GraphicContext &gc, double width, double height );

		/*
		 * TODO: different method to determine appearance
		 */
		CL_Colorf getColor() { return color; }

		/*
		 * TODO: More functionality
		 */


	private:
		// The type of this cell
		Type type;

		// TODO: different method to determine appearance
		CL_Colorf color;
};

#endif
