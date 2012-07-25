/**
 * File:    entity.cpp
 *
 * Author:  Ben Reeves
 *
 * Implementation of base class for all non-board objects
 */

#include "entity.h"

/**
 * Entity(map, x, y, color)
 *
 * Creates a new entity at the starting location defined by x and y
 */
Entity::Entity(Map *map, double startLocationX, double startLocationY, CL_Colorf startColor)
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
void Entity::draw(CL_GraphicContext &gc, double cell_width, double cell_height)
{
    gc.push_modelview();

	// draw a diamond
	double small_side = std::min( cell_width, cell_height ) / 2.0;
	int new_side  = hypot( small_side, small_side );

    gc.set_translate(current_x*cell_width + cell_width/2, current_y*cell_height - std::min(0.0, cell_width/2.0 - cell_height/2.0), 0);
	gc.mult_rotate(CL_Angle::from_radians(M_PI/4.0));
    CL_Draw::fill(gc, 0, 0, new_side, new_side, entity_color);

    gc.pop_modelview();
}

/**
 * update()
 *
 * Updates the Entity
 */
void Entity::update(CL_GraphicContext &gc, double cell_width, double cell_height)
{
    //TODO: update position here

    draw(gc, cell_width, cell_height);
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
