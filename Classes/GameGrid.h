#ifndef __GAMEGRID_H__
#define __GAMEGRID_H__

#include "Snake_types.h"
#include <vector>


namespace NS_Snake
{
	class GameGrid
	{
	public:
		struct Cell
		{
			Cell(int _cix, int _ciy) : cix(_cix), ciy(_ciy)
			{}

			int cix;
			int ciy;
		};

		enum class CellType
		{
			EMPTY = 0,
			OCCUPIED
			// supports extension to some specific cell types: food, wall...
		};

	public:
		GameGrid(int x, int y, int width, int height, size_t cell_size);
		Cell boundToRect(Cell& cell) const;
		Cell xyToCell(int x, int y) const;
		Cell xyToCell(Point2d& point) const;
		Point2d cellToXy(Cell& cell, bool bound = true) const;
		bool isCellOccupied(Cell& cell) const;
		CellType getCellType(Cell& cell) const;
		bool occupyCell(Cell& cell, CellType type = CellType::OCCUPIED);  // you can not occupy cell just assigning some other cell type. You must release it first
		void releaseCell(Cell& cell);
		//getCellRect(Cell& cell);
		bool getRandomFreeCell(Cell& out) const;
		bool isFull() const { return m_numOccupied >= m_numCellsX * m_numCellsY; }

	private:
		std::vector<std::vector<CellType>> m_grid;
		int m_x;
		int m_y;
		int m_width;
		int m_height;
		size_t m_cellSize;
		size_t m_numCellsX;  // num columns
		size_t m_numCellsY;  // num rows
		size_t m_numOccupied;

	};

}
#endif // __GAMEGRID_H__