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

	// constructors
	Mat4(
		float r1c1, float r1c2, float r1c3, float r1c4,
		float r2c1, float r2c2, float r2c3, float r2c4,
		float r3c1, float r3c2, float r3c3, float r3c4,
		float r4c1, float r4c2, float r4c3, float r4c4
	);

	Mat4(
		const MATRIX_ROW& row1,
		const MATRIX_ROW& row2,
		const MATRIX_ROW& row3,
		const MATRIX_ROW& row4
	);
	
	Mat4(const MATRIX_DATA& data) : m_Data(data) {
	}

	// pass in number to fill matrix with that number
	Mat4(int element);

	// default constructor as if we called it with 0 as param
	Mat4() : Mat4(0) {}

	// copy constructor
	Mat4(const Mat4& other) : m_Data(other.m_Data) {}

	// move constructors (pog)
	Mat4(MATRIX_DATA&& data) noexcept : m_Data(std::move(data)) {}

	// not sure if i need to set other.m_Data to null or not
	Mat4(Mat4&& other) : m_Data(std::move(other.m_Data)) {}

	// gross
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

	Mat4& operator=(Mat4&& other) noexcept; // move assignment operator

	// mathematical operations
	Mat4 operator*(float scalar) const; // scalar multiplication
	Mat4 operator*(const Mat4& other) const; // matrix matrix multiplication
	
	Mat4 operator+(const Mat4& other) const; // matrix matrix addition
	Mat4 operator+(Mat4&& other) const; // rvalue reference

	// transformations
	static Mat4 scale(float x, float y);

	const float* getPtr() const { return m_Data[0].data();  }

	operator const char*();
	operator std::string();
	friend std::ostream& operator<<(std::ostream& os, const Mat4& matrix);

private:
	MATRIX_DATA m_Data;

};