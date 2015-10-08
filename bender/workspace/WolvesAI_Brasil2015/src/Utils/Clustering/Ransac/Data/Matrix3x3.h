/*
 * Matrix3x3.h
 *
 *  Created on: Apr 7, 2012
 *      Author: Jan Carstensen
 */

#ifndef MATRIX3X3_H_
#define MATRIX3X3_H_

using namespace std;

/**
 * Matrix3x3 is a matrix for 3 x 3 integer values
 */
class Matrix3x3 {
public:

	/**
	 * Constructor
	 * @param firstCol		first column
	 * @param secondCol		second column
	 * @param thirdCol		third column
	 */
	Matrix3x3(const int firstCol[3], const int secondCol[3], const int thirdCol[3]);

	/**
	 * Constructor
	 * @param data		given integer array
	 */
	Matrix3x3(const int data[3][3]);

	/**
	 * Constructor
	 * @param matrix	other matrix
	 */
	Matrix3x3(const Matrix3x3* matrix);

	/**
	 * Destructor
	 */
	virtual ~Matrix3x3();

	/**
	 * set matrix
	 * @param data		given integer array
	 */
	void setValue(const int data[3][3]);

	/**
	 * set matrix entry with value
	 * @param row		index of row
	 * @param column	index of column
	 * @param value		value
	 */
	void setValue(int row, int column, int value);


	/**
	 * set column
	 * @param column	index of column
	 * @param data		integer array
	 */
	void setColumn(int column, const int data[3]);

	/**
	 * set row
	 * @param row		index of row
	 * @param data		integer array
	 */
	void setRow(int row, const int data[3]);

	/**
	 * get column
	 * @param column	index of column
	 * @return column values
	 */
	int* getColumn(int column) const;

	/**
	 * get row
	 * @param row		index of row
	 * @return row values
	 */
	int* getRow(int row);

	/**
	 * calculate determinante
	 * @return determinante
	 */
	int calculateDeterminante() const;

private:
	int mData[3][3];	//!< matrix
};

#endif /* MATRIX3X3_H_ */
