/*
 * File:	main.cpp
 * Author:	James Letendre
 *
 * Main program
 */
#include <ClanLib/core.h>
#include <ClanLib/application.h>

#include "game.h"

// secondary main function
int _main( const std::vector<CL_String> &args )
{
	Game *game = new Game();
	game->run();

	return 0;
}

// create entry point
CL_ClanApplication app(&_main);
