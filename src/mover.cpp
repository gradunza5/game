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

using namespace std;

Mover::Mover(Map *map, double startLocationX, double startLocationY, CL_Colorf startColor)
    : Entity(map, startLocationX, startLocationY, startColor)
{
}

void Mover::update(
        CL_GraphicContext &gc, double cell_width, double cell_height, 
        double map_origin_x, double map_origin_y)
{
    // update location here
    if (path.size() > 0)
    {
        // update location here
    }
    else if (has_destination)
    {
        // calculate path, then update location
    }

    Entity::update(gc, cell_width, cell_height, map_origin_x, map_origin_y);
}

void Mover::setDestination(double destination_x, double destination_y)
{
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
    double eps = 0.05;
	return fabs(a.x-b.x) < eps && fabs(a.y-b.y) < eps;
}

uint64_t astar_pose2d_hash(const astar_pose2d_t &p)
{
	return ( ((int64_t)p.x) << (int64_t)32) | ((int64_t)p.y) ;
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

bool Mover::findPath(double *cost, const int accuracy, int step_size)
{
	//
	// Define a list of possible successors that we would like to consider
	//
	#define ASTAR_NODE_SUCCESSORS 8
	const astar_node_t successors[ASTAR_NODE_SUCCESSORS] = {
		{{ step_size,  0        }, step_size},
		{{-step_size,  0        }, step_size},
		{{ 0,          step_size}, step_size},
		{{ 0,         -step_size}, step_size},
		{{ step_size,  step_size}, sqrt(2*step_size*step_size)},
		{{ step_size, -step_size}, sqrt(2*step_size*step_size)},
		{{-step_size,  step_size}, sqrt(2*step_size*step_size)},
		{{-step_size, -step_size}, sqrt(2*step_size*step_size)},
	};

	std::unordered_map<uint64_t, bool> seen; // path that has been evaluated already
	std::unordered_map<uint64_t, double> costs; // real cost accumulated
    
    // queue of nodes that need to be explored
	std::priority_queue<astar_node_t, std::vector<astar_node_t>, BestWeightCompare> queue; 
	std::unordered_map<uint64_t, astar_pose2d_t> parents; // node parent to back trace the optimal path

	const double goal_accuracy = accuracy;

	const astar_pose2d_t init = {current_x, current_y}; // start position node
	const astar_pose2d_t goal = {destination_x, destination_y}; // goal pose

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
		if (goal_accuracy > 0.0 && hypot((double)(goal.x - head.pose.x), (double)(goal.y - head.pose.y)) <= goal_accuracy) break;
		
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

            if (child.pose.x >= 0 && child.pose.x < map->getWidth() && 
                    child.pose.y >= 0 && child.pose.y < map->getHeight())
            {
                p = map[child.pose.x][child.pose.y]->getMoveCost();
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

    // get the parent of the head node
    astar_pose2d_t next = parents[astar_pose2d_hash(head.pose)];

    for (;;)
    {
        // push the current node
        path.push(CL_Point(next.x, next.y));

        // get the parent of the current node
        next = parents[astar_pose2d_hash(next)];

        // we're done if the next node is the start node
        if (next.x == current_x && next.y == current_y) break;
    }

    return true;
}

double Mover::getDistanceSquared(double originX, double originY, double destinationX, double destinationY)
{
    double dx = originX - destinationX;
    double dy = originY - destinationY;

    return (dx * dx) + (dy * dy);
}

vector<pair<double, double> > Mover::neighbors()
{
    vector<pair<double, double> > retVector;

    retVector.push_back(make_pair(current_x - 1, current_y + 1));
    retVector.push_back(make_pair(current_x - 0, current_y + 1));
    retVector.push_back(make_pair(current_x + 1, current_y + 1));

    retVector.push_back(make_pair(current_x - 1, current_y + 0));
    //retVector.push_back(make_pair(current_x - 0, current_y + 0)); not added, current point
    retVector.push_back(make_pair(current_x + 1, current_y + 0));

    retVector.push_back(make_pair(current_x - 1, current_y - 1));
    retVector.push_back(make_pair(current_x - 0, current_y - 1));
    retVector.push_back(make_pair(current_x + 1, current_y - 1));

    return retVector;
}
