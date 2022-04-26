#pragma once

#include <array>
#include <iostream>
#include <string>

class Mat4
{
public:
	constexpr static unsigned int ORDER = 4;

	using MATRIX_ROW = std::array<float, ORDER>;
	using MATRIX_DATA = std::array<MATRIX_ROW, ORDER>;

private:
	/**
	 * @brief Internal data structure.
	*/
	MATRIX_DATA m_Data;

public:
	/**
	 * @brief Constructs a 4x4 matrix using 16 floats.
	 * @param r1c1 Value for row 1, column 1.
	 * @param r1c2 Value for row 1, column 2.
	 * @param r1c3 Value for row 1, column 3.
	 * @param r1c4 Value for row 1, column 4.
	 * @param r2c1 Value for row 2, column 1.
	 * @param r2c2 Value for row 2, column 2.
	 * @param r2c3 Value for row 2, column 3.
	 * @param r2c4 Value for row 2, column 4.
	 * @param r3c1 Value for row 3, column 1.
	 * @param r3c2 Value for row 3, column 2.
	 * @param r3c3 Value for row 3, column 3.
	 * @param r3c4 Value for row 3, column 4.
	 * @param r4c1 Value for row 4, column 1.
	 * @param r4c2 Value for row 4, column 2.
	 * @param r4c3 Value for row 4, column 3.
	 * @param r4c4 Value for row 4, column 4.
	*/
	Mat4(
		float r1c1, float r1c2, float r1c3, float r1c4,
		float r2c1, float r2c2, float r2c3, float r2c4,
		float r3c1, float r3c2, float r3c3, float r3c4,
		float r4c1, float r4c2, float r4c3, float r4c4
	);

	/**
	 * @brief Constructs a 4x4 matrix using 4 arrays of length 4.
	 * @param row1 Values for the first row.
	 * @param row2 Values for the second row.
	 * @param row3 Values for the third row.
	 * @param row4 Values for the fourth row.
	*/
	Mat4(
		const MATRIX_ROW& row1,
		const MATRIX_ROW& row2,
		const MATRIX_ROW& row3,
		const MATRIX_ROW& row4
	);
	
	/**
	 * @brief Constructs a
	 * @param data 
	*/
	Mat4(const MATRIX_DATA& data) : m_Data(data) {
	}

	Mat4() : m_Data({0}) {}

	// copy constructor
	Mat4(const Mat4& other) : m_Data(other.m_Data) {}

	// move constructors (pog)
	Mat4(MATRIX_DATA&& data) noexcept : m_Data(std::move(data)) {}

	// not sure if i need to set other.m_Data to null or not
	Mat4(Mat4&& other) noexcept : m_Data(std::move(other.m_Data)) {}

	const static Mat4 identity;

	// functional operators
	Mat4& operator=(Mat4&& other) noexcept; // move assignment operator
	const MATRIX_ROW& operator[](int index) const;
	MATRIX_ROW& operator[](int index);
	Mat4& operator=(const Mat4& mat);

	// mathematical operations
	Mat4 operator*(float scalar) const; // scalar multiplication
	Mat4 operator*(const Mat4& other) const; // matrix matrix multiplication
	Mat4 operator+(const Mat4& other) const; // matrix matrix addition
	Mat4 operator+(Mat4&& other) const; // rvalue reference

	// transformations
	Mat4 scale(float xScale, float yScale, float zScale) const;
	Mat4 scale(float xyzScale) const { return scale(xyzScale, xyzScale, xyzScale); }
	Mat4 rotate(float xRotate, float yRotate, float zRotate) const;
	Mat4 translate(float xTranslate, float yTranslate, float zTranslate) const;

	// projection
	static Mat4 ortho(float left, float right, float top, float bottom, float near = -1.0f, float far = 1.0f);
	static Mat4 perspective(float fov, float aspect, float near, float far);

	const float* getPtr() const { return m_Data[0].data(); }

	friend std::ostream& operator<<(std::ostream& os, const Mat4& matrix);

};