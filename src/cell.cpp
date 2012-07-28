/*
 * File:	cell.cpp
 * 
 * Author:	James Letendre
 *
 * A cell from the map
 */

#include "cell.h"
#include <ClanLib/core.h>

#define CELL_TYPE(x, r,g,b, c)	 {std::string(#x), CL_Colorf(r,g,b), (c)} 
const Cell::cell_type Cell::Types[] = 
{
	//			Name	  r,   g,   b	move_cost
	CELL_TYPE(	Grass,	  1, 142,  14,	 5),
	CELL_TYPE(	Path,	120,  72,   0,	 1),
	CELL_TYPE(	Lava,	255,   0,   0,	-1),
	CELL_TYPE(	Wall,	 80,  80,  80,	-1),
	CELL_TYPE(	Empty,	  0,   0,   0,	 10)
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
