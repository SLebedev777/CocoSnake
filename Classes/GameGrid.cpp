#include "GameGrid.h"
#include <cstdlib>
#include <ctime>

namespace NS_Snake
{
	GameGrid::GameGrid(int ox, int oy, int width, int height, size_t cell_size) :
		m_ox(ox), m_oy(oy), m_width(width), m_height(height), m_cellSize(cell_size), m_numOccupied(0)
	{
		m_width -= m_width % m_cellSize;
		m_height -= m_height % m_cellSize;
		m_numCellsX = m_width / m_cellSize;
		m_numCellsY = m_height / m_cellSize;
		for (size_t row = 0; row < m_numCellsY; ++row)
		{
			m_grid.push_back(std::vector<CellType>(m_numCellsX, CellType::EMPTY));
		}

		srand(time(NULL));
	}

	GameGrid::Cell GameGrid::boundToRect(Cell& cell) const
	{
		int bcix = cell.cix;
		int bciy = cell.ciy;
		if (bcix < 0)
			bcix = 0;
		if (bcix > m_numCellsX - 1)
			bcix = m_numCellsX - 1;
		if (bciy < 0)
			bciy = 0;
		if (bciy > m_numCellsY - 1)
			bciy = m_numCellsY - 1;
		return GameGrid::Cell(bcix, bciy);
	}

	GameGrid::Cell GameGrid::xyToCell(int x, int y) const
	{
		x -= m_ox;
		y -= m_oy;
		int dx = x % m_cellSize;
		int dy = y % m_cellSize;
		int cix = (x - dx) / m_cellSize;
		int ciy = (y - dy) / m_cellSize;
		return boundToRect(GameGrid::Cell(cix, ciy));
	}

	GameGrid::Cell GameGrid::xyToCell(Point2d& point) const
	{
		return xyToCell(point.x, point.y);
	}

	Point2d GameGrid::cellToXy(Cell& cell, bool bound) const
	{
		if (bound)
			cell = boundToRect(cell);
		int x = m_ox + cell.cix * m_cellSize;
		int y = m_oy + cell.ciy * m_cellSize;
		return Point2d(x, y);
	}

	Point2d GameGrid::cellToXyCenter(Cell& cell, bool bound) const
	{
		Point2d point = cellToXy(cell, bound);
		point.x += m_cellSize * 0.5f;
		point.y += m_cellSize * 0.5f;
		return point;
	}

	GameGrid::CellType GameGrid::getCellType(Cell& cell) const
	{
		cell = boundToRect(cell);
		return m_grid[cell.ciy][cell.cix];
	}

	bool GameGrid::isCellOccupied(Cell& cell) const
	{
		return getCellType(cell) != CellType::EMPTY;
	}

	bool GameGrid::occupyCell(Cell& cell, CellType type)
	{
		if (isFull())
			return false;

		cell = boundToRect(cell);
		if (isCellOccupied(cell))
			return false;
		m_grid[cell.ciy][cell.cix] = type;
		m_numOccupied++;
		return true;
	}

	void GameGrid::releaseCell(Cell& cell)
	{
		cell = boundToRect(cell);
		if (!isCellOccupied(cell))
			return;
		m_grid[cell.ciy][cell.cix] = CellType::EMPTY;
		m_numOccupied--;
	}

	bool GameGrid::getRandomFreeCell(Cell& out) const
	{
		if (isFull())
			return false;
		// TODO: replace while (true) to max_attempts loop
		while (true)
		{
			int choice_row = rand() % m_numCellsY;
			int choice_col = rand() % m_numCellsX;
			if (!isCellOccupied(Cell(choice_col, choice_row)))
			{
				out.cix = choice_col;
				out.ciy = choice_row;
				return true;
			}
		}

	}
}