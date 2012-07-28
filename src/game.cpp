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

#define MAP_WIDTH	100
#define MAP_HEIGHT	100

#define CELL_MIN_SIZE	20

#define CURSOR_BLINK_RATE 10

#define SCROLL_BORDER_WIDTH 20
#define SCROLL_SPEED	0.1


// static variables
CL_Sprite Game::tileset;


Game::Game( )
	: map_origin_x(0), map_origin_y(0), 
	cur_cell_id(0), 
	cursor_pos_x(0), cursor_pos_y(0), cursor_blink_rate(CURSOR_BLINK_RATE), cursor_color(CL_Color::white)
{
	// Setup modules
	setup_core = new CL_SetupCore;
	setup_display = new CL_SetupDisplay;
	setup_gl = new CL_SetupGL;
	setup_gui = new CL_SetupGUI;

	// create windows
	CL_ConsoleWindow console("Console", 80, 200);

	window_width = WIN_WIDTH;
	window_height = WIN_HEIGHT;

	// Load resources
	resources = new CL_ResourceManager("resources.xml");

	// setup GUI
	CL_GUIThemeDefault theme;
	CL_GUIWindowManagerSystem win_manager;
	gui_manager = new CL_GUIManager("resources/GUIThemeBasic");
	//gui_manager = new CL_GUIManager();
	gui_manager->set_window_manager(win_manager);

	// setup Window
	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib Game");
	desc.set_size(CL_Size(window_width, window_height), true);

	top_window = new CL_Window( gui_manager, desc );
	game_frame = new GameWindow( this, top_window );

	// load tileset
	tileset = CL_Sprite(top_window->get_gc(), "tileset", resources);

	// add list view for cell types
	setup_cell_listview();

	// setup components
	resize();

	// create the map
	map = new Map(MAP_WIDTH, MAP_HEIGHT);
	min_cell_size = CELL_MIN_SIZE;

    // TODO: remove this
    // create some test entities
    Entity e (map, 10, 10);
    entities.push_back(e);

	// setup input
	ic = top_window->get_ic();

	// Slots
	keyboard_press_slot = ic.get_keyboard().sig_key_down().connect(this, &Game::handle_keyboard);
	mouse_evt_slot      = ic.get_mouse().sig_key_down().connect(this, &Game::handle_mouse);
	cell_list->func_selection_changed().set(this, &Game::cell_selection_change);
	top_window->func_resized().set(this, &Game::resize);
	top_window->func_close().set(this, &Game::quit);
}

void Game::setup_cell_listview()
{
	// create list view
	cell_list = new CL_ListView( top_window );
	

	cell_list->set_display_mode( listview_mode_details );
	cell_list->set_multi_select( false );

	// create the column
	CL_ListViewHeader *header = cell_list->get_header();
	header->append( header->create_column( "cell_type_id", "cell type" ) ).set_width(100);

	// for each cell type, add item
	for( size_t i = 0; i < Cell::num_cell_types; i++ )
	{
		CL_ListViewItem item;
		item = cell_list->create_item();

		item.set_id( i );
		item.set_editable( false );
		item.set_column_text( "cell_type_id", Cell::Types[i].name );
		
		cell_list->get_document_item().append_child(item);
	}

	cell_list->set_scroll_position(0);
	cell_list->set_focus(false);
	cell_list->set_focus_policy( CL_GUIComponent::FocusPolicy::focus_refuse );
}

bool Game::quit()
{
	top_window->exit_with_code(0);
	return true;
}

void Game::run()
{
	gui_manager->exec();
}

void Game::updateLogic()
{
	// set new cell size
	cell_width = std::max((double)min_cell_size, (double)window_width/MAP_WIDTH);
	cell_height = std::max((double)min_cell_size, (double)window_height/MAP_HEIGHT);

	// mouse position over window
	int mouse_x = ic.get_mouse().get_x();
	int mouse_y = ic.get_mouse().get_y();

	// if we're in the game frame
	if( game_frame->get_geometry().contains( ic.get_mouse().get_position() ) )
	{
		// Scrolling
		CL_Rect frame_geom = game_frame->get_geometry();
		// X
		if( mouse_x < frame_geom.left + SCROLL_BORDER_WIDTH && map_origin_x < 0)
		{
			map_origin_x+=SCROLL_SPEED;
		}
		if( mouse_x > frame_geom.right - SCROLL_BORDER_WIDTH && map_origin_x > (double)window_width/cell_width - map->getWidth() * cell_width )
		{
			map_origin_x-=SCROLL_SPEED;
		}
		// Y
		if( mouse_y < frame_geom.top + SCROLL_BORDER_WIDTH && map_origin_y < 0)
		{
			map_origin_y+=SCROLL_SPEED;
		}
		if( mouse_y > frame_geom.bottom - SCROLL_BORDER_WIDTH && map_origin_y > (double)window_height/cell_height - map->getHeight() * cell_height )
		{
			map_origin_y-=SCROLL_SPEED;
		}

		// do bounds checking, in the case of zooming
		if( map_origin_x > 0 ) map_origin_x = 0;
		if( map_origin_y > 0 ) map_origin_y = 0;

		if( map_origin_x < (double)window_width/cell_width - map->getWidth() * cell_width ) 
			map_origin_x = (double)window_width/cell_width - map->getWidth() * cell_width;

		if( map_origin_y < (double)window_height/cell_height - map->getHeight() * cell_height ) 
			map_origin_y = (double)window_height/cell_height - map->getHeight() * cell_height;

		// convert to map space
		cursor_pos_x = (mouse_x - map_origin_x) / cell_width;
		cursor_pos_y = (mouse_y - map_origin_y) / cell_height;

		// is the mouse button down, while inside the game frame, and the cell is a different type
		if( ic.get_mouse().get_keycode( CL_MOUSE_LEFT ) &&
			cursor_pos_x >= 0 && (size_t)cursor_pos_x < map->getWidth() &&
			cursor_pos_y >= 0 && (size_t)cursor_pos_y < map->getHeight() &&
			(*map)[cursor_pos_x][cursor_pos_y].getId() != cur_cell_id )
		{
			// cell type change
			// TODO eventually take into account cost to "build" cell
			(*map)[cursor_pos_x][cursor_pos_y].setId( cur_cell_id );
		}

	}
}

void Game::redraw( CL_GraphicContext &gc )
{
	// draw the map
	map->draw( gc, map_origin_x, map_origin_y, cell_width, cell_height, window_width, window_height );

	// handle curosr blink color change
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
	
	// draw cursor if mouse is in window
	if( game_frame->get_geometry().contains( ic.get_mouse().get_position() ) )
	{
		gc.push_modelview();

		gc.set_translate( cursor_pos_x*cell_width + map_origin_x, cursor_pos_y*cell_height+map_origin_y, 0 );
		CL_Draw::box(gc, 0, 0, cell_width, cell_height, cursor_color );

		gc.pop_modelview();
	}

    // update/redraw any entities on the board
    if (entities.size() != 0)
    {
        for (size_t i = 0; i < entities.size(); i++)
        {
            entities[i].update(gc, cell_width, cell_height, map_origin_x, map_origin_y);
        }
    }

}

void Game::handle_mouse( const CL_InputEvent &evt, const CL_InputState &state )
{
	if( evt.type == CL_InputEvent::Type::pressed && game_frame->get_geometry().contains( ic.get_mouse().get_position() ) )
	{
		double mouse_x = ic.get_mouse().get_x();
		double mouse_y = ic.get_mouse().get_y();

		double frac_x = (double)(mouse_x - map_origin_x) / cell_width;
		double frac_y = (double)(mouse_y - map_origin_y) / cell_height;

		switch( evt.id )
		{
			case CL_MOUSE_WHEEL_UP:
				min_cell_size++;

				break;
			case CL_MOUSE_WHEEL_DOWN:
				min_cell_size--;
				break;

		}

		double new_cell_width = std::max((double)min_cell_size, (double)window_width/MAP_WIDTH);
		double new_cell_height = std::max((double)min_cell_size, (double)window_height/MAP_HEIGHT);

		double new_frac_x = (double)(mouse_x - map_origin_x) / new_cell_width;
		double new_frac_y = (double)(mouse_y - map_origin_y) / new_cell_height;
		
		map_origin_x += (new_frac_x-frac_x)*new_cell_width;
		map_origin_y += (new_frac_y-frac_y)*new_cell_height;
	}
}

void Game::handle_keyboard( const CL_InputEvent &key, const CL_InputState &state )
{
	switch( key.id )
	{
		case CL_KEY_ESCAPE:
			top_window->exit_with_code(0);
			break;

			/* old version
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
			if( (*map)[cursor_pos_x][cursor_pos_y].getId() == 0 )
				(*map)[cursor_pos_x][cursor_pos_y].setId( 1 );
			else
				(*map)[cursor_pos_x][cursor_pos_y].setId( 0 );

			break;
			*/
	}
}

void Game::cell_selection_change( CL_ListViewSelection sel )
{
	cur_cell_id = sel.get_first().get_item().get_id();
}

void Game::resize( )
{
	// set new game frame size
	CL_Rect area = top_window->get_client_area();
	window_width = area.get_width();
	window_height = area.get_height() - 100;

	// resize ui
	game_frame->set_geometry( CL_Rect( 0, 0, CL_Size( window_width, window_height ) ) );
	cell_list->set_geometry( CL_Rect( 0, area.bottom-100, CL_Size(area.get_width(), 100) ) );
}
