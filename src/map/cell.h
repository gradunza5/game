/*
 * File:	cell.h
 * 
 * Author:	James Letendre
 *
 * A cell from the map
 */
#ifndef CELL_H
#define CELL_H

#include <ClanLib/display.h>
#include <string.h>

class Cell
{
	public:
		typedef struct
		{
			const std::string name;
			const int type;
			const double move_cost;
			const double build_cost;
		} cell_type;

		static const cell_type Types[];
		static const size_t num_cell_types;

		/*
		 * Cell(type=Empty)
		 *
		 * Create a Cell of the specified type
		 */
		Cell(int id = 0);

		/*
		 * getBaseId()
		 *
		 * Get the base type of this Cell
		 */
		int getBaseId() const { return base_id; }

		/*
		 * setBaseId(id)
		 * 
		 * Change the base type of this cell
		 */
		void setBaseId( int id );

		/**
		 * Get the type of the improvement to this cell
		 */
		int getBuildingId() const { return improve_id; }

		/**
		 * Set the type of the improvement in the cell
		 */
		void setBuildingId( int id );

		/*
		 * draw(gc)
		 *
		 * Draw this cell
		 */
		void draw( CL_GraphicContext &gc, double width, double height, int idx ) const ;

		/*
		 * return the cell base type
		 */
		int getBaseType() const { return Cell::Types[base_id].type; }

		/**
		 * Does this cell have a building
		 */
		bool hasBuilding() const { return improve_id != -1; }
		/*
		 * return the cell improvement type
		 */
		int getBuildingType() const { return improve_id == -1 ? 0 : Cell::Types[improve_id].type; }

		/*
		 * Get the cost of movement through this cell
		 */
		double getMoveCost() const;

		/*
		 * Contribute to building this cell
		 */
		void build( double speed );

		/*
		 * Is this cell built?
		 */
		bool isBuilt() const { return improve_id == -1 || build_amount >= Cell::Types[improve_id].build_cost; }

		/*
		 * TODO: More functionality
		 */


	private:
		// The type of this cell
		int base_id, improve_id;

		// the amount built this cell is
		double build_amount;
};

#endif
