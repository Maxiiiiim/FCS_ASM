/*
Студент: Минец Максим
БПИ193
 
Вариант 15
Вывести список всех целых чисел, содержащих от 4 до 9 значащих
цифр, которые после умножения на n, будут содержать все те же самые
цифры в произвольной последовательности и в произвольном количестве.
Входные данные: целое положительное число n, больше единицы и меньше
десяти. Количество потоков является входным параметром.
*/


#include <iostream>
#include <cstdlib>
#include <thread>
#include <string>
#include <stdio.h>
#include <vector>

const int values_amount = 999999000; // Количество чисел от первого с 4 значащами цифрами по максимальное из 9 цифр.
int n, threads_amount; // Число n (из условия) и количество тредов.
std::vector<int> vec;

/// <summary>
/// Пользователь вводит число n, на которое умножаются числа.
/// </summary>
/// <returns> Возвращает число n, на которое умножаются числа </returns>
int input_n()
{
	int num;
	std::cin >> num;

	while (num <= 1 || num >= 10) // Границы заданы по условию.
	{
		std::cout << "Incorrect input. Try again (number should be > 0 and < 10): ";
		std::cin >> num;
	}

	return num;
}

/// <summary>
/// Пользователь вводит число потоков.
/// </summary>
/// <returns> Возвращает число потоков </returns>
int inputnumerOfThreads()
{
	int num;
	std::cin >> num;

	while (num <= 0 || num >= 100) // Поставлена верхняя граница для количества потоков (по усмотрению): 100.
	{
		std::cout << "Incorrect input. Try again (number should be > 0 and < 100): ";
		std::cin >> num;
	}

	return num;
}

/// <summary>
/// Метод, осуществляющий бинарный поиск.
/// </summary>
/// <param name="i"> Индекс </param>
/// <param name="num"> Число </param>
/// <returns> Возвращает переменную типа bool. </returns>
bool binSearch(int i, std::string num)
{
	if (num.length() == 0)
		return false;

	int lhs = 0;
	int rhs = num.length() - 1;
	while (lhs < rhs - 1)
	{
		int m = lhs + (rhs - lhs) / 2;

		if (num[m] < i)
			lhs = m;
		else
			rhs = m;
	}

	return num[lhs] == i || num[rhs] == i;
}

bool compare(std::string modified, std::string original) {

	for (int i = 0; i < original.length(); ++i) {
		if (!binSearch(original[i], modified)) {
			return false;
		}
	}

	return true;
}

void stream(int index) {
	int amount = values_amount / threads_amount;  // Длина каждого подотрезка из отрезка длиной в values_amount.

	int lower = 1000 + amount * (index - 1); // Нижняя граница для текущего треда.
	int upper; // Верхняя граница для текущего треда.

	if (index == 10)
		upper = amount* index;
	else
		upper = 999 + amount * index;

	for (int i = lower; i <= upper; ++i) {
		if (compare(std::to_string(i * n), std::to_string(i))) {
			std::cout << "\n" << i; 
		}
	}
}

/// <summary>
/// Метод, реализующий создание необходимого количество потоков.
/// </summary>
void makeThreads() {
	std::vector<std::thread> ts;

	std::cout << "\nProgram has started its work.\n"; // Показываем в консоли, что программа начала свою работу.

	for (int i = 1; i <= threads_amount; ++i) {
		ts.push_back(std::thread(stream, i));
	}

	for (int i = 0; i < threads_amount; ++i) {
		ts[i].join();
	}
}

int main(int argc, char* argv[]) { 
	std::cout << "Input number of threads: ";
	threads_amount = inputnumerOfThreads(); // Количество потоков.
	std::cout << "Input n: ";
	n = input_n(); // Число n.

	makeThreads();

	return 0;
}