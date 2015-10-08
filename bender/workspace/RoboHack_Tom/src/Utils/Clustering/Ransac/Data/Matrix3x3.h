/*
 * Matrix3x3.h
 *
 *  Created on: Apr 7, 2012
 *      Author: Jan Carstensen
 */

#ifndef MATRIX3X3_H_
#define MATRIX3X3_H_

using namespace std;

class Matrix3x3 {
public:

	Matrix3x3(const int firstCol[3], const int secondCol[3], const int thirdCol[3]);
	Matrix3x3(const int data[3][3]);
	Matrix3x3(const Matrix3x3* matrix);
	virtual ~Matrix3x3();

	void setValue(const int data[3][3]);
	void setValue(int row, int collumn, int value);


	void setCollumn(int column, const int data[3]);
	void setRow(int row, const int data[3]);

	int* getCollumn(int column) const;
	int* getRow(int row);

	int calculateDeterminante() const;

	int mData[3][3];

private:




};

#endif /* MATRIX3X3_H_ */
