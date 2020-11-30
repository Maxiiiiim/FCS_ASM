#include <iostream>
#include "omp.h"
#include <iomanip>
#include <string>
#include <chrono>
#include <mutex>

/*
	Минец Максим
	БПИ-193

	Вариант 15
	Вывести список всех целых чисел, содержащих от 4 до 9 значащих цифр,
	которые после умножения на n, будут содержать все те же самые цифры
	в произвольной последовательности и в произвольном количестве. Входные
	данные: целое положительное число n, больше единицы и меньше десяти.
	Количество потоков является входным параметром.

*/

// Количество чисел от первого с 4 значащами цифрами по максимальное из 9 цифр.
constexpr int VALUES_AMOUNT = 999999000;
// lowerBound - минимальное четырехзначное число - 1;
// upperBound - максимальное девятизначное число.
constexpr int lowerBound = 999, upperBound = 999999999;
std::mutex mut;

/// <summary>
/// Пользователь вводит число n, на которое умножаются числа.
/// </summary>
/// <returns> Возвращает число n, на которое умножаются числа </returns>
int input_n()
{
	int num;

	do
	{
		std::cin >> num;
		if (num <= 1 || num >= 10)
			std::cout << "Incorrect input. Try again (number should be > 0 and < 10): ";
	} while (num <= 1 || num >= 10);

	return num;
}

/// <summary>
/// Пользователь вводит границы, в интервале которых ищутся числа, 
/// подходящие в решении данной задачи.
/// </summary>
/// <param name="min"> Минимальная граница введенного числа </param>
/// <returns> Возвращает одну из границ, необходимую при работе программы </returns>
int inputBounds(int min)
{
	int num;

	do
	{
		std::cin >> num;
		if (num <= min || num > upperBound)
			std::cout << "Incorrect input. Try again(number should be > " 
			<< min << " and < " << upperBound + 1 << "): ";
	} while (num <= min || num > upperBound);

	return num;
}

/// <summary>
/// Пользователь вводит число потоков.
/// </summary>
/// <returns> Возвращает число потоков </returns>
int inputnumerOfThreads()
{
	int num;
	// Поставлена верхняя граница для количества потоков (по усмотрению разработчика): 100.
	do
	{
		std::cin >> num;
		if (num < 1 || num > 1000)
			std::cout << "Incorrect input. Try again (number should be > 0 and < 1000): ";
	} while (num < 1 || num > 1000);

	return num;
}

void process(int n, int t, int min, int max) {
	// Устанавливаем количество потоков.
	omp_set_num_threads(t);
#pragma omp parallel for num_threads(t)
	for (int i = min; i <= max; ++i) {
		// Текущая цифра.
		std::string number = std::to_string(i);
		// Текущая цифра, умноженная на n.
		std::string result = std::to_string(i * n);
		// Флаг, показывающий, соблюдает ли новое число необходимым условиям.
		bool flag = true;

		for (char num : number) {
			unsigned int lhs = 0;
			unsigned int rhs = result.length() - 1;
			while (lhs < rhs - 1) {
				unsigned int mid = lhs + (rhs - lhs) / 2;
				result[mid] < num ? lhs = mid : rhs = mid;
			}
			if (!(result[lhs] == num || result[rhs] == num)) {
				flag = false;
				break;
			}
		}

		// Вывод подходящих нам чисел.
		if (flag) {
			mut.lock();
			std::cout << std::setfill(' ') << std::setw(25) << "num = " << number
				<< std::setfill(' ') << std::setw(25) << "n*num = " << result
				<< std::setfill(' ') << std::setw(30) << "current thread = " 
				<< std::to_string(omp_get_thread_num()) << "\n";
			mut.unlock();
		}
	}
}

int main(int argc, char* argv[]) {
	std::cout << "Input n: ";
	int n = input_n();

	std::cout << "Input number of threads: ";
	int threads = inputnumerOfThreads();

	std::cout << "Input minimum number for search: ";
	int minBound = inputBounds(lowerBound);

	std::cout << "Input maximum number for search: ";
	int maxBound = inputBounds(minBound);

	std::cout << "\nProgram has started its work.\n\n";
	
	process(n, threads, minBound, maxBound);

	std::cout << "\nProgram has finished its work.\n";

	return 0;
}