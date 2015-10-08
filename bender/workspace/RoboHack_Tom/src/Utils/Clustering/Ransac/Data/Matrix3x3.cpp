/*
 * Matrix3x3.cpp
 *
 *  Created on: Apr 7, 2012
 *      Author: Jan Carstensen
 */

#include "Matrix3x3.h"


Matrix3x3::Matrix3x3(const int data[3][3]) {
	for (int row = 0; row < 3; ++row) {
		for (int column = 0; column < 3; ++column) {
			mData[row][column] = data[row][column];
		}
	}

}

Matrix3x3::Matrix3x3(const int firstCol[3], const int secondCol[3], const int thirdCol[3]){
	for (int row = 0; row < 3; ++row) {
		mData[0][row] = firstCol[row];
		mData[1][row] = secondCol[row];
		mData[2][row] = thirdCol[row];
	}
}

Matrix3x3::Matrix3x3(const Matrix3x3* matrix) {
	for (int row = 0; row < 3; ++row) {
		for (int column = 0; column < 3; ++column) {
			mData[row][column] = matrix->mData[row][column];
		}
	}
}

Matrix3x3::~Matrix3x3() {

}

void Matrix3x3::setValue(const int data[3][3]) {
	for (int row = 0; row < 3; ++row) {
		for (int column = 0; column < 3; ++column) {
			mData[row][column] = data[row][column];
		}
	}
}

void Matrix3x3::setValue(int row, int collumn, int value) {
	mData[row][collumn] = value;
}

void Matrix3x3::setCollumn(int column, const int data[3]) {
	for (int i = 0; i < 3; ++i) {
		mData[i][column] = data[i];
	}
}

void Matrix3x3::setRow(int row, const int data[3]) {
	for (int i = 0; i < 3; ++i) {
		mData[row][i] = data[i];
	}
}

int* Matrix3x3::getCollumn(int column) const{
	int* data = new int[3];
	for (int i = 0; i < 3; ++i) {
		data[i] = mData[i][column];
	}
	return data;
}

int* Matrix3x3::getRow(int row){
	return mData[row];
}

int Matrix3x3::calculateDeterminante() const {
	int det = 0;

	det += mData[0][0] * mData[1][1] * mData[2][2];
	det += mData[0][1] * mData[1][2] * mData[2][0];
	det += mData[0][2] * mData[1][0] * mData[2][1];
	det -= mData[0][2] * mData[1][1] * mData[2][0];
	det -= mData[0][1] * mData[1][0] * mData[2][2];
	det -= mData[0][0] * mData[1][2] * mData[2][1];

	return det;
}
