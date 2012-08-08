/**
 * File:    entity.h
 *
 * Author:  Ben Reeves
 *
 * Base class for all non-board objects
 */

#ifndef ENTITY_H
#define ENTITY_H

#include "map/map.h"
#include <ClanLib/display.h>

class Entity
{
    public:
        /**
         * Entity(map)
         *
         * creates a new entity on the map at the specified location
         */
        Entity(Map *map, int startLocationX, int startLocationY, CL_Colorf startColor = CL_Colorf::hotpink);

        /**
         * ~Entity()
         *
         * Entity destructor
         */
        ~Entity();

        /**
         * getXLocation()
         *
         * @return the x value of this Entity's location
         */
        int getCurrentX() {return current_x;};

        /**
         * getYLocation()
         *
         * @return the Y value of this Entity's location
         */
        int getCurrentY() {return current_y;};

        /**
         * update()
         *
         * updates the Entity
         */
        virtual void update();

        /**
         * draw(gc)
         *
         * Draws the entity
         */
        void draw(CL_GraphicContext &gc, double cell_width, double cell_height, double map_origin_x, double map_origin_y);

        /**
         * setColor(r, g, b)
         *
         * sets the color that this entity will display as
         */
        void setColor(float r, float g, float b);
        void setColor(CL_Colorf newColor);

		virtual bool isIdle() = 0;

    protected:
        // The map this entity is on
        Map *map;

        // storage for the entity's position
        int current_x;
        int current_y;

        // color storage
        CL_Colorf entity_color;

};

#endif
