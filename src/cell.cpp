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

#define MIN_BUILD_ALPHA	0.3

#define CELL_TYPE(x, id, c, b)	 {std::string(#x), id, (c), (b)} 
const Cell::cell_type Cell::Types[] = 
{
	//
	// Base Types
	//
	//			Name	id			move_cost,	build_cost
	CELL_TYPE(	Grass,	GRASS_ID,	 5,			0),
	CELL_TYPE(	Lava,	LAVA_ID,	-1,			0),
	CELL_TYPE(	Empty,	EMPTY_ID,	 10,		0),

	//
	// Building Types
	// 
	CELL_TYPE(	Path,	PATH_ID,	 1,			1),
	CELL_TYPE(	Wall,	WALL_ID,	-1,			5),
	CELL_TYPE(	Water,	WATER_ID,	15,			1)
};
const size_t Cell::num_cell_types = sizeof( Cell::Types ) / sizeof( cell_type );

/*
 * Cell(type=EmptyCell)
 *
 * Create a Cell of the specified type
 */
Cell::Cell(int id)
{
	setBaseId(id);
	setBuildingId(-1);
}

void Cell::setBaseId( int id )
{
	this->base_id = id;
}

void Cell::setBuildingId( int id )
{
	improve_id = id;
	/*
	if( improve_id != -1 )
		build_amount = Cell::Types[id].build_cost;
	else
	*/
		build_amount = 0;
}

/*
 * draw(gc)
 *
 * Draw this cell
 */
void Cell::draw( CL_GraphicContext &gc, double width, double height, int idx )
{
	double build_percent = (build_amount / Cell::Types[improve_id].build_cost) * ( 1.0 - MIN_BUILD_ALPHA ) + MIN_BUILD_ALPHA;

	CL_Sprite &sprite( Game::get_tileset() );

	// Draw the base tile
	sprite.set_frame( getBaseType() );
	sprite.set_scale( width/TILESET_SIZE, height/TILESET_SIZE );
	sprite.set_alpha( 1.0 );
	sprite.draw( gc, 0, 0 );

	// if there is an improvement, draw that too
	if( !hasBuilding() || idx >= sprite.get_frame_count() ) return;

	sprite.set_frame( idx );
	sprite.set_scale( width/TILESET_SIZE, height/TILESET_SIZE );
	sprite.set_alpha( build_percent );
	sprite.draw( gc, 0, 0 );
}

void Cell::build( double speed )
{
	if( !isBuilt() )
		build_amount += speed;
}

double Cell::getMoveCost() 
{ 
	if( improve_id == 0x3ff00000 ) printf("WTF\n");
	return Cell::Types[improve_id == -1 ? base_id : improve_id].move_cost; 
}
