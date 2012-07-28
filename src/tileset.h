/*
 * File:	tileset.h
 * Author:	James Letendre
 *
 * Constants for tileset indicies
 */
#ifndef _TILESET_H_
#define _TILESET_H_

#define TILESET_SIZE	(64)

// single tile types (index in tileset)
#define GRASS_ID	(0)
#define LAVA_ID		(1)
#define EMPTY_ID	(0)

// multi tile types -(start_index)
#define PATH_ID		(-2)
#define WALL_ID		(-18)

// Mask for multi visual tiles
#define MULTI_N	(8)
#define MULTI_S	(4)
#define MULTI_E	(2)
#define MULTI_W	(1)


#endif
