/*
 * File:	game_window.h
 * Author:	James Letendre
 *
 * The top level game window
 */
#ifndef _GAME_WINDOW_H_
#define _GAME_WINDOW_H_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/gui.h>

class Game;

class GameWindow : public CL_Window
{
	public:
		GameWindow( Game *game, CL_GUIManager *manger, const CL_DisplayWindowDescription &desc );

	private:
		void render( CL_GraphicContext &gc, const CL_Rect &clipRect );

		Game *game;
};

#endif
