/*
 * File:	game_window.cpp
 * Author:	James Letendre
 *
 * The top level game window
 */
#include "game_window.h"
#include "game.h"

GameWindow::GameWindow( Game *game, CL_GUIManager *manager, const CL_DisplayWindowDescription &desc )
	: CL_Window( manager, desc ), game(game)
{
	// set render function and set to always redraw
	func_render().set( this, &GameWindow::on_render );
	set_constant_repaint(true);

}

void GameWindow::on_render( CL_GraphicContext &gc, const CL_Rect &clipRect )
{
	game->updateLogic();
	game->redraw(gc);

	//CL_Window::render( gc, clipRect );
}
