/*
 * File:	cell.cpp
 * 
 * Author:	James Letendre
 *
 * A cell from the map
 */

#include "cell.h"
#include <ClanLib/core.h>

const Cell::cell_type Cell::Types[] = 
{
	//			Name	  r,   g,   b
	CELL_TYPE(	Empty,	  0,   0,   0),
	CELL_TYPE(	Lava,	255,   0,   0),
	CELL_TYPE(	Wall,	 80,  80,  80)
};
const size_t Cell::num_cell_types = sizeof( Cell::Types ) / sizeof( cell_type );

/*
 * Cell(type=EmptyCell)
 *
 * Create a Cell of the specified type
 */
Cell::Cell(int id)
{
	setId(id);
}

void Cell::setId( int id )
{
	this->id = id;
}

/*
 * draw(gc)
 *
 * Draw this cell
 */
void Cell::draw( CL_GraphicContext &gc, double width, double height )
{
	CL_Draw::fill(gc, 0, 0, width, height, Cell::Types[id].color);
}
