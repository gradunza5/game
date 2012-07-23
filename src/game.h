/*
 * File:	game.h
 * Author:	James Letendre
 *
 * Contains game logic, and ties pieces together
 */
#ifndef _GAME_H_
#define _GAME_H_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/gui.h>

#include "map.h"
class GameWindow;

class Game
{
	public:
		Game( );

		void run();

		void updateLogic();
		void redraw( CL_GraphicContext &gc );

		bool quit( );
	private:

		// GUI setup
		void setup_cell_listview();

		// input events
		void handle_keyboard( const CL_InputEvent &key, const CL_InputState &state );
	    void resize();


		GameWindow *window;
		CL_ListView *cell_list;

		CL_InputContext ic;

		Map *map;
		int cursor_pos_x, cursor_pos_y;
		int window_width, window_height;
		int cursor_blink_rate;
		CL_Colorf cursor_color;
		CL_GUIManager *gui_manager;

		// slots
		CL_Slot keyboard_press_slot;

		// setup modules, because
		CL_SetupCore *setup_core;
		CL_SetupDisplay *setup_display;
		CL_SetupGL *setup_gl;
		CL_SetupGUI *setup_gui;
};

#endif
