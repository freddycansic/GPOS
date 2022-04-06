#pragma once

#include <array>
#include <iostream>
#include <string>

#include <maths/Vectors.h>

class Mat4
{
public:
	constexpr static unsigned int ORDER = 4;
	
	using MATRIX_ROW = std::array<float, ORDER>;
	using MATRIX_DATA = std::array<MATRIX_ROW, ORDER>;

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
	Mat4(Mat4&& other) noexcept : m_Data(std::move(other.m_Data)) {}

	const static Mat4 identity;

	const MATRIX_ROW& operator[](int index) const;
	MATRIX_ROW& operator[](int index);

	Mat4& operator=(Mat4&& other) noexcept; // move assignment operator

	// mathematical operations
	Mat4 operator*(float scalar) const; // scalar multiplication
	Mat4 operator*(const Mat4& other) const; // matrix matrix multiplication
	
	template<typename T>
	Vec4<T> operator*(const Vec4<T>& other) const {
		T values[]{ other.x, other.y, other.z, other.w }; // put it into an array so i can subscript operator them

		for (int row = 0; row < Mat4::ORDER; row++) { // for each row in matrix4
			T total = 0; // running total
			for (int element = 0; element < 4; element++) { // for each element in vector
				total += m_Data[row][element] * values[element];
			}
			values[row] = total;
		}

		return Vec4<T>(values);

	}

	Mat4 operator+(const Mat4& other) const; // matrix matrix addition
	Mat4 operator+(Mat4&& other) const; // rvalue reference

	// transformations
	Mat4 scale(float xScale, float yScale, float zScale) const;
	Mat4 scale(float xyzScale) const { return scale(xyzScale, xyzScale, xyzScale); }
	Mat4 rotate(float xRotate, float yRotate, float zRotate) const;
	Mat4 translate(float xTranslate, float yTranslate, float zTranslate) const;

	// projection
	static Mat4 ortho(float left, float right, float top, float bottom, float near = -1.0f, float far = 1.0f);

	const float* getPtr() const { return m_Data[0].data(); }

	operator std::string();
	friend std::ostream& operator<<(std::ostream& os, const Mat4& matrix);

private:
	MATRIX_DATA m_Data;

};