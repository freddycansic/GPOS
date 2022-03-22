#include "Mat4.h"

Mat4::Mat4(
	float r1c1, float r1c2, float r1c3, float r1c4,
	float r2c1, float r2c2, float r2c3, float r2c4,
	float r3c1, float r3c2, float r3c3, float r3c4,
	float r4c1, float r4c2, float r4c3, float r4c4) {

	m_Data = { { // weird double brackets
		{r1c1, r1c2, r1c3, r1c4},
		{r2c1, r2c2, r2c3, r2c4},
		{r3c1, r3c2, r3c3, r3c4},
		{r4c1, r4c2, r4c3, r4c4}
	} };

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

Mat4& Mat4::operator=(Mat4&& other) noexcept {
	
	if (this != &other) {
		m_Data = std::move(other.m_Data);
		return *this;
	}
	
	return *this;
}

Mat4::Mat4(int element) {
	for (int i = 0; i < ORDER; i++) {
		memset(&m_Data[i], element, ORDER * sizeof(float));
	}
}

const Mat4::MATRIX_ROW& Mat4::operator[](int index) const {
	return m_Data[index];
}

Mat4::MATRIX_ROW& Mat4::operator[](int index) {
	return m_Data[index];
}

Mat4 Mat4::operator+(const Mat4& other) const noexcept {
	
	Mat4 result = Mat4();

	for (int row = 0; row < Mat4::ORDER; row++) {
		for (int col = 0; col < Mat4::ORDER; col++) {
			result[row][col] = m_Data[row][col] + other[row][col];
		}
	}

	return result;
}

Mat4 Mat4::operator+(Mat4&& other) const noexcept { // saves on memory as we don't have to construct another matrix to store result in, instead we can steal the memory already allocated in the temporary rvalue
	for (int row = 0; row < Mat4::ORDER; row++) {
		for (int col = 0; col < Mat4::ORDER; col++) {
			other[row][col] = m_Data[row][col] + other[row][col];
		}
	}

	return other;
}

Mat4 Mat4::operator*(float scalar) const noexcept {
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

Mat4 Mat4::operator*(const Mat4& other) const noexcept {
	
	Mat4 result = Mat4(0);

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

Mat4::operator const char *() {
	std::string output;
	for (int row = 0; row < Mat4::ORDER; row++) {
		output += "| ";
		for (int col = 0; col < Mat4::ORDER; col++) {
			output += m_Data[row][col];
		}
		output += "|\n";
	}

	return output.c_str();
}

Mat4::operator std::string() {
	std::string output;
	for (int row = 0; row < Mat4::ORDER; row++) {
		output += "| ";
		for (int col = 0; col < Mat4::ORDER; col++) {
			output += std::to_string(m_Data[row][col]) + "\t";
		}
		output += "|\n";
	}

	return output;
}