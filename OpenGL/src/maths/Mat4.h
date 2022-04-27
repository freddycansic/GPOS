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
	 * @brief Constructs a 4x4 matrix using 4 float arrays of length 4.
	 * @param row1 Float array of length 4 for the first row.
	 * @param row2 Float array of length 4 for the second row.
	 * @param row3 Float array of length 4 for the third row.
	 * @param row4 Float array of length 4 for the fourth row.
	*/
	Mat4(
		const MATRIX_ROW& row1,
		const MATRIX_ROW& row2,
		const MATRIX_ROW& row3,
		const MATRIX_ROW& row4
	);
	
	/**
	 * @brief Constructs a matrix using a single float array of length 16.
	 * @param data Float array of length 16.
	*/
	Mat4(const MATRIX_DATA& data) : m_Data(data) {
	}

	/**
	 * @brief Default contructor. Initialises all values in 4x4 matrix with 0.
	 * @see Mat4::identity
	*/
	Mat4() : m_Data({0}) {}

	/**
	 * @brief Copy constructor.
	 * @param other Matrix to copy from.
	*/
	Mat4(const Mat4& other) : m_Data(other.m_Data) {}

	/**
	 * @brief Move constructor.
	 * @param other R value matrix to move data from.
	*/
	Mat4(Mat4&& other) noexcept : m_Data(std::move(other.m_Data)) {}

	/**
	 * @brief 4x4 identity matrix.
	*/
	const static Mat4 identity;

	/**
	 * @brief Move assignment operator.
	 * @param other R value matrix to move data from.
	 * @return New matrix.
	*/
	Mat4& operator=(Mat4&& other) noexcept;

	/**
	 * @brief Constant subscript operator. Indexes into the underlying data structure.
	 * @param Unsigned index of data to return.
	 * @return Constant reference to float at provided index.
	*/
	const MATRIX_ROW& operator[](unsigned int index) const;

	/**
	 * @brief Subscript operator. Indexes into the underlying data structure.
	 * @param Unsigned index of data to return.
	 * @return Modifiable reference to float at provided index.
	*/
	MATRIX_ROW& operator[](unsigned int index);

	/**
	 * @brief Assignment operator.
	 * @param mat Matrix to assign to.
	 * @return Assigned matrix.
	*/
	Mat4& operator=(const Mat4& mat);

	/**
	 * @brief Scalar matrix multiplication.
	 * @param scalar Scalar to multiply with.
	 * @return This matrix with all values multiplied with the scalar.
	 * @see operator*(const Mat4& other)
	*/
	Mat4 operator*(float scalar) const;

	/**
	 * @brief Matrix matrix multiplication.
	 * @param other Matrix to multiply with.
	 * @return This matrix multiplied with the other matrix.
	 * @see operator*(float scalar)
	*/
	Mat4 operator*(const Mat4& other) const;

	/**
	 * @brief Matrix addition.
	 * @param other Matrix to add.
	 * @return This matrix with all values added from the other matrix.
	*/
	Mat4 operator+(const Mat4& other) const;

	/**
	 * @brief Move matrix addition.
	 * @param other Matrix to add.
	 * @return This matrix with all values added from the other matrix.
	 * @see operator+(const Mat4& other)
	*/
	Mat4 operator+(Mat4&& other) const;

	/**
	 * @brief Scale operation.
	 * @param xScale Magnitude to scale on the x axis.
	 * @param yScale Magnitude to scale on the y axis.
	 * @param zScale Magnitude to scale on the z axis.
	 * @return New scaled matrix.
	*/
	Mat4 scale(float xScale, float yScale, float zScale) const;

	/**
	 * @brief Scale operation.
	 * @param xyzScale Magnitude to scale on the x, y and z axis.
	 * @return New scaled matrix.
	*/
	Mat4 scale(float xyzScale) const { return scale(xyzScale, xyzScale, xyzScale); }

	/**
	 * @brief Rotation operation.
	 * @param xRotate Angle in degrees to rotate on the x axis.
	 * @param yRotate Angle in degrees to rotate on the y axis.
	 * @param zRotate Angle in degrees to rotate on the z axis.
	 * @return New rotated matrix.
	*/
	Mat4 rotate(float xRotate, float yRotate, float zRotate) const;

	/**
	 * @brief Translation operation.
	 * @param xTranslate Distance to translate on the x axis.
	 * @param yTranslate Distance to translate on the y axis.
	 * @param zTranslate Distance to translate on the z axis.
	 * @return New translated matrix.
	*/
	Mat4 translate(float xTranslate, float yTranslate, float zTranslate) const;

	/**
	 * @brief Generates an orthographic projection matrix.
	 * @param left Left plane.
	 * @param right Right plane.
	 * @param top Top plane.
	 * @param bottom Bottom plane.
	 * @param near Near plane. Default = -1.
	 * @param far Far plane. Default = 1.
	 * @return Orthographic projection matrix.
	*/
	static Mat4 ortho(float left, float right, float top, float bottom, float near = -1.0f, float far = 1.0f);
	
	/**
	 * @brief Generates a perspective projection matrix.
	 * @param fov Field of view.
	 * @param aspect Aspect ratio of screen.
	 * @param near Near plane. Default = -1.
	 * @param far Far plane. Default = 1.
	 * @return Perspective projection matrix.
	*/
	static Mat4 perspective(float fov, float aspect, float near = -1.0f, float far = 1.0f);

	/**
	 * @brief Pointer to the first element in the underlying data structure.
	 * @return Pointer to the first element in the underlying data structure.
	*/
	const float* getPtr() const { return m_Data[0].data(); }

	friend std::ostream& operator<<(std::ostream& os, const Mat4& matrix);

};