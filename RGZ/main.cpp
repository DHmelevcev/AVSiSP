#include <vector>
#include <string>
#include <ctime>
#include <stdexcept>
#include <chrono>
#include <iostream>
#include <iomanip>

class MyMatrix
{
	std::vector<float> matrix;
	const size_t size;

public:
	MyMatrix(size_t n) : size(n)
	{
		if (n > 1)
			matrix = std::vector<float>(size * size, 0);
		else
			throw std::invalid_argument("Martix should have size at least 2x2\n");
	}

	size_t get_size() const { return size; }

	float get_element(const size_t i, const size_t j) const { return matrix[i * size + j]; }

	void fill_randomly(int max_value = 100, int accuracy = 0)
	{
		srand(time(NULL) % rand());
		for (size_t i = 0; i < size; i++)
			for (size_t j = 0; j < size; j++)
				matrix[i * size + j] = float(std::rand() % int(max_value * pow(10, accuracy)) / (pow(10, accuracy)));
	}

	void transpose()
	{
		for (size_t i = 0; i < size - 1; i++)
			for (size_t j = i + 1; j < size; j++)
				std::swap(matrix[i * size + j], matrix[j * size + i]);
	}

	MyMatrix multiply(const MyMatrix& other) const
	{
		if (other.size == size)
		{
			MyMatrix result(size);

			auto t1 = std::chrono::high_resolution_clock::now(); 

			for (int i = 0; i < size; i++)
				for (int j = 0; j < size; j++)
					for (int k = 0; k < size; k++)
						result.matrix[i * size + j] += this->matrix[i * size + k] * other.matrix[k * size + j];

			auto t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> time = (t2 - t1);
			std::cout << "Перемножение матриц размером " << size << ":                     " << time.count() << " секунд\n";

			return result;
		}
		else
		{
			throw std::invalid_argument("Martices must be the same size\n");
		};
	}

	MyMatrix multiply_transposed(const MyMatrix& other) const
	{
		if (other.size == size)
		{
			MyMatrix result(size);

			auto t1 = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < size; i++)
				for (int j = 0; j < size; j++)
					for (int k = 0; k < size; k++)
						result.matrix[i * size + j] += this->matrix[i * size + k] * other.matrix[j * size + k];

			auto t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> time = (t2 - t1);
			std::cout << "Перемножение матриц с транспонированием размером " << size << ": " << time.count() << " секунд\n";

			return result;
		}
		else
		{
			throw std::invalid_argument("Martices must be the same size\n");
		};
	}

	friend std::ostream& operator<<(std::ostream& os, const MyMatrix& m)
	{
		os << std::setprecision(7);
		os << "/" << std::string(m.size * 13, ' ') << "\\\n";
		for (size_t i = 0; i < m.size; i++)
		{
			os << '|';
			for (size_t j = 0; j < m.size; j++)
				os << ' ' << std::setw(11) << m.matrix[i * m.size + j] << ' ';
			os << "|\n";
		}
		os << "\\" << std::string(m.size * 13, ' ') << "/\n\n";
		return os;
	}
};

int main()
{
	setlocale(LC_ALL, "");

	std::vector<size_t> sizes =
	{
		512, 640, 768, 896, 1024, 1152, 1280, 1408, 1536, 1664, 1792, 1920,
	};

	try
	{
		for (auto& size : sizes)
		{
			MyMatrix matrix1(size);
			MyMatrix matrix2(size);
			matrix1.fill_randomly(100, 2);
			matrix2.fill_randomly(100, 2);

			MyMatrix result1 = matrix1.multiply(matrix2);

			matrix2.transpose();
			MyMatrix result2 = matrix1.multiply_transposed(matrix2);

			//std::cout << matrix1;
			//std::cout << matrix2;
			//std::cout << std::string(matrix1.get_size() * 13 + 2, '=') << "\n\n";
			//std::cout << result1;
			//std::cout << result2;
		}
	}
	catch (std::invalid_argument ex)
	{
		std::cout << ex.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}