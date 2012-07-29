/**
 * File:    entity.cpp
 *
 * Author:  Ben Reeves
 *
 * Implementation of base class for all non-board objects
 */

#include "entity.h"
#include "game.h"
#include "tileset.h"

/**
 * Entity(map, x, y, color)
 *
 * Creates a new entity at the starting location defined by x and y
 */
Entity::Entity(Map *map, int startLocationX, int startLocationY, CL_Colorf startColor)
    : map(map), current_x(startLocationX), current_y(startLocationY), entity_color(startColor)
{
}

/**
 * ~Entity()
 *
 * Destructor
 */
Entity::~Entity()
{
    //TODO: something here?
}

/**
 * draw(gc)
 *
 * draws the entity on the given graphic context
 */
void Entity::draw(CL_GraphicContext &gc, double cell_width, double cell_height, double map_origin_x, double map_origin_y )
{
    gc.push_modelview();

    gc.set_translate(current_x*cell_width + map_origin_x, current_y*cell_height + map_origin_y, 0);
	CL_Sprite &sprite( Game::get_tileset() );

	sprite.set_frame( ROBOT_NS_ID );
	sprite.set_scale( cell_width/TILESET_SIZE, cell_height/TILESET_SIZE );

	sprite.draw( gc, 0, 0 );

    gc.pop_modelview();
}

/**
 * update()
 *
 * Updates the Entity
 */
void Entity::update( )
{
}

/**
 * setColor(r, g, b)
 *
 * sets the color of this entity
 */
void Entity::setColor(float r, float g, float b)
{
    entity_color = CL_Colorf(r, g, b);
}

/**
 * setColor(newColor)
 *
 * sets the color of this entity
 */
void Entity::setColor(CL_Colorf newColor)
{
    entity_color = newColor;
}
