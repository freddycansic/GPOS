#pragma once

#include <array>
#include <iostream>
#include <string>

using std::array;

class Mat4
{
public:
	constexpr static unsigned int ORDER = 4;
	
	using MATRIX_DATA = array<array<float, ORDER>, ORDER>;
	using MATRIX_ROW = array<float, ORDER>;

	Mat4(
		float r1c1, float r1c2, float r1c3, float r1c4,
		float r2c1, float r2c2, float r2c3, float r2c4,
		float r3c1, float r3c2, float r3c3, float r3c4,
		float r4c1, float r4c2, float r4c3, float r4c4) {

		m_Data = { { // weird double brackets
			{r1c1, r1c2, r1c3, r1c4},
			{r2c1, r2c2, r2c3, r2c4},
			{r3c1, r3c2, r3c3, r3c4},
			{r4c1, r4c2, r4c3, r4c4}
		}};

	}

	Mat4(const MATRIX_DATA& data) : m_Data(data) {
	}

	Mat4(
		const MATRIX_ROW& row1,
		const MATRIX_ROW& row2,
		const MATRIX_ROW& row3,
		const MATRIX_ROW& row4
	)
	{
		m_Data[0] = row1;
		m_Data[1] = row2;
		m_Data[2] = row3;
		m_Data[3] = row4;
	}

	Mat4(int element) {
		for (int i = 0; i < ORDER; i++) {
			memset(&m_Data[i], element, ORDER * sizeof(float));
		}
	}

	// default constructor as if we called it with 0 as param
	Mat4() : Mat4(0) {
	}

	Mat4(const Mat4& other) : m_Data(other.m_Data) {
		std::cout << "Copied" << std::endl;
	}

	Mat4(Mat4&& other) : m_Data(other.m_Data) {
		std::cout << "Copied" << std::endl;
	}

	static Mat4 identity() {
		return Mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}

	const MATRIX_ROW& operator[](int index) const;
	MATRIX_ROW& operator[](int index);

	Mat4 operator*(float scalar) const; // scalar multiplication
	Mat4 operator*(const Mat4& other) const; // matrix matrix multiplication
	Mat4 operator+(const Mat4& other) const; // matrix matrix addition
	Mat4 operator+(Mat4&& other) const; // rvalue reference
	int determinant() const;

	operator const char*();
	operator std::string();
	friend std::ostream& operator<<(std::ostream& os, const Mat4& matrix);

private:
	MATRIX_DATA m_Data;

};