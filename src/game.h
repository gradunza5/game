/*
 * File:	game.h
 * Author:	James Letendre
 *
 * Contains game logic, and ties pieces together
 */
#ifndef _GAME_H_
#define _GAME_H_

#include <vector>

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/gui.h>

#include "map.h"
#include "entity.h"

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
		void cell_selection_change( CL_ListViewSelection sel );
	    void resize();

		CL_GUIManager *gui_manager;

		CL_Window *top_window;
		GameWindow *game_frame;
		CL_ListView *cell_list;

		CL_InputContext ic;

		// the map of cells
		Map *map;
		double map_origin_x;
		double map_origin_y;

		int cur_cell_id;
		double cell_width;
		double cell_height;

		// cursor data
		int cursor_pos_x, cursor_pos_y;
		int cursor_blink_rate;
		CL_Colorf cursor_color;

		// size of game frame
		int window_width, window_height;


        // list of entities
        std::vector<Entity> entities;

		// slots
		CL_Slot keyboard_press_slot;

		// setup modules, because
		CL_SetupCore *setup_core;
		CL_SetupDisplay *setup_display;
		CL_SetupGL *setup_gl;
		CL_SetupGUI *setup_gui;
};

#endif
