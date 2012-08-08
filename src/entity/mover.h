/**
 * File:    mover.h
 *
 * Author:  Ben Reeves
 *
 * Base class for all entities that are self-propelled
 */

#ifndef MOVER_H
#define MOVER_H

#include "entity.h"

#include<ClanLib/core.h>
#include <queue>
#include <vector>

class Mover : public Entity
{
    public:
        /**
         * Constructor
         */
        Mover(Map *map, int startLocationX, int startLocationY, 
                CL_Colorf startColor = CL_Colorf::hotpink);

        /**
         * function update()
         *
         * overridden from Entity to provide motion to the object
         */
        virtual void update();

        /**
         * function setDestination(point)
         *
         * tells the mover to travel to a point
         */
        void setDestination(int destination_x, int destination_y);

		/**
		 * Returns true if this mover has no current path to follow
		 */
		virtual bool isIdle();

    protected:

        /**
         * Used to create a path to the current destination
         */
        bool findPath(double *cost = NULL, const int accuracy = 1.0 );//, int step_size = 1);

        /**
         * Storage for the destination point
         */
        int destination_x, destination_y;

		/**
		 * Delay for movement cost
		 */
		double delay_count;

        /**
         * Storage for the path
         */
        std::queue<CL_Point> path;

        /**
         * flag set when there's a new destination
         */
        bool has_destination = false;

    private:
        double getDistanceSquared(double originX, double originY, double destinationX, double destinationY);
        std::vector<std::pair<double, double> > neighbors();
};

#endif
