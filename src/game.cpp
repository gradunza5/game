/*
 * File:	game.cpp
 * Author:	James Letendre
 *
 * Contains game logic, and ties pieces together
 */
#include "game.h"
#include "game_window.h"

#define WIN_WIDTH	500
#define WIN_HEIGHT	500

#define MAP_WIDTH	50
#define MAP_HEIGHT	50

#define CURSOR_BLINK_RATE 10

Game::Game( )
	: cursor_pos_x(0), cursor_pos_y(0), cursor_blink_rate(CURSOR_BLINK_RATE), cursor_color(CL_Color::white)
{
	// Setup modules
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;

	// create windows
	CL_ConsoleWindow console("Console", 80, 200);

	window_width = WIN_WIDTH;
	window_height = WIN_HEIGHT;

	// setup GUI
	CL_GUIThemeDefault theme;
	CL_GUIWindowManagerSystem win_manager;
	gui_manager = new CL_GUIManager();
	gui_manager->set_window_manager(win_manager);

	// setup Window
	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib Game");
	desc.set_size(CL_Size(window_width, window_height), true);

	window = new GameWindow(this, gui_manager, desc);

	// create the map
	map = new Map(MAP_WIDTH, MAP_HEIGHT);

	// setup input
	ic = window->get_ic();

	// Slots
	keyboard_press_slot = ic.get_keyboard().sig_key_down().connect(this, &Game::handle_keyboard);
	window->func_resized().set(this, &Game::resize);
	window->func_close().set(this, &Game::quit);
}

bool Game::quit()
{
	window->exit_with_code(0);
	return true;
}

void Game::run()
{
	gui_manager->exec();
}

void Game::updateLogic()
{
	
}

void Game::redraw( CL_GraphicContext &gc )
{
	double cell_width = (double)window_width/MAP_WIDTH;
	double cell_height = (double)window_height/MAP_HEIGHT;

	double origin_x = 0;
	double origin_y = 0;

	map->draw( gc, origin_x, origin_y, cell_width, cell_height );

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

}

void Game::handle_keyboard( const CL_InputEvent &key, const CL_InputState &state )
{
	switch( key.id )
	{
		case CL_KEY_ESCAPE:
			window->exit_with_code(0);
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

void Game::resize( )
{
	CL_Rect area = window->get_client_area();
	window_width = area.get_width();
	window_height = area.get_height();
}
