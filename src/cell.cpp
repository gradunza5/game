/*
 * File:	cell.cpp
 * 
 * Author:	James Letendre
 *
 * A cell from the map
 */

#include "cell.h"
#include <ClanLib/core.h>

/*
 * Cell(type=EmptyCell)
 *
 * Create a Cell of the specified type
 */
Cell::Cell(Cell::Type type)
	: type( type )
{
	setType(type);
}

void Cell::setType( Cell::Type type )
{
	this->type = type;
	// set the color based on type
	switch( type )
	{
		case Empty:
			color = CL_Colorf(.0f, .0f, .0f);
			break;
		case Wall:
			color = CL_Colorf(.3f,.3f,.3f);
			break;
		default:
			CL_Console::write_line("Unknown type %1\n", type);
			color = CL_Colorf(1.0f, .75f, .8f);
	}
}

/*
 * draw(gc)
 *
 * Draw this cell
 */
void Cell::draw( CL_GraphicContext &gc, double width, double height )
{
	CL_Draw::fill(gc, 0, 0, width, height, color);
}
