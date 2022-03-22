#include "Mat4.h"

const Mat4::MATRIX_ROW& Mat4::operator[](int index) const {
	return m_Data[index];
}

Mat4::MATRIX_ROW& Mat4::operator[](int index) {
	return m_Data[index];
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