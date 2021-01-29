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
		GameGrid(int ox, int oy, int width, int height, size_t cell_size);
		Cell boundToRect(Cell& cell) const;
		Cell xyToCell(int x, int y) const;
		Cell xyToCell(Point2d& point) const;
		Point2d cellToXy(Cell& cell, bool bound = true) const;
		Point2d cellToXyCenter(Cell& cell, bool bound = true) const;
		bool isCellOccupied(Cell& cell) const;
		CellType getCellType(Cell& cell) const;
		bool occupyCell(Cell& cell, CellType type = CellType::OCCUPIED);  // you can not occupy cell just assigning some other cell type. You must release it first
		void releaseCell(Cell& cell);
		//getCellRect(Cell& cell);
		bool getRandomFreeCell(Cell& out) const;
		bool isFull() const { return m_numOccupied >= m_numCellsX * m_numCellsY; }
		Point2d getOrigin() const { return Point2d(m_ox, m_oy); }
		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }
		size_t getCellSize() const { return m_cellSize; }
		size_t getNumCellsY() const { return m_numCellsY; }
		size_t getNumCellsX() const { return m_numCellsX; }
		bool isLocked() const { return m_lock; }
		void lock() { m_lock = true; }
		void unlock() { m_lock = false; }

	private:
		std::vector<std::vector<CellType>> m_grid;
		int m_ox;  // origin
		int m_oy;
		int m_width;
		int m_height;
		size_t m_cellSize;
		size_t m_numCellsX;  // num columns
		size_t m_numCellsY;  // num rows
		size_t m_numOccupied;
		bool m_lock;

	};

}
#endif // __GAMEGRID_H__