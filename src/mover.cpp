/**
 * File:    mover.cpp
 *
 * Author:  Ben Reeves
 *
 * Base class for all entities which can move themselves
 */

#include "mover.h"

#include <map>
#include <unordered_map>
#include <limits>

#define MOVE_SPEED 0.005

using namespace std;

Mover::Mover(Map *map, int startLocationX, int startLocationY, CL_Colorf startColor)
    : Entity(map, startLocationX, startLocationY, startColor)
{
}

void Mover::update()
{
    // update location here
    if (path.size() > 0)
    {
        // update location here
		delay_count += MOVE_SPEED;
		if( delay_count >= (*map)[current_x][current_y].getMoveCost() )
		{
			delay_count = 0;
			
			CL_Point node = path.front();
			path.pop();

			current_x = node.x;
			current_y = node.y;
		}
    }
    else if (has_destination)
    {
        // calculate path, then update location
		if( !findPath() )
		{
			// can't find a path
			fprintf(stderr, "Mover: Can't find path from %i, %i to %i, %i\n", current_x, current_y, destination_x, destination_y );
		}
		has_destination = false;
		delay_count = 0;
    }
	else
	{
		// TODO debug
		setDestination( rand()*map->getWidth()/RAND_MAX, rand()*map->getHeight()/RAND_MAX );
	}

    Entity::update();
}

void Mover::setDestination( int destination_x, int destination_y)
{
	printf("Mover: Moving to %i, %i\n", destination_x, destination_y);
    this->destination_x = destination_x;
    this->destination_y = destination_y;

    this->has_destination = true;
}

typedef struct {
    int x,y;
} astar_pose2d_t;

bool operator<(const astar_pose2d_t &a, const astar_pose2d_t &b)
{
	return (a.x != b.x) ? a.x < b.x : a.y < b.y;
}

bool operator==(const astar_pose2d_t &a, const astar_pose2d_t &b)
{
	return a.x == b.x && a.y == b.y;
}

uint64_t astar_pose2d_hash(const astar_pose2d_t &p)
{
	return ( ((int64_t)p.x) << 32) | ((int64_t)p.y) ;
}

// node on the map that stores cost and heuristic information
typedef struct astar_node
{
	astar_pose2d_t pose; // node location
	double weight;
} astar_node_t;

// this comparison is used by priority_queue to find which node has the lowest cost
class BestWeightCompare {
    public:
        bool operator()(const astar_node_t &a, const astar_node_t &b)
        {
            return a.weight > b.weight;
        }
};

bool Mover::findPath(double *cost, const int accuracy ) //, int step_size)
{
	//
	// Define a list of possible successors that we would like to consider
	//
	#define ASTAR_NODE_SUCCESSORS 8
	const astar_node_t successors[ASTAR_NODE_SUCCESSORS] = {
		{{ 1,  0}, 1},
		{{-1,  0}, 1},
		{{ 0,  1}, 1},
		{{ 0, -1}, 1},
		{{ 1,  1}, sqrt(2)},
		{{ 1, -1}, sqrt(2)},
		{{-1,  1}, sqrt(2)},
		{{-1, -1}, sqrt(2)},
	};

	std::unordered_map<uint64_t, bool> seen; // path that has been evaluated already
	std::unordered_map<uint64_t, double> costs; // real cost accumulated
    
    // queue of nodes that need to be explored
	std::priority_queue<astar_node_t, std::vector<astar_node_t>, BestWeightCompare> queue; 
	std::unordered_map<uint64_t, astar_pose2d_t> parents; // node parent to back trace the optimal path

	const double goal_accuracy = accuracy;

	// backwards so path comes out in the right order
	const astar_pose2d_t init = {destination_x, destination_y}; // goal pose
	const astar_pose2d_t goal = {current_x, current_y}; // start position node

	// insert first node which is the start pose
	costs[astar_pose2d_hash(init)] = 0.001;
	queue.push((astar_node_t){init, hypot(goal.x - init.x, goal.y - init.y)});

	astar_node_t head, child;
	double head_cost, child_cost;
	int i;

    int depth = 0;
	for(;;)
	{
		// no path found?
		if (queue.empty())
		{
			if (cost) *cost = std::numeric_limits<double>::infinity();
			return false;
		}

		// copy the head of the queue
		head = queue.top();
		queue.pop();

		// found the goal yet?
		if (head.pose == goal) break;
		if (goal_accuracy > 0.0 && hypot((double)(goal.x - head.pose.x), (double)(goal.y - head.pose.y)) < goal_accuracy) break;
		
		// mark it as already seen
		if (seen[astar_pose2d_hash(head.pose)]) continue;
		seen[astar_pose2d_hash(head.pose)] = true;

		head_cost = costs[astar_pose2d_hash(head.pose)];

		// find sucessors
		for (i = 0; i < ASTAR_NODE_SUCCESSORS; i++)
		{
			// compute successor
			child.pose.x = head.pose.x + successors[i].pose.x;
			child.pose.y = head.pose.y + successors[i].pose.y;

			if (seen[astar_pose2d_hash(child.pose)]) continue;

			// calculate cost and heuristic

            unsigned char p = 127;

            if (child.pose.x >= 0 && (size_t)child.pose.x < map->getWidth() && 
                    child.pose.y >= 0 && (size_t)child.pose.y < map->getHeight())
            {
                p = (*map)[child.pose.x][child.pose.y].getMoveCost();
            }
            else
            {
                p = -1;
            }

            if (p < 0) 
            {
                // don't consider obstacles at all
                seen[astar_pose2d_hash(child.pose)] = true;
                continue;
            }

            // accumulate cost
            child_cost = head_cost + successors[i].weight * p;

			// if the cell is already in the tentative list,
			// we need to make sure we don't have a higher cost here
			if (costs[astar_pose2d_hash(child.pose)] > 0.0 
                    && costs[astar_pose2d_hash(child.pose)] < child_cost) continue;
			
			// store the parent only if we are interested in knowing the path
			parents[astar_pose2d_hash(child.pose)] = head.pose;
			costs[astar_pose2d_hash(child.pose)] = child_cost;

			// weight is cost + heuristic
			// put the sucessor into the queue
			child.weight = child_cost + hypot((double)(goal.x - child.pose.x), 
                                                (double)(goal.y - child.pose.y));
			queue.push(child);
		}
        depth++;

        if( depth > 10000000 ) 
		{
			return false;
		}
	}

	// cost output
	if (cost) *cost = costs[astar_pose2d_hash(head.pose)];

	// path output
    // add the goal to the path
    path.push(CL_Point(head.pose.x, head.pose.y));
	//printf("\t%i, %i\n", head.pose.x, head.pose.y);

    // get the parent of the head node
    astar_pose2d_t next = parents[astar_pose2d_hash(head.pose)];

    for (;;)
    {
        // we're done if the next node is the start node
        if (next.x == init.x && next.y == init.y ) break;

        // push the current node
        path.push(CL_Point(next.x, next.y));
		//printf("\t%i, %i\n", next.x, next.y);

        // get the parent of the current node
        next = parents[astar_pose2d_hash(next)];
    }

    return true;
}
