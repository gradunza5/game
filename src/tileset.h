/*
 * File:	tileset.h
 * Author:	James Letendre
 *
 * Constants for tileset indicies
 */
#ifndef _TILESET_H_
#define _TILESET_H_

#define TILESET_SIZE	(64)

#define ROBOT_NS_ID	(0)
#define ROBOT_EW_ID	(1)

// single tile types (index in tileset)
#define GRASS_ID	(2)
#define EMPTY_ID	(2)
#define LAVA_ID		(3)

// multi tile types -(start_index)
#define PATH_ID		(-4)
#define WALL_ID		(-20)
#define WATER_ID	(-36)

// Mask for multi visual tiles
#define MULTI_N	(8)
#define MULTI_S	(4)
#define MULTI_E	(2)
#define MULTI_W	(1)


#endif
