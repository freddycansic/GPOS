#include "Mat4.h"

#include "Maths.h"
#include "Vectors.h"

Mat4::Mat4(
	float r1c1, float r1c2, float r1c3, float r1c4,
	float r2c1, float r2c2, float r2c3, float r2c4,
	float r3c1, float r3c2, float r3c3, float r3c4,
	float r4c1, float r4c2, float r4c3, float r4c4) :
	m_Data( {{
		{r1c1, r1c2, r1c3, r1c4},
		{r2c1, r2c2, r2c3, r2c4},
		{r3c1, r3c2, r3c3, r3c4},
		{r4c1, r4c2, r4c3, r4c4}
	}})
{
}

Mat4::Mat4(
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

const Mat4 Mat4::identity = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

Mat4& Mat4::operator=(Mat4&& other) noexcept {
	
	if (this != &other) {
		m_Data = other.m_Data;
		return *this;
	}
	
	return *this;
}

const Mat4::MATRIX_ROW& Mat4::operator[](unsigned int index) const {
	return m_Data[index];
}

Mat4::MATRIX_ROW& Mat4::operator[](unsigned int index) {
	return m_Data[index];
}

Mat4& Mat4::operator=(const Mat4& mat) {
	m_Data = mat.m_Data;
	return *this;
}

Mat4 Mat4::operator+(const Mat4& other) const {
	
	Mat4 result = Mat4();

	for (int row = 0; row < Mat4::ORDER; row++) {
		for (int col = 0; col < Mat4::ORDER; col++) {
			result[row][col] = m_Data[row][col] + other[row][col];
		}
	}

	return result;
}

Mat4 Mat4::operator+(Mat4&& other) const { // saves on memory as we don't have to construct another matrix to store result in, instead we can steal the memory already allocated in the temporary rvalue
	for (int row = 0; row < Mat4::ORDER; row++) {
		for (int col = 0; col < Mat4::ORDER; col++) {
			other[row][col] = m_Data[row][col] + other[row][col];
		}
	}

	return other;
}

Mat4 Mat4::scale(float xScale, float yScale, float zScale) const
{
	return 
		*this

		*

		Mat4 (
		xScale, 0, 0, 0,
		0, yScale, 0, 0,
		0, 0, zScale, 0,
		0, 0, 0, 1
	);
}

Mat4 Mat4::rotate(float xRotate, float yRotate, float zRotate) const
{
	const float xRadRotate = Maths::radians(xRotate);
	const float yRadRotate = Maths::radians(yRotate);
	const float zRadRotate = Maths::radians(zRotate);

	return (
		*this

		*

		Mat4 (
		1, 0, 0, 0,
		0, cos(xRadRotate), -sin(xRadRotate), 0,
		0, sin(xRadRotate), cos(xRadRotate), 0,
		0, 0, 0, 1) 
		
		*
		
		Mat4 (
		cos(yRadRotate), 0, sin(yRadRotate), 0,
		0, 1, 0, 0,
		-sin(yRadRotate), 0, cos(yRadRotate), 0,
		0, 0, 0, 1) 
		
		*
		
		Mat4 (
		cos(zRadRotate), -sin(zRadRotate), 0, 0,
		sin(zRadRotate), cos(zRadRotate), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1)
	);
}

Mat4 Mat4::rotateAxis(float xAxis, float yAxis, float zAxis, float angle) const
{
	const float theta = Maths::radians(angle);

	return (
		*this

		*

		Mat4(
			cos(theta) + xAxis*xAxis*(1-cos(theta)), xAxis*yAxis*(1-cos(theta))-zAxis*sin(theta), xAxis*zAxis*(1-cos(theta)) + yAxis*sin(theta), 0,
			yAxis*xAxis*(1-cos(theta)) + zAxis*sin(theta), cos(theta) + yAxis*yAxis*(1-cos(theta)), yAxis*zAxis*(1-cos(theta))-xAxis*sin(theta), 0,
			zAxis*xAxis*(1-cos(theta))-yAxis*sin(theta), zAxis*yAxis*(1-cos(theta)) + xAxis*sin(theta), cos(theta) + zAxis*zAxis*(1-cos(theta)), 0,
			0, 0, 0, 1
		));
}

Mat4 Mat4::translate(float xTranslate, float yTranslate, float zTranslate) const {
	return 
		*this 
		
		* 
		
		Mat4(
		1, 0, 0, xTranslate,
		0, 1, 0, yTranslate,
		0, 0, 1, zTranslate,
		0, 0, 0, 1
	);
}

Mat4 Mat4::operator*(float scalar) const {
	// copy current matrix
	Mat4 result = *this;
	
	// multiply each element by the scalar
	for (int row = 0; row < Mat4::ORDER; row++) {
		for (int col = 0; col < Mat4::ORDER; col++) {
			result[row][col] = scalar * m_Data[row][col];
		}
	}

	return result;
}

Mat4 Mat4::operator*(const Mat4& other) const {
	
	Mat4 result;

	// for each row in matrix A
	for (int k = 0; k < Mat4::ORDER; k++) {
		// for every column in matrix B
		for (int j = 0; j < Mat4::ORDER; j++) {
			// for every piece of data in the column and row
			for (int i = 0; i < Mat4::ORDER; i++) {
				// sum of them
				result[k][j] += m_Data[k][i] * other[i][j];
			}

		}
	}

	return result;
}

Mat4 Mat4::ortho(float left, float right, float top, float bottom, float near, float far) {
	return {
		2 / (right - left), 0, 0, -(right + left) / (right - left),
		0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
		0, 0, -2 / (far - near), -(far + near) / (far - near),
		0, 0, 0, 1
	};
}

Mat4 Mat4::perspective(float fovDeg, float aspect, float near, float far) {
	const float fov = Maths::radians(fovDeg);
	const float f = 1 / tan(fov/2);

	return {
		f / aspect, 0, 0, 0,
		0, f, 0, 0,
		0, 0, (far + near) / (near - far), (2 * far * near) / (near - far),
		0, 0, -1, 0
	};
}

Mat4 Mat4::lookAt(const Vec3& position, const Vec3& target, const Vec3& up)
{
	const Vec3 camDir = (position - target).normalise(); // vector from cam pos to target
	const Vec3 camRight = (camDir.cross(up)).normalise(); // vector pointing in right direction from camera
	const Vec3 camUp = camDir.cross(camRight); // vector pointing in up direction from camera

	return

	Mat4(
		camRight.x, camRight.y, camRight.z, 0,
		camUp.x, camUp.y, camUp.z, 0,
		camDir.x, camDir.y, camDir.z, 0,
		0, 0, 0, 1
	)

	*

	Mat4(
		1, 0, 0, -position.x,
		0, 1, 0, -position.y,
		0, 0, 1, -position.z,
		0, 0, 0, 1
	);
}

std::ostream& operator<<(std::ostream& os, const Mat4& matrix) {
	for (int row = 0; row < Mat4::ORDER; row++) {
		os << "| ";
		for (int col = 0; col < Mat4::ORDER; col++) {
			os << matrix[row][col] << "\t";
		}
		os << "|\n";
	}

	return os;
};