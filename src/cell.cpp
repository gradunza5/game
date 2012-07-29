/*
 * File:	cell.cpp
 * 
 * Author:	James Letendre
 *
 * A cell from the map
 */

#include "cell.h"
#include "game.h"
#include "tileset.h"
#include <ClanLib/core.h>

#define CELL_TYPE(x, id, c)	 {std::string(#x), id, (c)} 
const Cell::cell_type Cell::Types[] = 
{
	//			Name	id			move_cost
	CELL_TYPE(	Grass,	GRASS_ID,	 5),
	CELL_TYPE(	Path,	PATH_ID,	 1),
	CELL_TYPE(	Wall,	WALL_ID,	-1),
	CELL_TYPE(	Water,	WATER_ID,	15),
	CELL_TYPE(	Lava,	LAVA_ID,	-1),
	CELL_TYPE(	Empty,	EMPTY_ID,	 10)
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
void Cell::draw( CL_GraphicContext &gc, double width, double height, int idx )
{
	CL_Sprite &sprite( Game::get_tileset() );

	if( idx >= sprite.get_frame_count() ) return;

	sprite.set_frame( idx );
	sprite.set_scale( width/TILESET_SIZE, height/TILESET_SIZE );

	sprite.draw( gc, 0, 0 );
}
