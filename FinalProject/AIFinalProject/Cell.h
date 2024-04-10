#pragma once

/**
 * @brief Represents a cell in a grid.
 */
class Cell
{
private:
	int row; /**< The row index of the cell. */
	int column; /**< The column index of the cell. */
	double distance; /**< The distance of the cell to a target (if applicable). */
	Cell* parent; /**< Pointer to the parent cell (if applicable). */

public:
	/**
	 * @brief Default constructor for the Cell class.
	 */
	Cell();

	/**
	 * @brief Parameterized constructor for the Cell class.
	 * @param r The row index of the cell.
	 * @param c The column index of the cell.
	 * @param p Pointer to the parent cell.
	 */
	Cell(int r, int c, Cell* p);

	/**
	 * @brief Parameterized constructor for the Cell class.
	 * @param r The row index of the cell.
	 * @param c The column index of the cell.
	 * @param distance The distance of the cell to a target.
	 * @param p Pointer to the parent cell.
	 */
	Cell(int r, int c, double distance, Cell* p);

	/**
	 * @brief Sets the row index of the cell.
	 * @param x The row index to set.
	 */
	void setRow(int x) { row = x; }

	/**
	 * @brief Sets the column index of the cell.
	 * @param y The column index to set.
	 */
	void setColumn(int y) { column = y; }

	/**
	 * @brief Destructor for the Cell class.
	 */
	~Cell();

	/**
	 * @brief Gets the row index of the cell.
	 * @return The row index of the cell.
	 */
	int getRow() { return row; }

	/**
	 * @brief Gets the column index of the cell.
	 * @return The column index of the cell.
	 */
	int getColumn() { return column; }

	/**
	 * @brief Gets the distance of the cell to a target.
	 * @return The distance of the cell to a target.
	 */
	double getDistanceToTarget() { return distance; }

	/**
	 * @brief Gets a reference to the parent cell.
	 * @return Reference to the parent cell.
	 */
	Cell*& getParent() { return parent; }
};
