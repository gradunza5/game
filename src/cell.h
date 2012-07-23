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
#include <string.h>

#define CELL_TYPE(x, r,g,b)	 {std::string(#x), CL_Colorf(r,g,b)} 


class Cell
{
	public:
		typedef struct
		{
			const std::string name;
			const CL_Colorf color;
		} cell_type;

		static const cell_type Types[];
		static const size_t num_cell_types;

		/*
		 * Cell(type=Empty)
		 *
		 * Create a Cell of the specified type
		 */
		Cell(int id = 0);

		/*
		 * getId()
		 *
		 * Get the Type of this Cell
		 */
		int getId() { return id; }

		/*
		 * setId(id)
		 * 
		 * Change the type of this cell
		 */
		void setId( int id );

		/*
		 * draw(gc)
		 *
		 * Draw this cell
		 */
		void draw( CL_GraphicContext &gc, double width, double height );

		/*
		 * TODO: different method to determine appearance
		 */
		CL_Colorf getColor() { return Cell::Types[id].color; }

		/*
		 * TODO: More functionality
		 */


	private:
		// The type of this cell
		int id;
};

#endif
