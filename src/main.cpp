/*
 * File:	main.cpp
 * Author:	James Letendre
 *
 * Main program
 */
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#define WIN_WIDTH	500
#define WIN_HEIGHT	500

#define MAP_WIDTH	50
#define MAP_HEIGHT	50

#define CURSOR_BLINK_RATE 10

#include "map.h"

Map *map;
bool quit = false;
int cursor_pos_x, cursor_pos_y;
int window_width, window_height;
int cursor_blink_rate = CURSOR_BLINK_RATE;
CL_Colorf cursor_color(CL_Color::white);

void resize(int width, int height)
{
	window_width = width;
	window_height = height;
}

void quit_game()
{
	quit = true;
}

void handle_keyboard( const CL_InputEvent &key, const CL_InputState &state )
{
	switch( key.id )
	{
		case CL_KEY_ESCAPE:
			quit_game();
			break;

		case CL_KEY_W: case CL_KEY_UP:
			if( cursor_pos_y > 0 ) cursor_pos_y--;
			break;
		case CL_KEY_S: case CL_KEY_DOWN:
			if( cursor_pos_y < MAP_HEIGHT-1 ) cursor_pos_y++;
			break;
		case CL_KEY_A: case CL_KEY_LEFT:
			if( cursor_pos_x > 0 ) cursor_pos_x--;
			break;
		case CL_KEY_D: case CL_KEY_RIGHT:
			if( cursor_pos_x < MAP_WIDTH-1 ) cursor_pos_x++;
			break;

		case CL_KEY_SPACE:
			if( key.repeat_count > 0 ) return;
			if( (*map)[cursor_pos_x][cursor_pos_y].getId() == Cell::Type::Empty )
				(*map)[cursor_pos_x][cursor_pos_y].setType( Cell::Type::Wall );
			else
				(*map)[cursor_pos_x][cursor_pos_y].setType( Cell::Type::Empty );

			break;
	}
}

// secondary main function
int _main( const std::vector<CL_String> &args )
{
	// Setup modules
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;

	// create windows
	CL_ConsoleWindow console("Console", 80, 200);
	CL_DisplayWindow win("Clanlib", WIN_WIDTH, WIN_HEIGHT);
	window_width = WIN_WIDTH;
	window_height = WIN_HEIGHT;

	// get contexts
	CL_GraphicContext gc = win.get_gc();
	CL_InputContext ic = win.get_ic();

	// Slots
	CL_Slot keyboard_press_slot = ic.get_keyboard().sig_key_down().connect(&handle_keyboard);
	CL_Slot resize_slot = win.sig_resize().connect(&resize);

	// create a map
	map = new Map(MAP_WIDTH, MAP_HEIGHT);

	while( !quit )
	{
		gc.clear(CL_Colorf::white);

		double cell_width = (double)window_width/MAP_WIDTH;
		double cell_height = (double)window_height/MAP_HEIGHT;
		double origin_x = 0;
		double origin_y = 0;
		map->draw(gc, origin_x, origin_y, cell_width, cell_height);


		if( cursor_blink_rate-- == 0 )
		{
			cursor_blink_rate = CURSOR_BLINK_RATE;

			if( cursor_color.get_alpha() == 0 )
			{
				cursor_color.set_alpha(1.0);
			}
			else
			{
				cursor_color.set_alpha(0.0);
			}
		}
		
		gc.push_modelview();
	
		gc.set_translate( cursor_pos_x*cell_width, cursor_pos_y*cell_height, 0 );
		CL_Draw::box(gc, 0, 0, cell_width, cell_height, cursor_color );

		gc.pop_modelview();
		win.flip();


		CL_KeepAlive::process();
		CL_System::sleep(10);
	}

	return 0;
}

// create entry point
CL_ClanApplication app(&_main);
